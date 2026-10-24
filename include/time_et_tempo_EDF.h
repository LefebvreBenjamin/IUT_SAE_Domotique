#pragma once
#include "SensorData.h"
#include <time.h>

#define FUSEAU_PARIS "CET-1CEST,M3.5.0,M10.5.0/3"

// Codes couleur Tempo
#define TEMPO_INCONNU 0
#define TEMPO_BLEU    1
#define TEMPO_BLANC   2
#define TEMPO_ROUGE   3

void initEdf();
void updateTimer();
void updateTempo();
