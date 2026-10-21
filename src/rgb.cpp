#include "rgb.h"

// Réglages PWM
#define LED_FREQ 5000   // 5 kHz
#define LED_RES  8      // 0 à 255

// Dernière couleur et luminosité
static uint8_t couleurR = 0, couleurG = 0, couleurB = 0;
static uint8_t luminosite = 100;   // en %

void initLed() {
  // PWM sur les 3 broches
  ledcAttach(LED_R, LED_FREQ, LED_RES);
  ledcAttach(LED_G, LED_FREQ, LED_RES);
  ledcAttach(LED_B, LED_FREQ, LED_RES);

  setLed(0, 0, 0);        // Éteinte
  setLed(255, 255, 255);  // Test : blanc
  setLuminosite(30);
}

// Couleur de 0 à 255
void setLed(uint8_t r, uint8_t g, uint8_t b) {
  couleurR = r;
  couleurG = g;
  couleurB = b;

  // Applique la luminosité
  ledcWrite(LED_R, r * luminosite / 100);
  ledcWrite(LED_G, g * luminosite / 100);
  ledcWrite(LED_B, b * luminosite / 100);
}

// Luminosité de 0 à 100 %
void setLuminosite(uint8_t pourcent) {
  if (pourcent > 100) pourcent = 100;
  luminosite = pourcent;
  setLed(couleurR, couleurG, couleurB);   // Applique tout de suite
}