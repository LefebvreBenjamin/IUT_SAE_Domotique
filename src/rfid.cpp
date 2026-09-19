#include "rfid.h"

MFRC522 rfid(SS_PIN, RST_PIN);

void initRfid(){
  Serial.println("Initialisation RFID...");

  //SPI.begin(SCK_PIN, MISO_PIN, MOSI_PIN, -1);

  rfid.PCD_Init();

  delay(100);

  byte version = rfid.PCD_ReadRegister(MFRC522::VersionReg);
  Serial.print("RC522 VersionReg = 0x");
  if (version < 0x10) {
    Serial.print("0");
  }
  Serial.println(version, HEX);

  if (version == 0x91 || version == 0x92 || version == 0x88) {
    Serial.println("RFID initialise correctement");
  } else {
    Serial.println("ERREUR RFID : le RC522 ne repond pas");
    Serial.println("Verifier 3.3V, GND, CS/SDA=10, RST=3 et SPI 6/2/7");
  }
}

void verifierRfid(){
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

}



