#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/ioctl.h>
#include <asm/io.h>         // Para ioremap, iounmap, readl, writel
#include <linux/ioport.h>   // Para request_mem_region, release_mem_region

// --- Definiciones para los comandos ioctl ---
#define TP_IOC_MAGIC 'k'
#define TP_SELECT_SIGNAL1 _IO(TP_IOC_MAGIC, 0)
#define TP_SELECT_SIGNAL2 _IO(TP_IOC_MAGIC, 1)

// --- Variables globales del driver ---
static int current_signal = 1; // Variable global para saber qué señal está seleccionada (por defecto la 1)

static dev_t first;         // Variable global para el primer device number
static struct cdev c_dev;   // Variable global para la estructura del character device
static struct class *cl;    // Variable global para la clase del dispositivo

// --- Direcciones y Registros GPIO para Raspberry Pi 4 (BCM2711) ---
// Base de direcciones de los periféricos en Raspberry Pi 4 (BCM2711)
#define BCM2711_PERI_BASE 0xFE000000
// Offset del bloque GPIO desde la base de periféricos
#define GPIO_BASE (BCM2711_PERI_BASE + 0x200000)
// Tamaño del bloque de memoria GPIO a mapear
#define GPIO_SIZE 0xB4 // El tamaño necesario para los registros GPFSEL y GPLEV

// Offsets de los registros GPIO dentro del bloque mapeado (desde GPIO_BASE)
#define GPFSEL0_OFFSET 0x00 // GPIO Function Select 0 (Pines 0-9)
#define GPFSEL1_OFFSET 0x04 // GPIO Function Select 1 (Pines 10-19)
#define GPFSEL2_OFFSET 0x08 // GPIO Function Select 2 (Pines 20-29)
// ... otros GPFSEL hasta GPFSEL5
#define GPLEV0_OFFSET  0x34 // GPIO Pin Level 0 (Pines 0-31)

// Puntero a la dirección de memoria virtual mapeada de los GPIOs
static void __iomem *gpio_base_addr;

// --- Pines GPIO a usar para las señales (Números GPIO Broadcom) ---
#define SIGNAL1_GPIO 17 // Ejemplo: GPIO 17 (Pin físico 11)
#define SIGNAL2_GPIO 27 // Ejemplo: GPIO 27 (Pin físico 13)

// --- Funciones auxiliares para GPIO ---
// Función para configurar un pin GPIO como INPUT
static void gpio_set_input(unsigned int gpio_num) {
    unsigned int reg_offset = (gpio_num / 10) * 4;
    unsigned int bit_shift = (gpio_num % 10) * 3;

    unsigned long reg_value = readl(gpio_base_addr + reg_offset);
    reg_value &= ~(7 << bit_shift); // Poner a cero los 3 bits (000 para INPUT)
    writel(reg_value, gpio_base_addr + reg_offset);
    printk(KERN_INFO "tp_driver: GPIO %u configurado como INPUT.\n", gpio_num);
}

// Función para leer el estado actual de un pin GPIO
static int gpio_get_level(unsigned int gpio_num) {
    unsigned int reg_offset = GPLEV0_OFFSET;
    unsigned int bit_shift = gpio_num;

    unsigned long reg_value = readl(gpio_base_addr + reg_offset);
    return (reg_value >> bit_shift) & 1;
}

// --- Implementación de las operaciones de archivo ---
static int my_open(struct inode *i, struct file *f)
{
    printk(KERN_INFO "tp_driver: open()\n");
    return 0;
}

static int my_close(struct inode *i, struct file *f)
{
    printk(KERN_INFO "tp_driver: close()\n");
    return 0;
}

// my_read() lee el estado de un pin GPIO real
static ssize_t my_read(struct file *f, char __user *buf, size_t len, loff_t *off)
{
    unsigned int gpio_num;
    char value_char;
    int gpio_level;

    if (*off != 0) {
        return 0; // EOF si ya se leyó una vez en esta sesión
    }

    if (current_signal == 1) {
        gpio_num = SIGNAL1_GPIO;
    } else {
        gpio_num = SIGNAL2_GPIO;
    }

    if (!gpio_base_addr) {
        printk(KERN_ERR "tp_driver: Error: gpio_base_addr no está mapeado.\n");
        return -EFAULT;
    }
    gpio_level = gpio_get_level(gpio_num);
    value_char = (gpio_level == 1) ? '1' : '0';

    printk(KERN_INFO "tp_driver: read() - Leyendo estado de GPIO %d (Senal %d): %c\n",
           gpio_num, current_signal, value_char);

    if (len < 1) {
        return -EINVAL; // Invalid argument
    }

    if (copy_to_user(buf, &value_char, 1) != 0) {
        return -EFAULT;
    } else {
        (*off)++;
        return 1;
    }
}

// my_write() no se modifica
static ssize_t my_write(struct file *f, const char __user *buf, size_t len, loff_t *off)
{
    printk(KERN_INFO "tp_driver: write() - Se intento escribir %zu bytes\n", len);
    return len;
}

