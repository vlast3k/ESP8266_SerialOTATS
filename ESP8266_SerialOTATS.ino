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
#define EE_WIFI_P1_30B 30
#define EE_WIFI_P2_30B 60
#define EE_IOT_HOST
#define EE_LAST 90
void setup() {
  Serial.begin(9600);
  EEPROM.begin(1024);
  Serial << endl << "ready" << endl;
  //startWifi();
  Serial << "Waiting for auto-connect" << endl;
}


boolean startedOTA = false;
void loop() {
  handleWifi();
  handleOTA();
  if (!startedOTA) {
    processUserInput();
  }
}
