#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// wifi
#define WIFI_SSID "Hirohito"
#define WIFI_PASS "123456789a"
//hime mqtt
#define MQTT_SERVER "4f7757dcfbe34fce84f94779c72a4976.s1.eu.hivemq.cloud"
#define MQTT_PORT 8883
#define MQTT_USER "otmahye"
#define MQTT_PASS "Khongnoi123aa"

//tele
#define BOT_TOKEN "8628303149:AAGVSiAB1C0ltuLYtil9FZufBO3Px2Jc_Es"
#define CHAT_ID   "5140203528"


#define LED_ALERT   4
#define BUZZER_PIN  13
#define DHT_PIN     18
#define LDR_PIN     34
#define OLED_SDA    21
#define OLED_SCL    22
#define LED_LIGHT   27

#define TEMP_LIMIT 35.0f
#define HUM_LIMIT  75.0f
#define LIGHT_THRESHOLD 4000


#define DHT_INTERVAL_MS 2000
#define BLINK_INTERVAL_MS 300
#define MQTT_INTERVAL_MS 3000
#define TELEGRAM_COOLDOWN_MS 30000
#define TELEGRAM_INTERVAL_MS 1000


extern float currentTemp;
extern float currentHum;
extern int currentLight;

extern bool led27State;
extern bool autoMode;
extern bool climateAlert;

extern bool blinkState;
extern unsigned long lastBlinkTime;

extern unsigned long lastMqttTime;
extern unsigned long lastTelegramAlertTime;

#endif