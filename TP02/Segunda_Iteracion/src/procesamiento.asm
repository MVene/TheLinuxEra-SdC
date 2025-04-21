; procesamiento64.asm

section .text
    global convertir_float_a_int

; int convertir_float_a_int(float gini);

convertir_float_a_int:
    ; Convención de llamadas en Linux x86_64:
    ;   - El argumento de tipo float se pasa en el registro xmm0
    ;   - El valor de retorno de tipo int se devuelve en el registro eax

    cvttss2si eax, xmm0    ; Convertir el valor float en xmm0 a int en eax (trunca)
    add eax, 1             ; Sumar 1 al valor entero resultante
          
    ret

section .note.GNU-stack noalloc noexec nowrite ; Indica que la pila no debe ser ejecutable

