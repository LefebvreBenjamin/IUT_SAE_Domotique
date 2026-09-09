#include "BH1750_LightSensor.h"

BH1750 lightMeter;

void initBH1750()
{
    bool success = lightMeter.begin();
    if (!success)
    {
        Serial.println("BH1750 non détecté");
    }
    else
    {
        Serial.println("BH1750 OK");
    }
    Serial.println("BH1750 initialisé\n");
}

void updateBH1750Data()
{
    float lux = lightMeter.readLightLevel();
    sensorData.light = lux;
}