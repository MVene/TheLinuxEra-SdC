# Trabajo Práctico Nº1: Rendimiento - Sistemas de Computación

## **1️⃣ Objetivo**

El objetivo de esta tarea es poner en práctica los conocimientos sobre **performance** y **rendimiento** de los computadores. 

El trabajo consta de dos partes principales:

1. Uso de **benchmarks** para evaluar el rendimiento de hardware y tomar decisiones informadas.
2. Medición de la **performance del código** utilizando herramientas de profiling.

---

## **2️⃣ Ejercicio 1: Benchmarks**

### **🔹 2.1 Lista de benchmarks útiles**

A continuación, se detallan algunos benchmarks relevantes según distintos tipos de pruebas:

- **CPU:**
  - SPEC CPU
  - Geekbench
  - Cinebench
  - Phoronix Test Suite

- **Memoria:**
  - STREAM
  - Memtest86

- **Almacenamiento (Discos):**
  - CrystalDiskMark
  - fio

- **Compilación de código:**
  - Phoronix Test Suite - Build Kernel

- **Gaming y gráficos:**
  - 3DMark
  - Unigine Superposition

- **IA y cálculo científico:**
  - TensorFlow Benchmark
  - LINPACK

### **🔹 2.2 Tareas diarias y benchmarks correspondientes**

| **Tarea**                   | **Benchmark recomendado**        |
|-----------------------------|---------------------------------|
| Compilación de código       | Phoronix Test Suite - Build Kernel |
| Desarrollo de software      | SPEC CPU, Geekbench            |
| Análisis de datos           | LINPACK, TensorFlow Benchmark  |
| Edición de video            | Cinebench, 3DMark              |
| Juegos                      | 3DMark, Unigine Superposition  |
| Uso general (ofimática, navegación) | Geekbench, PCMark       |

---

## **3️⃣ Ejercicio 2: Benchmarking de Procesadores**

### **🔹 3.1 Rendimiento en la compilación del kernel de Linux**

Se evaluará el rendimiento de los siguientes procesadores en la compilación del kernel de Linux:

- **Intel Core i5-13600K**
- **AMD Ryzen 9 5900X 12-Core**
- **AMD Ryzen 9 7950X 16-Core**

Se utilizarán resultados de benchmarking obtenidos de **Phoronix Test Suite**.

### **🔹 3.2 Cálculo de Speedup**

La aceleración (**speedup**) se calcula con la siguiente fórmula:

\[ \text{Speedup} = \frac{\text{Tiempo en CPU más lenta}}{\text{Tiempo en CPU más rápida}} \]

### **🔹 3.3 Análisis de eficiencia y costo**

Se analizará cuál de estos procesadores usa mejor sus núcleos y cuál ofrece mejor eficiencia en términos de **costo y consumo energético**.

---

## **4️⃣ Ejercicio 3: Profiling y análisis de rendimiento del código**

### **🔹 4.1 Implementación y pruebas de código**

Se implementará un código de prueba y se analizará su rendimiento usando herramientas de **profiling**:

- **gprof** (Tiempo de ejecución basado en instrumentación)
- **perf** (Sampling de uso de CPU)
- **Heaptrack** (Análisis de memoria y detección de fugas)

### **🔹 4.2 Capturas y análisis de resultados**

Se adjuntarán **capturas de pantalla** y se redactarán **conclusiones** sobre el impacto del tiempo de ejecución en las funciones del programa.

---


