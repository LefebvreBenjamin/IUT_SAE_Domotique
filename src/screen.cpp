#include "screen.h"
#include "cartesRfid.h"
#include "rgb.h"
#include "time_et_tempo_EDF.h"

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

static const uint16_t DK_BG = 0xFFFF;


static EcranId g_screen = ECRAN_DASHBOARD;
static bool g_dirty = true;
static uint32_t g_lastDrawMs = 0;
static uint32_t g_lastDrawVarMs = 0;
static uint32_t g_lastCartesVersion = 0xFFFFFFFF;
static bool g_tempoForce = true;   // vrai = redessiner la zone Tempo

// Barre Tempo du dashboard : 6h -> 22h -> 6h
#define TEMPO_X  15    // x de 6h
#define TEMPO_L  288   // 24 h = 288 px (12 px par heure)
#define TEMPO_HP 192   // 16 h pleines
#define TEMPO_Y  209   // haut de la barre
#define TEMPO_H  7     // hauteur de la barre

void drawCurrentScreen();
static void drawDashboardVariable();
static void drawTempoDashboard();
static void drawGestionCartesVariable();
void handleUiActions();

static void drawCenteredText(const char *text, int16_t x, int16_t y)
{
  int16_t x1;
  int16_t y1;
  uint16_t width; 
  uint16_t height;
  tft.getTextBounds(text, x, y, &x1, &y1, &width, &height);
  tft.setCursor(x - (width / 2), y - (height / 2));
  tft.print(text);
}

int x_vmc = 162;
int y_vmc = 58;
int vmcPercent = 50;
int eclairagePercent = 50;

void moveSlider(int x, int y)
{
  int *percent;
  int trackY;

  if (y < 100)
  {
    percent = &vmcPercent;
    trackY = 78;
  }
  else
  {
    percent = &eclairagePercent;
    trackY = 142;
  }

  *percent = constrain(map(x, 172, 262, 0, 100), 0, 100);

  tft.fillRoundRect(169, trackY + 1, 96, 8, 4, DK_BG);
  tft.fillRect(269, trackY - 3, 36, 16, DK_BG);
  tft.setTextColor(NOIR);
  tft.setTextSize(1);
  tft.setCursor(272, trackY);
  tft.printf("%d%%", *percent);
  tft.drawRoundRect(168, trackY, 98, 10, 5, NOIR);
  tft.fillCircle(map(*percent, 0, 100, 172, 262), trackY + 5, 4, NOIR);
  if (percent == &eclairagePercent) setLuminosite(*percent);   // Slider éclairage seulement
}




void initScreen()
{
  Serial.println("Init screen");

  // Dashboard
  addButton({5, 13, 63, 30, actionGoHome, (int)ECRAN_DASHBOARD});
  
  // Accueil
  addButton({22, 128, 80, 28, actionGoDashboard, (int)ECRAN_ACCUEIL});
  addButton({214, 128, 80, 28, actionGoControlPanel, (int)ECRAN_ACCUEIL});
  addButton({120, 128, 80, 28, actionGoGestionCartes, (int)ECRAN_ACCUEIL});

  // Panneau de controle
  addButton({12, 16, 87, 31, actionGoHome, (int)ECRAN_CONTROLE}); // Retour accueil

  addButton({218, 207, 60, 17, actionVoletDescendre, (int)ECRAN_CONTROLE}); // Volet descendre
  addButton({129, 207, 60, 17, actionVoletStop, (int)ECRAN_CONTROLE}); // Volet stop
  addButton({35, 207, 60, 17, actionVoletMonter, (int)ECRAN_CONTROLE}); // Volet monter

  addButton({17, 156, 63, 21, actionActiverAlarme, (int)ECRAN_CONTROLE}); // Alarme activer
  addButton({87, 156, 63, 21, actionDesactiverAlarme, (int)ECRAN_CONTROLE}); // Alarme desactiver

  addButton({17, 93, 133, 21, actionActiverGachePorte, (int)ECRAN_CONTROLE}); // Gache porte activer

  // Cartes RFID
  addButton({12, 16, 87, 31, actionGoHome, (int)ECRAN_CARTES});// Retour accueil
  addButton({205, 90, 100, 30, actionAjouterCarte, (int)ECRAN_CARTES});// Ajouter
  addButton({205, 130, 100, 30, actionSupprimerCarte, (int)ECRAN_CARTES});// Supprimer
  addButton({205, 170, 100, 30, actionAnnulerModeRfid, (int)ECRAN_CARTES});// Annuler





  tft.init(240, 320);
  tft.setRotation(1);
  tft.invertDisplay(false); // Rétablit les vraies couleurs (fond blanc)
  tft.fillScreen(DK_BG);

  Serial.println("Screen Initalized");
}

