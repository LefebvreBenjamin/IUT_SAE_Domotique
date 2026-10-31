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


// Numeros des ecrans
enum EcranId
{
  ECRAN_ACCUEIL = 0,
  ECRAN_DASHBOARD = 3,
  ECRAN_CONTROLE = 5,
  ECRAN_CARTES = 6
};


void loadScreen(int num);
int getCurrentScreen();


void initScreen();
void updateScreen();
void moveSlider(int x, int y);