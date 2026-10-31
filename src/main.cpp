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
#include "gachePorte.h"
#include "time_et_tempo_EDF.h"
#include "zigbeeLink.h"
#include "rgb.h"
#include "driver/temperature_sensor.h"

// Temperature interne de l'ESP (-10 a 80 C)
// temperatureRead() d'Arduino s'arrete a 50 C
static temperature_sensor_handle_t capteurCpu = NULL;

static void initTempCpu() {
  temperature_sensor_config_t cfg = TEMPERATURE_SENSOR_CONFIG_DEFAULT(-10, 80);
  temperature_sensor_install(&cfg, &capteurCpu);
  temperature_sensor_enable(capteurCpu);
}

// NAN si la lecture rate
static float lireTempCpu() {
  float t;
  if (temperature_sensor_get_celsius(capteurCpu, &t) != ESP_OK) return NAN;
  return t;
}

void setup() {
  setCpuFrequencyMhz(80);   // 80 MHz : moins de chauffe
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
  initGachePorte();
  initLed();

  initEdf();
  initTempCpu();
  initZigbee();

  delay(500);
}

void loop() {
  unsigned long maintenant = millis();

  // Bus SPI : ecran et tactile deselectionnes
  digitalWrite(TFT_CS, 1);
  digitalWrite(CS_TACTIL, 1);

  // RFID : toutes les 100 ms 
  static unsigned long derniereRfid = 0;
  if (maintenant - derniereRfid >= 100) {
    derniereRfid = maintenant;
    digitalWrite(SS_PIN, 0);
    verifierRfid();
    digitalWrite(SS_PIN, 1);
    updateCartesRfid();
  }

  // Capteurs : toutes les secondes
  static unsigned long derniersCapteurs = 0;
  if (maintenant - derniersCapteurs >= 1000) {
    derniersCapteurs = maintenant;
    sensorData.esp32Data.cpuTemp = lireTempCpu();
    updateMQ9Data();
    updateBMP280Data();
    updateBH1750Data();
  }

 
  updateGachePorte();
  updateTimer();
  updateTempo();

  digitalWrite(TFT_CS, 0);
  updateScreen();

  digitalWrite(TFT_CS, 1);
  digitalWrite(CS_TACTIL, 0);
  updateTouchScreen();
  updateZigbee();
}
