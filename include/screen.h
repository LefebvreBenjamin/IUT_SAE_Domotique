#pragma once

#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ST7789.h"
#include "main.h" 
#include "SensorData.h"

#define BLANC 0xFFFF
#define NOIR 0x0000
#define ROUGE 0xF800
#define VERT 0x07E0
#define JAUNE 0xFFE0


enum DkScreenId
{
  DK_SCREEN_0,
  DK_SCREEN_1,
  DK_SCREEN_2,
  DK_SCREEN_3,
  DK_SCREEN_4,
  DK_SCREEN_5,
  DK_SCREEN_6   // Gestion des cartes RFID
};


void loadScreen(int num);
int getCurrentScreen();


void initScreen();
void updateScreen();
void moveSlider(int x, int y);