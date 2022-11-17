
/* Designed for CHUK-IE IERG4230 course
 * Created on 2016.01.20
 * Revised on 2018.01.12, 2021.07.20, 2021.07.29
 * Copyright reserved
 * 
 * Native Hardware Serial (Tools >>> Serial Monitor), 115200bps  for debug
 */


#include <Arduino.h>
#include <SoftwareSerial.h>
#include <Wire.h>
SoftwareSerial xBeeSerial(9, 8); //RX=9, TX=8, use software for to XBee
#include "IERG4230.h"

// ==== define program information, it will be shown after reset
#define programMark "IERG4230 simple OS"      //-- You can modify this line. 
#define versionMark 1234                    //-- Any number, You can modify this line. 

// === start #define =============================================================
#define debugPerformance            //Print Performance count, you can comment it 
#define YunI2CAddress  0x18         //Dont modify, YUN I2C address
#define ESP8266I2CAddress 0x66      //Dont modify, ESP8266 I2C address
#define xbeeI2CAddress 0x27         //Dont modify, XBee I2C address
#define keyI2CAddress  0x23         //Dont modify, 16-key keypad (4x4) I2C address
#define lcdI2CAddress  0x26         //Dont modify, LCD I2C address
unsigned int performanceCounter;    //Dont modify, for performance test
char xbeeId0, xbeeId1;              //Variables store XBee ID for DIP switches
char keyI2Cdata0, keyI2Cdata1;      //Variables store current 16-key keypad (4x4) status
char keyI2Cdata0old, keyI2Cdata1old;//Variables store pervious 16-key keypad (4x4) status
char keyI2Cchanged;                 //Variable indicates keypad is changed

// HX711
extern unsigned long HX711_Read(void);
extern long Get_Weight();
///变量定义
float Weight = 0;
int HX711_SCK = PD6;   ///     作为输出口
int HX711_DT = PD5;    ///     作为输入口
long HX711_Buffer = 0;
long Weight_Maopi = 0, Weight_Shiwu = 0;
#define GapValue 365       ///该值需校准 每个传感器都有所不同

 
LcdI2C8Bit Lcd(lcdI2CAddress);      //Dont modify, create object LCD
osEvent task0;                      //Dont modify, for performance test
// ===== end of #include, #define, essential object and golbal variables  ===========
// ===== create user defined objects
osEvent task1, task2, task3, task4, task5, task6, task7, task8, task9, task10; //10 tasks object are created.
dOut ledL(13);                       //create objects, LED(L)
dOut led1(3), led2(5), led3(6);      //create objects, three on project board LEDs as digital outputs
dIn sw11(2), sw12(7);                //create objects, two on project board switches as digital inputs
aIn aI0(A0), aI1(A1), aI2(A2), aI3(A3), aI6(A6), aI7(A7); //create object, six analog inputs
aOut led1a(3), led2a(5), led3a(6); //create objects, three on board LEDs as analog PWM outputs
// == add your golbal variables below ==
//int temp_i;    //-- uncomment this line for Exp-1.9
////////////////////////////////////////////////////////////////
void setup()
{
  Wire.begin(); //start I2C bus
  Wire.setClock(50); //-- slow down I2C speed
  I2CRead2Byte(xbeeI2CAddress, &xbeeId0, &xbeeId1);  //read two bytes for XBee ID
  I2CRead2Byte(keyI2CAddress, &keyI2Cdata0old, &keyI2Cdata1old);  //read 16-key keypad (4x4) status
  keyI2Cchanged = 0;
  Serial.begin(115200);   //--enable hwUART for debug
  printProgramMark();
  xBeeSerial.begin(9600); //enable swUART

  // HX711
  //初始化HX711的两个io口       
  pinMode(HX711_SCK, OUTPUT);  ///SCK 为输出口 ---输出脉冲
  pinMode(HX711_DT, INPUT); ///  DT为输入口  ---读取数据
  Serial.print("Welcome to use!\n");
  delay(3000);    ///延时3秒  
  //获取毛皮重量
  Weight_Maopi = HX711_Read();

  
  osEvent::osTimer = millis();
  Lcd.setUp();
  //=== dont modify above setup
  //=== user can start task at starting
  LCDshowWelcomePage();
  task0.timerSet(100);  //the first time to exectue performance test after 100ms
  task1.timerSet(100);  //the first time to exectue task1 after 100ms
  task10.timerSet(100);  //the first time to exectue task10 after 100ms
  Lcd.displayString(3, "XBee ID:");   // (3)=4th line
  Lcd.showFigure(3, 8, 3,  (byte) xbeeId0); // (3)=4th line, (8)=9th position (3)=length
  Lcd.showFigure(3, 12, 3,  (byte) xbeeId1);  // (3)=4th line, (12)=3th position (3)=length
}

