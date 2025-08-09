# ESP32_Canopus Module Quick Start Guide

## ✌️✌️ Welcome! ✌️✌️  
Hi, im Namnam 🕴🏼.  
Please contact me if you need to develop ioT products.  
<p align="left">
<a href="https://fb.com/kuem0912" target="blank"><img align="center" src="https://raw.githubusercontent.com/rahuldkjain/github-profile-readme-generator/master/src/images/icons/Social/facebook.svg" alt="kuem0912" 
height="30" width="40" /></a>
<a href="https://wa.me/84969809444" target="blank"><img align="center" src="https://raw.githubusercontent.com/rahuldkjain/github-profile-readme-generator/master/src/images/icons/Social/whatsapp.svg" alt="84969809444" height="30" width="40" /></a>
</p>


### Information ESP32_Canopus board  
  
#### 🕵🏻‍♀️Version 1.0  
|     **Object**    |      **Detail conponent**      |          **Note**     |
| ----------------- | ------------------------------ | ----------------------|
|        Power      | MC34063A                       | 5-35VDC               |
|       Ethernet    | LAN8720A  (RJ45: HR911105A)    | -                     |
|        RS485      | SP485EE                        | Serial2, auto DERE    |
|         I2C       | -                              | SDA = IO33, SCL = IO4 |
|      Led blue     | -                              | PIN IO15              |
|      Led yellow   | -                              | PIN IO2               |
|       Button      | -                              | PIN IO36              |
|       ESP32       | ESP32 WROOM 32D                | -                     |
|       Case        | Plastic                        | Mount dinrail         |  

#### 🧑🏻‍🔧 Hardware Setup 
-Cable USB type C for power and upload code.  
-Power 5-24VDC.  
  There's really no need because we used 5V power via type C cable.  
  Both type C cable and power supply via terminal can be connected at the same time.  
-Cable ethernet, Use for ethernet demos.  

#### 🧑🏼‍💻 Software Setup
+Install IDE arduino. Follow [this](https://support.arduino.cc/hc/en-us/articles/360019833020-Download-and-install-Arduino-IDE), skip if you done.  
##### ⚠️ WARNING: Install in Document folder.  
![image](https://user-images.githubusercontent.com/49629370/235334499-7e59c503-6423-4381-9ae5-eeccecb57699.png)  
+Install driver CH340. Follow [this](https://electropeak.com/learn/how-to-install-ch340-driver/), skip if you done.  
+Install ESP32 in manager board:
  - Down load this folder: [hardware](https://mega.nz/file/e3wlwIAI#vFoR5nT5x3zT5wkQSVknqihluzf_9Ng89H5CxPmu3Io)  
  - Extract at ...\Documents\Arduino. Ex my computer: C:\Users\NamPC\Documents\Arduino  
  - Go to ...\Documents\Arduino\hardware\espressif\esp32\tools  
  - Click get and gen:  
  ![ESP32 install](https://user-images.githubusercontent.com/49629370/235330978-d56cb96c-8032-4c83-995e-6e6720e585da.png)
  + Check it in IDE:  
  ![image](https://user-images.githubusercontent.com/49629370/235331088-db8737b9-3053-4350-bb5f-e3c348a513bb.png)  
    
    
## At here , you are ready 👌 to execute the sample code for the ESP32_CANOPUS board  
####  ⚠️ WARNING: Prepare yourself a cup of coffee ☕☕☕, because the next is a fascinating journey with Canopus board 🤪.   
You will be stronger after working through the next 12 examples.  
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
You can find it in examples: [Download](https://github.com/NamNamIoT/ESP32_CANOPUS/tree/master/examples)  
#### Modbus_Slave
You can find it in examples: [Download](https://github.com/NamNamIoT/ESP32_CANOPUS/tree/master/examples)  
#### 3. Read modbus slave via TCP/IP  
You can find it in examples: [Download](https://github.com/NamNamIoT/ESP32_CANOPUS/tree/master/examples)  
#### 4. Serve master RTU (board is slave)  
You can find it in examples: [Download](https://github.com/NamNamIoT/ESP32_CANOPUS/tree/master/examples)  
#### 5. Serve master TCP/IP (board is slave)  
You can find it in examples: [Download](https://github.com/NamNamIoT/ESP32_CANOPUS/tree/master/examples)  
#### 6. Auto switch WiFi-Ethernet  
You can find it in examples: [Download](https://github.com/NamNamIoT/ESP32_CANOPUS/tree/master/examples)  
#### 7. Auto switch many WiFi in list  
You can find it in examples: [Download](https://github.com/NamNamIoT/ESP32_CANOPUS/tree/master/examples)  
#### 8. Push value modbus to MQTT  
You can find it in examples: [Download](https://github.com/NamNamIoT/ESP32_CANOPUS/tree/master/examples)  
#### 9. On/off led via MQTT  
You can find it in examples: [Download](https://github.com/NamNamIoT/ESP32_CANOPUS/tree/master/examples)  
#### 10. FOTA - update firmware via Internet with FTP  
You can find it in examples: [Download](https://github.com/NamNamIoT/ESP32_CANOPUS/tree/master/examples)  
#### 11. WATCHDOG timmer  
You can find it in examples: [Download](https://github.com/NamNamIoT/ESP32_CANOPUS/tree/master/examples)  
#### 12. Deep sleep  
You can find it in examples: [Download](https://github.com/NamNamIoT/ESP32_CANOPUS/tree/master/examples)  

