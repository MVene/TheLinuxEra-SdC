import requests
import json

# ----------------------------
# Paso 1: definir la URL de la API
# ----------------------------
url = "http://api.worldbank.org/v2/country/AR/indicator/SI.POV.GINI?format=json&per_page=100"

# ----------------------------
# Paso 2: hacer la consulta HTTP
# ----------------------------
response = requests.get(url)
if response.status_code != 200:
    print("Error al conectar con la API.")
    exit(1)

# ----------------------------
# Paso 3: procesar la respuesta JSON
# ----------------------------
data = response.json()  # Esto devuelve una lista con 2 elementos
# data[0] = metadata
# data[1] = lista de resultados

# ----------------------------
# Paso 4: extraer los valores útiles
# ----------------------------
gini_values = []

for entry in data[1]:
    year = entry['date']
    value = entry['value']
    if value is not None:
        gini_values.append((int(year), float(value)))

# Ordenar por año ascendente
gini_values.sort()

# ----------------------------
# Paso 5: guardar los datos en archivo (para C)
# ----------------------------
with open("TP02/datos/gini_data.txt", "w") as f:
    for year, value in gini_values:
        f.write(f"{year},{value}\n")

print("Archivo 'gini_data.txt' generado correctamente.")
