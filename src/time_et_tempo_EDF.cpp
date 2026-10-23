/*
Heure et date de la station.
L'heure vient du coordinateur Zigbee (Zigbee2MQTT) : voir zigbeeLink.cpp.
Plus de WiFi : la radio reste libre pour Zigbee.
*/
#include "time_et_tempo_EDF.h"

void initEdf()
{
    // Heure de Paris
    setenv("TZ", FUSEAU_PARIS, 1);
    tzset();
}

// Met a jour l'heure et la date
void updateTimer()
{
    // Une fois par seconde suffit
    static unsigned long derniereMaj = 0;
    if (millis() - derniereMaj < 1000) return;
    derniereMaj = millis();

    // Faux tant que l'heure Zigbee n'est pas recue
    struct tm heureLocale;
    if (getLocalTime(&heureLocale, 0))   // 0 = ne bloque pas
    {
        timeDate.heure = heureLocale.tm_hour;
        timeDate.minute = heureLocale.tm_min;
        timeDate.seconde = heureLocale.tm_sec;
        timeDate.jour = heureLocale.tm_mday;
        timeDate.mois = heureLocale.tm_mon + 1;
        timeDate.annee = heureLocale.tm_year + 1900;
    }
}
