#!/bin/bash

# Activar entorno virtual
echo "Activando entorno virtual..."
source venv/bin/activate

# Verificar e instalar numpy
echo "Verificando e instalando numpy..."
pip install numpy

# Verificar e instalar requests
echo "Verificando e instalando requests..."
pip install requests

# Compilar archivos C y ASM
echo "Compilando archivos C.."
gcc -m64 -c -o main.o main.c
echo "Compilando archivos ASM..."
nasm -f elf64 -o procesamiento.o procesamiento.asm
echo "Creando libreria compartida..."
gcc -m64 -shared -o main.so main.o procesamiento.o

# Ejecutar el script de Python
echo "Ejecutando script de Python..."
python3 api_rest.py

# Desactivar entorno virtual
echo "Desactivando entorno virtual..."
deactivate

echo "Proceso completado."
