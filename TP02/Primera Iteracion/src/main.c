#include <stdio.h>
#include <stdlib.h>

#define MAX_LINEA 100

int main() {
    FILE *archivo = fopen("../datos/gini_data.txt", "r");
    if (!archivo) {
        perror("No se pudo abrir el archivo gini_data.txt");
        return 1;
    }

    char linea[MAX_LINEA];
    int anio;
    float gini;
    int gini_convertido;

    printf("Índice GINI de Argentin procesado por año:\n\n");

    while (fgets(linea, MAX_LINEA, archivo)) {
        // Leer año y valor GINI
        if (sscanf(linea, "%d,%f", &anio, &gini) == 2) {
            gini_convertido = (int)gini + 1;
            printf("Año %d: GINI original = %.2f, convertido = %d\n", anio, gini, gini_convertido);
        }
    }

    fclose(archivo);
    return 0;
}
