#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

#define TFT_SCK   6
#define TFT_MISO  2
#define TFT_MOSI  7
#define TFT_CS    18
#define TFT_DC    19
#define TFT_RST   21

void initScreen();
void updateScreen();