void sendTS() {
  //sendHTTP("api.thingspeak.com", "GET", "/update?key=8U1HL3MF593FILFK&field1=456", false);
}

void getTS(const char* line) {
  sendHTTP("api.thingspeak.com", "GET", line + 4, NULL, NULL, false, false);
}


boolean traceHttp = true;

void setSAPAuth(const char *line)  {
  char user[20], pass[20];
  if (extractTwoStringsFromQuotes(line, user, pass)) {
    setSAPGuestCredentials(user, pass);
  } else {
    Serial << "Could not parse SAP Auth credentials" << endl;
  } 
}

int setSAPGuestCredentials(char *user, char *pass) {
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

int sendPing() {
  return sendHTTP("ping.eu", "HEAD", "/", NULL, NULL, false, true);

  //302 - no connection
  //200 - connection ok
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

int sendHTTP(const char* host, const char* method, const char *url, const char* headers, const char* postData, boolean secure, boolean sendHeaders) {
  WiFiClient *client;
  if (secure) client = new WiFiClientSecure();
  else client = new WiFiClient();
  const int httpPort = secure? 443 : 80;
  
  Serial << "Connecting to: " << host << ":" << httpPort << endl;
  if (!client->connect(host, httpPort)) {
    Serial.println("connection failed");
    return -1;
  }

  
  //Serial << "Requesting URL: " << url << endl;
  String rq = String(method) + " " + url + (sendHeaders?" HTTP/1.1":"") + "\r\n";
  if (sendHeaders)
     rq += String("Host: ") + host + "\r\n" + 
           (postData? (String("Content-Length: ") + strlen(postData) + "\r\n") : "") +
           (headers ? headers : "\r\n");
   rq += "\r\n";
  // This will send the request to the server
  Serial << "Requesting: " << endl << rq << endl;
  client->print(rq);

  if (postData) client->print(postData);
  
  Serial << "Waiting Respomse " << endl;
  int timeout = 0;
  while (!client->available()) {
    if (timeout += 10 > 3000) break;
    delay(10);
  }
  Serial << "Response time: " << timeout << endl ;
  
  if (timeout < 3000) {
    Serial << endl;
    char buf[201];
    int responseCode = -1;
    while(client->available()){
      buf[client->readBytes(buf, 200)] = 0;
      if (responseCode == -1) responseCode = detectResponseCode(buf);
      if (traceHttp) Serial << (buf);
    }
    Serial << endl << "--------- done, rc=" << responseCode << endl;
    Serial << endl << "CLOSED" << endl;
    return responseCode;
  } else {
    Serial << "Timeout !" << endl;
    return -2;
  }
}

int detectResponseCode(char *buf) {
  char *ch = strchr(buf, ' ');
  if (ch) {
    char *ch2 = strchr(ch+1, ' ');
    if (ch2) {
      char res[4];
      strncpy(res, ch+1, 3);
      res[3] = 0;
      return atoi(res);
    }
  }
  return 0;
}
  
