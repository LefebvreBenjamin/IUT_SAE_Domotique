/*
Heure, date et couleurs Tempo EDF de la station.

Heure : serveur NTP (WiFi) ou coordinateur Zigbee (voir zigbeeLink.cpp).
Tempo : serveur RTE en WiFi, cases 0 a 13 = logigramme du prof.
URL tempo -> https://www.services-rte.com/cms/open_data/v1/tempoLight
Inspire de -> https://f1atb.fr/fr/routeur-photovoltaique-realisation-logicielle/

LED RGB : couleur du jour -> couleur de demain -> pause -> ...
(actualisation au demarrage + 6h00)

NVS : Preferences.h garde les couleurs apres un reboot.
Ne pas utiliser EEPROM.h sur ESP32 (usure de la Flash).
*/
#include "time_et_tempo_EDF.h"
#include "rgb.h"
#include <WiFi.h>
#include <WiFiMulti.h>
#include <WiFiClientSecure.h>
#include <Preferences.h>

// Durees du timer en us
#define T_5S    5000000ULL
#define T_15S   15000000ULL
#define T_5MIN  300000000ULL
#define T_10MIN 600000000ULL

#define DEBUG // mettre en commentaire pour couper les messages serie

#ifdef DEBUG
#define DPRINT(...) Serial.print(__VA_ARGS__)
#define DPRINTLN(...) Serial.println(__VA_ARGS__)
#define DPRINTF(...) Serial.printf(__VA_ARGS__)
#else
#define DPRINT(...)
#define DPRINTLN(...)
#define DPRINTF(...)
#endif

// Reseaux WiFi 
static const char *reseaux[][3] = {
    {"POCOF8Pro", "motdepasse"},          // partage du telephone
    {"Freebox-1889A5", "5bmm7q99c9qvdf66qr7hnx"},    // box de la maison
    {"SFR_4BFF", "2yer5a6yy2m4r3tk5e8d"},
};
static WiFiMulti wifiMulti;

// Serveur NTP
static const char *ntpServer = "pool.ntp.org";

// Serveur RTE
#define HOTE_RTE "www.services-rte.com"
#define URL_RTE  "/cms/open_data/v1/tempoLight"

hw_timer_t *timerReqTempo = NULL;    // timer de la requete Tempo
volatile bool flagTReqTempo = true;  // vrai = requete a faire (au demarrage aussi)

static int nbEssais = 0;             // essais pour avoir l'heure
static char dateJ[11];               // jour Tempo en cours "AAAA-MM-JJ"
static char dateJ1[11];              // jour Tempo suivant
static int couleurJ = TEMPO_INCONNU;
static int couleurJ1 = TEMPO_INCONNU;

// Recupere la valeur d'un champ dans un texte JSON
static String StringJson(String nom, String Json)
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

// Fonction d'interruption du timer
void IRAM_ATTR timer_isr_ReqTempo()
{
    flagTReqTempo = true;   // indicateur passage dans IT
}

// Texte RTE -> code couleur
static int codeCouleur(String texte)
{
    if (texte == "BLUE") return TEMPO_BLEU;
    if (texte == "WHITE") return TEMPO_BLANC;
    if (texte == "RED") return TEMPO_ROUGE;
    return TEMPO_INCONNU;
}

// Date decalee de n jours, ecrite en "AAAA-MM-JJ"
static void dateDecalee(struct tm t, int nJours, char *texte)
{
    t.tm_mday += nJours;
    t.tm_hour = 12;      // midi : pas de souci avec le changement d'heure
    t.tm_min = 0;
    t.tm_sec = 0;
    t.tm_isdst = -1;
    mktime(&t);          // corrige jour, mois et annee
    strftime(texte, 11, "%Y-%m-%d", &t);
}

// Lance le timer (duree en us)
static void reglerTimer(uint64_t duree)
{
    timerWrite(timerReqTempo, 0);
    timerAlarm(timerReqTempo, duree, false, 0);   // une seule fois
}

// Duree en us jusqu'a 6h01 (nJours = 0 : aujourd'hui, 1 : demain)
static uint64_t dureeJusqua6h(struct tm t, int nJours)
{
    t.tm_mday += nJours;
    t.tm_hour = 6;
    t.tm_min = 1;        // 1 min de marge
    t.tm_sec = 0;
    t.tm_isdst = -1;     // mktime gere l'heure d'ete / d'hiver
    time_t cible = mktime(&t);
    time_t maintenant = time(NULL);
    if (cible <= maintenant) return T_15S;
    return (uint64_t)(cible - maintenant) * 1000000ULL;
}

// Lit un texte en NVS ("" s'il n'existe pas, sans message d'erreur)
static String lireTexteNVS(Preferences &memo, const char *cle)
{
    if (!memo.isKey(cle)) return "";
    return memo.getString(cle, "");
}

