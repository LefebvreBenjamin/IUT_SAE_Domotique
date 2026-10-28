#include <Arduino.h>
#include "Zigbee.h"
#include "zigbeeLink.h"
#include "SensorData.h"
#include "time_et_tempo_EDF.h"
#include <sys/time.h>
#include <Preferences.h>
#include <esp_coexist.h>

// +1 a chaque ajout ou retrait de capteur Zigbee
// (l'ESP efface alors sa memoire Zigbee une fois)
#define ZB_VERSION_MODELE 6

// Capteur temperature (endpoint 10)
ZigbeeTempSensor zbTemp(10);

// Capteur pression (endpoint 11)
ZigbeePressureSensor zbPressure(11);

// Capteur lumiere (endpoint 12)
ZigbeeIlluminanceSensor zbLux(12);

// Gaz MQ-9 en ppm (endpoint 13)
ZigbeeAnalog zbGaz(13);

// Couleurs Tempo : aujourd'hui (endpoint 14) et demain (endpoint 15)
// 0 = inconnue, 1 = bleu, 2 = blanc, 3 = rouge
ZigbeeMultistate zbTempoJour(14);
ZigbeeMultistate zbTempoDemain(15);

// Capteur temperature CPU (endpoint 16)
ZigbeeTempSensor zbCpuTemp(16);

// Lux -> format Zigbee (10000 x log10(lux) + 1)
static uint16_t luxVersZigbee(float lux) {
  if (lux < 1) return 0;
  float v = 10000.0f * log10f(lux) + 1.0f;
  if (v > 65534.0f) v = 65534.0f;
  return (uint16_t)v;
}

void initZigbee() {
  // Nom vu dans Zigbee2MQTT
  zbTemp.setManufacturerAndModel("DIY", "StationC6");
  zbTemp.setMinMaxValue(0, 60);
  zbTemp.setTolerance(1);

  // Pour demander l'heure au coordinateur
  zbTemp.addTimeCluster();

  // Ajout au reseau
  Zigbee.addEndpoint(&zbTemp);

  // Pression en hPa
  zbPressure.setManufacturerAndModel("DIY", "StationC6");
  zbPressure.setMinMaxValue(300, 1100);
  zbPressure.setTolerance(1);

  // Ajout au reseau
  Zigbee.addEndpoint(&zbPressure);

  // Lumiere
  zbLux.setManufacturerAndModel("DIY", "StationC6");
  zbLux.setMinMaxValue(0, 50000);
  zbLux.setTolerance(1);
  Zigbee.addEndpoint(&zbLux);

  // Gaz MQ-9 (valeur libre en ppm)
  zbGaz.setManufacturerAndModel("DIY", "StationC6");
  zbGaz.addAnalogInput();
  zbGaz.setAnalogInputDescription("CO (ppm)");
  zbGaz.setAnalogInputResolution(1);
  Zigbee.addEndpoint(&zbGaz);

  // Tempo du jour (4 etats)
  zbTempoJour.setManufacturerAndModel("DIY", "StationC6");
  zbTempoJour.addMultistateInput();
  zbTempoJour.setMultistateInputDescription("Tempo jour");
  zbTempoJour.setMultistateInputStates(4);
  Zigbee.addEndpoint(&zbTempoJour);

  // Tempo de demain (4 etats)
  zbTempoDemain.setManufacturerAndModel("DIY", "StationC6");
  zbTempoDemain.addMultistateInput();
  zbTempoDemain.setMultistateInputDescription("Tempo demain");
  zbTempoDemain.setMultistateInputStates(4);
  Zigbee.addEndpoint(&zbTempoDemain);

  // Temp CPU 
  zbCpuTemp.setManufacturerAndModel("DIY", "StationC6");
  zbCpuTemp.setMinMaxValue(-10, 80);
  zbCpuTemp.setTolerance(1);
  Zigbee.addEndpoint(&zbCpuTemp);

  // Capteurs changes ? On efface la memoire Zigbee
  // (les cartes RFID ne sont pas touchees)
  Preferences memo;
  memo.begin("zigbee", false);
  bool effacer = memo.getInt("modele", 0) != ZB_VERSION_MODELE;
  if (effacer) {
    Serial.println("Zigbee : capteurs changes, memoire Zigbee effacee");
  }

  Zigbee.setTimeout(10000);

  // Demarrage
  if (Zigbee.begin(ZIGBEE_END_DEVICE, effacer)) {
    Serial.println("Zigbee : connecte");
  } else {
    // Raspberry hors de portee : la station marche sans
    Serial.println("Zigbee : reseau absent, recherche en fond");
  }
  memo.putInt("modele", ZB_VERSION_MODELE);
  memo.end();

  // WiFi et Zigbee partagent la meme radio :
  // sans cette ligne, le Zigbee la garde et le WiFi ne passe plus
#if CONFIG_ESP_COEX_SW_COEXIST_ENABLE && CONFIG_SOC_IEEE802154_SUPPORTED
  if (esp_coex_wifi_i154_enable() == ESP_OK) {
    Serial.println("Zigbee : partage radio WiFi active");
  }
#endif
}