// Función para manejar las llamadas ioctl
static long tp_sensor_ioctl(struct file *f, unsigned int cmd, unsigned long arg)
{
    f->f_pos = 0; // Reinicia el offset interno del archivo del driver
    printk(KERN_INFO "TP_CDD: IOCTL - Offset de f->f_pos reiniciado a 0.\n");

    switch (cmd) {
        case TP_SELECT_SIGNAL1:
            current_signal = 1;
            printk(KERN_INFO "TP_CDD: IOCTL - Seleccionada Senal 1\n");
            break;
        case TP_SELECT_SIGNAL2:
            current_signal = 2;
            printk(KERN_INFO "TP_CDD: IOCTL - Seleccionada Senal 2\n");
            break;
        default:
            printk(KERN_WARNING "TP_CDD: IOCTL - Comando desconocido %u\n", cmd);
            return -ENOTTY;
    }
    return 0;
}

// --- Estructura file_operations ---
static struct file_operations pugs_fops =
{
    .owner = THIS_MODULE,
    .open = my_open,
    .release = my_close,
    .read = my_read,
    .write = my_write,
    .unlocked_ioctl = tp_sensor_ioctl
};

// --- Constructor del módulo ---
static int __init tp_driver_init(void)
{
    int ret;
    struct device *dev_ret;

    printk(KERN_INFO "tp_driver: Registrado exitosamente..!!\n");

    if ((ret = alloc_chrdev_region(&first, 0, 1, "tp_driver")) < 0)
    {
        printk(KERN_ALERT "tp_driver: Fallo al asignar chrdev_region\n");
        return ret;
    }

    // --- Inicialización y mapeo de GPIOs ---
    /*if (!request_mem_region(GPIO_BASE, GPIO_SIZE, "tp_driver_gpio")) {
        printk(KERN_ALERT "tp_driver: Fallo al solicitar region de memoria GPIO (%x).\n", GPIO_BASE);
        unregister_chrdev_region(first, 1);
        return -EBUSY;
    }*/

    gpio_base_addr = (unsigned int *)ioremap(GPIO_BASE, GPIO_SIZE);
    if (!gpio_base_addr) {
        printk(KERN_ALERT "tp_driver: Fallo al mapear memoria GPIO.\n");
        //release_mem_region(GPIO_BASE, GPIO_SIZE); // CORREGIDO
        //unregister_chrdev_region(first, 1);
        return -ENOMEM;
    }
    printk(KERN_INFO "GPIO SIGNAL: Successfully mapped GPIO memory.\n");
    
    gpio_set_input(SIGNAL1_GPIO);
    gpio_set_input(SIGNAL2_GPIO);
    printk(KERN_INFO "tp_driver: GPIOs %d y %d configurados como entrada.\n", SIGNAL1_GPIO, SIGNAL2_GPIO);
    // --- Fin de inicialización de GPIOs ---

    if (IS_ERR(cl = class_create("tp_class")))
    {
        printk(KERN_ALERT "tp_driver: Fallo al crear la clase de dispositivo\n");
        if (gpio_base_addr) iounmap(gpio_base_addr);
        release_mem_region(GPIO_BASE, GPIO_SIZE); // CORREGIDO
        unregister_chrdev_region(first, 1);
        return PTR_ERR(cl);
    }

    if (IS_ERR(dev_ret = device_create(cl, NULL, first, NULL, "tp_driver")))
    {
        printk(KERN_ALERT "tp_driver: Fallo al crear el dispositivo\n");
        class_destroy(cl);
        if (gpio_base_addr) iounmap(gpio_base_addr);
        release_mem_region(GPIO_BASE, GPIO_SIZE); // CORREGIDO
        unregister_chrdev_region(first, 1);
        return PTR_ERR(dev_ret);
    }

    cdev_init(&c_dev, &pugs_fops);
    if ((ret = cdev_add(&c_dev, first, 1)) < 0)
    {
        printk(KERN_ALERT "tp_driver: Fallo al añadir cdev\n");
        device_destroy(cl, first);
        class_destroy(cl);
        if (gpio_base_addr) iounmap(gpio_base_addr);
        release_mem_region(GPIO_BASE, GPIO_SIZE); // CORREGIDO
        unregister_chrdev_region(first, 1);
        return ret;
    }
    return 0;
}

// --- Destructor del módulo ---
static void __exit tp_driver_exit(void)
{
    cdev_del(&c_dev);
    device_destroy(cl, first);
    class_destroy(cl);
    unregister_chrdev_region(first, 1);

    // --- Desinicialización de GPIOs ---
    if (gpio_base_addr) {
        iounmap(gpio_base_addr);
        printk(KERN_INFO "tp_driver: Memoria GPIO desmapeada.\n");
    }
    release_mem_region(GPIO_BASE, GPIO_SIZE); // CORREGIDO
    printk(KERN_INFO "tp_driver: Region de memoria GPIO liberada.\n");
    // --- Fin de desinicialización de GPIOs ---

    printk(KERN_INFO "tp_driver: dice Adios mundo cruel..!!\n");
}

// --- Registro de las funciones de inicialización y salida del módulo ---
module_init(tp_driver_init);
module_exit(tp_driver_exit);

// --- Información del módulo ---
MODULE_LICENSE("GPL");
MODULE_AUTHOR("The Linux Era");
MODULE_DESCRIPTION("Driver para TP de sensado de señales con GPIOs reales");