#include "screen.h"
#include "cartesRfid.h"

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

static const uint16_t DK_BG = 0xFFFF;


static DkScreenId g_screen = DK_SCREEN_3;
static bool g_dirty = true;
static uint32_t g_lastDrawMs = 0;
static uint32_t g_lastDrawVarMs = 0;
static uint32_t g_lastCartesVersion = 0xFFFFFFFF;

void drawCurrentScreen();
static void drawDashboardVariable();
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

void initScreen()
{
  Serial.println("Init screen");

  //DACHBOARDE
  addButton({5, 13, 63, 30, actionGoHome, (int)DK_SCREEN_3});
  
  //HOMZ MENUZ
  addButton({22, 128, 80, 28, actionGoDashboard, (int)DK_SCREEN_0});
  addButton({214, 128, 80, 28, actionGoControlPanel, (int)DK_SCREEN_0});
  addButton({120, 128, 80, 28, actionGoGestionCartes, (int)DK_SCREEN_0});

  //CONTROL PANEL
  addButton({12, 16, 87, 31, actionGoHome, (int)DK_SCREEN_5}); //HOME

  addButton({218, 207, 60, 17, actionVoletDescendre, (int)DK_SCREEN_5}); // Vloet Descend
  addButton({129, 207, 60, 17, actionVoletStop, (int)DK_SCREEN_5}); //Volet Stop
  addButton({35, 207, 60, 17, actionVoletMonter, (int)DK_SCREEN_5}); //Volet Monter

  addButton({17, 156, 63, 21, actionActiverAlarme, (int)DK_SCREEN_5}); //Alarme Activer
  addButton({87, 156, 63, 21, actionDesactiverAlarme, (int)DK_SCREEN_5}); //Alarme Desactiver

  addButton({87, 93, 63, 21, actionPortailFermeture, (int)DK_SCREEN_5}); //Gache Porte Fermeture
  addButton({17, 93, 63, 21, actionPortailOuverture, (int)DK_SCREEN_5}); //Gache Porte Ouverture

  //GESTION CARTES RFID
  addButton({12, 16, 87, 31, actionGoHome, (int)DK_SCREEN_6});              //HOME
  addButton({205, 90, 100, 30, actionAjouterCarte, (int)DK_SCREEN_6});      //Ajouter
  addButton({205, 130, 100, 30, actionSupprimerCarte, (int)DK_SCREEN_6});   //Supprimer
  addButton({205, 170, 100, 30, actionAnnulerModeRfid, (int)DK_SCREEN_6});  //Annuler





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
  if (g_screen == DK_SCREEN_6 && getVersionCartesRfid() != g_lastCartesVersion)
  {
    drawGestionCartesVariable();
    g_lastCartesVersion = getVersionCartesRfid();
  }
  if (g_screen == DK_SCREEN_3 && (millis() - g_lastDrawVarMs) > 100)
  {
    drawDashboardVariable();
    g_lastDrawVarMs = millis();
  }
  delay(5);
}

const uint16_t icon_wifi_1_el_ur9q8wxw3[256] PROGMEM = {
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF};

const uint16_t icon_nuke_el_wc9q5tvoc[256] PROGMEM = {
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x4A69,
    0x9CF3, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x9CF3, 0x4A69, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0x4A69, 0xDEDB, 0xDEDB, 0x5ACB, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x5ACB, 0xDEDB,
    0xDEDB, 0x4A69, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xD69A, 0xDEDB, 0xDEDB, 0xCE59, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xCE59, 0xDEDB, 0xDEDB, 0xD69A, 0xFFFF, 0xFFFF, 0xFFFF, 0x73AE, 0xDEDB, 0xDEDB,
    0xDEDB, 0xDEDB, 0x73AE, 0xFFFF, 0xFFFF, 0x73AE, 0xDEDB, 0xDEDB, 0xDEDB, 0xDEDB, 0x73AE, 0xFFFF,
    0xFFFF, 0xB5B6, 0xDEDB, 0xDEDB, 0xDEDB, 0xCE59, 0xFFFF, 0x4228, 0x4208, 0xFFFF, 0xCE59, 0xDEDB,
    0xDEDB, 0xDEDB, 0xB5B6, 0xFFFF, 0xFFFF, 0xCE59, 0xDEDB, 0xDEDB, 0xDEDB, 0x632C, 0x4228, 0xDEDB,
    0xDEDB, 0x4208, 0x7BEF, 0xDEDB, 0xDEDB, 0xDEDB, 0xCE59, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0x4228, 0xDEDB, 0xDEDB, 0x4208, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x4208, 0x4228, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x5ACB, 0x7BEF,
    0x7BEF, 0x5ACB, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xCE79, 0xDEDB, 0xDEDB, 0xCE79, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x8410, 0xDEDB, 0xDEDB, 0xDEDB, 0xDEDB, 0x8430, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xDEDB, 0xDEDB, 0xDEDB,
    0xDEDB, 0xDEDB, 0xDEDB, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0x73AE, 0xB5B6, 0xD6BA, 0xD6BA, 0xB5B6, 0x73AE, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF};

