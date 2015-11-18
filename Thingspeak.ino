void sendTS() {
  sendHTTP("api.thingspeak.com", "GET", "/update?key=8U1HL3MF593FILFK&field1=456", false);
}

void sendGoogle() {
  sendHTTP("www.google.bg", "GET", "/", false);
}

void sendHTTP(const char* host, const char* method, const char *url, boolean secure) {
  WiFiClient *client;
  if (secure) client = new WiFiClientSecure();
  else client = new WiFiClient();
  const int httpPort = secure? 443 : 80;
  Serial << "Connecting to: " << host << ":" << httpPort << endl;
  if (!client->connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  
  Serial << "Requesting URL: " << url << endl;
  
  // This will send the request to the server
  client->print(String(method) + " " + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  int timeout = 0;
  while (!client->available()) {
    delay(10);
    if (timeout -= 10 > 3000) break;
  }
  
  Serial << "Response time: " << timeout << endl ;
  
  if (timeout < 3000) {
    Serial << endl;
    char buf[201];
    while(client->available()){
      buf[client->readBytes(buf, 200)] = 0;
      Serial << (buf);
    }
    Serial << endl << "--------- done" << endl;
  } else {
    Serial << "Timeout !" << endl;
  }
}

