void Line_Notify(String message) ;

#include <ESP8266WiFi.h>

// Config connect WiFi
#define WIFI_SSID "iot"
#define WIFI_PASSWORD "12345678"

// Line config
#define LINE_TOKEN "I6huv02d52dcoBxghxVGqSaZRvOE7J03MWpGfNTt0R4"

#define SW D2
float voltage;
float sensor_volt;
float sensor_ppm;
float RS_gas; // Get value of RS in a GAS
float ratio; // Get ratio RS_GAS/RS_air

String message = "%E0%B9%81%E0%B8%81%E0%B9%8A%E0%B8%AA%E0%B9%80%E0%B8%81%E0%B8%B4%E0%B8%99%E0%B8%84%E0%B9%88%E0%B8%B2%E0%B8%A1%E0%B8%B2%E0%B8%95%E0%B8%A3%E0%B8%90%E0%B8%B2%E0%B8%99";

void setup() {
  pinMode(SW, INPUT);

  Serial.begin(9600);

  WiFi.mode(WIFI_STA);
  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
}

void loop() {
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
    float WarningValue = 1000.00;
    if(sensor_ppm>WarningValue)
    {
    Line_Notify(message+sensor_ppm);
    // Serial.println();
    }
  delay(100);
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
}
