extern unsigned long HX711_Read(void);
extern long Get_Weight();
///变量定义
float Weight = 0;
int HX711_SCK = PD6;   ///     作为输出口
int HX711_DT = PD5;    ///     作为输入口
long HX711_Buffer = 0;
long Weight_Maopi = 0, Weight_Shiwu = 0;
 #define GapValue 365       ///该值需校准 每个传感器都有所不同

void setup()
{  
 //初始化HX711的两个io口       
pinMode(HX711_SCK, OUTPUT);  ///SCK 为输出口 ---输出脉冲
pinMode(HX711_DT, INPUT); ///  DT为输入口  ---读取数据

 Serial.begin(9600);
 Serial.print("Welcome to use!\n");
 delay(3000);    ///延时3秒  
 //获取毛皮重量
 Weight_Maopi = HX711_Read(); 
}
 void loop()    ///  一直循环{}内容  ----- 同while（1）{xxx}
{
 Weight = Get_Weight();  //计算放在传感器上的重物重量
 Serial.print(-float(Weight/1000),3);  //串口显示重量，3意为保留三位小数
 Serial.print(" kg\n"); //显示单位
 Serial.print("\n");  //显示单位
 delay(2000);  //延时2s 两秒读取一次传感器所受压力
}
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
