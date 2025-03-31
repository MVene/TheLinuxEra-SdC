#include <Arduino.h>

void sumaInt();
void sumaFloat();
unsigned long total_time = 0;

void setup() {
  Serial.begin(115200);  // Inicia la comunicación serial
  delay(2000);  // Espera para abrir el monitor serial

  // Setear la frecuencia
  setCpuFrequencyMhz(80);
  Serial.print("Frecuencia: ");
  Serial.print(getCpuFrequencyMhz());
  Serial.println(" MHz");

  sumaInt();
  sumaFloat();

  Serial.print("Tiempo total: ");
  Serial.print(total_time);
  Serial.println(" ms");
}

// Bucle de sumas con enteros durante 5 segundos
void sumaInt() {
  unsigned long startTime = millis();
  unsigned long currentTime = startTime;
  volatile int sumInt = 0;
  for (int i = 0; i < 35000000; i++) {
    sumInt += 1;
  }
  currentTime = millis();
  unsigned long tiempo = currentTime - startTime;
  total_time = tiempo;
  Serial.print("Tiempo suma de enteros: ");
  Serial.print(tiempo);
  Serial.println(" ms");
}

// Bucle de sumas con floats durante 5 segundos
void sumaFloat() {
  unsigned long startTime = millis();
  unsigned long currentTime = startTime;
  volatile float sumFloat = 0.0;
  for (int i = 0; i < 35000000; i++) {
    sumFloat += 0.1f;
  }
  currentTime = millis();
  unsigned long tiempo = currentTime - startTime;
  total_time += tiempo;
  Serial.print("Tiempo suma de floats: ");
  Serial.print(tiempo);
  Serial.println(" ms");
}

void loop() {
  // No es necesario
}