const uint16_t icon_temp_el_8iewusar9[256] PROGMEM = {
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFDF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x630C, 0xFFFF,
    0xAD55, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x7BCF, 0xFFFF, 0xFFFF,
    0xFFFF, 0x4228, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xEF7D, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF};

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
  tft.setTextColor(NOIR);
  tft.setTextSize(1);
  drawCenteredText("DASHBOARD", 62, 142);

  tft.fillRoundRect(120, 128, 80, 28, 6, BLANC);
  tft.drawRoundRect(120, 128, 80, 28, 6, NOIR);
  tft.setTextColor(NOIR);
  tft.setTextSize(1);
  drawCenteredText("CARTES RFID", 160, 142);

  // Action: goto screen DK_SCREEN_3 (trigger from your input handler)
  tft.fillRoundRect(214, 128, 80, 28, 6, BLANC);
  tft.drawRoundRect(214, 128, 80, 28, 6, NOIR);
  tft.setTextColor(NOIR);
  tft.setTextSize(1);
  tft.setCursor(215, 138);
  tft.print("CONTROL PANEL");

  // Action: goto screen DK_SCREEN_4 (trigger from your input handler)
  tft.setTextColor(NOIR);
  tft.setTextSize(1);
  tft.setCursor(110, 83);
  tft.print("12:13 - JJ/MM/AAAA");

  tft.drawRoundRect(107, 73, 107, 30, 4, BLANC);
}

void drawSettingsScreen()
{
  tft.fillScreen(DK_BG);
}

void drawNowPlayingCard()
{
  tft.fillScreen(DK_BG);

  tft.fillRect(0, 0, 240, 26, 0xFFFF);
  tft.drawLine(0, 25, 240, 25, 0xFFFF);
  tft.setTextColor(0x10C4);
  tft.setTextSize(1);
  tft.setCursor(4, 9);
  tft.print("Music");

  tft.fillRoundRect(12, 40, 216, 110, 6, 0xFFFF);
  tft.drawRoundRect(12, 40, 216, 110, 6, 0xFFFF);
  tft.setTextColor(0x10C4);
  tft.setTextSize(1);
  tft.setCursor(16, 44);
  tft.print("Card");

  tft.fillRoundRect(24, 52, 64, 64, 4, 0xFFFF);
  tft.drawRoundRect(24, 52, 64, 64, 4, 0xFFFF);
  tft.setTextColor(0x10C4);
  tft.setTextSize(1);
  tft.setCursor(100, 58);
  tft.print("Track Name");
  tft.setCursor(100, 74);
  tft.print("Artist");

  tft.drawRoundRect(24, 124, 192, 14, 3, 0xFFFF);
  tft.fillRoundRect(26, 126, 65, 10, 3, 0xFFFF);
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
  // tft.drawRGBBitmap(20, 95, icon_temp_el_8iewusar9, 16, 16);

  // tft.drawRGBBitmap(122, 117, icon_nuke_el_wc9q5tvoc, 16, 16);

  tft.setTextColor(NOIR);
  tft.setTextSize(1);
  tft.setCursor(19, 98);
  // tft.printf("%.2f C", sensorData.temperature);

  tft.setTextColor(NOIR);
  tft.setTextSize(1);
  tft.setCursor(125, 120);
  tft.print("Qualité");

  tft.setTextColor(NOIR);
  tft.setTextSize(1);
  tft.setCursor(212, 14);
  tft.print("Connected");

  // tft.drawRGBBitmap(298, 13, icon_wifi_1_el_ur9q8wxw3, 16, 16);
  /*
    tft.setTextColor(NOIR);
    tft.setTextSize(1);
    tft.setCursor(47, 165);
    tft.printf("%.2f ppm", sensorData.temperature);*/

  // tft.drawRGBBitmap(19, 162, icon_nuke_el_wc9q5tvoc, 16, 16);

  tft.setTextColor(NOIR);
  tft.setTextSize(1);
  tft.setCursor(231, 123);
  tft.print("Alarme");

  tft.setTextColor(NOIR);
  tft.setTextSize(1);
  tft.setCursor(231, 150);
  tft.print("Volet");

  // tft.fillCircle(285, 106, 6, 0x10C4);
  // tft.fillCircle(285, 134, 6, 0x10C4);
  // tft.fillCircle(285, 160, 6, 0x10C4);
  // tft.drawRGBBitmap(20, 119, icon_temp_el_8iewusar9, 16, 16);

  tft.setTextColor(NOIR);
  tft.setTextSize(1);
  tft.setCursor(19, 121);
  // tft.printf("%.2f lx", sensorData.light);

  // tft.drawRGBBitmap(20, 142, icon_temp_el_8iewusar9, 16, 16);

  tft.setTextColor(NOIR);
  tft.setTextSize(1);
  tft.setCursor(19, 145);
  // tft.printf("%.2f hPa", sensorData.pressure);

  // tft.fillCircle(190, 125, 6, NOIR);
  tft.fillRoundRect(5, 13, 63, 30, 6, BLANC);
  tft.drawRoundRect(5, 13, 63, 30, 6, NOIR);
  tft.setTextColor(NOIR);
  tft.setTextSize(1);
  drawCenteredText("HOME", 43, 28);

  // Action: goto screen DK_SCREEN_0 (trigger from your input handler)
  tft.setTextColor(NOIR);
  tft.setTextSize(1);
  tft.setCursor(237, 68);
  tft.print("Statuts");

  tft.setTextColor(NOIR);
  tft.setTextSize(1);
  tft.setCursor(212, 28);
  tft.print("12:35 DD/MM/AAAA");

  tft.drawRoundRect(206, 8, 111, 40, 4, 0x10C4);
  tft.drawRoundRect(215, 64, 94, 117, 4, 0x10C4);
  tft.setTextColor(NOIR);
  tft.setTextSize(1);
  tft.setCursor(233, 96);
  tft.print("Porte");

  tft.drawRoundRect(11, 64, 92, 117, 4, 0x10C4);
  tft.setTextColor(NOIR);
  tft.setTextSize(1);
  tft.setCursor(19, 68);
  tft.print("Environnement");

  tft.drawRoundRect(114, 64, 92, 117, 4, 0x10C4);
  tft.setTextColor(NOIR);
  tft.setTextSize(1);
  tft.setCursor(148, 68);
  tft.print("Air");
}

