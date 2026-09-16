#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ST7789.h"

#define TFT_SCK   6
#define TFT_MISO  2
#define TFT_MOSI  7
#define TFT_CS    18
#define TFT_DC    19
#define TFT_RST   21


#define BLANC 0xFFFF
#define NOIR 0x0000

void initScreen();
void updateScreen();