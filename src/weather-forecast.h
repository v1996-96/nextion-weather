#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

#define YANDEX_HEADER "Yandex-API-Key"
#define YANDEX_API_KEY "3aedc6bc-f68d-4ab9-8377-012f1db3cd48"
#define YANDEX_API_URI "https://api.weather.yandex.ru/v2/informers"

void getForecast(const char *latitude, const char *longitude) {
  WiFiClientSecure httpsClient;
  HTTPClient http;
  String url = String(YANDEX_API_URI) + F("?lat=") + latitude + F("&lon=") + longitude;
  DynamicJsonDocument result(2048);
  static bool succeeded = false;
  DeserializationError error;

  if (succeeded) {
    Serial.println(F("request was sent"));
    return;
  }

  httpsClient.setInsecure();
  httpsClient.connect(url, 443);
  http.begin(httpsClient, url);
  http.addHeader(YANDEX_HEADER, YANDEX_API_KEY);
  int httpCode = http.GET();
  String payload = http.getString();
  Serial.println(httpCode);
  Serial.println(payload);

  if (httpCode == 200) {
    error = deserializeJson(result, payload);
  }

  http.end();

  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

  JsonObject fact = result["fact"];
  Serial.println((int)fact["feels_like"]);
}

void simpleRequest() {
  WiFiClient client;
  HTTPClient http;

  http.begin(client, "https://jigsaw.w3.org/HTTP/connection.html");
  int httpCode = http.GET();
  String payload = http.getString();

  Serial.println(httpCode);
  Serial.println(payload);

  http.end();
}