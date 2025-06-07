#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/ioctl.h> // Necesario para ioctl

// --- Definiciones para los comandos ioctl ---
// Un "número mágico" para identificar tus comandos (puede ser cualquier letra)
#define TP_IOC_MAGIC 'k'
// Comando 0: seleccionar señal 1
#define TP_SELECT_SIGNAL1 _IO(TP_IOC_MAGIC, 0)
// Comando 1: seleccionar señal 2
#define TP_SELECT_SIGNAL2 _IO(TP_IOC_MAGIC, 1)

// --- Variables globales del driver ---
// Variable global para saber qué señal está seleccionada (por defecto la 1)
static int current_signal = 1;

static dev_t first;         // Variable global para el primer device number
static struct cdev c_dev;   // Variable global para la estructura del character device
static struct class *cl;    // Variable global para la clase del dispositivo

// No es necesaria la variable 'c' para la lógica actual de read/write

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

// my_read() lee un byte del buffer interno del driver y lo copia al espacio de usuario.
// Devuelve 1 si hay un byte para leer, 0 si ya se leyó todo (EOF).
static ssize_t my_read(struct file *f, char __user *buf, size_t len, loff_t *off)
{
    char value_to_send;
    printk(KERN_INFO "tp_driver: read() - Leyendo Senal %d\n", current_signal);

    // Decide qué valor devolver según la señal seleccionada
    if (current_signal == 1) {
        value_to_send = 'A';
    } else {
        value_to_send = 'B';
    }

    // Asegúrate de que el buffer de usuario tenga al menos 1 byte
    if (len < 1) {
        return -EINVAL; // Invalid argument
    }

    // Copia el carácter al búfer del espacio de usuario
    if (copy_to_user(buf, &value_to_send, 1) != 0)
        return -EFAULT; // Error al copiar al usuario
    else
    {
        // NO modificamos *off aquí, ya que el IOCTL es quien reinicia el offset de f->f_pos
        // Y el lseek en user_app ya ajusta el *off que llega aquí para la siguiente lectura.
        // Si no se hace (*off)++, y el lseek de la app pone off en 0, my_read siempre devolverá 1
        // y la app podrá seguir leyendo si lo desea.
        // Para este ejemplo de prueba, es mejor que *off avance para simular un recurso limitado.
        // Pero como f->f_pos se reinicia en ioctl, esto funcionará.

        (*off)++; // Incrementa el offset del archivo para indicar que ya se leyó
        return 1; // Se leyó 1 byte
    }
}

// my_write() escribe datos desde el espacio de usuario al driver.
// En esta versión, solo imprime un mensaje y devuelve la longitud de los datos.
static ssize_t my_write(struct file *f, const char __user *buf, size_t len, loff_t *off)
{
    printk(KERN_INFO "tp_driver: write() - Se intento escribir %zu bytes\n", len);

    // En esta etapa, el driver no hace nada útil con los datos de escritura.
    // Solo devuelve la longitud para indicar éxito.
    return len;
}

// Función para manejar las llamadas ioctl desde el espacio de usuario.
static long tp_sensor_ioctl(struct file *f, unsigned int cmd, unsigned long arg)
{
    // REINICIA EL OFFSET INTERNO DEL ARCHIVO DEL DRIVER.
    // Esto es crucial para que cada vez que la aplicación selecciona una señal,
    // la próxima lectura (`read`) comience desde el "principio" del recurso de la señal.
    // Esto complementa el `lseek` en la aplicación de usuario.
    f->f_pos = 0;
    printk(KERN_INFO "TP_CDD: IOCTL - Offset de f->f_pos reiniciado a 0.\n");


    switch (cmd) {
        case TP_SELECT_SIGNAL1:
            current_signal = 1; // Selecciona la señal 1
            printk(KERN_INFO "TP_CDD: IOCTL - Seleccionada Senal 1\n");
            break;
        case TP_SELECT_SIGNAL2:
            current_signal = 2; // Selecciona la señal 2
            printk(KERN_INFO "TP_CDD: IOCTL - Seleccionada Senal 2\n");
            break;
        default:
            printk(KERN_WARNING "TP_CDD: IOCTL - Comando desconocido %u\n", cmd);
            return -ENOTTY; // Error: operación de IOCTL no soportada
    }
    return 0; // Éxito
}

// --- Estructura file_operations que define las operaciones del driver ---
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

    // 1. Asigna dinámicamente un número major y registra el rango de dispositivos
    if ((ret = alloc_chrdev_region(&first, 0, 1, "tp_driver")) < 0)
    {
        printk(KERN_ALERT "tp_driver: Fallo al asignar chrdev_region\n");
        return ret;
    }

    // 2. Crea la clase de dispositivo para sysfs
    // Corregido: class_create solo toma un argumento en kernels modernos
    if (IS_ERR(cl = class_create("tp_class"))) // Puedes elegir un nombre de clase más descriptivo si quieres
    {
        printk(KERN_ALERT "tp_driver: Fallo al crear la clase de dispositivo\n");
        unregister_chrdev_region(first, 1);
        return PTR_ERR(cl);
    }

    // 3. Crea el archivo de dispositivo en /dev a través de sysfs/udev
    if (IS_ERR(dev_ret = device_create(cl, NULL, first, NULL, "tp_driver")))
    {
        printk(KERN_ALERT "tp_driver: Fallo al crear el dispositivo\n");
        class_destroy(cl);
        unregister_chrdev_region(first, 1);
        return PTR_ERR(dev_ret);
    }

    // 4. Inicializa la estructura cdev y la añade al kernel
    cdev_init(&c_dev, &pugs_fops);
    if ((ret = cdev_add(&c_dev, first, 1)) < 0)
    {
        printk(KERN_ALERT "tp_driver: Fallo al añadir cdev\n");
        device_destroy(cl, first);
        class_destroy(cl);
        unregister_chrdev_region(first, 1);
        return ret;
    }
    return 0;
}

// --- Destructor del módulo ---
static void __exit tp_driver_exit(void) /* Renombrado para claridad */
{
    cdev_del(&c_dev);
    device_destroy(cl, first);
    class_destroy(cl);
    unregister_chrdev_region(first, 1);
    printk(KERN_INFO "tp_driver: dice Adios mundo cruel..!!\n");
}

// --- Registro de las funciones de inicialización y salida del módulo ---
module_init(tp_driver_init);
module_exit(tp_driver_exit);

// --- Información del módulo ---
MODULE_LICENSE("GPL");
MODULE_AUTHOR("The Linux Era");
MODULE_DESCRIPTION("Driver para TP de sensado de señales");