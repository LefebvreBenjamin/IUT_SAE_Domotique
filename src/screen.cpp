#include "screen.h"

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

static const uint16_t DK_BG = 0xFFFF;
enum DkScreenId {
  DK_SCREEN_0,
  DK_SCREEN_1,
  DK_SCREEN_2,
  DK_SCREEN_3,
  DK_SCREEN_4,
  DK_SCREEN_5
};
static DkScreenId g_screen = DK_SCREEN_3;
static bool g_dirty = true;
static uint32_t g_lastDrawMs = 0;

void drawCurrentScreen();
void handleUiActions();

static void drawCenteredText(const char *text, int16_t x, int16_t y) {
  int16_t x1;
  int16_t y1;
  uint16_t width;
  uint16_t height;
  tft.getTextBounds(text, x, y, &x1, &y1, &width, &height);
  tft.setCursor(x - (width / 2), y - (height / 2));
  tft.print(text);
}

void initScreen(){
    Serial.println("Init screen"); 
   
    SPI.begin(TFT_SCK, TFT_MISO, TFT_MOSI, TFT_CS);
    tft.init(240, 320);
    tft.setRotation(1);
    tft.invertDisplay(false); // Rétablit les vraies couleurs (fond blanc)
    tft.fillScreen(DK_BG);

    Serial.println("Screen Initalized");
}

void updateScreen(){
    handleUiActions();
    // Redraw only when needed (more "pro" and faster)
    if (g_dirty || (millis() - g_lastDrawMs) > 1000) {
        drawCurrentScreen();
        g_dirty = false;
        g_lastDrawMs = millis();
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
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF
};

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
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF
};

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
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF
};






void drawHomeScreen() {
  tft.fillScreen(DK_BG);

  tft.drawRoundRect(72, 20, 177, 42, 4, 0x10C4);
  tft.setTextColor(NOIR);
  tft.setTextSize(2); // Taille ajustée pour rentrer dans le cadre 177px
  tft.setCursor(79, 32);
  tft.print("HOME ASSISTANT");

  tft.fillRoundRect(22, 128, 80, 28, 6, 0x10C4);
  tft.drawRoundRect(22, 128, 80, 28, 6, BLANC);
  tft.setTextColor(NOIR);
  tft.setTextSize(1);
  drawCenteredText("DASHBOARD", 62, 142);

  // Action: goto screen DK_SCREEN_3 (trigger from your input handler)
  tft.fillRoundRect(214, 128, 80, 28, 6, BLANC);
  tft.drawRoundRect(214, 128, 80, 28, 6, BLANC);
  tft.setTextColor(NOIR);
  tft.setTextSize(1);  
  drawCenteredText("CONTROL PANEL", 254, 142);

  // Action: goto screen DK_SCREEN_4 (trigger from your input handler)
  tft.setTextColor(NOIR);
  tft.setTextSize(1);  
  tft.setCursor(110, 83);
  tft.print("12:13 - JJ/MM/AAAA");

  tft.drawRoundRect(107, 73, 107, 30, 4, 0x10C4);
}

void drawSettingsScreen() {
  tft.fillScreen(DK_BG);
}

void drawNowPlayingCard() {
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

void drawDashboardScreen() {
  tft.fillScreen(DK_BG);

  tft.drawRoundRect(5, 53, 310, 174, 4, 0x10C4);
  tft.setTextColor(NOIR);
  tft.setTextSize(1);
  tft.setCursor(100, 22);
  tft.print("DASHBOARD");

  tft.drawRoundRect(86, 13, 109, 30, 4, 0x10C4);
  tft.drawRGBBitmap(20, 95, icon_temp_el_8iewusar9, 16, 16);

  tft.drawRGBBitmap(122, 117, icon_nuke_el_wc9q5tvoc, 16, 16);

  tft.setTextColor(NOIR);
  tft.setTextSize(1); 
  tft.setCursor(47, 98);
  tft.print("123 C"); // Supprimé le caractère '°' pour éviter les glitchs d'affichage avec Adafruit

  tft.setTextColor(NOIR);
  tft.setTextSize(1); 
  tft.setCursor(140, 120);
  tft.print("Normal");

  tft.setTextColor(NOIR);
  tft.setTextSize(1); 
  tft.setCursor(212, 14);
  tft.print("Connected");

  tft.drawRGBBitmap(298, 13, icon_wifi_1_el_ur9q8wxw3, 16, 16);

  tft.setTextColor(NOIR);
  tft.setTextSize(1); 
  tft.setCursor(47, 165);
  tft.print("250 Lux");

  tft.drawRGBBitmap(19, 162, icon_nuke_el_wc9q5tvoc, 16, 16);

  tft.setTextColor(NOIR);
  tft.setTextSize(1); 
  tft.setCursor(231, 123);
  tft.print("Alarme");

  tft.setTextColor(NOIR);
  tft.setTextSize(1); 
  tft.setCursor(231, 150);
  tft.print("Volet");

  tft.fillCircle(285, 106, 6, 0x10C4);
  tft.fillCircle(285, 134, 6, 0x10C4);
  tft.fillCircle(285, 160, 6, 0x10C4);
  tft.drawRGBBitmap(20, 119, icon_temp_el_8iewusar9, 16, 16);

  tft.setTextColor(NOIR);
  tft.setTextSize(1); 
  tft.setCursor(47, 121);
  tft.print("10%");

  tft.drawRGBBitmap(20, 142, icon_temp_el_8iewusar9, 16, 16);

  tft.setTextColor(NOIR);
  tft.setTextSize(1); 
  tft.setCursor(47, 145);
  tft.print("1123 hPA");

  tft.fillCircle(190, 125, 6, NOIR);
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

void drawRFIDCheck() {
  tft.fillScreen(DK_BG);

  tft.setTextColor(NOIR);
  tft.setTextSize(2);
  tft.setCursor(106, 101);
  tft.print("RFID CHECK.....");

}

void drawControlPannel() {
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
  tft.setCursor(65, 126);
  tft.print("Alarme");

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

void drawCurrentScreen() {
  switch (g_screen) {
    case DK_SCREEN_0: drawHomeScreen(); break;
    case DK_SCREEN_1: drawSettingsScreen(); break;
    case DK_SCREEN_2: drawNowPlayingCard(); break;
    case DK_SCREEN_3: drawDashboardScreen(); break;
    case DK_SCREEN_4: drawRFIDCheck(); break;
    case DK_SCREEN_5: drawControlPannel(); break;
  }
}

// TODO: Read touch/buttons and set `g_screen` (and element values).
void handleUiActions() {
  // COMPLETE default behavior: screen switching via Serial.
  // Type a screen number (0..N-1) in Serial Monitor and press Enter.
  static int num = -1;
  while (Serial.available()) {
    const int c = Serial.read();
    if (c >= '0' && c <= '9') {
      if (num < 0) num = 0;
      num = (num * 10) + (c - '0');
    } else if (c == '\n' || c == '\r') {
      if (num >= 0) {
        const int maxScreen = 5;
        if (num < 0) num = 0;
        if (num > maxScreen) num = maxScreen;
        const DkScreenId next = (DkScreenId)num;
        if (next != g_screen) { g_screen = next; g_dirty = true; }
      }
      num = -1;
    }
  }

  // Optional: auto-cycle every 5 seconds (uncomment to enable)
  //static uint32_t last = 0;
  //if (millis() - last > 5000) { last = millis(); g_screen = (DkScreenId)((g_screen + 1) % 6); g_dirty = true; }
}