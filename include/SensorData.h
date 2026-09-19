#pragma once

struct SensorData {
  float temperature;
  float pressure;
  float light;
  float gasVoltage;
  float altitude;

  bool porteOpen;
  bool alarmActive;
  bool voletOpen;

  struct AirQuality{
    bool good;
    bool moderate;
    bool bad;
  } airQuality;

};

extern SensorData sensorData;