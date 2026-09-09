#include "bmp280.h"
#include "SensorData.h"

Adafruit_BMP280 bmp;

void initBMP280(){
  delay(100);
  
  bool detected = bmp.begin(0x76);
  if (!detected) {
    Serial.println("  (0x76 non trouvé, essai 0x77...)");
    detected = bmp.begin(0x77);
  }

  if (!detected) {
    Serial.println("  BMP280 non détecté");
  } else {
    Serial.println("  BMP280 OK");
  }
}

void updateBMP280Data() {
  sensorData.temperature = bmp.readTemperature();
  sensorData.pressure = bmp.readPressure() / 100.0F;
}