#include "MQ9GazSensor.h"
#include "main.h"
#include "SensorData.h"

namespace {
MQUnifiedsensor mq9("ESP32-C6", 3.3, 12, MQ9_ADC_PIN, "MQ-9");
}

void initMQ9() {
	mq9.setVoltResolution(3.3);
	mq9.setVCC(5.0);
	mq9.init();
	Serial.println("MQ-9 initialise sur GPIO0");
}

void updateMQ9Data() {
	mq9.update();
	sensorData.gasVoltage = mq9.getVoltage(false);
}

