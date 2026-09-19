#include <Arduino.h>

#include <SPI.h>
#include <MFRC522.h>
#include "main.h"

#pragma once

extern MFRC522 rfid;

void initRfid();
void verifierRfid();