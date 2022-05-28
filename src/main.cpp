#include "display.h"
#include "sample.h"
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

void loadForecastTask() {
  Forecast forecast;
  bool hasError = parseResponse(&forecast, sample);

  if (hasError) {
    Serial.println(F("has error"));
  } else {
    Serial.println(forecast.current.feelsLike);
    Serial.println(forecast.periods[0].condition);
  }
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

  OS.attach(1, loadForecastTask, 10000);
  OS.exec(1);
}

void loop() {
  OS.tick();
  timeClient.update();
  updateDisplay();
}