#include "Adafruit_BMP280.h"
#include <Wire.h>
#include <SPI.h>
#include <Arduino.h>


extern Adafruit_BMP280 bmp;


void initBMP280();
void updateBMP280Data();