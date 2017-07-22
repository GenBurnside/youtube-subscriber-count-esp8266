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
String resource = "/youtube/v3/channels?id=" + channel + "&part=statistics&key=" + apiKey;
const char host[] = "content.googleapis.com";
const char* fingerprint = "ef d7 9c 64 94 7c 7e c4 7b d8 6f b3 0b 0a c4 b6 7a 97 7d f9";

Adafruit_7segment matrix = Adafruit_7segment();
WiFiClientSecure client;

void setup() {
  Serial.begin(115200);

  matrix.begin(0x70);
  matrix.print(100000, DEC);
  matrix.writeDisplay();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    connectToWifi();
  }

  if (!client.connect(host, 443)) {
    Serial.println("Could not connect to server");
    return;
  }

  if (!client.verify(fingerprint, host)) {
    Serial.println("Unexpected SSL certificate");
    return;
  }

  Serial.print("GET ");
  Serial.println(resource);

  String request = "GET " + resource + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n";
  client.print(request);

  while (client.connected()) {
    String line = client.readStringUntil('\n');
    Serial.println(line);
  }

  // wait 60 seconds
  delay(1000 * 60);
}

void connectToWifi() {
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
