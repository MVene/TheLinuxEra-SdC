#include <Arduino.h>

void medirTiempoOperaciones(int frecuencia) {
  // Configurar la frecuencia de la CPU
  setCpuFrequencyMhz(frecuencia);
  Serial.printf("\n--- Frecuencia de CPU configurada: %d MHz ---\n", frecuencia);
  Serial.printf("Frecuencia real: %d MHz\n", getCpuFrequencyMhz());  // Verificación

  const int loop_count = 26000000; // Número de iteraciones para las pruebas
  Serial.printf("Número de iteraciones: %d\n", loop_count);

  unsigned long inicio, fin, tiempo;
  volatile int sumaEnteros = 0;
  volatile float sumaFlotantes = 0.0;

  // Medir tiempo para sumas con enteros
  inicio = millis();
  for (volatile int i = 0; i < loop_count; i++) {
      sumaEnteros += i;
  }
  fin = millis();
  tiempo = (fin - inicio); 
  Serial.printf("Tiempo suma enteros: %lu ms\n", tiempo);

  // Medir tiempo para sumas con float
  inicio = millis();
  for (volatile int i = 0; i < loop_count; i++) {
      sumaFlotantes += 0.5f;
  }
  fin = millis();
  tiempo = (fin - inicio);
  Serial.printf("Tiempo suma flotantes: %lu ms\n", tiempo);
}

void setup() {
  Serial.begin(115200);
  delay(1000); // Esperar a que el puerto serie esté listo

  Serial.println("Medición de tiempo de operaciones aritméticas:");
  medirTiempoOperaciones(160); // Frecuencia de CPU a 80 MHz
}

void loop() {
  // No se necesita código en el bucle principal
}