void updateScreen()
{
  //handleUiActions();
  if (g_dirty || (millis() - g_lastDrawMs) > 15000)
  {
    drawCurrentScreen();
    g_dirty = false;
    g_lastDrawMs = millis();
    g_lastCartesVersion = 0xFFFFFFFF; // force le redessin des zones dynamiques
  }
  if (g_screen == ECRAN_CARTES && getVersionCartesRfid() != g_lastCartesVersion)
  {
    drawGestionCartesVariable();
    g_lastCartesVersion = getVersionCartesRfid();
  }
  if (g_screen == ECRAN_DASHBOARD && (millis() - g_lastDrawVarMs) > 100)
  {
    drawDashboardVariable();
    g_lastDrawVarMs = millis();
  }
  delay(5);
}


void drawHomeScreen()
{
  tft.fillScreen(DK_BG);

  tft.drawRoundRect(72, 20, 177, 42, 4, NOIR);
  tft.setTextColor(NOIR);
  tft.setTextSize(2); // Taille ajustée pour rentrer dans le cadre 177px
  tft.setCursor(79, 32);
  tft.print("HOME ASSISTANT");

  tft.fillRoundRect(22, 128, 80, 28, 6, BLANC);
  tft.drawRoundRect(22, 128, 80, 28, 6, NOIR);
  tft.setTextSize(1);
  drawCenteredText("DASHBOARD", 62, 142);

  tft.fillRoundRect(120, 128, 80, 28, 6, BLANC);
  tft.drawRoundRect(120, 128, 80, 28, 6, NOIR);
  drawCenteredText("CARTES RFID", 160, 142);

  // Bouton panneau de controle
  tft.fillRoundRect(214, 128, 80, 28, 6, BLANC);
  tft.drawRoundRect(214, 128, 80, 28, 6, NOIR);
  tft.setCursor(215, 138);
  tft.print("CONTROL PANEL");

  // Heure et date
  tft.setCursor(110, 83);
  tft.printf("%d:%d %d/%d/%d", timeDate.heure, timeDate.minute, timeDate.jour, timeDate.mois, timeDate.annee);

  tft.drawRoundRect(107, 73, 107, 30, 4, BLANC);
}


void drawDashboardScreen()
{
  tft.fillScreen(DK_BG);

  tft.drawRoundRect(5, 53, 310, 174, 4, 0x10C4);
  tft.setTextColor(NOIR);
  tft.setTextSize(1);
  tft.setCursor(100, 22);
  tft.print("DASHBOARD");

  tft.drawRoundRect(86, 13, 109, 30, 4, 0x10C4);

  tft.setCursor(19, 98);

  tft.setCursor(125, 120);
  tft.print("Qualite");

  tft.setCursor(231, 123);
  tft.print("Alarme");

  tft.setCursor(231, 150);
  tft.print("Volet");

  tft.setCursor(19, 121);

  tft.setCursor(19, 145);

  tft.fillRoundRect(5, 13, 63, 30, 6, BLANC);
  tft.drawRoundRect(5, 13, 63, 30, 6, NOIR);
  drawCenteredText("HOME", 43, 28);

  // Cadre statuts
  tft.setCursor(237, 68);
  tft.print("Statuts");

  tft.setCursor(212, 28);
  tft.printf("%d:%d %d/%d/%d", timeDate.heure, timeDate.minute, timeDate.jour, timeDate.mois, timeDate.annee);

  tft.drawRoundRect(206, 8, 111, 40, 4, 0x10C4);
  tft.drawRoundRect(215, 64, 94, 117, 4, 0x10C4);
  tft.setCursor(233, 96);
  tft.print("Porte");

  tft.drawRoundRect(11, 64, 92, 117, 4, 0x10C4);
  tft.setCursor(19, 68);
  tft.print("Environnement");

  tft.drawRoundRect(114, 64, 92, 117, 4, 0x10C4);
  tft.setCursor(148, 68);
  tft.print("Air");

  g_tempoForce = true;   // ecran efface : on redessine le Tempo
}

