# esp32-s2-mqtt-node
Firmware for an ESP32 board to publish metrics to MQTT, built with PlatformIO.

For now this is very specifically targetted at the AdaFruit feather-s2 with the onboard LiPo fuel gague and BME280 temperature/humidity sensor. JSON payloads of the sensor data are sent to an MQTT broker, whcih can then be consumed by applications such as Home Assistant.
