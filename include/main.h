#pragma once

// Bus I2C
#define I2C_SDA_PIN 9
#define I2C_SCL_PIN 8

// Bus SPI partage par le RFID et l'ecran
#define SCK_PIN  6
#define MISO_PIN 2
#define MOSI_PIN 7

// Module RFID RC522
#define SS_PIN  10
#define RST_PIN 3

// Ecran ST7789
#define TFT_CS  18
#define TFT_DC  19
#define TFT_RST 21
// Capteur de gaz MQ-9 (sortie analogique)
#define MQ9_ADC_PIN 0