#include "buttonAction.h"

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
    sensorData.voletRoulant.Monter = false;
    sensorData.voletRoulant.Stop = false;
    sensorData.voletRoulant.Descendre = true;
}

void actionVoletMonter()
{
    sensorData.voletRoulant.Descendre = false;
    sensorData.voletRoulant.Stop = false;
    sensorData.voletRoulant.Monter = true;
}

void actionVoletStop()
{
    sensorData.voletRoulant.Monter = false;
    sensorData.voletRoulant.Descendre = false;
    sensorData.voletRoulant.Stop = true;
}

void actionOuvrirPortail()
{
    sensorData.portail.fermeture = false;
    sensorData.portail.ouverture = true;
}

void actionActiverAlarme()
{
    sensorData.alarme.desactiver = false;
    sensorData.alarme.activer = true;
}

void actionDesactiverAlarme()
{
    sensorData.alarme.activer = false;
    sensorData.alarme.desactiver = true;
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

void actionGoControlPanel()
{
    actionChangeScreen(5);
}