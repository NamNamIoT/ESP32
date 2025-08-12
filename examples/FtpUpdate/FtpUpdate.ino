#include <WiFi.h>
#include <FS.h>
#include "FtpClientUpdate.h"
const char *ssid = "N-ELEKTRIK";
const char *password = "namnam_wifi";
const char *HwVersion = "0.1";
const char *FwVersion = "0.1.1";
String host = "194.59.164.44";
String user = "u281006516";
String pass = "thanhnambk1";
String path = "/domains/nelektrik.tech/public_html/V10.bin"; //V10.bin is name file bin
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
