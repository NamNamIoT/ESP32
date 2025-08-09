#include "modbusSlave.h"
#include "modbus.h"
#include "modbusDevice.h"
#include "Settings.h"
extern Settings MySettings;
#include <Arduino.h>
byte *_msg2, _len2;
modbusSlave::modbusSlave()
{
}
void modbusSlave::setBaud(word baud)
{
  _baud = baud;
  _frameDelay = 24000/_baud;
  Serial2.begin(baud);
  Serial2.flush();
}

void modbusSlave::calcCrc(void)
{
  byte  CRCHi = 0xFF,
      CRCLo = 0x0FF,
      Index,
      msgLen,
      *msgPtr;

  msgLen = _len-2;
  msgPtr = _msg;

  while(msgLen--)
  {
    Index = CRCHi ^ *msgPtr++;
    CRCHi = CRCLo ^ _auchCRCHi[Index];
    CRCLo = _auchCRCLo[Index];
  }
  _crc = (CRCHi << 8) | CRCLo;
}

/*
  Checks the UART for query data
*/
void modbusSlave::checkSerial(void)
{
  //while there is more data in the UART than when last checked
  while(Serial2.available()> _len)
  {
    //update the incoming query message length
    _len = Serial2.available();
    //Wait for 3 bytewidths of data (SOM/EOM)
//    delayMicroseconds(RTUFRAMETIME);
    delay(_frameDelay);
    //Check the UART again
  }
}

/*
Copies the contents of the UART to a buffer
*/
void modbusSlave::serialRx(void)
{
  byte i;

  //allocate memory for the incoming query message
  _msg = (byte*) malloc(_len);

    //copy the query byte for byte to the new buffer
    for (i=0 ; i < _len ; i++)
      _msg[i] = Serial2.read();
}

/*
Generates a query reply message for Digital In/Out status update queries.
*/
void modbusSlave::getDigitalStatus(byte funcType, word startreg, word numregs)
{
  //initialize the bit counter to 0
  byte bitn =0;
  
  //if the function is to read digital inputs then add 10001 to the start register
  //else add 1 to the start register
  if(funcType == READ_DI)
    startreg += 10001;
  else
    startreg += 1;

  //determine the message length
  //for each group of 8 registers the message length increases by 1
  _len = numregs/8;
  //if there is at least one incomplete byte's worth of data
  //then add 1 to the message length for the partial byte.
  if(numregs%8)
    _len++;
  //allow room for the Device ID byte, Function type byte, data byte count byte, and crc word
  _len +=5;

  //allocate memory of the appropriate size for the message
  _msg = (byte *) malloc(_len);

  //write the slave device ID
  _msg[0] = _device->getId();
  //write the function type
  _msg[1] = funcType;
  //set the data byte count
  _msg[2] = _len-5;

  //For the quantity of registers queried
  while(numregs--)
  {
    //if a value is found for the current register, set bit number bitn of msg[3]
    //else clear it
    if(_device->get(startreg))
      bitSet(_msg[3], bitn);
    else
      bitClear(_msg[3], bitn);
    //increment the bit index
    bitn++;
    //increment the register
    startreg++;
  }
  
  //generate the crc for the query reply and append it
  this->calcCrc();
  _msg[_len - 2] = _crc >> 8;
  _msg[_len - 1] = _crc & 0xFF;
}

void modbusSlave::getAnalogStatus(byte funcType, word startreg, word numregs)
{
  word val;
  word i = 0;

  //if the function is to read analog inputs then add 30001 to the start register
  //else add 40001 to the start register
  if(funcType == READ_AI)
    startreg += 30001;
  else
    startreg += 40001;

  //calculate the query reply message length
  //for each register queried add 2 bytes
  _len = numregs * 2;
  //allow room for the Device ID byte, Function type byte, data byte count byte, and crc word
  _len += 5;

  //allocate memory for the query response
  _msg = (byte *) malloc(_len);

  //write the device ID
  _msg[0] = _device->getId();
  //write the function type
  _msg[1] = funcType;
  //set the data byte count
  _msg[2] = _len - 5;

  //for each register queried
  while(numregs--)
  {
    //retrieve the value from the register bank for the current register
    val = _device->get(startreg+i);
    //write the high byte of the register value
    _msg[3 + i * 2]  = val >> 8;
    //write the low byte of the register value
    _msg[4 + i * 2] = val & 0xFF;
    //increment the register
    i++;
  }

  //generate the crc for the query reply and append it
  this->calcCrc();
  _msg[_len - 2] = _crc >> 8;
  _msg[_len - 1] = _crc & 0xFF;
}