// Lit une couleur gardee en NVS (2 cases : d0/c0 et d1/c1)
static int lireCouleurNVS(const char *date)
{
    Preferences memo;
    memo.begin("tempo", false);
    int couleur = TEMPO_INCONNU;
    if (lireTexteNVS(memo, "d0") == date) couleur = memo.getInt("c0", TEMPO_INCONNU);
    if (lireTexteNVS(memo, "d1") == date) couleur = memo.getInt("c1", TEMPO_INCONNU);
    memo.end();
    return couleur;
}

// Garde une couleur connue en NVS
static void sauverCouleurNVS(const char *date, int couleur)
{
    if (couleur == TEMPO_INCONNU) return;

    Preferences memo;
    memo.begin("tempo", false);
    String d0 = lireTexteNVS(memo, "d0");
    String d1 = lireTexteNVS(memo, "d1");

    // Deja gardee : on n'ecrit pas (use moins la Flash)
    if ((d0 == date && memo.getInt("c0", 0) == couleur) ||
        (d1 == date && memo.getInt("c1", 0) == couleur))
    {
        memo.end();
        return;
    }

    // Case a ecrire : meme date, sinon la plus ancienne
    // ("AAAA-MM-JJ" : ordre alphabetique = ordre des jours)
    bool case0;
    if (d0 == date) case0 = true;
    else if (d1 == date) case0 = false;
    else case0 = (d0 < d1);

    if (case0)
    {
        memo.putString("d0", date);
        memo.putInt("c0", couleur);
    }
    else
    {
        memo.putString("d1", date);
        memo.putInt("c1", couleur);
    }
    memo.end();
}

// Une couleur Tempo sur la LED RGB
static void allumerCouleur(int couleur)
{
    switch (couleur)
    {
    case TEMPO_BLEU:  setLed(0, 0, 255);     break;
    case TEMPO_BLANC: setLed(255, 255, 255); break;
    case TEMPO_ROUGE: setLed(255, 0, 0);     break;
    default:          setLed(128, 0, 128);   break;   // violet = inconnue
    }
}

// LED : jour -> eteinte -> demain -> pause (sans bloquer)
static void afficherLedTempo()
{
    static unsigned long debut = 0;
    static int phase = -1;
    const unsigned long duree[4] = {1000, 250, 1000, 1500};   // en ms

    if (phase >= 0 && millis() - debut < duree[phase]) return;
    phase = (phase + 1) % 4;
    debut = millis();

    if (phase == 0) allumerCouleur(tempoEdf.couleurJour);
    else if (phase == 2) allumerCouleur(tempoEdf.couleurDemain);
    else setLed(0, 0, 0);
}

void initEdf()
{
    // Reseaux connus
    for (auto &r : reseaux)
    {
        wifiMulti.addAP(r[0], r[1]);
    }

    // Connexion au plus fort (10 s max, puis on continue)
    DPRINTLN("Recherche des reseaux WiFi connus...");
    if (wifiMulti.run(10000) == WL_CONNECTED)
    {
        DPRINT("WiFi connecte a ");
        DPRINT(WiFi.SSID());
        DPRINT(", IP : ");
        DPRINTLN(WiFi.localIP());
    }
    else
    {
        DPRINTLN("Aucun reseau connu : nouvel essai a la prochaine demande Tempo");
    }

    // Heure de Paris + NTP
    configTzTime(FUSEAU_PARIS, ntpServer);

    // Timer a 1 MHz : 1 tick = 1 us
    timerReqTempo = timerBegin(1000000);
    timerAttachInterrupt(timerReqTempo, &timer_isr_ReqTempo);
}

