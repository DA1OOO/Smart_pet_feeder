# Smart Pet Feeder

## Introduction

> Use miniprogram to control the feeding and water adding of smart pet feeder through network, monitor the weight of grain storage bin, water storage bin, food bowl and water bowl, and remind users when there is a shortage of water or food.

## Hardware Materials
- **Electronic scale, weight sensor × 1** (3.3V, HX711AD, Digitals)

  ![img_4.png](sources/img_4.png)
- **IERG4230 half size Main project board × 1**
(6V, with  16-key keypad and LCD display. Nano and
ESP8266 are NOT included)

  ![img.png](sources/img1.png)
- **Arduino Nano × 1**

  ![img_1.png](sources/img_1.png)
- **ESP8266 × 1** (6V, with I/O extended board)

  ![img.png](sources/img.png)
- **2.4GHz USB WiFi LAN adapter × 1**
- **3V FeLiPO4 re-chargeable battery × 2**
- **Mini-USB cable × 1**
- **Micro-USB cable × 1**

## Software Environment
- **Arduino IDE 2.0.1**
- **IntelliJ IDEA Community Edition 2022.1**
- **WeChat DevTools 1.06.2210310 Stable**
- **JDK 1.8**

## Hardware Side

- **Transport Model**

![img.png](sources/img20983.png)

- **ESP8266 Pin Diagram**

![img.png](sources/img343.png)

## Server Side

## Miniprogram Side

 ![img.png](sources/img2232.png)

## Note
- **Lab 2.2** 
  - **Set Arduino IDE for ESP8266**
    - File >> Examples >> ESP8266WebServer >> HelloServer ———— ESP8266
- **Lab 2.4**
  - **Run ESP8266 Web client**
    - File >>> Examples >>> ESP8266HTTPClient >>> BasicHttpClient ———— ESP8266
- **Lab 2.5**
  - **Run ESP8266 as an I2C Slave with Nano Master**
    - OS_LED-Exp2.5 ———— Nano
    - Exp2.5_given_ESP8266_Slave ———— ESP8266

## Reference
- https://zhuanlan.zhihu.com/p/93838734
- https://blog.csdn.net/qq_43701073/article/details/116059977?ops_request_misc=%257B%2522request%255Fid%2522%253A%2522166843153216800182711941%2522%252C%2522scm%2522%253A%252220140713.130102334.pc%255Fall.%2522%257D&request_id=166843153216800182711941&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~first_rank_ecpm_v1~rank_v31_ecpm-8-116059977-null-null.142