/* IERG4230 Exp-4.5 Answer for ver.2021.07
 *  By Alex siu
 *  last modified date: 2021.08.02
 *  
 * This program ESP8266 is SLAVE
 * Hardware "Gereric ESP8266 module"
 * Arduino 1.8.13 (zip version)
 * 
 *  The program is modified from Arduino Example
  *  File >>> Examples >>> ESP8266WebSerer >>>Hello Server
  *  and combine the Simple OS for IERG4230
 */

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Wire.h>
#include "IERG4230_ESP8266.h"

// ==== define program information, it will be shown after reset
#define programMark "IERG4230 Exp2.5 Given program"      //-- You can modify this line. 
#define versionMark 11244                    //-- Any number, You can modify this line. 

#ifndef STASSID
#define STASSID "AP1"
#define STAPSK  "12341234"
#endif

#define ESP8266I2CAddress 0x66  //-- ESP8266 I2C Address
// const int ledBuildIn = 2;  //-- Do not use the build-in LED near the Antenna, Pin-2 is assigned to I2C in IERG4230
dOut ledUsb(16, 1); //-- set this pin is activeLow
dOut led4(15);  
dOut led5(14);
dIn sw53(4);
dIn sw54(5);
int iTemp;
const char* ssid = STASSID;
const char* password = STAPSK;

unsigned int hitCount =0; //--for debug
String i2cRx;

ESP8266WebServer server(80);

void handleRoot() 
{
  ledUsb.on();
  String message = "\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) 
  {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
    if (server.argName(i)=="led" && server.arg(i)=="on") ledUsb.on();
    else if (server.argName(i)=="led" && server.arg(i)=="off") ledUsb.off();
  }
  server.send(200, "text/plain", message);
  ledUsb.off();
}

void handleNotFound() {
  ledUsb.on();
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) 
  {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  ledUsb.off();
}

void setup(void) 
{  
  Serial.begin(115200);  //-- native serial port for debug, wait for stable, delay(500)
  for(int i=0; i<10; i++)
  {
    ledUsb.toggle();
    delay(100);    
  }
  led4.on();
  led5.on();
  Serial.println("");
  Serial.println("");
  Serial.println(programMark);
  Serial.print("Version mark: ");  //-- for debug
  Serial.println(versionMark);      //-- for debug
  Serial.print("ESP8266 I2C Slave address: 0x");  //-- for debug
  Serial.println(ESP8266I2CAddress, HEX);   //-- for debug
  Wire.begin(2,0, ESP8266I2CAddress);  //--Wire.begin(SDA-2, SCL-0, slave_address)  //-- ESP8266 I2C Address  
  Wire.setClock(10);   //-- for Exp-2.5
  Wire.onReceive(I2cRxEvent); //-- for Exp-2.5
  Wire.onRequest(I2cReqEvent); //-- for Exp-2.5  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("ESP8266 Board MAC Address:  ");
  Serial.println(WiFi.macAddress());

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });

  server.on("/gif", []() {
    static const uint8_t gif[] PROGMEM = {
      0x47, 0x49, 0x46, 0x38, 0x37, 0x61, 0x10, 0x00, 0x10, 0x00, 0x80, 0x01,
      0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x2c, 0x00, 0x00, 0x00, 0x00,
      0x10, 0x00, 0x10, 0x00, 0x00, 0x02, 0x19, 0x8c, 0x8f, 0xa9, 0xcb, 0x9d,
      0x00, 0x5f, 0x74, 0xb4, 0x56, 0xb0, 0xb0, 0xd2, 0xf2, 0x35, 0x1e, 0x4c,
      0x0c, 0x24, 0x5a, 0xe6, 0x89, 0xa6, 0x4d, 0x01, 0x00, 0x3b
    };
    char gif_colored[sizeof(gif)];
    memcpy_P(gif_colored, gif, sizeof(gif));
    // Set the background to a random set of colors
    gif_colored[16] = millis() % 256;
    gif_colored[17] = millis() % 256;
    gif_colored[18] = millis() % 256;
    server.send(200, "image/gif", gif_colored, sizeof(gif_colored));
  });

  server.onNotFound(handleNotFound);

  /////////////////////////////////////////////////////////
  // Hook examples

  server.addHook([](const String & method, const String & url, WiFiClient * client, ESP8266WebServer::ContentTypeFunction contentType) {
    (void)method;      // GET, PUT, ...
    (void)url;         // example: /root/myfile.html
    (void)client;      // the webserver tcp client connection
    (void)contentType; // contentType(".html") => "text/html"
    Serial.printf("A useless web hook has passed\n");
    Serial.printf("(this hook is in 0x%08x area (401x=IRAM 402x=FLASH))\n", esp_get_program_counter());
    return ESP8266WebServer::CLIENT_REQUEST_CAN_CONTINUE;
  });

  server.addHook([](const String&, const String & url, WiFiClient*, ESP8266WebServer::ContentTypeFunction) {
    if (url.startsWith("/fail")) {
      Serial.printf("An always failing web hook has been triggered\n");
      return ESP8266WebServer::CLIENT_MUST_STOP;
    }
    return ESP8266WebServer::CLIENT_REQUEST_CAN_CONTINUE;
  });

  server.addHook([](const String&, const String & url, WiFiClient * client, ESP8266WebServer::ContentTypeFunction) {
    if (url.startsWith("/dump")) {
      Serial.printf("The dumper web hook is on the run\n");

      // Here the request is not interpreted, so we cannot for sure
      // swallow the exact amount matching the full request+content,
      // hence the tcp connection cannot be handled anymore by the
      // webserver.
#ifdef STREAMSEND_API
      // we are lucky
      client->sendAll(Serial, 500);
#else
      auto last = millis();
      while ((millis() - last) < 500) {
        char buf[32];
        size_t len = client->read((uint8_t*)buf, sizeof(buf));
        if (len > 0) {
          Serial.printf("(<%d> chars)", (int)len);
          Serial.write(buf, len);
          last = millis();
        }
      }
#endif
      // Two choices: return MUST STOP and webserver will close it
      //                       (we already have the example with '/fail' hook)
      // or                  IS GIVEN and webserver will forget it
      // trying with IS GIVEN and storing it on a dumb WiFiClient.
      // check the client connection: it should not immediately be closed
      // (make another '/dump' one to close the first)
      Serial.printf("\nTelling server to forget this connection\n");
      static WiFiClient forgetme = *client; // stop previous one if present and transfer client refcounter
      return ESP8266WebServer::CLIENT_IS_GIVEN;
    }
    return ESP8266WebServer::CLIENT_REQUEST_CAN_CONTINUE;
  });

  // Hook examples
  /////////////////////////////////////////////////////////

  server.begin();
  Serial.println("HTTP server started");
}

//--------------------------- LOOP ---------------------------
void loop(void) 
{
  server.handleClient();
  MDNS.update();
  sw54.dataUpdate();
  if (sw54.eventDown) 
  {
    led5.toggle();
    sw54.eventDown=0;
  }
}

//====================== I2c Rx Event ============
void I2cRxEvent(size_t howMany)
{
  while ( Wire.available()>0) 
  { 
    char c = Wire.read(); // receive byte as a character
    Serial.println(c);         // print the character
  }
    //if (i2cRx.length()>=4) i2cRx="";
}
//======================= I2c Req Event =================
void I2cReqEvent()
{
  Wire.write("Hi");
}

//========================================================
