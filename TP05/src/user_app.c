#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>   // Para open
#include <unistd.h>  // Para close, read, lseek
#include <sys/ioctl.h> // Para ioctl

// Incluye las mismas definiciones de IOCTL que en tu driver
#define TP_IOC_MAGIC 'k'
#define TP_SELECT_SIGNAL1 _IO(TP_IOC_MAGIC, 0)
#define TP_SELECT_SIGNAL2 _IO(TP_IOC_MAGIC, 1)

int main() {
    int fd;
    char buffer;
    int ret;

    // Abrir el dispositivo
    fd = open("/dev/tp_driver", O_RDWR);
    if (fd < 0) {
        perror("Fallo al abrir /dev/tp_driver");
        return 1;
    }

    printf("Dispositivo /dev/tp_driver abierto exitosamente.\n");

    // --- Probar seleccionando la Señal 1 ---
    printf("\nSeleccionando Senal 1...\n");
    ret = ioctl(fd, TP_SELECT_SIGNAL1);
    if (ret < 0) {
        perror("Fallo en ioctl para TP_SELECT_SIGNAL1");
        close(fd);
        return 1;
    }
    printf("Senal 1 seleccionada. Intentando leer...\n");
    
    // IMPORTANTE: Resetear el offset ANTES de leer si el driver maneja offset (como el tuyo)
    lseek(fd, 0, SEEK_SET); 

    // Leer del dispositivo
    ret = read(fd, &buffer, 1);
    if (ret < 0) {
        perror("Fallo al leer del dispositivo");
    } else if (ret == 0) {
        printf("No se leyeron datos (EOF).\n");
    } else {
        printf("Leido del driver (Senal 1): %c\n", buffer);
    }

    // --- Probar seleccionando la Señal 2 ---
    printf("\nSeleccionando Senal 2...\n");
    ret = ioctl(fd, TP_SELECT_SIGNAL2);
    if (ret < 0) {
        perror("Fallo en ioctl para TP_SELECT_SIGNAL2");
        close(fd);
        return 1;
    }
    printf("Senal 2 seleccionada. Intentando leer...\n");

    // IMPORTANTE: Resetear el offset ANTES de leer si el driver maneja offset (como el tuyo)
    lseek(fd, 0, SEEK_SET); // <-- Ya estaba aquí, pero su importancia es mayor ahora

    // Leer del dispositivo
    ret = read(fd, &buffer, 1);
    if (ret < 0) {
        perror("Fallo al leer del dispositivo");
    } else if (ret == 0) {
        printf("No se leyeron datos (EOF).\n");
    } else {
        printf("Leido del driver (Senal 2): %c\n", buffer);
    }

    // Cerrar el dispositivo
    close(fd);
    printf("\nDispositivo cerrado.\n");

    return 0;
}