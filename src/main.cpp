#include "display.h"
#include <Arduino.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <GyverOS.h>
#include <NTPClient.h>
#include <OpenWeather.h>
#include <WiFiManager.h>
#include <WiFiUdp.h>

WiFiManager wm;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
GyverOS<3> OS;

bool isWiFiConnected = false;

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
}

void loop() {
  OS.tick();
  timeClient.update();
  updateDisplay();
}