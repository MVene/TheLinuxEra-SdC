#!/bin/bash

set -e  # Salir si hay un error

echo "Compilando para depuración (modo 64 bits)..."

# Ir al directorio raíz del proyecto
cd "$(dirname "$0")"

# Compilar el archivo ASM
echo "Compilando ASM: procesamiento.asm"
nasm -f elf64 -o procesamiento.o ../src/procesamiento.asm

# Compilar el archivo C con símbolos de depuración
echo "Compilando C: debug/debug.c"
gcc -m64 -g -c -o debug.o ../debug/debug.c

# Enlazar ambos objetos en un ejecutable
echo "Enlazando..."
gcc -m64 -g -o ../debug/debug_exec debug.o procesamiento.o

# Limpiar archivos intermedios
echo "Limpiando archivos intermedios..."
rm -f debug.o procesamiento.o

echo "Compilación completa. Ejecutable: debug/debug_exec"
echo "Podés iniciar GDB con:"
echo "gdb debug/debug_exec"
