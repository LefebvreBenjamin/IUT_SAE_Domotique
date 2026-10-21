#include "MQ9GazSensor.h"
#include "main.h"
#include "SensorData.h"
#include <math.h>

namespace {
MQUnifiedsensor mq9("ESP32-C6", 3.3, 12, MQ9_ADC_PIN, "MQ-9");

const float VCC_CAPTEUR = 5.0;      // alim du capteur (V)
const float RL = 10000.0;           // résistance de charge (ohms)
const float FACTEUR_DIV = 1.5;      // (R1+R2)/R2 du pont diviseur, 1.0 si pas de pont
const float RATIO_AIR_PUR = 9.6;    // Rs/R0 dans l'air pur (datasheet)

// Courbe du CO : ppm = A * (Rs/R0)^B
const float CO_A = 599.65;
const float CO_B = -2.244;

// Seuils de qualité de l'air (ppm de CO), à ajuster
const float SEUIL_MOYEN = 9.0;      // au-dessus : moyen
const float SEUIL_MAUVAIS = 35.0;   // au-dessus : mauvais

float r0 = 10000.0;                 // à calibrer avec calibrerMQ9()

// Calcule Rs (ohms) à partir de la tension lue
float calculRs(float vAdc) {
	float vCapteur = vAdc * FACTEUR_DIV;      // tension avant le pont
	if (vCapteur < 0.01 || vCapteur >= VCC_CAPTEUR) return -1.0;
	return RL * (VCC_CAPTEUR - vCapteur) / vCapteur;
}
}

void initMQ9() {
	mq9.setVoltResolution(3.3);
	mq9.setVCC(5.0);
	mq9.init();
	Serial.println("MQ-9 initialise sur GPIO0");
}

// À lancer dans l'air pur, capteur préchauffé (plusieurs heures)
void calibrerMQ9() {
	float somme = 0;
	int n = 0;
	for (int i = 0; i < 50; i++) {
		mq9.update();
		float rs = calculRs(mq9.getVoltage(false));
		if (rs > 0) {
			somme += rs;
			n++;
		}
		delay(100);
	}
	if (n > 0) {
		r0 = (somme / n) / RATIO_AIR_PUR;
		Serial.print("R0 = ");
		Serial.println(r0);
	}
}

void updateMQ9Data() {
	mq9.update();
	float v = mq9.getVoltage(false);
	sensorData.gasVoltage = v;

	float rs = calculRs(v);
	if (rs < 0) {
		sensorData.gasPpm = NAN;   // mesure impossible
		sensorData.airQuality.good = false;
		sensorData.airQuality.moderate = false;
		sensorData.airQuality.bad = false;
		return;
	}
	float ratio = rs / r0;
	float ppm = CO_A * powf(ratio, CO_B);
	sensorData.gasPpm = ppm;

	// Un seul état vrai à la fois
	sensorData.airQuality.good = (ppm < SEUIL_MOYEN);
	sensorData.airQuality.moderate = (ppm >= SEUIL_MOYEN && ppm < SEUIL_MAUVAIS);
	sensorData.airQuality.bad = (ppm >= SEUIL_MAUVAIS);
}