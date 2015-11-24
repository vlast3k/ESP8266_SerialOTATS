#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <Streaming.h>
#include <EEPROM.h>
#include <Math.h>
#include <algorithm>    // std::min

void setSAPAuth(const char *);
#define EE_WIFI_SSID_30B 0
#define EE_WIFI_PASS_30B 30
#define EE_WIFI_SG_USER_10B 60
#define EE_WIFI_SG_PASS_10B 70
#define EE_LAST 80
void setup() {
  Serial.begin(9600);
  EEPROM.begin(1024);
  Serial << endl << "ready" << endl;
  startWifi();
}


boolean startedOTA = false;
void loop() {
  handleOTA();
  if (!startedOTA) {
    processUserInput();
  }
}