static void drawDashboardVariable()
{

  tft.fillRect(210, 12, 100, 12, DK_BG);   // efface la zone
  tft.setTextSize(1);
  tft.setCursor(212, 14);

  if(connectedDevices.zigbeeConnection){
    tft.setTextColor(ST77XX_GREEN);
    tft.print("Connected");
  }

  else
  {
     tft.setTextColor(ST77XX_RED);
     tft.print("Disconnected");
  }

  tft.fillRect(19, 98, 65, 56, DK_BG);
  tft.setTextColor(NOIR);
  tft.setCursor(19, 98);
  tft.printf("%.2f Celsius", sensorData.temperature);

  tft.setCursor(19, 121);
  tft.printf("%.2f lux", sensorData.light);

  tft.setCursor(19, 145);
  tft.printf("%.2f hPa", sensorData.pressure);

  tft.fillRect(210, 26, 100, 12, DK_BG);   // efface la zone
  tft.setCursor(212, 28);
  tft.printf("%d:%02d %d/%d/%d", timeDate.heure, timeDate.minute, timeDate.jour, timeDate.mois, timeDate.annee);

  // ppm sous la qualite de l'air
  tft.fillRect(119, 140, 82, 16, DK_BG);
  tft.setCursor(125, 145);
  tft.printf("%.2f ppm", sensorData.gasPpm);

  // Statuts
  tft.fillRect(275, 83, 20, 50, DK_BG);
  if (sensorData.gacheporte.active)
  {
    tft.fillCircle(285, 100, 6, VERT); // Porte (gache)
  }
  else
  {
    tft.fillCircle(285, 100, 6, ROUGE); // Porte (gache)
  }

  if (sensorData.alarme.activer)
  {
    tft.fillCircle(285, 123, 6, VERT); // Alarme
  }
  else
  {
    tft.fillCircle(285, 123, 6, ROUGE); // Alarme
  }

  if (sensorData.portail.ouverture)
  {
    tft.fillCircle(285, 150, 6, VERT); // Portail
  }
  else
  {
    tft.fillCircle(285, 150, 6, ROUGE); // Portail
  }

  tft.setCursor(233, 96);

  // Qualite de l'air
  tft.fillRect(165, 78, 20, 50, DK_BG);
  if (sensorData.airQuality.good)
  {
    tft.fillCircle(180, 123, 6, VERT); // Air
  }
  else if (sensorData.airQuality.moderate)
  {
    tft.fillCircle(180, 123, 6, JAUNE); // Air
  }
  else if (sensorData.airQuality.bad)
  {
    tft.fillCircle(180, 123, 6, ROUGE); // Air
  }

  drawTempoDashboard();
}

// Couleur heures pleines d'un jour Tempo
static uint16_t couleurTempoHP(int couleur)
{
  switch (couleur)
  {
  case TEMPO_BLEU:  return 0x1B3C;   // bleu
  case TEMPO_BLANC: return BLANC;
  case TEMPO_ROUGE: return ROUGE;
  default:          return 0x8410;   // gris = inconnue
  }
}

// Couleur heures creuses (plus claire)
static uint16_t couleurTempoHC(int couleur)
{
  switch (couleur)
  {
  case TEMPO_BLEU:  return 0x95FF;   // bleu clair
  case TEMPO_BLANC: return 0xCE59;   // gris clair
  case TEMPO_ROUGE: return 0xFD14;   // rouge clair
  default:          return 0xC618;   // gris clair
  }
}

