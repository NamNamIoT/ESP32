<a href="https://www.tindie.com/stores/thanhnamlt5/?ref=offsite_badges&utm_source=sellers_thanhnamlt5&utm_medium=badges&utm_campaign=badge_large"><img src="https://d2ss6ovg47m0r5.cloudfront.net/badges/tindie-larges.png" alt="I sell on Tindie" width="200" height="104"></a>
# ESP32 Module Quick Start Guide

## ✌️✌️ Welcome! ✌️✌️  
Hi, im Namnam 🕴🏼.  
<p align="left">
<a href="https://fb.com/kuem0912" target="blank"><img align="center" src="https://raw.githubusercontent.com/rahuldkjain/github-profile-readme-generator/master/src/images/icons/Social/facebook.svg" alt="kuem0912" 
height="30" width="40" /></a>
<a href="https://wa.me/84969809444" target="blank"><img align="center" src="https://raw.githubusercontent.com/rahuldkjain/github-profile-readme-generator/master/src/images/icons/Social/whatsapp.svg" alt="84969809444" height="30" width="40" /></a>
</p>


### Information ESP32 board  
  
#### 🕵🏻‍♀️Version 1.0  
|     **Object**    |      **Detail conponent**      |          **Note**     |
| ----------------- | ------------------------------ | ----------------------|
|        Power      | MC34063A                       | 5-35VDC               |
|       Ethernet    | LAN8720A  (RJ45: HR911105A)    | Ethernet RMII 10/100M  |
|        RS485      | SP485EE                        | Serial2, auto DERE    |
|         I2C       | Pull up 4.7k                   | SDA = IO33, SCL = IO32 |
|      Led blue     | -                              | PIN IO15              |
|      Led yellow   | -                              | PIN IO2               |
|       Button      | -                              | PIN IO36              |

