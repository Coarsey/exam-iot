#include <Arduino.h>
#include "config.h"
#include "lighting/light.h"
#include "environment/climate.h"
#include "display/oled.h"
#include "communication/network.h"

float currentTemp = 0;
float currentHum = 0;
int currentLight = 0;

bool led27State = false;
bool autoMode = true;
bool climateAlert = false;

bool blinkState = false;
unsigned long lastBlinkTime = 0;

unsigned long lastMqttTime = 0;
unsigned long lastTelegramAlertTime = 0;

void setup() {
  Serial.begin(115200);

  pinMode(LED_ALERT, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_LIGHT, OUTPUT);

  digitalWrite(LED_ALERT, LOW);
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(LED_LIGHT, LOW);

  lightInit();
  climateInit();
  oledInit();
  networkInit();
}

void loop() {
  networkUpdate();
  
  // telegramBotUpdate(); 

  lightUpdate();
  climateUpdate();
  oledUpdate();
}