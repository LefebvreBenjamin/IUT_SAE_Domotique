#include "rgb.h"

void initLed() {
  setLed(0, 0, 0);   // Éteinte
  setLed(255,255,255);
}

// Couleur de 0 à 255
void setLed(uint8_t r, uint8_t g, uint8_t b) {
  analogWrite(LED_R, r);
  analogWrite(LED_G, g);
  analogWrite(LED_B, b);
}