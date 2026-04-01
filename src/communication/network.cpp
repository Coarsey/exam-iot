#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <UniversalTelegramBot.h>
#include "../config.h"
#include "network.h"

WiFiClientSecure mqttClient;
PubSubClient client(mqttClient);

WiFiClientSecure telegramClient;
UniversalTelegramBot bot(BOT_TOKEN, telegramClient);

unsigned long lastTelegramCheck = 0;
int lastUpdateId = 0;

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  String cmd = "";

  for (unsigned int i = 0; i < length; i++) {
    cmd += (char)payload[i];
  }

  cmd.trim();

  Serial.print("Nhan topic: ");
  Serial.println(topic);
  Serial.print("Lenh: ");
  Serial.println(cmd);

  if (String(topic) == "esp/light") {
    if (cmd == "1" || cmd == "ON" || cmd == "on" || cmd == "true") {
      autoMode = false;
      led27State = true;
    }
    else if (cmd == "0" || cmd == "OFF" || cmd == "off" || cmd == "false") {
      autoMode = false;
      led27State = false;
    }
    else if (cmd == "AUTO" || cmd == "auto") {
      autoMode = true;
    }

    digitalWrite(LED_LIGHT, led27State ? HIGH : LOW);
  }
}

void connectWifi() {
  Serial.print("Dang ket noi WiFi");
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("Da ket noi WiFi");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

void connectMqtt() {
  while (!client.connected()) {
    Serial.print("Dang ket noi MQTT...");

    if (client.connect("ESP32_Client", MQTT_USER, MQTT_PASS)) {
      Serial.println("OK");
      client.subscribe("esp/light");
      Serial.println("Sub: esp/light");
    } else {
      Serial.print("Loi MQTT: ");
      Serial.println(client.state());
      delay(3000);
    }
  }
}

String urlEncode(String text) {
  text.replace("%", "%25");
  text.replace(" ", "%20");
  text.replace("\n", "%0A");
  text.replace(":", "%3A");
  return text;
}

void sendTelegram(const String& message) {
  if (WiFi.status() != WL_CONNECTED) return;

  HTTPClient https;

  String url = "https://api.telegram.org/bot" + String(BOT_TOKEN)
             + "/sendMessage?chat_id=" + String(CHAT_ID)
             + "&text=" + urlEncode(message);

  telegramClient.setInsecure();
  https.begin(telegramClient, url);
  https.GET();
  https.end();
}

void publishClimateAlert(const String& message) {
  client.publish("esp/climate/alert", message.c_str());
}

void handleTelegramMessage(String text) {
  if (text == "/start") {
    String msg = "Menu:\n";
    msg += "/on\n";
    msg += "/off\n";
    msg += "/auto\n";
    msg += "/status";

    bot.sendMessage(CHAT_ID, msg, "");
  }
  else if (text == "/on") {
    autoMode = false;
    led27State = true;
    digitalWrite(LED_LIGHT, HIGH);
    bot.sendMessage(CHAT_ID, "LED ON", "");
  }
  else if (text == "/off") {
    autoMode = false;
    led27State = false;
    digitalWrite(LED_LIGHT, LOW);
    bot.sendMessage(CHAT_ID, "LED OFF", "");
  }
  else if (text == "/auto") {
    autoMode = true;
    bot.sendMessage(CHAT_ID, "AUTO MODE", "");
  }
  else if (text == "/status") {
    String msg = "TRANG THAI HE THONG\n";
    msg += "Nhiet do: " + String(currentTemp) + " C\n";
    msg += "Do am: " + String(currentHum) + " %\n";
    msg += "Anh sang: " + String(currentLight) + "\n";
    msg += "LED27: " + String(led27State ? "ON" : "OFF") + "\n";
    msg += "Mode: " + String(autoMode ? "AUTO" : "MANUAL");

    bot.sendMessage(CHAT_ID, msg, "");
  }
}

void telegramBotUpdate() {
  if (millis() - lastTelegramCheck < TELEGRAM_INTERVAL_MS) return;
  lastTelegramCheck = millis();

  int numNewMsg = bot.getUpdates(lastUpdateId + 1);

  while (numNewMsg) {
    for (int i = 0; i < numNewMsg; i++) {
      String chat_id = bot.messages[i].chat_id;
      String text = bot.messages[i].text;

      if (chat_id != CHAT_ID) continue;

      handleTelegramMessage(text);
      lastUpdateId = bot.messages[i].update_id;
    }

    numNewMsg = bot.getUpdates(lastUpdateId + 1);
  }
}

void networkInit() {
  connectWifi();

  mqttClient.setInsecure();
  telegramClient.setInsecure();

  client.setServer(MQTT_SERVER, MQTT_PORT);
  client.setCallback(mqttCallback);
}

void networkUpdate() {
  if (WiFi.status() != WL_CONNECTED) {
    connectWifi();
  }

  if (!client.connected()) {
    connectMqtt();
  }

  client.loop();

  if (millis() - lastMqttTime >= MQTT_INTERVAL_MS) {
    lastMqttTime = millis();

    String envJson = "{\"temp\":" + String(currentTemp, 1) +
                     ",\"hum\":" + String(currentHum, 1) + "}";

    client.publish("esp/environment", envJson.c_str());
    client.publish("esp/lighting", String(currentLight).c_str());
    client.publish("esp/light/status", led27State ? "1" : "0");
  }
}