// Demande l'heure a Zigbee2MQTT et regle l'horloge
static bool syncHeureZigbee() {
  // Calcul en UTC
  setenv("TZ", "UTC0", 1);
  tzset();
  struct tm t = zbTemp.getTime();
  time_t utc = mktime(&t);

  // Retour a l'heure de Paris
  setenv("TZ", FUSEAU_PARIS, 1);
  tzset();

  // Avant 2024 = pas de reponse
  if (t.tm_year < 124) return false;

  struct timeval tv = { utc, 0 };
  settimeofday(&tv, NULL);
  return true;
}

void updateZigbee() {
  static unsigned long dernierEnvoi = 0;

  // Etat pour l'icone Zigbee de l'ecran
  connectedDevices.zigbeeConnection = Zigbee.connected();

  // Pas connecte : on ne fait rien
  if (!Zigbee.connected()) return;

  // Heure : toutes les 60 s tant que pas recue, puis toutes les heures
  static unsigned long derniereSync = 0;
  static bool heureOk = false;
  unsigned long attente = heureOk ? 3600000UL : 60000UL;
  if (derniereSync == 0 || millis() - derniereSync >= attente) {
    derniereSync = millis();
    heureOk = syncHeureZigbee();
    Serial.println(heureOk ? "Zigbee : heure OK" : "Zigbee : pas d'heure");
  }

  // Envoi toutes les 10 s
  if (millis() - dernierEnvoi < 10000) return;
  dernierEnvoi = millis();

  // Temperature (si mesure valide)
  if (!isnan(sensorData.temperature)) {
    zbTemp.setTemperature(sensorData.temperature);
    zbTemp.report();
  }

  // Temperature CPU (si mesure valide)
  if (!isnan(sensorData.esp32Data.cpuTemp)) {
    zbCpuTemp.setTemperature(sensorData.esp32Data.cpuTemp);
    zbCpuTemp.report();
  }

  // Pression en hPa (si mesure valide)
  if (!isnan(sensorData.pressure)) {
    zbPressure.setPressure((int16_t)sensorData.pressure);
    zbPressure.report();
  }

  // Lumiere (si mesure valide)
  if (!isnan(sensorData.light)) {
    zbLux.setIlluminance(luxVersZigbee(sensorData.light));
    zbLux.report();
  }

  // Gaz en ppm (si mesure valide)
  if (!isnan(sensorData.gasPpm)) {
    zbGaz.setAnalogInput(sensorData.gasPpm);
    zbGaz.reportAnalogInput();
  }

  // Couleurs Tempo
  zbTempoJour.setMultistateInput(tempoEdf.couleurJour);
  zbTempoJour.reportMultistateInput();
  zbTempoDemain.setMultistateInput(tempoEdf.couleurDemain);
  zbTempoDemain.reportMultistateInput();

  Serial.printf("Zigbee : envoi (CPU %.1f C)\n", sensorData.esp32Data.cpuTemp);
}
