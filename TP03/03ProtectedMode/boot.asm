[bits 16]
org 0x7C00

start:
    cli
    lgdt [gdt_descriptor]

    mov eax, cr0
    or eax, 0x1
    mov cr0, eax

    jmp 0x08:protected_mode

[bits 32]
protected_mode:
    mov ax, 0x10        ; Selector del segmento de datos (solo lectura)
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    mov dword [0x0000], 0x12345678   ; Intento de escritura -> debe fallar

    hlt

; -------- GDT --------
gdt_start:
    dq 0x0000000000000000         ; Descriptor nulo
    dq 0x00CF9A000000FFFF         ; Código (0x08)
    dq 0x00CF82000000FFFF         ; Datos solo lectura (0x10 → Access Byte: 0x82)

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

gdt_end:

times 510 - ($ - $$) db 0
dw 0xAA55
