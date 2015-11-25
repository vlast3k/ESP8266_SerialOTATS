IPAddress ip = WiFi.localIP();

void handleWifi() {
  if (ip == WiFi.localIP()) return;
  else if (WiFi.status() == WL_CONNECTED) {
    Serial << "IP address: " << WiFi.localIP() << endl << "GOT IP" << endl; 
  }
}

void connectToWifi(const char *s1, const char *s2, const char *s3) {
  if (strstr(s1, "SAP-Guest")) {
    if (s2 && s3) {
      setSSIDPass("SAP-Guest", "aaaa");
      for (int i=0; i<10 && WiFi.status() != WL_CONNECTED; i--) handleWifi();
      setSAPGuestCredentials(s2, s3);
    }
  } else {
    setSSIDPass(s1, s2);
  }
}

int wifiConnectToStoredSSID() {
  char ssid[30], pass[30];
  EEPROM.get(EE_WIFI_SSID_30B, ssid);
  EEPROM.get(EE_WIFI_PASS_30B, pass);
  Serial << "Connecting to: " << ssid << "," << pass << "." << endl;
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
}

void setSSIDPass(const char *ssid, const char *pass) {
  char tmp[30];
  strcpy(tmp, ssid);
  EEPROM.put(EE_WIFI_SSID_30B, tmp);
  strcpy(tmp, pass);
  EEPROM.put(EE_WIFI_PASS_30B, tmp);
  EEPROM.commit();
  wifiConnectToStoredSSID();
}

