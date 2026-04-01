#ifndef NETWORK_H
#define NETWORK_H

void networkInit();
void networkUpdate();

void sendTelegram(const String& message);
void publishClimateAlert(const String& message);
void telegramBotUpdate();

#endif