static const char *nomTempo(int couleur)
{
  switch (couleur)
  {
  case TEMPO_BLEU:  return "BLEU";
  case TEMPO_BLANC: return "BLANC";
  case TEMPO_ROUGE: return "ROUGE";
  default:          return "?";
  }
}

// Petit carre de couleur avec un contour
static void carreTempo(int x, int y, int couleur)
{
  tft.fillRect(x, y, 8, 8, couleurTempoHP(couleur));
  tft.drawRect(x, y, 8, 8, NOIR);
}

// Zone Tempo en bas du dashboard
// Redessinee seulement si la minute ou les couleurs changent
static void drawTempoDashboard()
{
  static int derniereMinute = -1;
  static int dernierJour = -1;
  static int dernierDemain = -1;

  if (!g_tempoForce && timeDate.minute == derniereMinute &&
      tempoEdf.couleurJour == dernierJour && tempoEdf.couleurDemain == dernierDemain)
    return;

  g_tempoForce = false;
  derniereMinute = timeDate.minute;
  dernierJour = tempoEdf.couleurJour;
  dernierDemain = tempoEdf.couleurDemain;

  bool heureOk = timeDate.annee >= 2024;              // heure deja recue ?
  bool hp = timeDate.heure >= 6 && timeDate.heure < 22;

  tft.fillRect(8, 185, 304, 37, DK_BG);   // efface la zone
  tft.setTextColor(NOIR);
  tft.setTextSize(1);

  // Ligne du haut : aujourd'hui, demain, HP/HC
  tft.setCursor(15, 188);
  tft.print("Tempo");
  carreTempo(60, 188, tempoEdf.couleurJour);
  tft.setCursor(72, 188);
  tft.printf("Auj. %s", nomTempo(tempoEdf.couleurJour));
  carreTempo(150, 188, tempoEdf.couleurDemain);
  tft.setCursor(162, 188);
  tft.printf("Dem. %s", nomTempo(tempoEdf.couleurDemain));
  if (heureOk)
  {
    tft.setCursor(279, 188);
    tft.print(hp ? "En HP" : "En HC");
  }

  // Heures au-dessus des traits
  tft.setCursor(TEMPO_X, 198);
  tft.print("6h");
  tft.setCursor(TEMPO_X + TEMPO_HP - 9, 198);
  tft.print("22h");
  tft.setCursor(TEMPO_X + TEMPO_L - 12, 198);
  tft.print("6h");

  // Barre : heures pleines puis heures creuses
  tft.fillRect(TEMPO_X, TEMPO_Y, TEMPO_HP, TEMPO_H, couleurTempoHP(tempoEdf.couleurJour));
  tft.fillRect(TEMPO_X + TEMPO_HP, TEMPO_Y, TEMPO_L - TEMPO_HP, TEMPO_H, couleurTempoHC(tempoEdf.couleurJour));
  tft.drawRect(TEMPO_X, TEMPO_Y, TEMPO_L + 1, TEMPO_H, NOIR);

  // Traits a 6h, 22h et 6h
  tft.drawFastVLine(TEMPO_X, TEMPO_Y - 2, TEMPO_H + 4, NOIR);
  tft.drawFastVLine(TEMPO_X + TEMPO_HP, TEMPO_Y - 2, TEMPO_H + 4, NOIR);
  tft.drawFastVLine(TEMPO_X + TEMPO_L, TEMPO_Y - 2, TEMPO_H + 4, NOIR);

  // Curseur de l'heure actuelle
  if (heureOk)
  {
    int m = (timeDate.heure * 60 + timeDate.minute - 360 + 1440) % 1440;   // minutes depuis 6h
    int x = TEMPO_X + m / 5;                                                // 1440 min -> 288 px
    tft.fillTriangle(x - 3, TEMPO_Y - 6, x + 3, TEMPO_Y - 6, x, TEMPO_Y - 1, NOIR);
    tft.fillRect(x - 1, TEMPO_Y - 1, 3, TEMPO_H + 4, NOIR);
  }
}