void loop()
{
  while (Serial.available())
  {
    //read data from hardware buffer
  } 
  while (xBeeSerial.available())
  {
    //read data from software buffer
  }

  // HX711
  Weight = Get_Weight();  //计算放在传感器上的重物重量
  Serial.print(-float(Weight/1000),3);  //串口显示重量，3意为保留三位小数
  Serial.print(" kg\n"); //显示单位
  Serial.print("\n");  //显示单位
  delay(2000);  //延时2s 两秒读取一次传感器所受压力
  
  performanceCounter++;   //dont remove, for performace test
  hal_eventUpdate();               //check Hardware events
  if (osEvent::osTimer != millis()) timeStampUpdate();
  if (task0.isSet()) task0_handler();  //for performance test
  
  //---- dont modify above lines, user add own task handlers below ---
  if (task1.isSet()) task1_handler(); //  when task-1 timeout, run task1_handler() subroutine

  // HX711
  Serial.print("send to ESP8266...\n");
  Wire.beginTransmission(ESP8266I2CAddress);
  char temp[10];
  dtostrf(Weight, 1, 2, temp);
  Wire.write(temp);
  Wire.endTransmission();
  
  if (sw11.eventDown) sw11_downHandler();  //-- for EXp-2.5
  if (sw12.eventDown) sw12_downHandler();  //-- for EXp-2.5
//    if (sw11.eventUp) sw11_upHandler();
//    if (task2.isSet()) task2_handler();
//    if (task3.isSet()) task3_handler();
//    if (task4.isSet()) task4_handler();
  if (task10.isSet()) task10_handler(); //  when task-10 timeout, run task1_handler() subroutine
//    if (aI0.value < 500) aI0_low_handler();
//    if (aI0.value > 600) aI0_high_handler();
//    if (aI7.value < 500) aI7_low_handler();     //--uncomment this line and use suitable value for Exp1.6
//    else aI7_high_handler();                    //--uncomment this line for Exp1.6
//    if (keyI2Cchanged) keyI2C_handler();        //--uncomment this line for Exp1.8
} 
//----------------------------------------
//--- user's add handlers and subroutines below here

void sw11_downHandler() //-- for Exp-2.5
{
  Serial.print("sw11 was pressed!");
  led2.toggle();  //--uncomment for debug
  Wire.beginTransmission(ESP8266I2CAddress);
  Wire.write("LED4");
  Wire.endTransmission();
  sw11.eventDown=0;   // reset sw11 press down event 
}

void  sw12_downHandler()  //-- for Exp-24..5
{
  led3.toggle();   //-- uncomment for debug
  Wire.requestFrom(ESP8266I2CAddress, 2);  //--Wire.requestFrom(address, datasize)
  if (Wire.available()) 
  {
    Serial.print("Data returned: ");
    while (Wire.available()) Serial.print((char) Wire.read());
    Serial.println();
    }
    Wire.endTransmission() ;
    sw12.eventDown=0;   // reset sw11 press down event 
}


void task1_handler(void)  //LED1 flashing
{
  task1.clean(); //-- clear the planning flag
  led1.toggle(); //-- toggle the LED1 
  task1.timerSet(1000); //-- set timer (1000ms), planning flag will be set when it timeout.
}

void task10_handler(void) //--LED(L) on Nano
{
  task10.clean(); //-- clear the planning flag
  ledL.toggle(); //-- toggle the LED(L) 
  task10.timerSet(100); //-- set timer (1000ms), planning flag will be set when it timeout.
}

void aI7_low_handler()
{
  //-- your Exp1.6 program here
}

void aI7_high_handler()
{
  //-- your Exp1.6 program here
}

void keyI2C_handler() //-- have not completed
{
  Lcd.showFigure(1,0,3,(byte) keyI2Cdata0 );  //--line:1, poisiton:0, digit:3
}

/*  Hints for Exp-1.9, haven't completed
void task1_handler(void)  
{
  led3a.setValue(temp_i);  //-- set PWM value to led3a
  task1.timerSet(40);
}
*/

//void sw11_downHandler(void) // when SW11 Down
//{
//  task3.clean();
//  sw11.eventDown=0;   // reset sw11 press down event
//}

// --- user LCD handlers ----
void LCDshowWelcomePage(void)
{
  Lcd.displayString(0, ". CUHK-IE IERG4230 .");
  Lcd.displayString(1, ".                  .");
  Lcd.displayString(2, ".                  .");
  Lcd.displayString(3, "....................");
}

