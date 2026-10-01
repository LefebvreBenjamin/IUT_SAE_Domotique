#include "buttonAction.h"
#include "cartesRfid.h"

void actionChangeScreen(int screenNumber)
{
    loadScreen(screenNumber);
}

void actionGoDashboard()
{
    actionChangeScreen(3);
}

void actionGoHome()
{
    actionChangeScreen(0);
}

void actionVoletDescendre()
{
    Serial.println("Volet Descend");
    sensorData.voletRoulant.Monter = false;
    sensorData.voletRoulant.Stop = false;
    sensorData.voletRoulant.Descendre = true;
}

void actionVoletMonter()
{
    Serial.println("Volet Monte");
    sensorData.voletRoulant.Descendre = false;
    sensorData.voletRoulant.Stop = false;
    sensorData.voletRoulant.Monter = true;
}

void actionVoletStop()
{
    Serial.println("Volet Stop");
    sensorData.voletRoulant.Monter = false;
    sensorData.voletRoulant.Descendre = false;
    sensorData.voletRoulant.Stop = true;
}

void actionOuvrirPortail()
{
    sensorData.portail.fermeture = false;
    sensorData.portail.ouverture = true;
}

void actionActiverGachePorte()
{
    sensorData.gacheporte.activer = true;
}

void actionDesactiverGachePorte()
{
    sensorData.gacheporte.activer = false;
}

void actionPortailOuverture()
{
    sensorData.portail.fermeture = false;
    sensorData.portail.ouverture = true;
}

void actionPortailFermeture()
{
    sensorData.portail.ouverture = false;
    sensorData.portail.fermeture = true;
}
void actionActiverAlarme()
{
    sensorData.alarme.activer = true;
    sensorData.alarme.desactiver = false;
}

void actionDesactiverAlarme()
{
    sensorData.alarme.activer = false;
    sensorData.alarme.desactiver = true;
}

void actionGoControlPanel()
{
    actionChangeScreen(5);
}

void actionGoGestionCartes()
{
    actionChangeScreen(6);
}

void actionAjouterCarte()
{
    demarrerAjoutCarte();
}

void actionSupprimerCarte()
{
    demarrerSuppressionCarte();
}

void actionAnnulerModeRfid()
{
    annulerModeRfid();
}