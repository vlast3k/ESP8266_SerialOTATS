void mockATCommand(const char *line) {
  if (line[0] == 'A') {
    if (strstr(line, "AT+CWJAP_DEF")) {
      int ss_start, ss_end, pass_start, pass_endl;
      char *ssid, *pass;
      char *x;
      if (!(ssid = strchr(line, '\"'))) goto end;
      ssid ++;;
      if (!(x = strchr(ssid, '\"'))) goto end;
      *x = 0;
      if (!(pass = strchr(x+1, '\"'))) goto end;
      pass ++;
      if (!(x = strchr(pass, '\"'))) goto end;
      *x = 0;
      Serial << "found : " << ssid << " , " << pass << endl;
      atSetWifi(ssid, pass);
    }

    end:
    if (strstr(line, "AT+CIPSEND")) {
      Serial << ">" << endl; 
    } else {
      Serial << "OK" << endl;
    }
  }

}


void atSetWifi(char *ssid, char *pass) {
  char tmp[30];
  strcpy(tmp, ssid);
  EEPROM.put(EE_WIFI_SSID_30B, tmp);
  strcpy(tmp, pass);
  EEPROM.put(EE_WIFI_PASS_30B, tmp);
  EEPROM.commit();
  startWifi();
}

