#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#define USE_SERIAL Serial
const char* ssid     = "iot";            //Set ssid
const char* password = "12345678";                    //Set Password
const char* Server   = "10.153.58.100";           //set Server Domain or Server ip
const char* port     = "5001";
const char* nodeid   = "3";
ESP8266WiFiMulti WiFiMulti;

float voltage;
float sensor_volt;
float sensor_ppm;
float RS_gas; // Get value of RS in a GAS
float ratio; // Get ratio RS_GAS/RS_air
#define led_pin D4
#define R1  D5 
#define L2  D6 
#define L1  D7 
#define R2  D8 
void SendData(float sensor_ppm);
void forward();
void backward();
void right();
void left();
void stopping();

void setup() 
{
  pinMode(led_pin,OUTPUT);
  pinMode(L1, OUTPUT);
  pinMode(L2, OUTPUT);
  pinMode(R1, OUTPUT);
  pinMode(R2, OUTPUT);
  //Serial.begin(115200);
  USE_SERIAL.begin(115200);
      for(uint8_t t = 6; t > 0; t--) 
      {
        USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
        USE_SERIAL.flush();
        delay(1000);
      }
    WiFiMulti.addAP(ssid, password);    //Set SSID and Password (SSID, Password)
    WiFi.begin(ssid, password);         //Set starting for Wifi
    Serial.println(WiFi.localIP());

}

void loop() 
{
  int sensorValue = analogRead(A0);
    sensor_volt=(float)sensorValue/1024*3.3;
    RS_gas = (5-sensor_volt)/sensor_volt; // omit *RL

          /*-Replace the name "R0" with the value of R0 in the demo of First Test -*/
    ratio = RS_gas/3.53;  // ratio = RS/R0
          /*-----------------------------------------------------------------------*/
    sensor_ppm=1450+(-200*ratio);

    Serial.print("sensor_volt = ");
    Serial.println(sensor_volt);
    Serial.print("RS_ratio = ");
    Serial.println(RS_gas);
    Serial.print("Rs/R0 = ");
    Serial.println(ratio);
    Serial.print("sensor_ppm = ");
    Serial.print(sensor_ppm);
    Serial.println(" ppm  ");
    Serial.print("\n\n");
    SendData(sensor_ppm);
    delay(100);
    float WarningValue = 1000.00;
    if(sensor_ppm>WarningValue)
    {
      digitalWrite(led_pin,HIGH);
    }
    if(sensor_ppm<WarningValue)
    {
      digitalWrite(led_pin,LOW);
    }
}
void SendData(float sensor_ppm) 
{
  
  // wait for WiFi connection
    if((WiFiMulti.run() == WL_CONNECTED)) 
    {
        HTTPClient http;
        String str = "http://" +String(Server)+":"+port+"/data"+nodeid+"/" + String(sensor_ppm)+"/";
        Serial.println(str);
        http.begin(str);
        int httpCode = http.GET();
        USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);
        if(httpCode > 0) 
        {
            if(httpCode == HTTP_CODE_OK) 
              {
                String payload = http.getString();
      //-------------------Control----------------------------//
                USE_SERIAL.print("payload ");
                USE_SERIAL.println(payload);
                if(payload == "F")
                {
                  forward();
                }
                if(payload == "B")
                {
                  backward();
                }
                if(payload == "R")
                {
                  right();
                }
                if(payload == "L")
                {
                  left();
                }
                if(payload == "S")
                {
                  stopping();
                }
                
              }
        }
    }
}
void forward()
{
  digitalWrite(L1, HIGH);  
  digitalWrite(R1, HIGH);  
  digitalWrite(L2, LOW);   
  digitalWrite(R2, LOW);   
  Serial.println("Forward");
  //server.send(204," "); 
}
void backward()
{
  digitalWrite(L2, HIGH);  
  digitalWrite(R2, HIGH);  
  digitalWrite(L1, LOW);   
  digitalWrite(R1, LOW);  
  Serial.println("Backward");
  //server.send(204," "); 
}
void right()
{
  digitalWrite(R1, LOW);   
  digitalWrite(R2, LOW);   
  digitalWrite(L1, HIGH); 
  digitalWrite(L2, LOW);   
  delay(300); 
  digitalWrite(L1,LOW);
  digitalWrite(L2,LOW);
  digitalWrite(R1,LOW);
  digitalWrite(R2,LOW);
  delay(100);
  Serial.println("Right");
  //server.send(204,"");
}
void left()
{
  digitalWrite(L1, LOW);   
  digitalWrite(L2, LOW);   
  digitalWrite(R1, HIGH);   
  digitalWrite(R2, LOW);   
  delay(300); 
  digitalWrite(L1, LOW);  
  digitalWrite(L2, LOW);  
  digitalWrite(R1, LOW);  
  digitalWrite(R2, LOW);   
    delay(100);
  Serial.println("Left");
  // server.send(204," "); 
}
void stopping()
{
  digitalWrite(L1, LOW);  
  digitalWrite(L2, LOW);  
  digitalWrite(R1, LOW);  
  digitalWrite(R2, LOW);   
    delay(1000);
  Serial.println("Stop");
  //server.send(204," "); 
}
