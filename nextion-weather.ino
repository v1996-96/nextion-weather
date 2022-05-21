#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiManager.h>
#include <WiFiUdp.h>

long current_time;
WiFiManager wm;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

void setup() {
  WiFi.mode(WIFI_STA); // STA+AP

  Serial.begin(9600);

  wm.setConfigPortalBlocking(false);
  wm.setConfigPortalTimeout(60);

  if (wm.autoConnect("nextion")) {
    Serial.println("connected...yeey :)");

    timeClient.begin();
  } else {
    Serial.println("Configportal running");
  }
}

void loop() {
  timeClient.update();
  wm.process();

  if (millis() - current_time >= 10000) {
    current_time = millis();

    Serial.println(timeClient.getFormattedTime());
  }
}

// WiFiClient client;
// HTTPClient http;

// http.begin(client, "http://jsonplaceholder.typicode.com/users/1");
// int httpCode = http.GET();

// if (httpCode > 0) {
//   String payload = http.getString();
//   Serial.println(payload);
// }

// http.end();