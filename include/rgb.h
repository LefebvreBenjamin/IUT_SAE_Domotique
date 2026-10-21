#pragma once
#include <Arduino.h>

#define LED_R 11
#define LED_G 12
#define LED_B 13

void initLed();
void setLed(uint8_t r, uint8_t g, uint8_t b);
void setLuminosite(uint8_t pourcent);   // 0 à 100 %