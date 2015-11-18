#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <Streaming.h>
#include <EEPROM.h>


void startOTA();

#define EE_WIFI_SSID_30B 0
#define EE_WIFI_PASS_30B 30
#define EE_LAST 60
void setup() {
  Serial.begin(9600);
  EEPROM.begin(1024);
  Serial.println("ready");
  startWifi();
}

void startWifi() {
  char ssid[30], pass[30];
  EEPROM.get(EE_WIFI_SSID_30B, ssid);
  EEPROM.get(EE_WIFI_PASS_30B, pass);
  Serial << "Connecting to: " << ssid << endl;
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! cannot connect to Wifi...");
    return;
  }
  Serial << "IP address: " << WiFi.localIP() << endl;
  Serial << "GOT IP" << endl;
}

void loop() {
  handleOTA();
  processUserInput();
}
