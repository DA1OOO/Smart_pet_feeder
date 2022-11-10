//Designed for CHUK-IE IERG4230 course
//2021.08.03 by Alex Siu
//Copyright reserved

#include <Arduino.h>
#include "IERG4230_ESP8266.h"
//==== class dIn functions ====================================
int dIn::totalDin = 0;
dIn::dIn(int p) //-- this pin active HIGH
{
  this->pin = p;
  this->totalDin++;
  pinMode(p, INPUT);
}

void dIn::dataUpdate()
{
  int temp = this->state;
  this->state = digitalRead(this->pin);
	if (temp == 1 and this->state == 0) this->eventDown = 1;
	if (temp == 0 and this->state == 1) this->eventUp = 1;
}
//== end of class dIn functions ===


//== class dOut functions ====================================
int dOut::totalDout = 0;

dOut::dOut(int p)  // -- set pin as active High 
{
  this->pin = p;
  pinMode(p, OUTPUT);
  this->value = 0;
  this->activeLow=0;
  digitalWrite(pin, LOW);
  this->totalDout++;
}

dOut::dOut(int p, int invert)  //-- set pin as active Low
{
  this->pin = p;
  pinMode(p, OUTPUT);
  this->value = 0;
  if (invert!=0) this->activeLow=1;
  if (activeLow!=0)   digitalWrite(pin, LOW);
  else digitalWrite(pin, HIGH);
  this->totalDout++;	
}

void dOut::on(void)
{
  this->value = 1;
  if (activeLow!=0)   digitalWrite(pin, LOW);
  else digitalWrite(pin, HIGH);  
}
void dOut::off(void)
{
  this->value = 0;
  if (activeLow!=0)   digitalWrite(pin, HIGH);
  else digitalWrite(pin, LOW);  
}

void dOut::toggle(void)
{
  if (this->value == 0)
  {
    this->value = 1;
	if (activeLow!=0)   digitalWrite(pin, LOW);
	else digitalWrite(pin, HIGH);  	
  }
  else
  {
    this->value = 0;
	if (activeLow!=0)   digitalWrite(pin, HIGH);
	else digitalWrite(pin, LOW); 
  }
}

boolean dOut::state(void)
{
  return (this->value);
}
//== end of class dOut functions ===

//====class aIn functions ====================================
int aIn::totalAIn = 0;
aIn::aIn(int p)
{
  this->pin = p;
  pinMode(p, INPUT);
  this->totalAIn++;
}

void aIn::dataUpdate(void)
{
  this->value = analogRead(this->pin);
}
//== end of class aIn functions ===

//====class aOut functions ====================================
int aOut::totalAOut = 0;
aOut::aOut(int p)
{
  this->pin = p;
  pinMode(p, OUTPUT);
  this->totalAOut++;
}

void aOut::setValue(int v)
{
  this->value = v;
   analogWrite(pin, v);
}

int aOut::getValue(void)
{
  return this->value;
}
//== end of class aOut functions ===


//==== class LcdI2C8Bit functions ====================================
LcdI2C8Bit::LcdI2C8Bit(byte I2cAddress)
{
  this->byte01 = 0;
  this->byte02 = 0;
  this->address = I2cAddress;
  this->buzzer = 0;
}

void LcdI2C8Bit::setUp(void)
{
  this->send(0x3F, 0x00); //Function set B001x xxxx, two lines
  this->send(0x1F, 0x00);
  this->send(0x0C, 0x00); //B00001xxx
  this->send(0x06, 0x00); //I/D=1, S=0
  this->send(0x02, 0x00); //Home
  this->send(0x01, 0x00); //clear
  delay(10); //clear need time
}

void LcdI2C8Bit::send(byte b1, byte b2)
{
  Wire.beginTransmission(address);
  Wire.write(b1);
  b2 |= 0x08;
  if (this->lcdBackLight == 1) b2 |= 0x01;
  else b2 &= 0xFE;
  if (this->buzzer == 0) b2 |= 0xF0; //set buzzer on/off
  else b2 &= 0x7F; //set buzzer on/off
  Wire.write(b2);
  Wire.endTransmission();
  Wire.beginTransmission(address);
  Wire.write(b1);
  b2 &= 0xF7;
  Wire.write(b2);
  Wire.endTransmission();
}


void LcdI2C8Bit::displayString(byte lineNum, String st)
{
  int i;
  int length = st.length();
  if (length > 20) length = 20;
  if (lineNum == 3) this->send(212, 0x00); //line 3
  else if (lineNum == 2) this->send(148, 0x00); //line 2
  else if (lineNum == 1) this->send(192, 0x00); //line 1
  else this->send(128, 0x00); //line 0
  for (i = 0; i < 20; i++)
  {
    if (i < length) this->send(st[i], 0x02);
    else this->send(0x20, 0x02);
  }
}

void LcdI2C8Bit::showFigure(int lineNum, int offset, int length, long figure)
{
  String figureText;
  int i, textLength;
  if (lineNum == 3) this->send((212 + offset), 0x00); //line 3
  else if (lineNum == 2) this->send((148 + offset), 0x00); //line 2
  else if (lineNum == 1) this->send((192 + offset), 0x00); //line 1
  else this->send((128 + offset) , 0x00); //line 0
  figureText = String(figure);
  textLength = figureText.length();
  for (i = 0; i < (length - textLength); i++) this->send(0x20, 0x02);
  for (i = 0; i < textLength; i++)
  {
    this->send(figureText[i], 0x02);
  }
}

void LcdI2C8Bit::clear(void)
{
  this->send(0x01, 0x00); //clear
  delay(10);
}

void LcdI2C8Bit::backLightOnOff(byte onOff)
{
  if (onOff == 0)this->lcdBackLight = 1;
  else this->lcdBackLight = 0;
}

void LcdI2C8Bit::buzzerOnOff(byte onOff)
{
  byte b2;
  if (onOff == 0) this->buzzer = 0;
  else this-> buzzer = 1;
  Wire.beginTransmission(address);
  Wire.write(0x00);
  if (this->lcdBackLight == 1) b2 |= 0x01;
  else b2 &= 0xFE;
  if (this->buzzer == 0) b2 |= 0xF0; //set buzzer on/off
  else b2 &= 0x7F; //set buzzer on/off
  Wire.write(b2);
  Wire.endTransmission();
}
//== end of class LcdI2C8bit functions ===

//== class osEvent functions ====================================
unsigned long osEvent::osTimer = 0;
int osEvent::totalTask = 0;
osEvent::osEvent(void)
{
  this->timer = 0;
  this->event = 0;
  this->num = totalTask;
  this->totalTask++;
}

void osEvent::timerSet(int t)
{
  this->enable = 1; 
  this->timer = t;
}

boolean osEvent::isSet(void)
{
  return this->event;
}

void osEvent::clean(void)
{
  this->enable =0;
  this->event = 0;
}

void osEvent::timerUpdate(int i)
{
  if (this->timer <= i)
  {
    if (this->enable==1) this->event = 1;
    this->timer = 0;
  }
  else this->timer -= i;
}
//== end of class osEvent functions ===




//--- comment functions, no need to modify ============================-
void I2CRead2Byte(char address, char* data0Ptr, char* data1Ptr)
{
  Wire.requestFrom(address, 2);
  *data0Ptr = Wire.read();
  *data1Ptr = Wire.read();
}
