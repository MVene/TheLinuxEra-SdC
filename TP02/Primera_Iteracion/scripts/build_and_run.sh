#!/bin/bash

# Activar entorno virtual
echo "Activando entorno virtual..."
if [ ! -d "venv" ]; then
    echo "El entorno virtual no existe. Creando uno nuevo..."
    python3 -m venv venv || { echo "Error al crear el entorno virtual."; exit 1; }
fi

source venv/bin/activate || { echo "No se pudo activar el entorno virtual."; exit 1; }

# Verificar e instalar numpy
echo "Verificando e instalando numpy..."
pip install numpy
# Verificar e instalar requests
echo "Verificando e instalando requests..."
pip install requests 

# Compilar archivos C
echo "Compilando archivos C.."
gcc -c -o main.o ../src/main.c

# Crear librería compartida
echo "Creando librería compartida..."
gcc -shared -o main.so main.o


# Ejecutar el script de Python
echo "Ejecutando script de Python..."
python3 ../src/api_rest.py

# Eliminar archivos objeto y librería compartida
echo "Eliminando archivos objeto y librería compartida..."
rm -f main.o main.so

# Desactivar entorno virtual
echo "Desactivando entorno virtual..."
deactivate

# Eliminar entorno virtual
echo "Eliminando entorno virtual..."
rm -rf venv

echo "Proceso completado."