🚀🚀🚀Download examples: 🐞[Download](https://github.com/NamNamIoT/ESP32/tree/master/examples)🐞  
[Blink led](#Blink-led)  
[Read modbus slave via RTU](#Modbus-slave-RTU)  
[Read modbus slave via TCP/IP](#Modbus-slave-TCP/IP)  
[Serve master RTU](#Modbus-master-RTU)  
[Serve master TCP/IP](#Modbus-master-TCP/IP)  
[Auto switch WiFi_Ethernet](#Auto-switch-WiFi-Ethernet)  
[Auto switch many WiFi in list](#Auto-switch-many-WiFi-in-list)  
[Push value modbus to MQTT](#Push-value-modbus-to-MQTT)  
[On/off led via MQTT](#On/off-led-via-MQTT)  
[FOTA - update firmware via Internet with FTP](#FOTA-update-firmware-via-Internet-with-FTP)  
[WATCHDOG timmer](#WATCHDOG-timmer)  
[Deep sleep](#Deep-sleep)  

#### Blink led  

```c
#define LED_YELLOW 2 //Pin led color yellow is IO2
void setup() {
  // initialize digital pin LED_YELLOW as an output.
  pinMode(LED_YELLOW, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(LED_YELLOW, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LED_YELLOW, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
}
```

#### Modbus slave RTU

```c
#include <ModbusRTU.h>
#define DERE_PIN -1
ModbusRTU mb;
void setup() {
  Serial2.begin(9600, SERIAL_8N1, 16, 17); // RX2=16, TX2=17
  pinMode(DERE_PIN, OUTPUT);
  mb.begin(&Serial2, DERE_PIN);
  mb.slave(1); // ID slave is 1
  mb.addCoil(0, false); // Ex coil
}
void loop() {
  mb.task();
}
```

```c
#include <ModbusRTU.h>
#define DERE_PIN -1
ModbusRTU mb;
void setup() {
  Serial2.begin(9600, SERIAL_8N1, 16, 17);
  pinMode(DERE_PIN, OUTPUT);
  mb.begin(&Serial2, DERE_PIN);
  mb.slave(2); // ID slave is 2
  mb.addHreg(100, 123); // Holding register
}
void loop() {
  mb.task();
}
```
#### Modbus slave TCP/IP

```c
#include <ETH.h>
#include <ModbusIP_ESP8266.h>
ModbusIP mb;
void setup() {
  ETH.begin(); // LAN8720A Ethernet
  mb.server(); // Run as Modbus TCP server (slave)
  mb.addCoil(0, false);
}
void loop() {
  mb.task();
}
```

```c
#include <ETH.h>
#include <ModbusIP_ESP8266.h>
ModbusIP mb;
void setup() {
  ETH.begin();
  mb.server();
  mb.addHreg(100, 456);
}
void loop() {
  mb.task();
}
```

#### Modbus master RTU

```c
#include "Canopus_Modbus.h"
ModbusMaster node;

void setup()
{
  Serial.begin(115200);
  Serial.print("\r\n*****************CANOPUS BOARD*******************");
  Serial_Canopus.begin(9600, SERIAL_8N1); 
  node.begin(1, Serial_Canopus); //ID node 1
}
void loop()
{ 
  Serial.println("");
  Serial.println("Wait Read");
  uint8_t result;
  result = node.readHoldingRegisters(1, 3);//Read 40001, 40002, 40003
  delay(10);
  if (result == node.ku8MBSuccess) //Read ok
  {
    uint16_t data[3];
    data[0]=node.getResponseBuffer(0);
    data[1]=node.getResponseBuffer(1);
    data[2]=node.getResponseBuffer(2);
    Serial.printf("\r\nValue 40001: %d",data[0]);
    Serial.printf("\r\nValue 40002: %d",data[1]);
    Serial.printf("\r\nValue 40003: %d",data[2]);
  }
  else Serial.print("Read Fail");
  delay(500); 
}
```

#### Modbus master TCP/IP

```c
#include <WiFi.h>
#include "ETH_MB.h"
#include "ModbusIP_ESP8266.h"
ModbusIP mb;
extern bool eth_connected;
  
void setup() {
  pinMode(14, OUTPUT);
  digitalWrite(14, HIGH); 
  delay(1000);
  Serial.begin(115200);
  Serial.println("\r\nHello\r\n");
  if(WiFi.status() != WL_CONNECTED&&!eth_connected)
    {
      IPAddress local_IP(192, 168, 1, 123);
      IPAddress gateway(192, 168, 1, 1);
      IPAddress subnet(255, 255, 255, 0);
      IPAddress primaryDNS(8, 8, 8, 8); //optional
      IPAddress secondaryDNS(8, 8, 4, 4); //optional
      if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
        Serial.printf("\r\nWiFi Failed to configure");
        }
    }
  ETH_begin();
  delay(5000);
  WiFi.begin("TEN_WIFI", "PASS_WIFI");
  while (WiFi.status() != WL_CONNECTED||eth_connected) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  if(!eth_connected)
  Serial.println(WiFi.localIP());
  else
  Serial.println(ETH.localIP());
  mb.begin();
  mb.addHreg(2, 0);//FC03 040002
  mb.addIreg(2, 0);//FC04 030002
}
 
void loop() {
   mb.task();
   mb.Hreg(2, 1);//Write value 1 to 040002
   mb.Ireg(2, 1);//Write value 1 to 030002
   delay(500);
   mb.Hreg(2, 0);//Write value 0 to 040002
   mb.Ireg(2, 1);//Write value 0 to 030002
   delay(500);
}
```

#### Auto switch WiFi Ethernet  

```c
#include <WiFi.h>
#include <ETH.h>
const char* ssid = "wifi1";
const char* password = "password";
bool isEthernet = false;
void setup() {
  Serial.begin(115200);
  ETH.begin();
  delay(1000);
  isEthernet = ETH.linkUp();
  if (!isEthernet) {
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) delay(500);
    Serial.println("WiFi connected");
  } else {
    Serial.println("Ethernet connected");
  }
}
void loop() {
  // Check connect
  if (ETH.linkUp() && !isEthernet) {
    isEthernet = true;
    WiFi.disconnect();
    Serial.println("Switched to Ethernet");
  } else if (!ETH.linkUp() && isEthernet) {
    isEthernet = false;
    WiFi.begin(ssid, password);
    Serial.println("Switched to WiFi");
  }
  delay(1000);
}
```

#### Auto switch many WiFi in list  

```c
#include <WiFi.h>
const char* ssidList[] = {"wifi1", "wifi2", "wifi3"};
const char* passList[] = {"pass1", "pass2", "pass3"};
void setup() {
  Serial.begin(115200);
  for (int i = 0; i < 3; i++) {
    WiFi.begin(ssidList[i], passList[i]);
    int timeout = 0;
    while (WiFi.status() != WL_CONNECTED && timeout < 10) {
      delay(500);
      timeout++;
    }
    if (WiFi.status() == WL_CONNECTED) {
      Serial.print("Connected to: ");
      Serial.println(ssidList[i]);
      break;
    }
  }
}
void loop() {
  // Other fn
}
```

#### Push value modbus to MQTT  

```c
#include <ModbusRTU.h>
#include <WiFi.h>
#include <PubSubClient.h>
#define DERE_PIN -1
ModbusRTU mb;
WiFiClient espClient;
PubSubClient client(espClient);
const char* ssid = "wifi1";
const char* password = "password";
const char* mqtt_server = "broker.hivemq.com";
void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(500);
  client.setServer(mqtt_server, 1883);
  Serial2.begin(9600, SERIAL_8N1, 16, 17);
  mb.begin(&Serial2, DERE_PIN);
  mb.slave(1);
  mb.addHreg(100, 0);
}
void loop() {
  mb.task();
  int value = mb.Hreg(100);
  if (!client.connected()) client.connect("ESP32Client");
  String payload = String(value);
  client.publish("esp32/modbus", payload.c_str());
  delay(5000);
}
```

#### On/off led via MQTT  

```c
#include <WiFi.h>
#include <PubSubClient.h>
#define LED_YELLOW 2
WiFiClient espClient;
PubSubClient client(espClient);
const char* ssid = "wifi1";
const char* password = "password";
const char* mqtt_server = "broker.hivemq.com";
void callback(char* topic, byte* payload, unsigned int length) {
  if (payload[0] == '1') digitalWrite(LED_YELLOW, HIGH);
  else digitalWrite(LED_YELLOW, LOW);
}
void setup() {
  pinMode(LED_YELLOW, OUTPUT);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(500);
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  client.subscribe("esp32/led");
}
void loop() {
  client.loop();
}  
```

#### FOTA update firmware via Internet with FTP  

```c
#include <WiFi.h>
#include <FS.h>
#include "FtpClientUpdate.h"
const char *ssid = "N-ELEKTRIK";
const char *password = "namnam_wifi";
const char *HwVersion = "1.0";
const char *FwVersion = "1.0";
String host = "194.59.164.44";
String user = "u281006516";
String pass = "mypass";
String path = "V2.bin"; //V2.bin is name bin file
extern String md5;

#define DEBUG(fmt, ...) Serial.printf_P(PSTR("\r\nP" fmt) ,##__VA_ARGS__)
boolean debug = false;  // true = more messages
// provide text for the WiFi status
const char *str_status[] = {
  "WL_IDLE_STATUS",
  "WL_NO_SSID_AVAIL",
  "WL_SCAN_COMPLETED",
  "WL_CONNECTED",
  "WL_CONNECT_FAILED",
  "WL_CONNECTION_LOST",
  "WL_DISCONNECTED"
};
// provide text for the WiFi mode
const char *str_mode[] = { "WIFI_OFF", "WIFI_STA", "WIFI_AP", "WIFI_AP_STA" };

// change to your server
IPAddress server(185,224,138,17);

// change fileName to your file (8.3 format!)
String fileName = "BaseStation/ftpupdateV1.bin";

//----------------------- WiFi handling
void connectWifi() {
  Serial.print("Connecting as wifi client to SSID: ");
  Serial.println(ssid);
  WiFi.disconnect();
  if (WiFi.getMode() != WIFI_STA) {
    WiFi.mode(WIFI_STA);
  }

  WiFi.begin ( ssid, password );
  if (debug ) WiFi.printDiag(Serial);
  unsigned long startTime = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startTime < 10000) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("WiFi connected; IP address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.print("WiFi connect failed to ssid: ");
    Serial.println(ssid);
    Serial.print("WiFi password <");
    Serial.print(password);
    Serial.println(">");
    Serial.println("Check for wrong typing!");
  }
}  // connectWiFi()
void setup() {
  delay(1000);
  Serial.begin(115200);
  delay(1000);
  Serial.printf("\r\nN-ELEKTRIK");
  delay(500);
  Serial.println();

  Serial.printf_P(PSTR("\r\nHwVersion: %s"),HwVersion);
  DEBUG("FwVersion: %s",FwVersion);
  Serial.println ( "Connect to Router requested" );
  connectWifi();
  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("WiFi mode: ");
    Serial.println(str_mode[WiFi.getMode()]);
    Serial.print ( "Status: " );
    Serial.println (str_status[WiFi.status()]);
  } else {
    Serial.printf("\r\n");
    Serial.printf("\r\nWiFi connect failed, push RESET button.");
  }
  Serial.printf("\r\nReady. Press N if u want update");
  Serial.printf("\r\nN-ELEKTRIK");
}

void loop() {
  byte inChar;
  if (Serial.available() > 0) {
    inChar = Serial.read();
  }
  if (inChar == 'N') {    
    if (FtpClientUpdate.update(host,user,pass,path,md5)) {
      Serial.println(F("FTP with N-ELEKTRIK done"));
      delay(1000);
      ESP.restart();
    }
    else Serial.println(F("FTP FAIL"));
  }
  delay(10);
}
```

#### WATCHDOG timmer  

```c
#include <esp_task_wdt.h>
#define WDT_TIMEOUT 5
void setup() {
  Serial.begin(115200);
  esp_task_wdt_init(WDT_TIMEOUT, true);
  esp_task_wdt_add(NULL);
}
void loop() {
  esp_task_wdt_reset(); // Reset watchdog
  delay(1000);
}
```

#### Deep sleep  

```c
#define uS_TO_S_FACTOR 1000000
#define TIME_TO_SLEEP 60 // 60 giây
void setup() {
  Serial.begin(115200);
  Serial.println("ESP32 deep sleep 60s...");
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  Serial.println("enter deep sleep...");
  delay(1000);
  esp_deep_sleep_start();
}
void loop() {
}

