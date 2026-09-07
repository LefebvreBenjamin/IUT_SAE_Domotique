#include <Arduino.h>

#include <SPI.h>
#include <MFRC522.h>

#pragma once

// Broches ESP32-C6
#define SS_PIN   10
#define RST_PIN  3
#define SCK_PIN  6
#define MISO_PIN 2
#define MOSI_PIN 7

extern MFRC522 rfid;

void initRfid();
void verifierRfid();