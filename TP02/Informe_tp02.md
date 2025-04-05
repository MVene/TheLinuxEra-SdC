# TP02 – Sistemas de Computación  
## Primera Iteración – API REST, Python y C

### 🔹 Objetivo

En esta primera etapa del trabajo práctico se busca:
- Obtener datos reales del índice GINI desde una API REST (Banco Mundial).
- Guardar esos datos en un archivo.
- Procesarlos utilizando un programa en C.
- Realizar conversiones básicas sin usar Assembler.

---

### 🐍 Script en Python

Se desarrolló un script en Python (`api_rest.py`) que:
- Consulta la API REST del Banco Mundial.
- Extrae los valores del índice GINI para Argentina.
- Filtra los datos no nulos.
- Los guarda en el archivo `gini_data.txt` con el formato `año,valor`.

#### Ejemplo de datos generados (`gini_data.txt`):
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


