#include "bmp280.h"
#include "SensorData.h"

Adafruit_BMP280 bmp;

void initBMP280(){
  Wire.begin(8, 9); // SDA = 8, SCL = 9
  bool detected = bmp.begin(0x76);
  if (!detected) {
    detected = bmp.begin(0x77);
  }

  if (!detected) {
    Serial.println("BMP280 non detecte !");
    while (1);
  }

  Serial.println("BMP280 OK");
}

void updateBMP280Data() {
  sensorData.temperature = bmp.readTemperature();
  sensorData.pressure = bmp.readPressure() / 100.0F;
}