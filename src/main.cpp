#include <Arduino.h>
#include "rfid.h"
#include "bmp280.h"
#include "SensorData.h"

void setup() {
  Serial.begin(115200);
  initRfid();
  initBMP280();
  delay(1000);
}

void loop() {
  verifierRfid();
  updateBMP280Data();

  Serial.print("Temperature: ");
  Serial.print(sensorData.temperature);
  Serial.print(" °C, Pressure: ");
  Serial.print(sensorData.pressure);
  Serial.println(" hPa");
  delay(500);
}