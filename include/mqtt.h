#include <PubSubClient.h>

void initMQTT();
void updateMQTT();

bool publierMesure(const char* nom, float valeur);
void sendTrame();