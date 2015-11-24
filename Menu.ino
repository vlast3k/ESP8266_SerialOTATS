#define LINE_LEN 100
char line[LINE_LEN];
typedef int (*MenuCallback) ();
MenuCallback menuHandler;

void processUserInput() {
  byte len = 0;

  Serial.setTimeout(500);
  if (!Serial.available()) return;
  
  Serial.setTimeout(30000);
  while (readLine(30000) >= 0) {
    Serial.flush();
    removeCRNL(line);
    if (!handleCommand()) {
      Serial << endl << F("Exiting menu") << endl;
      return;
    }
  }

  Serial << F("Menu Timed out") << endl;
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
  else if (strstr(line, "sapAuth")) setSAPAuth(line);
  else if (strstr(line, "wifi")) setWifi(line);
  return 0;
}

byte readLine(int timeout) {
  unsigned long deadline = millis() + timeout;
  byte i = 0;
  while (millis() < deadline) {
    if (Serial.available()) {
      line[i++] = (char) Serial.read();
       if (line[i-1] == 10) {
          i--;
          continue;
        }

      if (i == LINE_LEN - 1) {
        break;
      } else if (line[i-1] == 13) {
        line[i-1] = 0;
        Serial.flush();
        return i-1;
      }
    }
  }
  line[i] = 0;
  return i;
}

void removeCRNL(char * str) {
  for (int i=0; i < strlen(str); i++) {
    if (str[i] == 10 || str[i] == 13) { 
      str[i] =0;
      return;
    }
  }
}

int setWifi(const char* p) {
  char s1[20], s2[20], s3[20];
  p = extractStringFromQuotes(p, s1);
  p = extractStringFromQuotes(p, s2);
  p = extractStringFromQuotes(p, s3);

  if (strstr(s1, "SAP-Guest")) {
    if (s2 && s3) {
      setSSIDPass("SAP-Guest", "aaaa");
      setSAPGuestCredentials(s2, s3);
    }
  } else {
    setSSIDPass(s1, s2);
  }
  return 0;
}

void mockATCommand(const char *line) {
  if (line[0] == 'A') {
    if (strstr(line, "AT+CWJAP_DEF")) setWifi(line);

    if (strstr(line, "AT+CIPSEND"))  Serial << ">" << endl; 
    else                             Serial << "OK" << endl;
  }
}
