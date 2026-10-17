/*
Récupère la couleur Tempo EDF (bleu / blanc / rouge) sur ESP32
URL : https://www.services-rte.com/cms/open_data/v1/tempoLight
Inspiré de : https://f1atb.fr/fr/routeur-photovoltaique-realisation-logicielle/

Utiliser Preferences.h (NVS) pour garder des variables après un reboot.
Ne pas utiliser EEPROM.h sur ESP32 (usure de la Flash).
*/
#include "time_et_tempo_EDF.h"

#define DEBUG // mettre en commentaire pour couper les messages série

#ifdef DEBUG
#define DPRINT(...) Serial.print(__VA_ARGS__)
#define DPRINTLN(...) Serial.println(__VA_ARGS__)
#define DPRINTF(...) Serial.printf(__VA_ARGS__)
#else
#define DPRINT(...)
#define DPRINTLN(...)
#define DPRINTF(...)
#endif

const char *ssid = "POCOF8Pro";
const char *password = "motdepasse";

// Serveur NTP et fuseau horaire de Paris
const char *ntpServer = "pool.ntp.org";
const char *timezone = "CET-1CEST-2,M3.5.0/02:00:00,M10.5.0/03:00:00";

// Récupère la valeur d'un champ dans un texte JSON
String StringJson(String nom, String Json)
{
    int p = Json.indexOf(nom + "\":");
    Json = Json.substring(p);
    p = Json.indexOf(":");
    Json = Json.substring(p + 1);
    p = Json.indexOf("\"");
    Json = Json.substring(p + 1);
    p = Json.indexOf("\"");
    Json = Json.substring(0, p);
    return Json;
}

void initEdf()
{
    Serial.begin(115200);
#ifdef DEBUG
    while (!Serial)
    {
        delay(100);
    }
#endif

    // Connexion WiFi
    DPRINT("Connexion a ");
    DPRINT(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        DPRINT(".");
    }
    DPRINTLN("");
    DPRINT("WiFi connecte, IP : ");
    DPRINTLN(WiFi.localIP());
    connectedDevices.networkConnection = 1;

    // Heure NTP (une seule fois suffit)
    configTzTime(timezone, ntpServer);
}

// Met à jour l'heure et la date
void updateTimer()
{
  
    if (WiFi.status() == WL_CONNECTED)   // 3 = connecté
    {
        connectedDevices.networkConnection = 1;
    }
    else if (WiFi.status() == WL_DISCONNECTED || WiFi.status() == WL_CONNECTION_LOST)   // 6 ou 5
    {
        connectedDevices.networkConnection = 0;
    }
    else
    {
        connectedDevices.networkConnection = 0;   // tout le reste = déconnecté
    }
    
    
    struct tm heureLocale;
    if (getLocalTime(&heureLocale, 10000))
    {
        timeDate.heure = heureLocale.tm_hour;
        timeDate.minute = heureLocale.tm_min;
        timeDate.seconde = heureLocale.tm_sec;
        timeDate.jour = heureLocale.tm_mday;
        timeDate.mois = heureLocale.tm_mon + 1;
        timeDate.annee = heureLocale.tm_year + 1900;
    }
    else
    {
        DPRINTLN("Echec synchro NTP");
    }
}