#include <stdio.h>
#include <stdlib.h>

extern int convertir_float_a_int(float value);  // Función ASM que convierte float a int

// Función de conversión usando la función en ASM
void convert(float* input, int* output, int size){

    printf("\nAntes de la conversión (en función 'convert')");

    for(int i = 0; i < size; i++) {
        output[i] = convertir_float_a_int(input[i]);  // Llamada a la función ASM
    }
    
    printf("\nDespués de la conversión (en función 'convert')\n");

}

int main() {
    // Datos de prueba
    float input[] = {1.23, 4.56, 7.89};
    int output[3];
    
    
    printf("Antes de llamar a la función convert\n");

    convert(input, output, 3);
    
    printf("\nDespues de llamar a la funcion convert\n");

    // Imprimir resultados
    for (int i = 0; i < 3; i++) {
        printf("Valor convertido de %.2f a %d\n", input[i], output[i]);
    }
    
    return 0;
}
