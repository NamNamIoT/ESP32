#include "kitdevesp32.h"
ModbusMaster node;
#include <WiFi.h>
#include <ETH.h>
static bool eth_connected = false;

void WiFiEvent(WiFiEvent_t event)
{
  switch (event) {
    case SYSTEM_EVENT_ETH_START:
      Serial.println("ETH Started");
      //set eth hostname here
      ETH.setHostname("esp32-ethernet");
      break;
    case SYSTEM_EVENT_ETH_CONNECTED:
      Serial.println("ETH Connected");
      break;
    case SYSTEM_EVENT_ETH_GOT_IP:
      Serial.print("ETH MAC: ");
      Serial.print(ETH.macAddress());
      Serial.print(", IPv4: ");
      Serial.print(ETH.localIP());
      if (ETH.fullDuplex()) {
        Serial.print(", FULL_DUPLEX");
      }
      Serial.print(", ");
      Serial.print(ETH.linkSpeed());
      Serial.println("Mbps");
      eth_connected = true;
      break;
    case SYSTEM_EVENT_ETH_DISCONNECTED:
      Serial.println("ETH Disconnected");
      eth_connected = false;
      break;
    case SYSTEM_EVENT_ETH_STOP:
      Serial.println("ETH Stopped");
      eth_connected = false;
      break;
    default:
      break;
  }
}

void testClient(const char * host, uint16_t port)
{
  Serial.print("\nconnecting to ");
  Serial.println(host);

  WiFiClient client;
  if (!client.connect(host, port)) {
    Serial.println("connection failed");
    return;
  }
  client.printf("GET / HTTP/1.1\r\nHost: %s\r\n\r\n", host);
  while (client.connected() && !client.available());
  while (client.available()) {
    Serial.write(client.read());
  }

  Serial.println("closing connection\n");
  client.stop();
}

void setup()
{
  Serial.begin(115200);
  Serial.print("\r\n*****************KIT DEV ESP32*******************");
  pinMode(14, OUTPUT);
  digitalWrite(14, HIGH);
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
  pinMode(15, OUTPUT);
  digitalWrite(15, HIGH);
  delay(1000);
  WiFi.onEvent(WiFiEvent);
  ETH.begin();
  Serial_kitdev.begin(9600, SERIAL_8N1); 
  node.begin(1, Serial_kitdev); //ID node 1
  
}
void loop()
{ 
  Serial.println("");
  Serial.println("Wait Read");
  uint8_t result;
  result = node.readInputRegisters(1, 3);//Read 30001, 30002, 30003
  delay(10);
  if (result == node.ku8MBSuccess) //Read ok
  {
    uint16_t data[3];
    data[0]=node.getResponseBuffer(0);
    data[1]=node.getResponseBuffer(1);
    data[2]=node.getResponseBuffer(2);
    Serial.printf("\r\nValue 30001: %d",data[0]);
    Serial.printf("\r\nValue 30002: %d",data[1]);
    Serial.printf("\r\nValue 30003: %d",data[2]);
  }
  else Serial.print("Read Fail");
//
//  result = node.readHoldingRegisters(1, 3);//Read 40001, 40002, 40003
//  delay(10);
//  if (result == node.ku8MBSuccess) //Read ok
//  {
//    uint16_t data[3];
//    data[0]=node.getResponseBuffer(0);
//    data[1]=node.getResponseBuffer(1);
//    data[2]=node.getResponseBuffer(2);
//    Serial.printf("\r\nValue 40001: %d",data[0]);
//    Serial.printf("\r\nValue 40002: %d",data[1]);
//    Serial.printf("\r\nValue 40003: %d",data[2]);
//  }
//  else Serial.print("Read Fail");
  delay(1000); 
}
