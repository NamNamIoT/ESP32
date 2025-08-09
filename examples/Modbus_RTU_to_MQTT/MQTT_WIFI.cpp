#include "MQTT_WIFI.h"
#include "HardwareSerial.h"
#include "Settings.h"
Settings MySettings;
WiFiClient espClient;
PubSubClient client(espClient);
void callback(char* topic, byte *payload, unsigned int length);
extern int PORT;
extern String SERVER;
extern String USER;
extern String PASS;
String topic_sucribe[6]={"RESET", "Configure", "INFOR", "WRITE", "OTA", "CHECK_WIFI"};
/**************************khoi tao giao tiep MQTT************************/
void MQTT_WIFI::Init()
{
  StateWifi = S_CONNECTING;
  StateMqtt = S_CONNECTING;
  WiFi.mode(WIFI_STA); //mode  station
  delay(1000);
  uint8_t m_result = ReconnectWIFI();
  delay(1000);
  client.setServer(SERVER.c_str(), PORT);
  if (m_result == S_CONNECTED) ReconnectMqtt();
  client.setCallback(callback);
}
/********************ham subscribe******************************************/
extern String Client_ID;
void MQTT_WIFI::Subscribe(void)
{
  if (ReconnectWIFI() != S_CONNECTED) return;
  if (ReconnectMqtt() != S_CONNECTED) return;
  Serial.printf("\r\nSizeof: %d",sizeof(topic_sucribe)/sizeof(topic_sucribe[0]));
  for(int count=0; count<(sizeof(topic_sucribe)/sizeof(topic_sucribe[0])); count++)
  {
    if(topic_sucribe[count]!=""&&topic_sucribe[count]!=" ")
    {
      client.subscribe((Client_ID+"/" + topic_sucribe[count]).c_str());
      Serial.printf("\r\nSubcribe topic: ");
      Serial.print(Client_ID + "/" + topic_sucribe[count]);
    }
  }
  client.subscribe((Client_ID + "/PING").c_str());
  
}
/********************ham reconnect wifi**************************************/
int station=0;
extern String SSID_WIFI, PASS_WIFI;
int station_wifi=0;
uint8_t MQTT_WIFI::ReconnectWIFI()
{
  static uint32_t PreTime = 0; const uint32_t TimeOut = 15000;
  switch (StateWifi)
  {
    case S_CONNECTING:
    if(station_wifi==0) WiFi.begin(SSID_WIFI.c_str(), PASS_WIFI.c_str());
    else WiFi.begin("LAN", "116117118");
    PreTime = millis(); StateWifi = S_WAIT;
    break;
    case S_WAIT:
      if (WiFi.status() == WL_CONNECTED)
      {
        Serial.printf("\r\n[WIFI] Wifi connected");
        StateWifi = S_CONNECTED;
      }
      if (((uint32_t)(millis() - PreTime)) > TimeOut)
      {
        Serial.printf("\r\n[WIFI] WiFi connect failed, try to connect again");
        //if(station_wifi==0) station_wifi=1;
        //else station_wifi=0;
        StateWifi = S_CONNECTING;
      }
      break;
    case S_CONNECTED:
      if (WiFi.status() != WL_CONNECTED)
      {
        Serial.printf("\r\nWiFi disconnected");
        StateMqtt = S_CONNECTING;
        StateWifi = S_CONNECTING;
      }
      break;
  }
  return StateWifi;
}
/********************* check ket noi wifi *************************************/
bool MQTT_WIFI::MQTTConnected()
{
  if (StateMqtt == S_CONNECTED)
  {
    Serial.printf("\r\nMQTT CONNECTED");
    return true;
  }
  else
  {
    return false;
  }
}
/*********************ham reconnect mqtt***************************************/
uint8_t MQTT_WIFI::ReconnectMqtt()
{
  static uint32_t PreTime = 0; const uint32_t TimeOut = 15000; uint32_t id2;String str2;
  switch (StateMqtt)
  {
    case S_CONNECTING:
      Serial.printf("\r\n[WIFI] Attempting MQTT connection...");
      id2=millis();
      str2="EX" + String(id2);
      client.connect(str2.c_str(),USER.c_str(),PASS.c_str(), (Client_ID+"/Online").c_str(), 2, true, "0");
      PreTime = millis(); 
      StateMqtt = S_WAIT;
      break;
    case S_WAIT:
      if (client.connected())
      { 
      
        Serial.printf("\r\n[WIFI]MQTT connected to server");
        StateMqtt = S_CONNECTED;
      }
      else if (((uint32_t)(millis() - PreTime)) > TimeOut)
      {
        Serial.printf("\r\n[WIFI] MQTT connect failed, try to connect again, millis()-PreTime=%d", millis() - PreTime);
        StateMqtt = S_CONNECTING;
      }
      break;
    case S_CONNECTED:
      if (!client.connected())
      {
        Serial.printf("\r\n[WIFI] MQTT disconnected");
        StateMqtt = S_CONNECTING;
      }
      break;
  }
  return StateMqtt;
}

