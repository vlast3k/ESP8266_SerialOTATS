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
/*
 * "AT+CIPSTART=\"TCP\",\"184.106.153.149\",80")
 * "GET /update?key=%s&field1=%d&field2=%d.%d&field3=%d.%d&field4=%d.%d&field5=%d.%d\n\n"
 *   esp << F("AT+CIPSEND=") << len << endl;
  if (!serialFind(">", true, 6000)) return -3;
  esp << sendstr;

   if (!serialFind("GOT IP", true, 30000)) {

     esp << F("AT+CWMODE_DEF=1") << endl;
       esp << F("AT") << endl;
         if (!serialFind(OK)) return -1;
  serialFind(OK, ESP_DEBUG, 1000);
  esp << F("AT+CWJAP_DEF=") << F("\"") << ssid << F("\"") << F(",") << F("\"") << pass << F("\"") << endl;
  if (!serialFind(GOTIP, ESP_DEBUG, 20000)) return -2;
  esp << F("AT+CWAUTOCONN=1") << endl;

  serialFind("ready", ESP_DEBUG, 6000);
 */
int handleCommand() {
//  String data = String(line).substring(2);
  Serial << "Received command: " << line << endl;
  if      (line[0] == 'o') startOTA();
  else if (line[0] == 't') sendTS();
  else if (line[0] == 'p') sendPing();
  else if (line[0] == 'A') mockATCommand(line);
  else if (line[0] == 'S') httpAuthSAP();
  else if (line[0] == 'C') checkSAPAuth();
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
