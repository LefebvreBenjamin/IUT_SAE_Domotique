#include "SensorData.h"

SensorData sensorData;
ConnectedDevices connectedDevices;
ScreenElement screenElement = {};
TimeDate timeDate = {};
TempoEdf tempoEdf = {};

void addButton(const Button &button) {
    if (screenElement.count >= MAX_BUTTONS) {
        Serial.println("Button list is full");
        return;
    }

    screenElement.button[screenElement.count] = button;
    screenElement.count++;
}