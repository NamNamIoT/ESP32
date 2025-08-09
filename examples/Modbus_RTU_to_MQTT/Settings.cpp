#include "Settings.h"
#include "esp_system.h"
#include <WiFi.h>
extern hw_timer_t *timer;
long int wdtTimeout;
Settings::Settings()
{
}
String line="";
void Settings::Configure()
{
   while(Serial.available())
    {  
      line+=(char)Serial.read();
      if(line[line.length()-1]=='^')
      {
       line=""; 
      }
      if(line[line.length()-1]=='~')
      {
       ESP.restart();
      }
      if(line[line.length()-1]=='$')
      {
        if(line.length()>=20)
        {
          line="1"+line;
          SaveAllSetting(&line);
          line="";
          Serial.print("Done#####");
          delay(100);
          ESP.restart();
        }
      }
    }     
}
void Settings::Init()
{
   bool val=EEPROM.begin(EEPROM_SIZE); //EEPROM_SIZE byte 
   if(val !=true) Serial.printf("\nEEPROM, Init error");
}
/*****************luu mot gia tri Value vao EEPROM**********************************/
void Settings::SaveValue(int Address ,char Value)
{
  EEPROM.write(Address, Value);
  EEPROM.commit();
}
/****************lay 1 giai tri tu EEPROM ra ngoai**********************************/
char Settings::RestoreValue(int Address)
{
   return( EEPROM.read(Address));
}
/******************restore toan bo du lieu tu EEPROM ra bien String************************/
char Settings::RestoreEeprom(String *p_str)
{
  int i=0;
  char m_array=0;
  unsigned char tmp= EEPROM.read(0);
  Serial.println();
  if(tmp !='1') //chua co setting trong bo nho
   {
      (*p_str)="";
      Serial.printf("\r\nNot yet have setting in device");
      return 0;
   }
  else
  {
    
  }
  for(i=0;i<EEPROM_SIZE-1;i++)
  {
    m_array=RestoreValue(i);
    if( m_array !=0)
    {
      (*p_str) +=" "; //(*p_str)[i]= m_array;
      (*p_str).setCharAt(i,m_array);
    }
    else
    {
      return 0;
    }    
  }
   return 1;
}
/*******************luu toan bo setting vao EEPROM**********************************/
char Settings::SaveAllSetting(String *p_str)
 {
  uint32_t len=(*p_str).length(); 
  if(len>=EEPROM_SIZE) 
  {
    Serial.printf("\r\nSaveAllSetting Error, Out of memory");
    Serial.printf("\r\nSaveAllSetting length=%d", len);
    return 1; // day bo nho EEPROM
  }
  else 
  Serial.printf("\r\nSaveAllSetting length=%d\r\n", len);
  uint32_t i=0;
  for(i=0;i<len;i++) 
  {
    EEPROM.write(i, (*p_str).charAt(i));
    //Serial.print((*p_str).charAt(i));
  }
  EEPROM.write(i, 0);
  EEPROM.commit();
  return 0;
}

extern int PORT;
extern String SERVER;
extern String USER;
extern String PASS;
extern String SSID_WIFI;
extern String PASS_WIFI;
String data_modbus, Client_ID="000000";
int time_poll=10;
/******************restore toan bo cac setting tu EEPROM************************/
void Settings::RestoreSettings()
{
  String s="";
  RestoreEeprom(&s);
  if(s == "") return;
  unsigned int i;
  unsigned int tach=0, index=0;
  String SubStr="";
  unsigned int leng=s.length();
  Serial.printf("\r\nSetting length=%d", leng);
    for(int i=0;i<leng;i++)
    {  
      if(s.charAt(i)=='\n'||s.charAt(i)=='*')
      {
        SubStr=s.substring(tach,i);
        tach=i+1; index++;
        if(index==1)
        {
          SubStr.remove(0,1);
          SSID_WIFI=SubStr;
          Serial.printf("\r\nName wifi: ");Serial.print(SubStr);
        }
        else if(index==2)
        {
          PASS_WIFI=SubStr;
          Serial.printf("\r\npass wifi: ");Serial.print(SubStr);
        }
        else if(index==3)
        {
          SERVER=SubStr;
          Serial.printf("\r\nmqtt_server: ");Serial.print(SubStr);
        }
        else if(index==4)
        {
          PORT=SubStr.toInt();
          Serial.printf("\r\nport_mqtt: ");Serial.print(SubStr);
        }
        else if(index==5)
        {
          USER=SubStr;
          Serial.printf("\r\nssid_mqtt: ");Serial.print(SubStr);
        }
        else if(index==6)
        {
          PASS=SubStr;
          Serial.printf("\r\npassword_mqtt: ");Serial.print(SubStr);
        }
        else if(index==7)
        {
          ModbusBaud=SubStr;
          Serial.printf("\r\nModbus baud: ");Serial.print(ModbusBaud);
        }
        else if(index==8)
        {
          ModbusUARTConfig=SubStr.toInt();
          Serial.printf("\r\nUARTconfigure: %d",ModbusUARTConfig);
        }
        else if(index==9)
        {
          //Client_ID="000001";
          Client_ID=SubStr;
          Serial.printf("\r\nClient_ID: "); Serial.print(Client_ID);
        }
        else if(index==10)
        {
          time_poll=SubStr.toInt();
          Serial.printf("\r\ntime_poll: %d",time_poll);
          data_modbus=s.substring(i+1,s.length()+1)+"\n";
          Serial.printf("\r\nData modbus: \r\n");
          Serial.print(data_modbus);
        }
      }
    }
    Serial.println("");
}


/***********************************************************************************/
int Settings::getRSSIasQuality(int RSSI) 
{ 
  int quality = 0;

  if (RSSI <= -100) 
  {
    quality = 0;
  } 
  else if (RSSI >= -50) 
  {
    quality = 100;
  } 
  else 
  {
    quality = 2 * (RSSI + 100);
  }
  return quality;
}
/************************ham Scan WIFI co san***************************************/
void Settings::ScanWiFI(String *StrSend)
{
  signed char n=WiFi.scanNetworks();
  if(n !=0){
    signed char indices[n];
    for(unsigned char i=0;i<n; i++){
      indices[i]=i;
    }
  //RSSI soft
  for(unsigned char i=0;i<n;i++){
    for(unsigned char j=i+1;j<n;j++){
      if(WiFi.RSSI(indices[j]) > WiFi.RSSI(indices[i])){
        std::swap(indices[i], indices[j]);}
    }
  }
  //remove duplicates -1
  String cssid="";
  for(unsigned char i=0;i<n;i++){
    if(indices[i] == -1) continue;
    cssid = WiFi.SSID(indices[i]);
    for(unsigned char j=i+1; j<n;j++){
      if(cssid== WiFi.SSID(indices[j]))
      indices[j]=-1;
    }
  }
  //put string ssid
  for(unsigned char i=0;i<n;i++){
  if(indices[i]==-1) continue;
  int quality= getRSSIasQuality(WiFi.RSSI(indices[i]));
  if(quality>=8){
    (*StrSend) +="\n";
    (*StrSend) +=WiFi.SSID(indices[i]);
    //(*StrSend) +=", RSSI: "+ String(WiFi.RSSI(indices[i]));
     }
   }
 }
}
