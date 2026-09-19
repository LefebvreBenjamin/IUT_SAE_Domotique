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

void initScreen();
void updateScreen();