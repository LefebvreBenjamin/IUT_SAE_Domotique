#include <Arduino.h>
#include "Zigbee.h"
#include "zigbeeLink.h"
#include "SensorData.h"

// Capteur temperature (endpoint 10)
ZigbeeTempSensor zbTemp(10);

void initZigbee() {
  // Nom vu dans Zigbee2MQTT
  zbTemp.setManufacturerAndModel("DIY", "StationC6");
  zbTemp.setMinMaxValue(0, 60);
  zbTemp.setTolerance(1);

  // Ajout au reseau
  Zigbee.addEndpoint(&zbTemp);

  // Demarrage
  if (!Zigbee.begin()) {
    Serial.println("Zigbee : erreur, redemarrage");
    ESP.restart();
  }
  Serial.println("Zigbee : demarre");
}



void updateZigbee() {
  static unsigned long dernierEnvoi = 0;

  // Etat pour l'icone Zigbee de l'ecran
  connectedDevices.zigbeeConnection = Zigbee.connected();

  // Pas connecte : on ne fait rien
  if (!Zigbee.connected()) return;

  // Envoi toutes les 10 s
  if (millis() - dernierEnvoi < 2000) return;
  dernierEnvoi = millis();

  // Temperature
  zbTemp.setTemperature(sensorData.temperature);
  zbTemp.report();
  Serial.println("Temp Send");
}