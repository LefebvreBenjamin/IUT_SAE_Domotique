#include <Arduino.h>

#include <SPI.h>
#include <MFRC522.h>
#include "main.h"
#include "buttonAction.h"
#include "cartesRfid.h"
#pragma once

extern MFRC522 rfid;

void initRfid();
void verifierRfid();