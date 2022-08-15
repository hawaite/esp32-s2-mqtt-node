#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Adafruit_LC709203F.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <MQTT.h>

#include "defs.h"

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
  char buf[256];
  serializeJson(doc, buf);

  // post to mqtt
  MQTTClient mqttClient;
  mqttClient.begin(MQTT_BROKER_HOSTNAME, MQTT_BROKER_PORT, client);
  mqttClient.connect(MQTT_BOKER_CLIENT_ID, MQTT_BROKER_USERNAME, MQTT_BROKER_PASSWORD);
  mqttClient.publish(MQTT_TOPIC, buf);
  mqttClient.disconnect();

  // flush if needed
  client.flush();
  
  // deep sleep
  deepSleepNow();
}

void loop() {
  // do nothing
}