import requests
import ctypes
import os
import numpy as np

# Cargar la libreria
lib_path = os.path.join(os.path.dirname(__file__), 'main.so')
lib = ctypes.CDLL(lib_path)

# Definir argumentos
lib.convert.argtypes = (ctypes.POINTER(ctypes.c_float), ctypes.POINTER(ctypes.c_int), ctypes.c_int)

# Definir tipo return
lib.convert.restype = ctypes.c_void_p

# wrapper
def convert(input, output, length):
    lib.convert(input, output, length)

# -------------------------------------
# Paso 1: definir la URL de la API
# -------------------------------------
url = "http://api.worldbank.org/v2/country/AR/indicator/SI.POV.GINI?format=json&per_page=100"

# -------------------------------------
# Paso 2: hacer la consulta HTTP
# -------------------------------------
response = requests.get(url)
if response.status_code != 200:
    print("Error al conectar con la API")
    exit(1)

# -------------------------------------
# Paso 3: procesar la respuesta JSON
# -------------------------------------
data = response.json()

# -------------------------------------
# Paso 4: procesar la respuesta JSON
# -------------------------------------
gini_values = []

for entry in data[1]:
    year = entry['date']
    value = entry['value']
    if value is not None:
        gini_values.append((int(year), float(value)))

# Ordenar por año ascendente
gini_values.sort()

float_values = [value for (year, value) in gini_values]
int_year = [year for (year, value) in gini_values]

length = len(float_values)
input_array = (ctypes.c_float * length)(*float_values)
output_array = (ctypes.c_int * length)()

convert(input_array, output_array, length)

result = np.ctypeslib.as_array(output_array)
print(result)