BITS 16
ORG 0x7C00

start:
    ; Inicializa los segmentos
    xor ax, ax
    mov ds, ax
    mov es, ax

    ; Dirección base de memoria de video (modo texto color)
    mov ax, 0xB800
    mov es, ax
    xor di, di  ; posición inicial (esquina superior izquierda)

    ; Texto a mostrar
    mov si, msg

.print_loop:
    lodsb           ; carga siguiente carácter en AL
    cmp al, 0
    je done         ; fin de string (null terminator)

    mov ah, 0x0F    ; atributo: blanco sobre negro
    stosw           ; escribe AX en [ES:DI] y avanza DI
    jmp .print_loop

done:
    cli
    hlt

msg db "HELLO THINKPAD!", 0

times 510 - ($ - $$) db 0
dw 0xAA55


;nasm -f bin boot.asm -o boot.img
;qemu-system-x86_64 -drive format=raw,file=boot.img
;sudo dd if=boot.img of=/dev/sdX bs=512 count=1
