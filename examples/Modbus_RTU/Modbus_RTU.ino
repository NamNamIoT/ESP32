#include "Canopus_Modbus.h"
ModbusMaster node;

void setup()
{
  pinMode(25, OUTPUT);
  pinMode(27, OUTPUT);
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
