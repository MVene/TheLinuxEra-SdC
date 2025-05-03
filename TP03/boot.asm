[bits 16]        ; Modo real
org 0x7C00

start:
    cli                     ; Desactivar interrupciones (por seguridad antes del cambio de modo)

    lgdt [gdt_descriptor]   ; Cargar la GDT y tamaño de la GDT (estructura con segmentos)

    mov eax, cr0
    or eax, 0x1             ; Setear bit PE (Protection Enable) del registro de control CR0
    mov cr0, eax
; A partir de ahora esta en modo protegido por lo que no es valido ejecutar codigo de 16 bits 
   
    jmp 0x08:protected_mode ; Salto lejano: cambia CS con un valor valido del nuevo entorno protegido

; -------- Código en modo protegido ----------
[bits 32]
protected_mode:
    ; Ya estamos en modo protegido
    mov ax, 0x10           ; Data segment selector, para que se haga en modo protegido
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    hlt                    ; Halt -> detiene la CPU

; -------- GDT ----------
gdt_start:
    dq 0x0000000000000000     ; Descriptor nulo
    dq 0x00CF9A000000FFFF     ; Code segment (base=0, limit=4GB, exec)
    dq 0x00CF92000000FFFF     ; Data segment (base=0, limit=4GB, read/write)

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

gdt_end:

times 510 - ($ - $$) db 0 ; Rellena con ceros hasta los 510 bytes.
dw 0xAA55                  ; Boot signature
