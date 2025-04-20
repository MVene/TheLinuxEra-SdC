# TP02 – Sistemas de Computación  

´´´
TP02/
├─ src/
│  ├─ api_rest.py       # Script Python para consumir API y preparar datos
│  └─ main.c            # Biblioteca en C que implementa la función convert()
└─ scripts/
   └─ build_and_run.sh  # Script de automatización: venv, dependencias, compilación y ejecución
´´´
## Primera Iteración – API REST, Python y C

### 🔹 Objetivo

En esta primera etapa del Trabajo Práctico #2 se busca:

- Obtener datos reales del índice GINI de Argentina desde la API REST del Banco Mundial.
- Filtrar y ordenar los datos en Python.
- Exportar una función en C para procesar esos datos (convertir de float a int y sumar 1).
- Integrar Python y C mediante ctypes.

---

### 🐍 Script en Python (api_rest.py)

Se desarrolló un script en Python (`api_rest.py`) que:

- Consulta la API REST del Banco Mundial.
- Extrae los valores del índice GINI para Argentina.
- Filtra los datos no nulos.
- Los guarda en la lista `gini_values` con el formato `año,valor`.

#### Ejemplo de datos generados (`gini_data.txt`)

        1980,40.8
        1986,42.8
        1987,45.3
        ...


---

### ⚙️ Procesamiento en C

El archivo `main.c` lee línea por línea el archivo `gini_data.txt`. Por cada entrada:

- Se separan el año y el valor GINI (float).
- Se convierte el valor de `float` a `int`.
- Se le suma 1 al valor convertido.
- Se imprimen el año, el valor original y el valor procesado.

#### Fragmento de código:


```c

while (fgets(linea, MAX_LINEA, archivo)) {
    if (sscanf(linea, "%d,%f", &anio, &gini) == 2) {
        gini_convertido = (int)gini + 1;
        printf("Año %d: GINI original = %.2f, convertido = %d\n", anio, gini, gini_convertido);
    }
}
```


### Ejemplo de salida por consola:

<div style="text-align: center;">
    <img src="Imagenes/1-1.png" alt="Salida por consola" width="300">
</div>

## Segunda Iteración – 

### 🔹 Objetivo

En la segunda etapa de este trabajo práctico se busca:
- Añadir un programa en Assembler que realice la tarea de conversión a entero y sumar uno.
- Utilizar la herramienta `gdb` para realizar un debug del código en Assembler.
- Mediante la misma, poder visualizar el estado del stack.

### 🐍 Script en Python

Se desarrolló un script en Python (`api_rest.py`) que:
- Consulta la API REST del Banco Mundial.
- Extrae los valores del índice GINI para Argentina.
- Filtra los datos no nulos.
- Los guarda en la lista `data` con el formato `año,valor`.

#### Ejemplo de datos generados (`gini_data.txt`):
        1980,40.8
        1986,42.8
        1987,45.3
        ...


---

### ⚙️ Procesamiento en C

- El archivo `main.c` lee uno por uno cada elemento del arreglo y se lo pasa a una función en Assembler para convertirlo a entero.
- Cada valor convertido se guarda en un arreglo de salida, que luego será presentado en el script de Python.