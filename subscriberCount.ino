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
    error();
    return;
  }

  Serial.print("GET ");
  Serial.println(resource);

  String request = "GET " + resource + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n";
  client.print(request);

  // skip to end of headers in response stream
  client.find("\r\n\r\n");

  // buffer size calculated from https://bblanchon.github.io/ArduinoJson/assistant/
  StaticJsonBuffer<680> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(client);

  if (!root.success()) {
    Serial.println("JSON parsing failed");
    error();
    return;
  }

  JsonObject& statistics = root["items"][0]["statistics"];

  const int videoCount = statistics["videoCount"];
  const int viewCount = statistics["viewCount"];
  const int subscriberCount = statistics["subscriberCount"];

  Serial.println("Video count: " + String(videoCount));
  Serial.println("View count: " + String(viewCount));
  Serial.println("Subscriber count: " + String(subscriberCount));

  matrix.print(subscriberCount, DEC);
  matrix.writeDisplay();

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

void error() {
  matrix.print(100000, DEC);
  matrix.writeDisplay();

  // wait 60 seconds
  delay(1000 * 60);
}