/**************************Ham publish len QMTT***************************/
void MQTT_WIFI::PublishData(String topic, String data)
{
  if (ReconnectWIFI() != S_CONNECTED) return;
  if (ReconnectMqtt() != S_CONNECTED) return;
  Serial.printf("\r\nPublish to mqtt, Topic=%s, messenger=%s", topic.c_str(), data.c_str());
  client.publish(topic.c_str() , data.c_str());}
void MQTT_WIFI::PublishData_Retain(String topic, String data)
{
  if (ReconnectWIFI() != S_CONNECTED) return;
  if (ReconnectMqtt() != S_CONNECTED) return;
  Serial.printf("\r\nPublish to mqtt, Topic=%s, messenger=%s", topic.c_str(), data.c_str());
  client.publish(topic.c_str() , data.c_str(), true);
}

void MQTT_WIFI::MQTT_ReceiveData()
{
  if (ReconnectWIFI() != S_CONNECTED) return;
  if (ReconnectMqtt() != S_CONNECTED) return;
  client.loop();
}
/**************************ham callback MQTT******************************/
String save_configure=""; extern String Client_ID;
void callback(char *topic, byte *payload, unsigned int length)//CHI NHAN DC BYTE HOAC UINT_8 (0-255)
{
  Serial.print("\r\nMessage arrived topic: ");
  Serial.print(topic);
  Serial.print("\r\n");
  //Serial.print("Payload: ");
  String StrTopic = String(topic);
  String msg;
  for (int i = 0; i < length; i++) 
  {
    //Serial.print((char)payload[i]);
    msg+=(char)payload[i];
  }
  if (StrTopic == (Client_ID+"/RESET")) //MySettings duoc khai bao extern tren dau file MQTT_WIWI.cpp
  {
    if(msg=="1")
    {
      Serial.print("\r\nRESET\r\n");
      ESP.restart();
    }
  }
  else if (StrTopic == (Client_ID+"/CHECK_WIFI")) //MySettings duoc khai bao extern tren dau file MQTT_WIWI.cpp
  {
    String new_nameWF="", new_passWF="";
    unsigned int i;
    unsigned int tach=0, index=0;
    String SubStr="";
    for(int i=0;i<length;i++)
    {  
      if(msg.charAt(i)=='\n'||msg.charAt(i)=='*')
      {
        SubStr=msg.substring(tach,i);
        tach=i+1; index++;
        if(index==1)
        {
          new_nameWF=SubStr;
          Serial.printf("\r\nnew_nameWF: ");Serial.print(SubStr);
        }
        else if(index==2)
        {
          new_passWF=SubStr;
          Serial.printf("\r\npass_nameWF: ");Serial.print(SubStr);
        }
      }
    }
    WiFi.disconnect();
    delay(3000);
    int time_wait=millis();;
    int time_current=0;
    WiFi.begin(SSID_WIFI.c_str(), PASS_WIFI.c_str());
    while(WiFi.status() != WL_CONNECTED) 
    {
      delay(500);
      time_current=millis();
      if(time_current-time_wait>12000)
      break;
    }
    if (WiFi.status() == WL_CONNECTED)
     {
       Serial.printf("\r\n[WIFI] Check done: can connect");
      SSID_WIFI=new_nameWF;
      PASS_WIFI=new_passWF;
     }
     else
     ESP.restart();
  }
  else if (StrTopic == (Client_ID+"/INFOR")) //MySettings duoc khai bao extern tren dau file MQTT_WIWI.cpp
  {
    if(msg=="1")
    {
       client.publish((Client_ID+"/FB").c_str() , ("Node online.\r\nConected to wifi "+ SSID_WIFI + ".\r\nReady for recieve configure.\r\n").c_str());
    }
  }
  else if (StrTopic == (Client_ID+"/PING")) //MySettings duoc khai bao extern tren dau file MQTT_WIWI.cpp
  {
    if(msg=="1")
    {
       client.publish((Client_ID+"/FB").c_str() , ("Node online.\r\nConected to wifi "+ SSID_WIFI + ".\r\nReady for recieve configure.\r\n").c_str());
    }
  }
  else if (StrTopic == (Client_ID+"/Configure")) //MySettings duoc khai bao extern tren dau file MQTT_WIWI.cpp
  {
    unsigned int tach=0, index=0;
    String SubStr="";
    unsigned int leng=msg.length();
    if(msg=="AAA")
    {
      save_configure="";
      //Serial.printf("\r\nStart configure");
    }
    else if(msg=="###")
    {
      save_configure="1"+save_configure;
      MySettings.SaveAllSetting(&save_configure);
      Serial.printf("\r\nReply setting done");
      //Serial.printf("\r\nFrame: \r\n"); Serial.print(save_configure);
      client.publish((Client_ID+"/FB").c_str() , "Node recieved configure.");
      delay(3000);
      Serial.print("\r\nRESET\r\n");
      ESP.restart();
    }
    else
    {
      save_configure+=msg;
      //Serial.printf("\r\nadd save_configure");
    } 
  }
}
