#include "display.h"
#include "weather-forecast.h"
#include <Arduino.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <GyverOS.h>
#include <NTPClient.h>
#include <WiFiManager.h>
#include <WiFiUdp.h>

WiFiManager wm;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
GyverOS<3> OS;

bool isWiFiConnected = false;

void loadForecast() {
  getForecast("55.7970047", "37.9381218");
  // simpleRequest();
}

void setup() {
  WiFi.mode(WIFI_STA); // STA+AP

  Serial.begin(9600);

  wm.setConfigPortalTimeout(60);
  isWiFiConnected = wm.autoConnect("nextion");

  if (!isWiFiConnected) {
    Serial.println("WIFI: configuration");
  } else {
    Serial.println("WIFI: connected");
  }

  timeClient.begin();
  setupDisplay();

  OS.attach(1, loadForecast, 10000);
}

void loop() {
  OS.tick();
  timeClient.update();
  updateDisplay();
}