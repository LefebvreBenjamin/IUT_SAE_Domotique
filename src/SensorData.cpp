#include "SensorData.h"

SensorData sensorData;
ScreenElement screenElement = {};

void addButton(const Button &button) {
    if (screenElement.count >= MAX_BUTTONS) {
        Serial.println("Button list is full");
        return;
    }

    screenElement.button[screenElement.count] = button;
    screenElement.count++;
}