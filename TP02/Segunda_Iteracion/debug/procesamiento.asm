; procesamiento64.asm
section .text
    global convertir_float_a_int

; int convertir_float_a_int(float gini);

convertir_float_a_int:
    ; 64-bit Linux calling convention:
    ; - float arg is in xmm0
    ; - Return value: int in eax
    
    cvttss2si eax, xmm0    ; Convert float in xmm0 to int -> eax (truncates)
    add eax, 1             ; Add 1 to result

    ret

section .note.GNU-stack noalloc noexec nowrite