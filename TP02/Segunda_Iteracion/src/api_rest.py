import requests
import ctypes
import os
import numpy as np

# Cargar la libreria
lib_path = os.path.join(os.path.dirname(__file__), '../scripts/main.so')
lib = ctypes.CDLL(lib_path)

# Definir argumentos
lib.convert.argtypes = (ctypes.POINTER(ctypes.c_float), ctypes.POINTER(ctypes.c_int), ctypes.c_int)

# Definir tipo return
lib.convert.restype = ctypes.c_void_p

# wrapper
def convert(input, output, length):
    lib.convert(input, output, length)

# Definir URL de la API
url = "http://api.worldbank.org/v2/country/AR/indicator/SI.POV.GINI?format=json&per_page=100"

# Hacer la consulta HTTP
response = requests.get(url)
if response.status_code != 200:
    print("Error al conectar con la API")
    exit(1)

# Procesar respuesta JSON
data = response.json() # Esto devuelve una lista con 2 elementos
# data[0] = metadata
# data[1] = lista de resultados

# Extraer valores
gini_values = []

for entry in data[1]:
    year = entry['date']
    value = entry['value']
    if value is not None:
        gini_values.append((int(year), float(value)))

# Ordenar por año ascendente
gini_values.sort()

# Guardar años y valores en arrays
float_values = [value for (year, value) in gini_values]
int_year = [year for (year, value) in gini_values]

# Argumentos para la conversión
length = len(float_values)
input_array = (ctypes.c_float * length)(*float_values)
output_array = (ctypes.c_int * length)()

# Convertir a entero y sumar 1
convert(input_array, output_array, length)

# Mostrar resultados
result = np.ctypeslib.as_array(output_array)

print("\nIndice GINI de Argentina procesado por año\n") 

for i in range(length):
    print(f"Año {int_year[i]}: GINI original = {float_values[i]}, GINI convertido = {result[i]}")

print("\n")