#include <stdio.h>
#include <stdlib.h>

//Declaracion externa de funcion ASM
extern int convertir_float_a_int(float value); //funcion en ASM

void convert(float* input, int* output, int size){
    for(int i = 0; i < size; i++){
        output[i] = convertir_float_a_int(input[i]);
    }
}