// Met a jour l'heure et la date
void updateTimer()
{
    // Une fois par seconde suffit
    static unsigned long derniereMaj = 0;
    if (millis() - derniereMaj < 1000) return;
    derniereMaj = millis();

    // Faux tant que l'heure n'est pas recue
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

// Couleurs Tempo : logigramme du prof
void updateTempo()
{
    afficherLedTempo();

    if (!flagTReqTempo) return;

    struct tm t = {};
    WiFiClientSecure client;
    String reponse;
    int fin = 0;             // nb de '}' recus (2 = JSON complet)
    int etape = 0;
    bool termine = false;

    while (!termine)
    {
        switch (etape)
        {
        case 0:   // Date et heure (NTP ou Zigbee)
            nbEssais++;
            if (getLocalTime(&t, 0)) etape = 3;   // date correcte
            else if (nbEssais < 10) etape = 1;
            else etape = 2;
            break;

        case 1:   // Pas d'heure : nouvel essai dans 15 s
            flagTReqTempo = false;
            reglerTimer(T_15S);
            termine = true;
            break;

        case 2:   // 10 echecs : nouvel essai dans 5 min
            nbEssais = 0;
            flagTReqTempo = false;
            reglerTimer(T_5MIN);
            termine = true;
            break;

        case 3:   // Annee, mois, jour
            nbEssais = 0;
            couleurJ = TEMPO_INCONNU;
            couleurJ1 = TEMPO_INCONNU;
            etape = (t.tm_hour < 6) ? 4 : 5;
            break;

        case 4:   // Avant 6h : jour j-1 et jour j
            dateDecalee(t, -1, dateJ);
            dateDecalee(t, 0, dateJ1);
            etape = 6;
            break;

        case 5:   // Apres 6h : jour j et jour j+1
            dateDecalee(t, 0, dateJ);
            dateDecalee(t, 1, dateJ1);
            etape = 6;
            break;

        case 6:   // Connexion au serveur RTE
        {
            DPRINTF("Tempo : demande %s et %s\n", dateJ, dateJ1);
            // Pas de WiFi : on cherche un reseau connu (quelques secondes)
            if (WiFi.status() != WL_CONNECTED && wifiMulti.run(5000) != WL_CONNECTED)
            {
                DPRINTLN("Tempo : aucun reseau WiFi connu");
                etape = 7;
                break;
            }

            // Le DNS du reseau ne repond pas : on passe par des DNS publics
            IPAddress ipRte;
            if (WiFi.hostByName(HOTE_RTE, ipRte) != 1)   // 1 = trouve
            {
                DPRINT("Tempo : DNS ");
                DPRINT(WiFi.dnsIP(0));
                DPRINTLN(" en echec, essai avec 1.1.1.1 et 8.8.8.8");
                WiFi.setDNS(IPAddress(1, 1, 1, 1), IPAddress(8, 8, 8, 8));
            }

            client.setInsecure();   // pas de verif du certificat
            etape = client.connect(HOTE_RTE, 443, 5000) ? 8 : 7;
            break;
        }

        case 8:   // Requete GET, reponse attendue 5 s max
        {
            // HTTP/1.0 : reponse en un seul bloc
            client.print("GET " URL_RTE " HTTP/1.0\r\n"
                         "Host: " HOTE_RTE "\r\n"
                         "Connection: close\r\n\r\n");
            unsigned long debut = millis();
            while (client.available() == 0 && millis() - debut < 5000) delay(10);
            etape = (client.available() > 0) ? 9 : 7;
            break;
        }

        case 9:   // Lecture des donnees RTE
        {
            unsigned long debut = millis();
            while ((client.connected() || client.available()) && millis() - debut < 5000)
            {
                while (client.available()) reponse += (char)client.read();
                delay(1);
            }
            client.stop();

            fin = 0;
            for (unsigned int i = 0; i < reponse.length(); i++)
            {
                if (reponse[i] == '}') fin++;
            }
            etape = (fin == 2) ? 10 : 7;
            break;
        }

        case 10:  // Couleurs J et J+1
            couleurJ = codeCouleur(StringJson(dateJ, reponse));
            couleurJ1 = codeCouleur(StringJson(dateJ1, reponse));
            DPRINTF("Tempo RTE : %d et %d\n", couleurJ, couleurJ1);
            if (couleurJ != TEMPO_INCONNU && couleurJ1 != TEMPO_INCONNU) etape = 11;
            else etape = 7;
            break;

        case 11:  // Sauvegarde NVS
            sauverCouleurNVS(dateJ, couleurJ);
            sauverCouleurNVS(dateJ1, couleurJ1);
            tempoEdf.couleurJour = couleurJ;
            tempoEdf.couleurDemain = couleurJ1;
            etape = (t.tm_hour < 6) ? 12 : 13;
            break;

        case 12:  // Prochaine requete : 6h ce matin
            reglerTimer(dureeJusqua6h(t, 0));
            flagTReqTempo = false;
            termine = true;
            break;

        case 13:  // Prochaine requete : 6h demain matin
            reglerTimer(dureeJusqua6h(t, 1));
            flagTReqTempo = false;
            termine = true;
            break;

        case 7:   // Echec ou couleur manquante : on prend la NVS
            if (couleurJ == TEMPO_INCONNU) couleurJ = lireCouleurNVS(dateJ);
            if (couleurJ1 == TEMPO_INCONNU) couleurJ1 = lireCouleurNVS(dateJ1);
            sauverCouleurNVS(dateJ, couleurJ);
            sauverCouleurNVS(dateJ1, couleurJ1);
            tempoEdf.couleurJour = couleurJ;
            tempoEdf.couleurDemain = couleurJ1;
            DPRINTF("Tempo (NVS) : %d et %d, nouvel essai dans 5 min\n", couleurJ, couleurJ1);
            reglerTimer(T_5MIN);
            flagTReqTempo = false;
            termine = true;
            break;
        }
    }
    client.stop();
}
