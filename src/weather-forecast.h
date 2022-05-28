#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

#define YANDEX_HEADER "X-Yandex-API-Key"
#define YANDEX_API_KEY "3aedc6bc-f68d-4ab9-8377-012f1db3cd48"
#define YANDEX_API_URI "https://api.weather.yandex.ru/v2/informers"

struct ForecastPeriod {
  const char *periodName;
  int8_t temperatureMin;
  int8_t temperatureMax;
  int8_t temperatureAvg;
  int8_t feelsLike;
  uint8_t humidity;
  const char *condition;

  float windSpeed;
  float windGust;
  const char *windDirection;
  unsigned int pressureMM;
  unsigned int pressurePA;

  unsigned int precipitationMM;
  uint8_t precipitationProbability;
  unsigned int precipitationMinutes;

  char *daytime;
};

struct ForecastCurrent {
  int8_t temperature;
  int8_t feelsLike;
  uint8_t humidity;
  const char *condition;
  float windSpeed;
  float windGust;
  const char *windDirection;
  unsigned int pressureMM;
  unsigned int pressurePA;
  const char *daytime;
  const char *season;

  const char *sunrise;
  const char *sunset;
  uint8_t moonCode;
};

struct Forecast {
  ForecastCurrent current;
  ForecastPeriod periods[2];
};

bool parseResponse(Forecast *forecast, String &response) {
  DynamicJsonDocument result(2048);
  DeserializationError error;

  error = deserializeJson(result, response);

  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return true;
  }

  JsonObject resultFact = result["fact"];
  JsonObject resultForecast = result["forecast"];

  forecast->current.temperature = resultFact["temp"];
  forecast->current.feelsLike = resultFact["feels_like"];
  forecast->current.humidity = resultFact["humidity"];
  forecast->current.condition = resultFact["condition"];
  forecast->current.windSpeed = resultFact["wind_speed"];
  forecast->current.windGust = resultFact["wind_gust"];
  forecast->current.windDirection = resultFact["wind_dir"];
  forecast->current.pressureMM = resultFact["pressure_mm"];
  forecast->current.pressurePA = resultFact["pressure_pa"];
  forecast->current.daytime = resultFact["daytime"];
  forecast->current.season = resultFact["season"];
  forecast->current.sunrise = resultForecast["sunrise"];
  forecast->current.sunset = resultForecast["sunset"];
  forecast->current.moonCode = resultForecast["moon_code"];

  int index = 0;
  for (JsonObject resultForecastPeriod : resultForecast["parts"].as<JsonArray>()) {
    forecast->periods[index].periodName = resultForecastPeriod["part_name"];
    forecast->periods[index].temperatureMin = resultForecastPeriod["temp_min"];
    forecast->periods[index].temperatureMax = resultForecastPeriod["temp_max"];
    forecast->periods[index].temperatureAvg = resultForecastPeriod["temp_avg"];
    forecast->periods[index].feelsLike = resultForecastPeriod["feels_like"];
    forecast->periods[index].humidity = resultForecastPeriod["humidity"];
    forecast->periods[index].condition = resultForecastPeriod["condition"];
    forecast->periods[index].windSpeed = resultForecastPeriod["wind_speed"];
    forecast->periods[index].windGust = resultForecastPeriod["wind_gust"];
    forecast->periods[index].windDirection = resultForecastPeriod["wind_dir"];
    forecast->periods[index].pressureMM = resultForecastPeriod["pressure_mm"];
    forecast->periods[index].pressurePA = resultForecastPeriod["pressure_pa"];
    forecast->periods[index].precipitationMM = resultForecastPeriod["prec_mm"];
    forecast->periods[index].precipitationProbability = resultForecastPeriod["prec_prob"];
    forecast->periods[index].precipitationMinutes = resultForecastPeriod["prec_period"];
    index++;
  }

  return false;
}

bool getForecast(Forecast *forecast, const char *latitude, const char *longitude) {
  WiFiClientSecure httpsClient;
  HTTPClient http;
  String url = String(YANDEX_API_URI) + F("?lat=") + latitude + F("&lon=") + longitude;

  httpsClient.setInsecure();
  httpsClient.connect(url, 443);

  http.begin(httpsClient, url);
  http.addHeader(YANDEX_HEADER, YANDEX_API_KEY);

  int httpCode = http.GET();
  String payload = http.getString();

  bool hasError = false;
  if (httpCode == 200) {
    hasError = parseResponse(forecast, payload);
  }

  http.end();

  return hasError;
}
