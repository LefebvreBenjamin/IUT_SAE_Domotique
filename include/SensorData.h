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
  float gasPpm;
  float altitude;

  struct AirQuality {
    bool good;
    bool moderate;
    bool bad;
  } airQuality;

  struct Portail
  {
    bool ouverture;
    bool fermeture;
  } portail;

  struct Alarme
  {
    bool activer;
    bool desactiver;
  } alarme;

  struct GachePorte
  {
    bool activer;
    bool active;
  } gacheporte;

  struct VoletRoulant
  {
    bool Monter;
    bool Stop;
    bool Descendre;
  } voletRoulant;
};

struct ConnectedDevices
{
  int zigbeeConnection;
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

#define MAX_BUTTONS 20

struct ScreenElement
{
  Button button[MAX_BUTTONS];
  int count;
};

struct TimeDate
{
  int seconde;
  int heure;
  int minute;
  int jour;
  int mois;
  int annee;
};

extern ScreenElement screenElement;
extern SensorData sensorData;
extern ConnectedDevices connectedDevices;
extern TimeDate timeDate;

void addButton(const Button &button);