#include "Settings.h"
#include "esp_system.h"
#include <WiFi.h>
extern hw_timer_t *timer;
long int wdtTimeout;
String line="";
Settings::Settings()
{
}
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

String data_modbus, Client_ID="0000";
int time_poll=10;
String LocalIP[4],GatewayIP[4], Subnet, PrimaryDNS, SecondaryDNS;
int our_board_is_master=false;
extern int id_modbus;
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
  //Serial.printf("\r\nSetting length=%d", leng);
  //Serial.printf("\r\nFrame: "); Serial.print(s);
    for(int i=0;i<leng;i++)
    {  
      if(s.charAt(i)=='\n'||s.charAt(i)=='*')
      {
        SubStr=s.substring(tach,i);
        tach=i+1; index++;
        if(index==1)
        {
          SubStr.remove(0,2);
          ModbusBaud=SubStr;
          Serial.printf("\r\nModbus baud: ");Serial.print(ModbusBaud);
        }
        else if(index==2)
        {
          ModbusUARTConfig=SubStr.toInt();
          Serial.printf("\r\nUARTconfigure: %d",ModbusUARTConfig);
        }
        else if(index==3)
        {
          Serial.printf("\r\nLocalIP: "); Serial.print(SubStr);
          SubStr=SubStr + ",";
          unsigned int tach2=0, index2=0;
          String SubStr2="";
          unsigned int leng2=SubStr.length();
          Serial.printf("\r\nSubStr length=%d", leng2);
          for(int i2=0;i2<leng2;i2++)
            {  
              if(SubStr.charAt(i2)==',')
              {
                SubStr2=SubStr.substring(tach2,i2);
                tach2=i2+1; 
                index2++;
                if(index2==1)
                {
                  LocalIP[0]=SubStr2;
                }
                else if(index2==2)
                {
                  LocalIP[1]=SubStr2;
                }
                else if(index2==3)
                {
                 LocalIP[2]=SubStr2;  
                }
                else if(index2==4)
                {
                 LocalIP[3]=SubStr2;  
                }
              }   
           }          
        }
        else if(index==4)
        {
          //GatewayIP=SubStr;
          Serial.printf("\r\nGatewayIP: "); Serial.print(SubStr);
          SubStr=SubStr + ",";
          unsigned int tach2=0, index2=0;
          String SubStr2="";
          unsigned int leng2=SubStr.length();
          Serial.printf("\r\nSubStr length=%d", leng2);
          for(int i2=0;i2<leng2;i2++)
            {  
              if(SubStr.charAt(i2)==',')
              {
                SubStr2=SubStr.substring(tach2,i2);
                tach2=i2+1; 
                index2++;
                if(index2==1)
                {
                  GatewayIP[0]=SubStr2;
                }
                else if(index2==2)
                {
                  GatewayIP[1]=SubStr2;
                }
                else if(index2==3)
                {
                 GatewayIP[2]=SubStr2;  
                }
                else if(index2==4)
                {
                 GatewayIP[3]=SubStr2;  
                }
              }   
           }
        }
        else if(index==5)
        {
          Subnet=SubStr;
          Serial.printf("\r\nSubnet: "); Serial.print(SubStr);
        }
        else if(index==6)
        {
          PrimaryDNS=SubStr;
          Serial.printf("\r\nPrimaryDNS: "); Serial.print(SubStr);
        }
        else if(index==7)
        {
          SecondaryDNS=SubStr;
          Serial.printf("\r\nSecondaryDNS: "); Serial.print(SubStr);
        }
         else if(index==8)
        {
          our_board_is_master=SubStr.toInt();
          if(our_board_is_master) Serial.printf("\r\nOur board is master");
          else Serial.printf("\r\nOur board is slave"); 
          //Serial.printf("\r\nki tu");Serial.print(SubStr);
        }
        else if(index==9)
        {
         id_modbus=SubStr.toInt();
         Serial.printf("\r\nID modbus: %d", id_modbus);
        }
        else if(index==10)
        {
          time_poll=SubStr.toInt();
          Serial.printf("\r\ntime_poll: %d",time_poll);
          data_modbus=s.substring(i+1,s.length()+1);
          //Serial.printf("\r\nData modbus: \r\n");
          //Serial.print(data_modbus);
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
