#include "SensorData.h"
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include "time.h"
#include <Preferences.h>

void initEdf();
void updateTimer();