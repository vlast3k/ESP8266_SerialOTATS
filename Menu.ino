#define LINE_LEN 100
char line[LINE_LEN];

void processUserInput() {
  Serial.setTimeout(500);
  if (!Serial.available()) return;
  
  Serial.setTimeout(30000);
  while (readLine(30000) >= 0) {
    Serial.flush();
    if (!handleCommand()) {
      Serial << endl << F("Exiting menu") << endl;
      return;
    }
  }
  Serial << F("Menu Timed out") << endl;
}

byte readLine(int timeout) {
  unsigned long deadline = millis() + timeout;
  byte i = 0;
  while (millis() < deadline) {
    if (Serial.available()) {
      line[i++] = (char) Serial.read();
      if      (line[i-1] == '\r')  i--;   
      else if (i == LINE_LEN - 1)  break; 
      else if (line[i-1] == '\n')  {i--; break;}
    }
  }
  line[i] = 0;
  return i;
}

int handleCommand() {
  Serial << "Received command: " << line << endl;
  if      (line[0] == 'o') startOTA();
  else if (line[0] == 't') sendTS();
  else if (line[0] == 'p') sendPing();
  else if (line[0] == 'A') mockATCommand(line);
  else if (line[0] == 'S') httpAuthSAP();
  else if (line[0] == 'C') checkSAPAuth();
  else if (line[0] == 'G') getTS(line);
  else if (strstr(line, "wifi")) setWifi(line);
  return 0;
}


void getTS(const char* line) {
  sendHTTP("api.thingspeak.com", "GET", line + 4, NULL, NULL, false, false);
}

void sendTS() {
  sendHTTP("api.thingspeak.com", "GET", "/update?key=8U1HL3MF593FILFK&field1=456", NULL, NULL, false, false);
}

int setWifi(const char* p) {
  char s1[20], s2[20], s3[20];
  p = extractStringFromQuotes(p, s1);
  p = extractStringFromQuotes(p, s2);
  p = extractStringFromQuotes(p, s3);
  Serial << "setWifi" << s1 << s2 << s3 << endl;

  connectToWifi(s1, s2, s3);
  return 0;
}

void mockATCommand(const char *line) {
  if (line[0] == 'A') {
    if (strstr(line, "AT+CWJAP_DEF")) setWifi(line);

    if (strstr(line, "AT+CIPSEND"))  Serial << ">" << endl; 
    else                             Serial << "OK" << endl;
  }
}
