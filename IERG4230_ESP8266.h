//Designed for CHUK-IE IERG4230 course
//2021.08.03 by Alex Siu
//Copyright reserved

#ifndef __IERG4230_ESP8266_
#define __IERG4230_ESP8266
#include "Arduino.h"
#include <Wire.h>

void I2CRead2Byte(char address, char* data0Ptr, char* data1Ptr);

//==  class dIn declaration ===
class dIn
{
  public:
    dIn(int);
    int state;
    boolean eventDown;  //have to clear ( =0) in handler
    boolean eventUp;    //have to clear ( =0) in handler
    void dataUpdate(void);
  private:  
    int pin;
    static int totalDin;
}; //end of 'dIn' declearation
//==  end of class dIn declaration ===

//==  class dOut declaration ===
class dOut
{
  public:
    dOut(int);
	dOut(int, int);
    void on(void);
    void off(void);
    void toggle(void);
    boolean state(void);
    static int totalDout;
  private:
    boolean value;
	boolean activeLow;
    int pin;
}; //end of class dOut declearation

//==  end of dOut declaration ===


//==  class aIn declaration ===
class aIn
{
  public:
    aIn(int);
    int value;   //return value 0-1023 
    void dataUpdate(void);
    static int totalAIn;
  private:
    int pin;
}; //end of class aIn
//==  end of aIn declaration ===

//==  class aOut declaration ===
class aOut
{
  public:
    aOut(int);
    void setValue(int); //out value 0-255 
    int getValue(void); //the value last set
    static int totalAOut;

  private:
    int pin;
    int value;
}; //end of class aOut

//==  end of aOut declaration ===


//==  class LcdI2C8bit declaration === 
class LcdI2C8Bit
{
    //PCF8575 P10=blacklight, P11=RS, P12=RW, P13=EN
  public:
    LcdI2C8Bit(byte I2cAddress);
    byte cursorBlink, cursorOn, displayOn, movingDirection, shiftEn, fontType;
    void backLightOnOff(byte onOff);
    void buzzerOnOff(byte onOff) ;
    void setUp(void);
    void displayLine(byte lineNum, byte charNum, char message[20]);
    void showFigure(int line, int offset, int length, long times);
    void send(byte b01, byte b02);
    void displayString(byte lineNum, String st);
    void clear(void);
  private:
    byte byte01, byte02,  address, lcdBackLight, buzzer;
};
//==  end of class LcdI2C8bit declaration ===


//==  class osEvent declaration ===
class osEvent
{
  public:
    osEvent(void);
    void timerSet(int);  //-- set event execute after ms
    boolean isSet(void); //-- return if event's planning flag is Set
    void clean(void);    //-- clean the event's planning flag
    void timerUpdate(int); //-- check current time, set event's planning is needed
    static unsigned long osTimer;
    static int totalTask;
    void handler(void);
  private:
    int timer = 0;       //-- time for planning flag 
    boolean event = 0;   //-- event planning flag
    int num;
    boolean enable = 0;
}; //end of osEvent declear
//==  end of osEvent declaration ===

#endif
