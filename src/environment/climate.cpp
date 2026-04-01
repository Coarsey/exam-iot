#include <Arduino.h>
#include <DHT.h>
#include "../config.h"
#include "../communication/network.h"
#include "climate.h"

DHT dht(DHT_PIN, DHT11);
unsigned long lastDhtRead = 0;

void climateInit() {
  dht.begin();
}

void climateUpdate() {
  if (millis() - lastDhtRead >= DHT_INTERVAL_MS) {
    lastDhtRead = millis();

    float t = dht.readTemperature();
    float h = dht.readHumidity();

    if (!isnan(t)) currentTemp = t;
    if (!isnan(h)) currentHum = h;

    if (currentTemp > TEMP_LIMIT || currentHum > HUM_LIMIT) {
      climateAlert = true;
    } else {
      climateAlert = false;
    }
  }

  if (climateAlert) {
    if (millis() - lastBlinkTime >= BLINK_INTERVAL_MS) {
      lastBlinkTime = millis();
      blinkState = !blinkState;
      digitalWrite(LED_ALERT, blinkState ? HIGH : LOW);
    }

    digitalWrite(BUZZER_PIN, HIGH);

    if (millis() - lastTelegramAlertTime >= TELEGRAM_COOLDOWN_MS) {
      lastTelegramAlertTime = millis();

      String msg = "CANH BAO MOI TRUONG!\n";
      msg += "Nhiet do: " + String(currentTemp) + " C\n";
      msg += "Do am: " + String(currentHum) + " %";

      // sendTelegram(msg); 
      
      publishClimateAlert(msg);
    }
  } else {
    digitalWrite(LED_ALERT, LOW);
    digitalWrite(BUZZER_PIN, LOW);
  }
}