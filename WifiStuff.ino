int wifiWaitForConnect() {
  Serial << "Wait for Auto-connect 10 sec" << endl;
  for (int i=10; i > 0; i++) {
    if (WiFi.status() == WL_CONNECTED) {
      Serial << endl;
      return 1;
    }
    Serial << ".";
  }
  Serial <<endl;
  return 0;
}

int wifiConnectToStoredSSID() {
  char ssid[30], pass[30];
  EEPROM.get(EE_WIFI_SSID_30B, ssid);
  EEPROM.get(EE_WIFI_PASS_30B, pass);
  Serial << "Connecting to: " << ssid << "," << pass << "." << endl;
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  return waitForConnect();
}


void startWifi(boolean doAutoconnect) {
  int res = 0;
  
  if (doAutoConnect) res = wifiWaitForConnect();
  if (!res)          res = wifiConnectToStoredSSD();

  if (!res) Serial << "Cannot connect to Wifi ! " << WiFi.status() << endl;
  else      Serial << "IP address: " << WiFi.localIP() << endl << "GOT IP" << endl;  
}

void setSSIDPass(char *ssid, char *pass) {
  char tmp[30];
  strcpy(tmp, ssid);
  EEPROM.put(EE_WIFI_SSID_30B, tmp);
  strcpy(tmp, pass);
  EEPROM.put(EE_WIFI_PASS_30B, tmp);
  EEPROM.commit();
  startWifi(false);
}

