#include <Arduino.h>
#include <Wire.h>
#include "rfid.h"
#include "bmp280.h"
#include "SensorData.h"
#include "BH1750_LightSensor.h"
#include "screen.h"
#include "main.h"

void setup() {
  Wire.begin(9, 8);
  SPI.begin(SCK_PIN, MISO_PIN, MOSI_PIN);
  Serial.begin(115200);
  delay(500);
  
  initRfid();
  //initScreen();
  //initBMP280();
  //initBH1750();

  delay(500);
}

void loop() {
  digitalWrite(TFT_CS, 1);

  digitalWrite(SS_PIN, 0);
  verifierRfid();
  digitalWrite(SS_PIN, 1);
  //updateBMP280Data();
  //updateBH1750Data();
  
  digitalWrite(TFT_CS, 0);
  updateScreen();
  digitalWrite(TFT_CS, 1);

  //Serial.println(sensorData.temperature);
  //Serial.println(sensorData.light);
  //Serial.println(sensorData.pressure);
  delay(500);
}