/*
Programme Arduino pour ESP32 récupération couleur Tempo EDF (Bleu/blanc/rouge)
URL tempo -> https://www.services-rte.com/cms/open_data/v1/tempoLight
(https://www.services-rte.com/cms/open_data/v1/tempo?season=2024-2025)
Inspiré de la source -> https://f1atb.fr/fr/routeur-photovoltaique-realisation-logicielle/
Solar_Router_V15_09.zip et supérieur -> Tempo_RTE.ino
RGB_BUILTIN 8 -> Couleur du jour -> Couleur du lendemain -> Pause ->
^ |
|_____________________________________________________|
(actualisation au démarrage + 6h00)
NVS (Non Volatile Storage) partition de la mémoire Flash permettant de stocker des variables non effacées en
cas de reboot. Attention, ne surtout pas utiliser EEPROM.h sur ESP car vous allez écrire toujours à la même
adresse Flash (100 000 cycles d'écritures supportés) et l'ESP finira par lâcher. Utiliser Preferences.h
(librairie native sur ESP32) qui réalloue une adresse mémoire différente à chaque écriture dans la partition
NVS disponible.
*/
#include "time_et_tempo_EDF.h"


//#define TEMPO_LED_PIN 2
hw_timer_t *timer = NULL;     // Instanciation Objet de class hw_timer_t
volatile bool flagT0 = false; // var global commune volatile car écrite dans 2 fcts

#define T_5S 5000000
#define T_15S 15000000
#define T_5MIN 300000000
#define T_10MIN 600000000
#define DEBUG // If you comment this line, the DPRINT & DPRINTLN & DPRINTF lines are defined as blank.
// lot of bytes save in Flash App sketch memory
#ifdef DEBUG                                      // Macros are usually in all capital letters.
#define DPRINT(...) Serial.print(__VA_ARGS__)     // DPRINT is a macro, debug print
#define DPRINTLN(...) Serial.println(__VA_ARGS__) // DPRINTLN is a macro, debug print with new line
#define DPRINTF(...) Serial.printf(__VA_ARGS__)   // DPRINTF is a macro, debug printf
#else
#define DPRINT(...)   // now defines a blank line
#define DPRINTLN(...) // now defines a blank line
#define DPRINTF(...)  // now defines a blank line
#endif
hw_timer_t *timerReqTempo = NULL;    // Instanciation Objet de class hw_timer_t
volatile bool flagTReqTempo = true;  // var global commune volatile car écrite dans 2 fcts
const char *ssid = "POCOF8Pro";      // Change this to your WiFi SSID
const char *password = "motdepasse"; // Change this to your WiFi password

// Server NTP and Config timezone Paris
const char *ntpServer = "pool.ntp.org";                                // "pool.ntp.org"
const char *timezone = "CET-1CEST-2,M3.5.0/02:00:00,M10.5.0/03:00:00"; // Paris CET-1CEST2,M3.5.0/02:00:00,M10.5.0/03:00:00
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
void IRAM_ATTR timer_isr()
{                                // Fonction d’interruption sur timer0
    static bool toggle0 = false; // variable représentant l’état LED
    flagT0 = true;               // indicateur passage dans IT
    //digitalWrite(TEMPO_LED_PIN, toggle0);
    toggle0 = !toggle0;
} // inversion état LED

void initEdf()
{
    //pinMode(TEMPO_LED_PIN, OUTPUT);
    Serial.begin(115200);
    uint32_t freqTB_clk = 1000000; // freq timer
    uint64_t alarmPeriod = 100000; // période IT en microseconde
    /********************** Init Timer0 ************************/
    timer = timerBegin(freqTB_clk);
    timerAttachInterrupt(timer, &timer_isr);
    timerAlarm(timer, alarmPeriod, true, 0);
#ifdef DEBUG
    // ********************* Init Serial *********************
    Serial.begin(115200);
    while (!Serial)
    {
        delay(100);
    }
#endif
    // ********************* WiFi Connect *********************
    DPRINT("Connecting to ");
    DPRINT(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        DPRINT(".");
    }
    DPRINTLN("");
    DPRINT("WiFi connected ");
    DPRINT("IP address: ");
    DPRINTLN(WiFi.localIP());
}

void updateTimer()
{
    if (flagT0)
    {
        flagT0 = false; // acquittement traitement IT
    }

    configTzTime(timezone, ntpServer);

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
        Serial.println("Echec de synchronisation NTP");
    }
}