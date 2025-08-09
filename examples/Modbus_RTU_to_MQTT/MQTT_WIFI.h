#ifndef MQTT_WIFI_H
#define MQTT_WIFI_H
#include <WiFi.h>
#include "PubSubClient.h"

enum {S_CONNECTING, S_WAIT, S_CONNECTED};

/*********************khai bao client MQTT*************************/

class Settings;
class MQTT_WIFI
{
public:
//bien con tro lay cac cai dat tu class setting
Settings* MySettings;


void Init();
uint8_t ReconnectWIFI();
uint8_t ReconnectMqtt();
void MQTT_ReceiveData();
void PublishData(String topic, String data);
void PublishData_Retain(String topic, String data);

void Subscribe();
uint8_t StateWifi;
uint8_t StateMqtt;
bool MQTTConnected();
};

#endif
