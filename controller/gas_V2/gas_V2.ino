#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#define USE_SERIAL Serial
const char* ssid     = "iot";            //Set ssid
const char* password = "12345678";                    //Set Password
const char* Server   = "128.199.198.154";           //set Server Domain or Server ip
const char* port1     = "5001";
const char* port2     = "9999";
const char* nodeid   = "2";
ESP8266WiFiMulti WiFiMulti;

float voltage;
float sensor_volt;
float sensor_ppm;
float sensor_ppmRead;
float RS_gas; // Get value of RS in a GAS
float ratio; // Get ratio RS_GAS/RS_air
#define led_pin D4
#define R1  D5 
#define L2  D6 
#define L1  D7 
#define R2  D8
#define LINE_TOKEN "Lttwf9asid98zwb2Td0K74CKvRgGnr0CqI1hBbf41hF" // node1
#define LINE_TOKEN "jzim5rSKqoKkKYuAnsHankJh746pFVgGsM7HtLWhoSh" // node2
#define LINE_TOKEN "zoq7KYTrikbefz3F2mq5h9m8TF4UwsHCuHTcOKimOIw" // node3
String message = "%E0%B9%81%E0%B8%81%E0%B9%8A%E0%B8%AA%E0%B9%80%E0%B8%81%E0%B8%B4%E0%B8%99%E0%B8%84%E0%B9%88%E0%B8%B2%E0%B8%A1%E0%B8%B2%E0%B8%95%E0%B8%A3%E0%B8%90%E0%B8%B2%E0%B8%99"; 

int buzzer = D3;
int vale = D2;
int fan = D1;

void Line_Notify(String message);
void SendData(float sensor_ppm);
void forward();
void backward();
void right();
void left();
void stopping();
void alarm();
void SendData_db(float sensor_ppm);

void setup() 
{
  pinMode(led_pin,OUTPUT);
  pinMode(L1, OUTPUT);
  pinMode(L2, OUTPUT);
  pinMode(R1, OUTPUT);
  pinMode(R2, OUTPUT);
  pinMode(fan,OUTPUT);
  pinMode(vale,OUTPUT);
  pinMode(buzzer,OUTPUT);
  digitalWrite(vale,LOW);
  digitalWrite(fan,HIGH);
  digitalWrite(buzzer,HIGH);
  delay(1000);
  digitalWrite(buzzer,LOW);
  delay(100);
  digitalWrite(buzzer,HIGH);
  delay(100);
  digitalWrite(buzzer,LOW);
  delay(100);
  digitalWrite(buzzer,HIGH);
  delay(100);
  //Serial.begin(115200);
  USE_SERIAL.begin(115200);
  WiFi.mode(WIFI_STA);
      for(uint8_t t = 6; t > 0; t--) 
      {
        USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
        USE_SERIAL.flush();
        delay(1000);
      }
    WiFiMulti.addAP(ssid, password);    //Set SSID and Password (SSID, Password)
    WiFi.begin(ssid, password);         //Set starting for Wifi
    Serial.println(WiFi.localIP());
    digitalWrite(fan,LOW);

}

void loop() 
{
  int sensorValue = analogRead(A0);
    sensor_volt=(float)sensorValue/1024*3.3;
    RS_gas = (5-sensor_volt)/sensor_volt; // omit *RL

          /*-Replace the name "R0" with the value of R0 in the demo of First Test -*/
    ratio = RS_gas/2.25 ;  // ratio = RS/R0
          /*-----------------------------------------------------------------------*/
    sensor_ppmRead=1200+(-200*ratio);
    if(sensor_ppmRead>0)
    {
      sensor_ppm = sensor_ppmRead;
    }
    else
    {
      sensor_ppm = 0;
    }
    Serial.print("sensor_volt = ");
    Serial.println(sensor_volt);
    Serial.print("RS_ratio = ");
    Serial.println(RS_gas);
    Serial.print("Rs/R0 = ");
    Serial.println(ratio);
    Serial.print("sensor_ppm = ");
    Serial.print(sensor_ppmRead);
    Serial.println(" ppm  ");
    Serial.print("\n\n");
    SendData(sensor_ppm);
    SendData_db(sensor_ppm);
    delay(100);
    float WarningValue = 1000.00;
    if(sensor_ppm>WarningValue)
    {
      digitalWrite(led_pin,HIGH);
      Line_Notify(message+sensor_ppm);
      alarm();
      digitalWrite(vale,LOW);
      //digitalWrite(fan,HIGH);
      delay(2000);
    }
    if(sensor_ppm<WarningValue)
    {
      digitalWrite(led_pin,LOW);
      digitalWrite(buzzer,LOW);
      //digitalWrite(fan,LOW);
    }
    if(sensor_ppm>0)
    {
      digitalWrite(fan,HIGH);
    }
    else
    {
      digitalWrite(fan,LOW);
      digitalWrite(vale,HIGH);
    }
}
void SendData(float sensor_ppm) 
{
  
  // wait for WiFi connection
    if((WiFiMulti.run() == WL_CONNECTED)) 
    {
        HTTPClient http;
        String str1 = "http://" +String(Server)+":"+port1+"/data"+nodeid+"/" + String(sensor_ppm)+"/";
        Serial.println(str1);
        http.begin(str1);
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
void Line_Notify(String message) {
  WiFiClientSecure client;

  if (!client.connect("notify-api.line.me", 443)) {
    Serial.println("connection failed");
    return;   
  }

  String req = "";
  req += "POST /api/notify HTTP/1.1\r\n";
  req += "Host: notify-api.line.me\r\n";
  req += "Authorization: Bearer " + String(LINE_TOKEN) + "\r\n";
  req += "Cache-Control: no-cache\r\n";
  req += "User-Agent: ESP8266\r\n";
  req += "Content-Type: application/x-www-form-urlencoded\r\n";
  req += "Content-Length: " + String(String("message=" + message).length()) + "\r\n";
  req += "\r\n";
  req += "message=" + message;
  // Serial.println(req);
  client.print(req);
    
  delay(20);

  // Serial.println("-------------");
  while(client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      break;
    }
    //Serial.println(line);
  }
  // Serial.println("-------------");
  //delay(1000);
}
void alarm()
{
  Serial.println("ALARM!!!");
  int f = 1000;
  int d = 100;
  digitalWrite(buzzer,HIGH);
}
void SendData_db(float sensor_ppm)
{
  if((WiFiMulti.run() == WL_CONNECTED)) 
    {
        HTTPClient http;
        String str2 = "http://" +String(Server)+":"+port2+"/serway"+"/data/"+nodeid+"/" + String(sensor_ppm)+"/";
        Serial.println(str2);
        http.begin(str2);
        
        int httpCode = http.GET();
        USE_SERIAL.printf("code_db: %d\n", httpCode);
        String Status = http.getString();
        USE_SERIAL.print("Status ");
        USE_SERIAL.println(Status);
    }
}

