#include <Wire.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

#include <Adafruit_GFX.h>          // https://github.com/adafruit/Adafruit-GFX-Library
#include <Adafruit_LEDBackpack.h>  // https://github.com/adafruit/Adafruit_LED_Backpack
#include <ArduinoJson.h>           // https://github.com/bblanchon/ArduinoJson

char ssid[] = "";
char password[] = "";

String channel = "";
String apiKey = "";
String url = "https://content.googleapis.com/youtube/v3/channels?id=" + channel + "&part=statistics&key=" + apiKey;

Adafruit_7segment matrix = Adafruit_7segment();
WiFiClientSecure client;

void connectToWifi() {
  // Attempt to connect to Wifi network:
  Serial.println();
  Serial.print("Connecting to network: ");
  Serial.print(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);
}

void setup() { 
  Serial.begin(115200);  
  delay(1000);
  
  matrix.begin(0x70);
  matrix.print(100000, DEC);
  matrix.writeDisplay();

  //connectToWifi();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    connectToWifi();
  }
        
  delay(1000);
}