void drawRFIDCheck()
{
  tft.fillScreen(DK_BG);

  tft.setTextColor(NOIR);
  tft.setTextSize(2);
  tft.setCursor(106, 101);
  tft.print("RFID CHECK.....");
}

void drawControlPannel()
{
  tft.fillScreen(DK_BG);

  tft.setTextColor(NOIR);
  tft.setTextSize(1);
  tft.setCursor(118, 25);
  tft.print("CONTROLE PANEL");

  tft.drawRoundRect(112, 16, 97, 31, 4, NOIR);

  // === GACHE PORTE ===
  tft.drawRoundRect(12, 58, 144, 60, 4, NOIR);
  tft.setCursor(55, 62);
  tft.print("Gache Porte");

  tft.fillRoundRect(17, 93, 133, 21, 6, BLANC);
  tft.drawRoundRect(17, 93, 133, 21, 6, NOIR);
  drawCenteredText("Activer", 83, 103);

  // === ALARME ===
  tft.drawRoundRect(12, 122, 144, 60, 4, NOIR);
  tft.setCursor(45, 126);
  tft.print("Alarme");

  tft.fillRoundRect(87, 156, 63, 21, 6, BLANC);
  tft.drawRoundRect(87, 156, 63, 21, 6, NOIR);
  drawCenteredText("Désactiver", 118, 166);

  tft.fillRoundRect(17, 156, 63, 21, 6, BLANC);
  tft.drawRoundRect(17, 156, 63, 21, 6, NOIR);
  drawCenteredText("Activer", 48, 166);

  // === VMC ===

  tft.drawRoundRect(x_vmc, y_vmc, 144, 60, 4, NOIR);
  tft.setCursor(222, 62);
  tft.print("VMC");

  tft.setCursor(272, 78);
  tft.printf("%d%%", vmcPercent);
  tft.drawRoundRect(168, 78, 98, 10, 5, NOIR);
  tft.fillCircle(map(vmcPercent, 0, 100, 172, 262), 83, 4, NOIR);

  // === ECLAIRAGE ===
  tft.drawRoundRect(162, 122, 144, 60, 4, NOIR);
  tft.setCursor(206, 126);
  tft.print("Eclairage");

  tft.setCursor(272, 142);
  tft.printf("%d%%", eclairagePercent);
  tft.drawRoundRect(168, 142, 98, 10, 5, NOIR);
  tft.fillCircle(map(eclairagePercent, 0, 100, 172, 262), 147, 4, NOIR);
  

  // === VOLET ROULANT ===
  tft.drawRoundRect(13, 187, 292, 45, 4, NOIR);
  tft.setCursor(121, 192);
  tft.print("Volet Roulant");

  tft.fillRoundRect(35, 207, 60, 17, 6, BLANC);
  tft.drawRoundRect(35, 207, 60, 17, 6, NOIR);
  drawCenteredText("Monte", 65, 215);

  tft.fillRoundRect(129, 207, 60, 17, 6, BLANC);
  tft.drawRoundRect(129, 207, 60, 17, 6, NOIR);
  drawCenteredText("Stop", 159, 215);

  tft.fillRoundRect(218, 207, 60, 17, 6, BLANC);
  tft.drawRoundRect(218, 207, 60, 17, 6, NOIR);
  drawCenteredText("Descend", 248, 215);

  // === HOME & HEURE ===
  tft.fillRoundRect(12, 16, 87, 31, 6, BLANC);
  tft.drawRoundRect(12, 16, 87, 31, 6, NOIR);
  drawCenteredText("HOME", 55, 31);

  tft.setCursor(242, 26);
  tft.print("12:13");

  tft.drawRoundRect(217, 16, 87, 31, 4, NOIR);
}

