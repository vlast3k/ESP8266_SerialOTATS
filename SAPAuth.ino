int setSAPGuestCredentials(const char *user, const char *pass) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial << "Cannot set SAP Guest credentials. No WiFi!" << endl;
    return -1;
  }
  Serial << "Setting SAP Auth: " << user << ", " << pass << endl;
  char tmp[20];
  strcpy(tmp, user);
  EEPROM.put(EE_WIFI_SG_USER_10B, tmp);
  strcpy(tmp, pass);
  EEPROM.put(EE_WIFI_SG_PASS_10B, tmp);
  EEPROM.commit();
  return checkSAPAuth();
}


int checkSAPAuth() {
  if (sendPing() == 302) {
    int as = httpAuthSAP();
    if (as == 302) {
      Serial << "SAP-Guest User/Pass - incorrect" << endl;
      return -1;
    }
    if (sendPing() != 200) {
      Serial << "Could not authenticate for SAP" << endl;
      return -1;
    }
  }

  Serial << "SAP Auth - OK!" << endl;
  return 1;
}


int httpAuthSAP() {
  char userName[10];// = "36121513";
  char pass[10];// = "57cKi5wE";
  EEPROM.get(EE_WIFI_SG_USER_10B, userName);
  EEPROM.get(EE_WIFI_SG_PASS_10B, pass);
  if (userName[0] < 2) {
    Serial << "Missing SAP-Guest user/pass" << endl;
    return -1;
  }
  char postData[100];
  sprintf(postData, "user=%s&password=%s&cmd=authenticate&url=http%3A%2F%2Fgoogle.bg%2F&Login=Log+In", userName, pass);

  String headers =  String("Content-Type: application/x-www-form-urlencoded\n")
                   + "User-Agent: Mozilla/5.0\n"
                   + "Referer: https://emea-guest.wlan.sap.com/guest/sap_guest_register_login.php?_browser=1\n";
                   
  int rc = sendHTTP("securelogin.wlan.sap.com", "POST", "/cgi-bin/login", headers.c_str(), postData, true, true);
  Serial << "SAP Auth Response is: " << rc << endl;
  if (rc == -1 || rc == 200) return 1;
  else return -1;
  // rc == -1 - timeout connect = connection exists
  // rc == 302 - bad user pass
  // rc == 200 - authenticated
}

int sendPing() {
  return sendHTTP("ping.eu", "HEAD", "/", NULL, NULL, false, true);

  //302 - no connection
  //200 - connection ok
}

