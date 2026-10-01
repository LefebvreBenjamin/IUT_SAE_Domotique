#include <Arduino.h>
#include <Wire.h>
#include "rfid.h"
#include "cartesRfid.h"
#include "bmp280.h"
#include "SensorData.h"
#include "BH1750_LightSensor.h"
#include "MQ9GazSensor.h"
#include "screen.h"
#include "main.h"
#include "tactilScreen.h"

void setup() {
  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
  SPI.begin(SCK_PIN, MISO_PIN, MOSI_PIN, SS_PIN);
  Serial.begin(115200);
  delay(500);

  pinMode(SS_PIN, OUTPUT);
  pinMode(RST_PIN, OUTPUT);
  pinMode(TFT_CS, OUTPUT);
  pinMode(CS_TACTIL, OUTPUT);
  pinMode(TIRQ_PIN, INPUT);
  
  
  pinMode(TFT_DC, OUTPUT);
  pinMode(TFT_RST, OUTPUT);

  digitalWrite(SS_PIN, 1);
  digitalWrite(RST_PIN, 1);
  digitalWrite(TFT_CS, 1);
  digitalWrite(CS_TACTIL, 1);

  initCartesRfid();
  initRfid();
  initScreen();
  initTouchScreen();
  initMQ9();
  initBMP280();
  initBH1750();

  delay(500);
}

void loop() {
  
  digitalWrite(TFT_CS, 1);
  digitalWrite(CS_TACTIL, 1);
  digitalWrite(SS_PIN, 0);
  verifierRfid();
  updateCartesRfid();

  updateMQ9Data();
  updateBMP280Data();
  updateBH1750Data();
  
  
  digitalWrite(SS_PIN, 1);
  digitalWrite(TFT_CS, 0);
  updateScreen();

  digitalWrite(TFT_CS, 1);
  digitalWrite(CS_TACTIL, 0);
  updateTouchScreen();

}