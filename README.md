# ESP32 Module Quick Start Guide

## ✌️✌️ Welcome! ✌️✌️  
Hi, im Namnam 🕴🏼.  
Please contact me if you need to develop ioT products.  
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
|       Ethernet    | LAN8720A  (RJ45: HR911105A)    | Ethernet RMII 10/100  |
|        RS485      | SP485EE                        | Serial2, auto DERE    |
|         I2C       | Pull up 4.7k                   | SDA = IO33, SCL = IO4 |
|      Led blue     | -                              | PIN IO15              |
|      Led yellow   | -                              | PIN IO2               |
|       Button      | -                              | PIN IO36              |

Download examples: [Download](https://github.com/NamNamIoT/ESP32/tree/master/examples)  
#### [Blink led](#Blink)
#### [Read modbus slave via RTU](#Modbus_Slave)
Read modbus slave via TCP/IP  
Serve master RTU  
Serve master TCP/IP  
Auto switch WiFi-Ethernet  
Auto switch many WiFi in list  
Push value modbus to MQTT  
On/off led via MQTT  
FOTA - update firmware via Internet with FTP  
WATCHDOG timmer  
Deep sleep  

#### 1. Blink  

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

#### 2. Modbus_Slave

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

#### 3. Read modbus slave via TCP/IP  

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

#### 4. Serve master RTU (board is slave)  

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

#### 5. Serve master TCP/IP (board is slave)  

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

#### 6. Auto switch WiFi-Ethernet  

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

#### 7. Auto switch many WiFi in list  

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

#### 8. Push value modbus to MQTT  

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

#### 9. On/off led via MQTT  

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

#### 10. FOTA - update firmware via Internet with FTP  

```c
Update late
```

#### 11. WATCHDOG timmer  

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

#### 12. Deep sleep  

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

