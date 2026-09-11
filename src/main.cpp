#include <Arduino.h>
#include <Wire.h>
#include "rfid.h"
#include "bmp280.h"
#include "SensorData.h"
#include "BH1750_LightSensor.h"
#include "screen.h"

void setup() {
  Wire.begin(9, 8);
  Serial.begin(115200);
  delay(500);
  
  initScreen();
  initRfid();
  initBMP280();
  initBH1750();

  delay(500);
}

void loop() {
  verifierRfid();
  updateBMP280Data();
  updateBH1750Data();
  updateScreen();

  Serial.print("Temperature: ");
  Serial.print(sensorData.temperature);
  Serial.println(" °C");
  Serial.print("Pressure: ");
  Serial.print(sensorData.pressure);
  Serial.println(" hPa");
  Serial.print(sensorData.light);
  Serial.println(" lux");
  delay(500);
}