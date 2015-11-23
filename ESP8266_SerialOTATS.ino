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
#define EE_WIFI_SG_USER_10B 60
#define EE_WIFI_SG_PASS_10B 70
#define EE_LAST 80
void setup() {
  Serial.begin(9600);
  EEPROM.begin(1024);
  Serial << endl << "ready" << endl;
  startWifi();
}

void startWifi() {
  char ssid[30], pass[30];
  EEPROM.get(EE_WIFI_SSID_30B, ssid);
  EEPROM.get(EE_WIFI_PASS_30B, pass);
  Serial << "Connecting to: " << ssid << endl;
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("3Cannot connect to Wifi !" );
    return;
  }
//  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
//    Serial.println("Cannot connect to Wifi !");
//    return;
//  }
  Serial << "IP address: " << WiFi.localIP() << endl;
  Serial << "GOT IP" << endl;
}

boolean startedOTA = false;
void loop() {
  handleOTA();
  if (!startedOTA) {
    processUserInput();
  }
}
