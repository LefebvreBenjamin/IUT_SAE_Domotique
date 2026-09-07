#include <Arduino.h>
#include "rfid.h"

void setup() {
  initRfid();
  Serial.begin(115200);
  delay(1000);
}

void loop() {
  verifierRfid();
  delay(500);
}