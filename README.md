```text
Broches RFID-RC522 :
  SDA  --> 10
  SCK  --> 6
  MOSI --> 7
  MISO --> 2
  IRQ  --> NC
  GND  --> GND
  RST  --> 3
  3.3V --> 3V3

Broches BMP280 :
  VCC  -->  3V3
  GND  -->  GND
  SCL  -->  8
  SDA  -->  9
  CSB  -->  3V3
  SSD  -->  GND

Broches BH1750 :
  VCC -> 3V3 or 5V
  GND -> GND
  SCL -> 8
  SDA -> 9
  ADD -> NC/GND or VCC (see library doc : https://github.com/claws/BH1750 )

Broches Ecrans :
  VCC -> 3.3
  GND  -> GND
  CS  -> 18
  RESET  -> RST 
  DC  -> 19
  MOSI  -> 7
  SCK  -> 6
  LED  -> 3.3
  MISO -> 2

  T_CLK (ou T_SCK) -> Pin 6 (Partagé avec TFT_SCK)
  T_DIN (ou T_MOSI) -> Pin 7 (Partagé avec TFT_MOSI)
  T_DO (ou T_MISO) -> Pin 2 (Partagé avec TFT_MISO)
  T_CS -> Pin 4 (Permet à l'ESP32 de cibler la puce tactile au lieu de la puce d'affichage)
  T_IRQ -> Pin 5 (Signal d'interruption : le tactile prévient l'ESP32 dès que tu poses le doigt)
```
