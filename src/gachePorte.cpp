
#include "gachePorte.h"



void initGachePorte()
{
    pinMode(GACHE_PORTE_PIN, OUTPUT);
}


void updateGachePorte()
{
    const uint32_t time = millis();
    static uint32_t activationTime = 0;

    if (sensorData.gacheporte.activer && !sensorData.gacheporte.active)
    {
        digitalWrite(GACHE_PORTE_PIN, HIGH);
        activationTime = time;
        sensorData.gacheporte.active = true;
    }

    sensorData.gacheporte.activer = false;

    if (sensorData.gacheporte.active && time - activationTime >= GACHE_PORTE_TIMEOUT_MS)
    {
        digitalWrite(GACHE_PORTE_PIN, LOW);
        sensorData.gacheporte.active = false;
    }
}