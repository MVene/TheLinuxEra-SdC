# TP05: Device drivers

## 🎯 Objetivos
- 
- 
## 🔶 Desafío
Diseñar y construir un CDD que permita sensar dos señales externas con un periodo de un segundo. Luego una aplicación a nivel de usuario deberá leer una de las dos señales y graficarla en función del tiempo. La aplicación tambien debe poder indicarle al CDD cuál de las dos señales leer. Las correcciones de escalas de las mediciones, de ser necesario, se harán a nivel de usuario. Los gráficos de la señal deben indicar el tipo de señal que se está sensando, unidades en abcisas y tiempo en ordenadas. Cuando se cambie de señal el gráfico se debe "resetear" y acomodar a la nueva medición.



## ¿Qué es un CDD?
Un Character Device Driver es un tipo de driver que controla aquellos dispositivos que transmiten datos como cadenas de carácteres.

## Generación de señales
Para generar las señales de un período de 1 segundo, se utilizó un circuito elaborado a partir de un NE555.

![555 pinout](imagenes/555pinout.png)