void drawGestionCartes()
{
  tft.fillScreen(DK_BG);

  // === TITRE & HOME ===
  tft.setTextColor(NOIR);
  tft.setTextSize(1);
  tft.setCursor(124, 25);
  tft.print("CARTES RFID");
  tft.drawRoundRect(112, 16, 97, 31, 4, NOIR);

  tft.fillRoundRect(12, 16, 87, 31, 6, BLANC);
  tft.drawRoundRect(12, 16, 87, 31, 6, NOIR);
  drawCenteredText("HOME", 55, 31);

  // === LISTE DES CARTES (contenu dans drawGestionCartesVariable) ===
  tft.drawRoundRect(12, 86, 185, 124, 4, NOIR);

  // === BOUTONS ===
  tft.fillRoundRect(205, 90, 100, 30, 6, BLANC);
  tft.drawRoundRect(205, 90, 100, 30, 6, NOIR);
  drawCenteredText("AJOUTER", 255, 105);

  tft.fillRoundRect(205, 130, 100, 30, 6, BLANC);
  tft.drawRoundRect(205, 130, 100, 30, 6, NOIR);
  drawCenteredText("SUPPRIMER", 255, 145);

  tft.fillRoundRect(205, 170, 100, 30, 6, BLANC);
  tft.drawRoundRect(205, 170, 100, 30, 6, NOIR);
  drawCenteredText("ANNULER", 255, 185);
}

static void drawGestionCartesVariable()
{
  char ligne[48];
  char uid[24];
  CarteRfid carte;

  // Fond explicite : on ecrase l'ancien texte sans effacer la zone (pas de clignotement)
  tft.setTextColor(NOIR, DK_BG);
  tft.setTextSize(1);

  snprintf(ligne, sizeof(ligne), "Cartes : %d/%d   ", getNombreCartes(), MAX_CARTES);
  tft.setCursor(12, 58);
  tft.print(ligne);

  snprintf(ligne, sizeof(ligne), "%-40s", getMessageRfid());
  tft.setCursor(12, 72);
  tft.print(ligne);

  for (int i = 0; i < MAX_CARTES; i++)
  {
    if (getCarte(i, carte))
    {
      formatUid(carte, uid, sizeof(uid));
      snprintf(ligne, sizeof(ligne), "%2d. %-21s", i + 1, uid);
    }
    else
    {
      snprintf(ligne, sizeof(ligne), "%-25s", "");
    }
    tft.setCursor(16, 92 + (i * 11));
    tft.print(ligne);
  }
}

void drawCurrentScreen()
{
  switch (g_screen)
  {
  case ECRAN_ACCUEIL:
    drawHomeScreen();
    break;
  case ECRAN_DASHBOARD:
    drawDashboardScreen();
    break;
  case ECRAN_CONTROLE:
    drawControlPannel();
    break;
  case ECRAN_CARTES:
    drawGestionCartes();
    break;
  }
}

// Changement d'ecran par le moniteur serie (non utilise : le tactile le fait)
void handleUiActions()
{
  // Taper un numero d'ecran puis Entree dans le moniteur serie
  static int num = -1;
  while (Serial.available())
  {
    const int c = Serial.read();
    if (c >= '0' && c <= '9')
    {
      if (num < 0)
        num = 0;
      num = (num * 10) + (c - '0');
    }
    else if (c == '\n' || c == '\r')
    {
      if (num >= 0)
      {
        const int maxScreen = 5;
        if (num < 0)
          num = 0;
        if (num > maxScreen)
          num = maxScreen;
        const EcranId next = (EcranId)num;
        if (next != g_screen)
        {
          g_screen = next;
          g_dirty = true;
        }
      }
      num = -1;
    }
  }

  // Option : change d'ecran toutes les 5 s (a decommenter)
  // static uint32_t last = 0;
  // if (millis() - last > 5000) { last = millis(); g_screen = (EcranId)((g_screen + 1) % 6); g_dirty = true; }
}

void loadScreen(int num)
{
  const int maxScreen = ECRAN_CARTES;
  if (num < 0)
    num = 0;
  if (num > maxScreen)
    num = maxScreen;
  const EcranId next = (EcranId)num;
  if (next != g_screen)
  {
    g_screen = next;
    g_dirty = true;
  }
  drawCurrentScreen();
}

int getCurrentScreen()
{
  return (int)g_screen;
}