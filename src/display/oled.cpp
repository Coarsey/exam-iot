#include <Arduino.h>
#include <Wire.h>
#include <U8g2lib.h>
#include "../config.h"
#include "oled.h"

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);
unsigned long lastOledTime = 0;

void oledInit() {
  Wire.begin(OLED_SDA, OLED_SCL);
  u8g2.begin();
}

void oledUpdate() {
  if (millis() - lastOledTime < 1000) return;
  lastOledTime = millis();

  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x12_tf);

  u8g2.drawStr(0, 12, "HE THONG GIAM SAT");

  String line1 = "Temp: " + String(currentTemp) + " C";
  String line2 = "Hum : " + String(currentHum) + " %";
  String line3 = "Light: " + String(currentLight);
  String line4 = String("LED27: ") + (led27State ? "ON" : "OFF");

  u8g2.drawStr(0, 28, line1.c_str());
  u8g2.drawStr(0, 40, line2.c_str());
  u8g2.drawStr(0, 52, line3.c_str());
  u8g2.drawStr(0, 64, line4.c_str());

  u8g2.sendBuffer();
}