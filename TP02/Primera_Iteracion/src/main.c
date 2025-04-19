#include <stdio.h>
#include <stdlib.h>

void convert(float* input, int* output, int size) {
 
    for(int i = 0; i < size; i++){
        output[i] = (int) input[i];
        output[i] += 1;
    }

}