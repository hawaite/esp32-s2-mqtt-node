#include <Arduino.h>

#define uS_TO_S_FACTOR 1000000ULL  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  600         /* Time ESP32 will go to sleep (in seconds) - 10 minutes */
#define MQTT_TOPIC "/home/office"
#define IS_BATTERY_POWERED false /* Used to determine if battery stats should be published */

void setup() {
  // get temperature & humidity
  // maybe get battery stats
  // start wifi
  // post to mqtt
  // flush if needed

  // deep sleep
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  esp_deep_sleep_start();
}

void loop() {
  // do nothing
}