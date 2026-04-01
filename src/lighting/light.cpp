#include <Arduino.h>
#include "../config.h"
#include "light.h"

void lightInit() {
  pinMode(LDR_PIN, INPUT);
}

void lightUpdate() {
  currentLight = analogRead(LDR_PIN);

  if (autoMode) {
    if (currentLight > LIGHT_THRESHOLD) {
      led27State = true;
    } else {
      led27State = false;
    }
  }

  digitalWrite(LED_LIGHT, led27State ? HIGH : LOW);
}