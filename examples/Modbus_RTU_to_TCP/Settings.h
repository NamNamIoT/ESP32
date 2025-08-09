#ifndef SETTING_H
#define SETTING_H
#include <Arduino.h>
#include <EEPROM.h>

#define EEPROM_SIZE 4096
#define BUTTON_SETTINGS -1

//dinh nghia struct thanh ghi modbus
typedef struct
{
  uint32_t Address;
  uint8_t Type;
  String Topic;
  String Value;
} ModbusReg;

typedef struct
{
 String ID;
 String Enable;
} Slave;
typedef struct
{
  String Topic;
  String Enable;
  String Value;
} IOInput;

class Settings
{
 public:
 long Add_MB[12];
 String ID_SLAVE[2];
 int type_address[12];
 bool cb[12];
 String ModbusBaud="9600";
 uint32_t ModbusUARTConfig=134217756;
 Settings();
 void Init();
 char SaveAllSetting(String *);
 void RestoreSettings();
 void Configure();
 int  getRSSIasQuality(int RSSI);
 char RestoreEeprom(String *);
 void ScanWiFI(String *);
 private:
 void SaveValue(int ,char );
 char RestoreValue(int );
 
};
#endif
