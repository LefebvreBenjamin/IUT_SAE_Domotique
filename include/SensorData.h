#pragma once

#include "screen.h"
#include "Arduino.h"
#include "buttonAction.h"

struct SensorData
{
  float temperature;
  float pressure;
  float light;
  float gasVoltage;
  float altitude;

  struct AirQuality
  {
    bool good;
    bool moderate;
    bool bad;
  } airQuality;

  struct Portail
  {
    bool ouverture;
    bool fermeture;
  }portail;

  struct Alarme
  {
    bool activer;
    bool desactiver;
  }alarme;

  struct VoletRoulant
  {
    bool Monter;
    bool Stop;
    bool Descendre;
  }voletRoulant;
};

typedef struct Button
{
  int x;
  int y;
  int width;
  int height;
  void (*action)();
  int scene;
} Button;

struct ScreenElement
{
  Button button[10];
  int count;
};


extern ScreenElement screenElement;
extern SensorData sensorData;

void addButton(const Button &button);