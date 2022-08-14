#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Adafruit_LC709203F.h>
#include <ArduinoJson.h>
#include <WiFi.h>

#define SSID "xxx"
#define WIFI_PASSWORD "xxx"
#define TARGET_HOSTNAME "xxx"
#define TARGET_PORT 9999
#define uS_TO_S_FACTOR 1000000ULL  /* Conversion factor for micro seconds to seconds */
//#define TIME_TO_SLEEP  600         /* Time ESP32 will go to sleep (in seconds) - 10 minutes */
#define TIME_TO_SLEEP  10         /* Time ESP32 will go to sleep (in seconds) - 10 seconds */
#define MQTT_TOPIC "/home/office"
//#define IS_BATTERY_POWERED /* Used to determine if battery stats should be published */

void deepSleepNow(){
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  esp_deep_sleep_start();
}

void setup() {
  DynamicJsonDocument doc(256);

  // get temperature & humidity
  Adafruit_BME280 bme;
  bool status = bme.begin();
  if(!status){
    deepSleepNow();
  }

  doc["temperature"] = String(bme.readTemperature());
  doc["humidity"] = String(bme.readHumidity());

  // maybe get battery stats
#ifdef IS_BATTERY_POWERED
  Adafruit_LC709203F lc;
  lc.begin();
  lc.setPowerMode(LC709203F_POWER_OPERATE);

  lc.setPackSize(LC709203F_APA_500MAH);

  doc["batt_voltage"] = String(lc.cellVoltage());
  doc["batt_percent"] = String(lc.cellPercent());

  lc.setPowerMode(LC709203F_POWER_SLEEP);
#endif
  // start wifi
  WiFi.mode(WIFI_MODE_STA);
  WiFi.begin(SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }

  WiFiClient client;
  client.connect(TARGET_HOSTNAME, TARGET_PORT);
  char buf[256];
  serializeJson(doc, buf);

  client.write(buf);
  client.write("\n");
  client.flush();
  // post to mqtt
  // flush if needed

  // deep sleep
  deepSleepNow();
}

void loop() {
  // do nothing
}