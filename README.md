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

#### Blink  

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
You can find it in examples: [Download](https://github.com/NamNamIoT/ESP32/tree/master/examples)  
#### Modbus_Slave
You can find it in examples: [Download](https://github.com/NamNamIoT/ESP32/tree/master/examples)  
#### 3. Read modbus slave via TCP/IP  
You can find it in examples: [Download](https://github.com/NamNamIoT/ESP32/tree/master/examples)  
#### 4. Serve master RTU (board is slave)  
You can find it in examples: [Download](https://github.com/NamNamIoT/ESP32/tree/master/examples)  
#### 5. Serve master TCP/IP (board is slave)  
You can find it in examples: [Download](https://github.com/NamNamIoT/ESP32/tree/master/examples)  
#### 6. Auto switch WiFi-Ethernet  
You can find it in examples: [Download](https://github.com/NamNamIoT/ESP32/tree/master/examples)  
#### 7. Auto switch many WiFi in list  
You can find it in examples: [Download](https://github.com/NamNamIoT/ESP32/tree/master/examples)  
#### 8. Push value modbus to MQTT  
You can find it in examples: [Download](https://github.com/NamNamIoT/ESP32/tree/master/examples)  
#### 9. On/off led via MQTT  
You can find it in examples: [Download](https://github.com/NamNamIoT/ESP32/tree/master/examples)  
#### 10. FOTA - update firmware via Internet with FTP  
You can find it in examples: [Download](https://github.com/NamNamIoT/ESP32/tree/master/examples)  
#### 11. WATCHDOG timmer  
You can find it in examples: [Download](https://github.com/NamNamIoT/ESP32/tree/master/examples)  
#### 12. Deep sleep  
You can find it in examples: [Download](https://github.com/NamNamIoT/ESP32/tree/master/examples)  

