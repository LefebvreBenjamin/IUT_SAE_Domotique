#pragma once

#include <Arduino.h>

// Nombre max de cartes enregistrables et taille max de l'uid
#define MAX_CARTES 10
#define UID_MAX_SIZE 10

// Un mode (ajout / suppression) est annule automatiquement apres ce delai
#define RFID_MODE_TIMEOUT_MS 15000

// Taille max du message affiche a l'ecran (40 caracteres + '\0')
#define RFID_MESSAGE_SIZE 41

struct CarteRfid //parametre des cartes
{
  uint8_t taille;
  uint8_t uid[UID_MAX_SIZE];
};

enum ModeRfid // différent mode pour le rfid 
{
  RFID_MODE_NORMAL,
  RFID_MODE_AJOUT_VALIDATION,
  RFID_MODE_AJOUT,
  RFID_MODE_SUPPR_VALIDATION,
  RFID_MODE_SUPPR
};

enum ResultatCarte 
{
  CARTE_OK,
  CARTE_DEJA_PRESENTE,
  CARTE_LISTE_PLEINE,
  CARTE_INCONNUE,
  CARTE_DERNIERE,  // 1 carte minimum
  CARTE_UID_INVALIDE
};

void initCartesRfid();
void updateCartesRfid();  // gere le timeout des modes

// Gestion liste sauvegardee  memoire flash
bool carteAutorisee(const uint8_t *uid, uint8_t taille);
ResultatCarte ajouterCarte(const uint8_t *uid, uint8_t taille);
ResultatCarte supprimerCarte(const uint8_t *uid, uint8_t taille);
int getNombreCartes();
bool getCarte(int index, CarteRfid &carte);
void formatUid(const CarteRfid &carte, char *buffer, size_t taille);

// bouton actions appelé par l'ecrans
void demarrerAjoutCarte();
void demarrerSuppressionCarte();
void annulerModeRfid();
ModeRfid getModeRfid();

// Interface ecran : message info
const char *getMessageRfid();
uint32_t getVersionCartesRfid();

// Appelee par verifierRfid() a chaque carte lue.
// Retourne true si la gache de porte doit etre activee.
bool traiterCarteRfid(const uint8_t *uid, uint8_t taille);
