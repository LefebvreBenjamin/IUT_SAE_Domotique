#include "mqtt.h"


#include "SensorData.h"
#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "mqtt.h"

namespace
{
    const char* mqttServeur = "192.168.1.54";
    const int mqttPort = 1883;
    const char* mqttUtilisateur = "lefebvre";
    const char* mqttMotDePasse = "121105";

    WiFiClient wifiClient;
    PubSubClient mqttClient(wifiClient);

    unsigned long derniereTentative = 0;
}

void initMQTT()
{
    //mqttClient.setServer(mqttServeur, mqttPort);
}

void updateMQTT()
{/*
    if (WiFi.status() != WL_CONNECTED)
        return;

    if (!mqttClient.connected())
    {
        if (millis() - derniereTentative < 5000)
            return;

        derniereTentative = millis();

        if (mqttClient.connect("esp32c6", mqttUtilisateur, mqttMotDePasse))
        {
            Serial.println("MQTT connecte !");
            publierMesure("temperature", 25.0);
            connectedDevices.zigbeeConnection = 1;
        }
        else
        {
            Serial.print("Echec MQTT, code : ");
            Serial.println(mqttClient.state());
            connectedDevices.zigbeeConnection = 0;
        }
    }
    else
    {
        mqttClient.loop();
        sendTrame();
    }*/
}


bool publierMesure(const char* nom, float valeur)
{
    if (!mqttClient.connected())
        return false;

    char sujet[100];
    char message[20];

    snprintf(sujet, sizeof(sujet), "esp32c6/mesures/%s", nom);
    snprintf(message, sizeof(message), "%.2f", valeur);

    return mqttClient.publish(sujet, message);
}


void sendTrame(){
    if(!mqttClient.connected())
        return;
    else{
        static unsigned long lastSendTime = 0;
        unsigned long currentTime = millis();
        if ( currentTime - lastSendTime >= 2000) {
            publierMesure("temperature", sensorData.temperature);
            publierMesure("pression", sensorData.pressure);
            publierMesure("luminosite", sensorData.light);
            lastSendTime = currentTime;
        } 
    }
}