static void drawDashboardVariable()
{
  tft.fillRect(19, 98, 65, 56, DK_BG);
  tft.setTextColor(NOIR);
  tft.setTextSize(1);
  tft.setCursor(19, 98);
  tft.printf("%.2f Celsius", sensorData.temperature);

  tft.setTextColor(NOIR);
  tft.setTextSize(1);
  tft.setCursor(19, 121);
  tft.printf("%.2f lux", sensorData.light);

  tft.setTextColor(NOIR);
  tft.setTextSize(1);
  tft.setCursor(19, 145);
  tft.printf("%.2f hPa", sensorData.pressure);

  tft.setTextColor(NOIR);
  tft.setTextSize(1);
  tft.setCursor(212, 28);
  tft.print("12:35 DD/MM/AAAA");

  // Statuts
  tft.fillRect(275, 83, 20, 50, DK_BG);
  if (sensorData.gacheporte.activer)
  {
    tft.fillCircle(285, 100, 6, VERT); // Portail
  }
  else
  {
    tft.fillCircle(285, 100, 6, ROUGE); // Portail
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
    tft.fillCircle(285, 150, 6, VERT); // Porte
  }
  else
  {
    tft.fillCircle(285, 150, 6, ROUGE); // Porte
  }

  // tft.drawRoundRect(206, 8, 111, 40, 4, 0x10C4);
  // tft.drawRoundRect(215, 64, 94, 117, 4, 0x10C4);
  tft.setTextColor(NOIR);
  tft.setTextSize(1);
  tft.setCursor(233, 96);

  // environement
  tft.fillRect(165, 78, 20, 50, DK_BG);
  tft.fillCircle(180, 123, 6, NOIR); // Air
  if (sensorData.airQuality.good)
  {
    tft.fillCircle(180, 120, 6, VERT); // Air
  }
  else if (sensorData.airQuality.moderate)
  {
    tft.fillCircle(180, 120, 6, JAUNE); // Air
  }
  else if (sensorData.airQuality.bad)
  {
    tft.fillCircle(180, 120, 6, ROUGE); // Air
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

  // === PORTAIL ===
  tft.drawRoundRect(12, 58, 144, 60, 4, NOIR);
  tft.setTextColor(NOIR);
  tft.setTextSize(1);
  tft.setCursor(61, 62);
  tft.print("Portail");

  tft.fillRoundRect(17, 93, 63, 21, 6, BLANC);
  tft.drawRoundRect(17, 93, 63, 21, 6, NOIR);
  tft.setTextColor(NOIR);
  tft.setTextSize(1);
  drawCenteredText("Ouverture", 48, 103);

  tft.fillRoundRect(87, 93, 63, 21, 6, BLANC);
  tft.drawRoundRect(87, 93, 63, 21, 6, NOIR);
  tft.setTextColor(NOIR);
  tft.setTextSize(1);
  drawCenteredText("Fermeture", 118, 103);

  // === ALARME ===
  tft.drawRoundRect(12, 122, 144, 60, 4, NOIR);
  tft.setTextColor(NOIR);
  tft.setTextSize(1);
  tft.setCursor(45, 126);
  tft.print("Gache Porte");

  tft.fillRoundRect(87, 156, 63, 21, 6, BLANC);
  tft.drawRoundRect(87, 156, 63, 21, 6, NOIR);
  tft.setTextColor(NOIR);
  tft.setTextSize(1);
  drawCenteredText("Désactiver", 118, 166);

  tft.fillRoundRect(17, 156, 63, 21, 6, BLANC);
  tft.drawRoundRect(17, 156, 63, 21, 6, NOIR);
  tft.setTextColor(NOIR);
  tft.setTextSize(1);
  drawCenteredText("Activer", 48, 166);

  // === VMC ===
  tft.drawRoundRect(162, 58, 144, 60, 4, NOIR);
  tft.setTextColor(NOIR);
  tft.setTextSize(1);
  tft.setCursor(222, 62);
  tft.print("VMC");

  tft.setCursor(272, 78);
  tft.print("50%");
  tft.drawRoundRect(168, 78, 98, 10, 5, NOIR);
  tft.fillCircle(217, 83, 4, NOIR);

  // === ECLAIRAGE ===
  tft.drawRoundRect(162, 122, 144, 60, 4, NOIR);
  tft.setTextColor(NOIR);
  tft.setTextSize(1);
  tft.setCursor(206, 126);
  tft.print("Eclairage");

  tft.setCursor(272, 142);
  tft.print("50%");
  tft.drawRoundRect(168, 142, 98, 10, 5, NOIR);
  tft.fillCircle(217, 147, 4, NOIR);

  // === VOLET ROULANT ===
  tft.drawRoundRect(13, 187, 292, 45, 4, NOIR);
  tft.setTextColor(NOIR);
  tft.setTextSize(1);
  tft.setCursor(121, 192);
  tft.print("Volet Roulant");

  tft.fillRoundRect(35, 207, 60, 17, 6, BLANC);
  tft.drawRoundRect(35, 207, 60, 17, 6, NOIR);
  tft.setTextColor(NOIR);
  tft.setTextSize(1);
  drawCenteredText("Monte", 65, 215);

  tft.fillRoundRect(129, 207, 60, 17, 6, BLANC);
  tft.drawRoundRect(129, 207, 60, 17, 6, NOIR);
  tft.setTextColor(NOIR);
  tft.setTextSize(1);
  drawCenteredText("Stop", 159, 215);

  tft.fillRoundRect(218, 207, 60, 17, 6, BLANC);
  tft.drawRoundRect(218, 207, 60, 17, 6, NOIR);
  tft.setTextColor(NOIR);
  tft.setTextSize(1);
  drawCenteredText("Descend", 248, 215);

  // === HOME & HEURE ===
  tft.fillRoundRect(12, 16, 87, 31, 6, BLANC);
  tft.drawRoundRect(12, 16, 87, 31, 6, NOIR);
  tft.setTextColor(NOIR);
  tft.setTextSize(1);
  drawCenteredText("HOME", 55, 31);

  tft.setTextColor(NOIR);
  tft.setTextSize(1);
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
  case DK_SCREEN_0:
    drawHomeScreen();
    break;
  case DK_SCREEN_1:
    drawSettingsScreen();
    break;
  case DK_SCREEN_2:
    drawNowPlayingCard();
    break;
  case DK_SCREEN_3:
    drawDashboardScreen();
    break;
  case DK_SCREEN_4:
    drawRFIDCheck();
    break;
  case DK_SCREEN_5:
    drawControlPannel();
    break;
  case DK_SCREEN_6:
    drawGestionCartes();
    break;
  }
}

// TODO: Read touch/buttons and set `g_screen` (and element values).
void handleUiActions()
{
  // COMPLETE default behavior: screen switching via Serial.
  // Type a screen number (0..N-1) in Serial Monitor and press Enter.
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
        const DkScreenId next = (DkScreenId)num;
        if (next != g_screen)
        {
          g_screen = next;
          g_dirty = true;
        }
      }
      num = -1;
    }
  }

  // Optional: auto-cycle every 5 seconds (uncomment to enable)
  // static uint32_t last = 0;
  // if (millis() - last > 5000) { last = millis(); g_screen = (DkScreenId)((g_screen + 1) % 6); g_dirty = true; }
}

void loadScreen(int num)
{
  const int maxScreen = 6;
  if (num < 0)
    num = 0;
  if (num > maxScreen)
    num = maxScreen;
  const DkScreenId next = (DkScreenId)num;
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