//-------------------  basic OS subroutines -------------------
//---You may need to modify them if you change the hardware ---
void hal_eventUpdate(void)
{
  //==read I2C 16key keypad
  I2CRead2Byte(keyI2CAddress, &keyI2Cdata0, &keyI2Cdata1);
  if (keyI2Cdata0 != keyI2Cdata0old) keyI2Cchanged = 1;
  if (keyI2Cdata1 != keyI2Cdata1old)  keyI2Cchanged = 1;
  keyI2Cdata0old = keyI2Cdata0;
  keyI2Cdata1old = keyI2Cdata1;
  //==end of read I2C 16key keypad
  //== comment/uncomment the followings for used hardware
  sw11.dataUpdate();
  sw12.dataUpdate();
  //aI0.dataUpdate();   //-- comment this line will get higher performance, Analog In takes time
  //aI1.dataUpdate();   //-- comment this line will get higher performance, Analog In takes time
  //aI2.dataUpdate();   //-- comment this line will get higher performance, Analog In takes time
  //aI3.dataUpdate(); //-- comment this line will get higher performance, Analog In takes time
  //aI6.dataUpdate(); //-- comment this line will get higher performance, Analog In takes time
  //aI7.dataUpdate();     //--uncomment this line for Exp1.6
}

void timeStampUpdate(void)   // no need to modify this function unless you know what you are doing.
{
  int i;
  unsigned long temp;
  temp = millis();
  if (osEvent::osTimer > temp) i = 1;
  else i = (int)(temp - osEvent::osTimer);
  osEvent::osTimer = temp;
  //---- user add their own tasks if necessary
  task0.timerUpdate(i);
  task1.timerUpdate(i);
  task2.timerUpdate(i);
  task3.timerUpdate(i);
  task4.timerUpdate(i);
  task5.timerUpdate(i);
  task6.timerUpdate(i);
  task7.timerUpdate(i);
  task8.timerUpdate(i);
  task9.timerUpdate(i);
  task10.timerUpdate(i);  
}

void task0_handler() // Dont modify, for performace test
{ 
  task0.clean();
  #ifdef debugPerformance
    Serial.print("Performance: ");
    Serial.println(performanceCounter/3);
  #endif
  performanceCounter = 0;
  task0.timerSet(3000);
}

void printProgramMark()   //-- Serial print Program Information for identification
{
  Serial.println("");
  Serial.print("Program: ");
  Serial.println(programMark);
  Serial.print("Version: ");
  Serial.println(versionMark);  
}

// HX711
 //称重函数
long Get_Weight()
{
 HX711_Buffer = HX711_Read();    ///读取此时的传感器输出值
 Weight_Shiwu = HX711_Buffer;     ///将传感器的输出值储存
 Weight_Shiwu = Weight_Shiwu - Weight_Maopi; //获取实物的AD采样数值。
 Weight_Shiwu = (long)((float)Weight_Shiwu/GapValue);    //AD值转换为重量（g）
 return Weight_Shiwu; 
}
unsigned long HX711_Read(void) //选择芯片工作方式并进行数据读取
{
 unsigned long count;   ///储存输出值  
 unsigned char i;     
   ////high--高电平 1  low--低电平 0  
 digitalWrite(HX711_DT, HIGH);   ////  digitalWrite作用： DT=1；
 delayMicroseconds(1); ////延时 1微秒  
 digitalWrite(HX711_SCK, LOW);  ////  digitalWrite作用： SCK=0；
 delayMicroseconds(1);   ////延时 1微秒  
 count=0; 
  while(digitalRead(HX711_DT)==HIGH);    //当DT的值为1时，开始ad转换
  for(i=0;i<24;i++)   ///24个脉冲，对应读取24位数值
 { 
   digitalWrite(HX711_SCK, HIGH);  ////  digitalWrite作用： SCK=0；
                                /// 利用 SCK从0--1 ，发送一次脉冲，读取数值
  delayMicroseconds(1);  ////延时 1微秒  
  count=count<<1;    ///用于移位存储24位二进制数值
  digitalWrite(HX711_SCK, LOW);   //// digitalWrite作用： SCK=0；为下次脉冲做准备
  delayMicroseconds(1);
   if(digitalRead(HX711_DT)==HIGH)    ///若DT值为1，对应count输出值也为1
   count++; 
 } 
  digitalWrite(HX711_SCK, HIGH);    ///再来一次上升沿 选择工作方式  128增益
 count ^= 0x800000;   //按位异或  不同则为1   0^0=0; 1^0=1;
///对应二进制  1000 0000 0000 0000 0000 0000  作用为将最高位取反，其他位保留原值
 delayMicroseconds(1);
 digitalWrite(HX711_SCK, LOW);      /// SCK=0；     
 delayMicroseconds(1);  ////延时 1微秒  
 return(count);     ///返回传感器读取值
}

//====  End of OS ========================
