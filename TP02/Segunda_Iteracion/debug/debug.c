#include <stdio.h>
#include <stdlib.h>

extern int convertir_float_a_int(float value);  // Función ASM que convierte float a int

// Función que muestra el estado del stack (puedes hacerlo usando un puntero a la pila)
void mostrar_estado_stack(const char *mensaje) {
    printf("%s\n", mensaje);
    // Aquí se podría usar 'backtrace' en sistemas Unix o alguna función similar para mostrar el stack
    // Dependiendo de lo que desees mostrar, puedes acceder a la dirección de la pila o imprimir registros específicos
}

// Función de conversión usando la función en ASM
void convert(float* input, int* output, int size){
    mostrar_estado_stack("Antes de la conversión (en función 'convert')");
    
    for(int i = 0; i < size; i++) {
        output[i] = convertir_float_a_int(input[i]);  // Llamada a la función ASM
    }
    
    mostrar_estado_stack("Después de la conversión (en función 'convert')");
}

int main() {
    // Datos de prueba
    float input[] = {1.23, 4.56, 7.89};
    int output[3];
    
    mostrar_estado_stack("Antes de llamar a la función 'convert'");
    
    // Llamada a la función que realiza la conversión
    convert(input, output, 3);
    
    mostrar_estado_stack("Después de llamar a la función 'convert'");
    
    // Imprimir resultados
    for (int i = 0; i < 3; i++) {
        printf("Valor convertido de %.2f a %d\n", input[i], output[i]);
    }
    
    return 0;
}