void modbusSlave::setStatus(byte funcType, word reg, word val)
{
  //Set the query response message length
  //Device ID byte, Function byte, Register byte, Value byte, CRC word
  _len = 8;
  //allocate memory for the message buffer.
  _msg = (byte *) malloc(_len);


  //write the device ID
  _msg[0] = _device->getId();
  //if the function type is a digital write
  if(funcType == WRITE_DO)
  {
    //Add 1 to the register value and set it's value to val
    _device->set(reg + 1, val);
    //write the function type to the response message
    _msg[1] = WRITE_DO;
  }
  else
  {
    //else add 40001 to the register and set it's value to val
    _device->set(reg + 40001, val);

    //write the function type of the response message
    _msg[1] = WRITE_AO;
  }
  
  //write the register number high byte value
  _msg[2] = reg >> 8;
  //write the register number low byte value
  _msg[3] = reg & 0xFF;
  //write the control value's high byte
  _msg[4] = val >> 8;
  //write the control value's low byte
  _msg[5] = val & 0xFF;

  //calculate the crc for the query reply and append it.
  this->calcCrc();
  _msg[_len - 2]= _crc >> 8;
  _msg[_len - 1]= _crc & 0xFF;
}
void modbusSlave::setStatus_floatABCD(byte funcType, word reg, word val1, word val2)
{
  //Set the query response message length
  //Device ID byte, Function byte, Register byte, Value byte, CRC word
  _len = 8;
  //allocate memory for the message buffer.
  _msg = (byte *) malloc(_len);


  //write the device ID
  _msg[0] = _device->getId();
  //if the function type is a digital write
  if(funcType == WRITE_CANOPUS_ADD)
  {
    //else add 40001 to the register and set it's value to val
    _device->set(reg + 40001, val1);
    _device->set(reg + 40002, val2);

    //write the function type of the response message
    _msg[1] = WRITE_CANOPUS_ADD;
  }
  //write the register number high byte value
  _msg[2] = reg >> 8;
  //write the register number low byte value
  _msg[3] = reg & 0xFF;
  //write the control value's high byte
  _msg[4] = 0;
  //write the control value's low byte
  _msg[5] = 0x02;
  
 
  //calculate the crc for the query reply and append it.
  this->calcCrc();
  _msg[_len - 2]= _crc >> 8;
  _msg[_len - 1]= _crc & 0xFF;
}
uint8_t modbusSlave::run(void)
{
  byte deviceId;
  byte funcType;
  word field1;
  word field2;
  word field3;
  word field4;
  int i;
  _len = 0;
  this->checkSerial();
  if(_len == 0)
  {
    return 0;
  }
  this->serialRx();
  _msg2=_msg;
  _len2=_len;
  if( (_msg[0] == 0xFF) || (_msg[0] == _device->getId()) )
  {   
    this->calcCrc();
    if ( _crc != ((_msg[_len - 2] << 8) + _msg[_len - 1]))
      return 0;
    funcType = _msg[1];
    field1  = (_msg[2] << 8) | _msg[3];
    field2  = (_msg[4] << 8) | _msg[5];
    if(_len2>=7){
    field3  = (_msg[7] << 8) | _msg[8];
    field4  = (_msg[9] << 8) | _msg[10];
    }
    free(_msg);
    _len = 0;
    switch(funcType)
    {
    case READ_DI:
      this->getDigitalStatus(funcType, field1, field2);//Serial.printf("\r\nRDI");
      break;
    case READ_DO:
      this->getDigitalStatus(funcType, field1, field2);//Serial.printf("\r\nRDO");
      break;
    case READ_AI:
      this->getAnalogStatus(funcType, field1, field2);//Serial.printf("\r\nRAI");
      break;
    case READ_AO:
      this->getAnalogStatus(funcType, field1, field2);//Serial.printf("\r\nRAO");
      break;
    case WRITE_DO:
      this->setStatus(funcType, field1, field2);//;Serial.printf("\r\nWDO");
      break;
    case WRITE_AO:
      this->setStatus(funcType, field1, field2);//Serial.printf("\r\nWAI");
      break;
   case WRITE_CANOPUS_ADD:
      this->setStatus_floatABCD(funcType, field1, field3, field4);Serial.printf("\r\nWRITE_CANOPUS_ADD");
      break;
    default:
      return 0;
      break;
    }
    if(_len)
    {
      int i;
      for(i = 0 ; i < _len ; i++)
        Serial2.write(_msg[i]);
      free(_msg);
      _len = 0;
    }
    return 0;
  }
  else if(_msg[0] == MySettings.ID_SLAVE[1].toInt())
  {
    return 1;
  }
  else if(_msg[0] == MySettings.ID_SLAVE[2].toInt())
  {
    return 2;
  }
  else if(_msg[0] == MySettings.ID_SLAVE[3].toInt())
  {
    return 3;
  }
    else if(_msg[0] == MySettings.ID_SLAVE[4].toInt())
  {
    return 4;
  }
   else
  {
    Serial.printf("\r\nNC, ID: %d\r\n", _msg[0]);
    return 10;
  }
  
}
void modbusSlave::run2(void)
{
  byte deviceId;
  byte funcType;
  word field1;
  word field2;
  word field3;
  word field4;
  int i;
  _msg=_msg2;
  _len=_len2;
   for(int i=0; i<_len; i++)
    {
    //Serial.printf("%d",_msg[i]);
    }
  if((_msg[0] == _device->getId()) )
  {
    this->calcCrc();
    if ( _crc != ((_msg[_len - 2] << 8) + _msg[_len - 1]))
      return;
    funcType = _msg[1];
    field1  = (_msg[2] << 8) | _msg[3];
    field2  = (_msg[4] << 8) | _msg[5];
  if(_len2>=7)
    {
      field3  = (_msg[7] << 8) | _msg[8];
      field4  = (_msg[9] << 8) | _msg[10];
    }
    free(_msg);
    _len = 0;
    switch(funcType)
    {
    case READ_DI:
      this->getDigitalStatus(funcType, field1, field2);
      break;
    case READ_DO:
      this->getDigitalStatus(funcType, field1, field2);
      break;
    case READ_AI:
      this->getAnalogStatus(funcType, field1, field2);
      break;
    case READ_AO:
      this->getAnalogStatus(funcType, field1, field2);
      break;
    case WRITE_DO:
      this->setStatus(funcType, field1, field2);
      break;
    case WRITE_AO:
      this->setStatus(funcType, field1, field2);
      break;
    case WRITE_CANOPUS_ADD:
      this->setStatus_floatABCD(funcType, field1, field3, field4);
      break;
    default:
      return;
      break;
    }
    if(_len)
    {
      int i;
      for(i = 0 ; i < _len ; i++)
        Serial2.write(_msg[i]);
      free(_msg);
      _len = 0;
    }
    return;
  }
}
