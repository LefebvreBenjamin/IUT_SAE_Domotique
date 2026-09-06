#include <Arduino.h>

#include <SPI.h>
#include <MFRC522.h>

// Broches ESP32-C6
#define SS_PIN   10
#define RST_PIN  3
#define SCK_PIN  6
#define MISO_PIN 2
#define MOSI_PIN 7

MFRC522 rfid(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("Initialisation RFID...");

  // SPI avec nos broches
  SPI.begin(SCK_PIN, MISO_PIN, MOSI_PIN, SS_PIN);

  rfid.PCD_Init();

  delay(100);

  Serial.println("RC522 pret !");
  Serial.println("Approche une carte RFID...");
}

void loop() {

  // Pas de nouvelle carte
  if (!rfid.PICC_IsNewCardPresent())
    return;

  // Impossible de lire la carte
  if (!rfid.PICC_ReadCardSerial())
    return;

  Serial.print("UID : ");

  for (byte i = 0; i < rfid.uid.size; i++) {
    if (rfid.uid.uidByte[i] < 0x10)
      Serial.print("0");

    Serial.print(rfid.uid.uidByte[i], HEX);
    Serial.print(" ");
  }

  Serial.println();

  // Afficher le type de carte
  MFRC522::PICC_Type type = rfid.PICC_GetType(rfid.uid.sak);

  Serial.print("Type : ");
  Serial.println(rfid.PICC_GetTypeName(type));

  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();

  delay(500);
}