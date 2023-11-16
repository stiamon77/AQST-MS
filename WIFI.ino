void startWIFI (){

  byte mac[6];
  WiFi.macAddress(mac);
  
  #if defined(ESP8266)
  
  chipId = ESP.getChipId();

  #elif defined(ESP32)

  for(int i=0; i<17; i=i+8) {
	chipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
	}

  #endif

  device.setUniqueId(mac, sizeof(mac));
  
  clientId = "AQST-MS-"+String(chipId, HEX);  //Please don't change this. Needed for version control

  //init the WiFi connection
  #ifdef DEBUG
  Serial.println();
  Serial.println();
  Serial.print("INFO: Connecting to ");
  #endif

  WiFi.mode(WIFI_STA);

  #ifdef DEBUG
  Serial.println(WIFI_SSID);
  #endif 

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    #ifdef DEBUG
    Serial.print(".");
    #endif 
  }
  
  #ifdef DEBUG
  Serial.println("");
  Serial.println("INFO: WiFi connected");
  Serial.print("INFO: IP address: ");
  Serial.println(WiFi.localIP());

  Serial.print(". Signal strength: ");
  Serial.print(2*(WiFi.RSSI()+100));
  Serial.println("%");

  Serial.println();
  Serial.print("MAC: ");
  Serial.println(WiFi.macAddress());    

      
  Serial.print("Free Heap size: ");
  Serial.print(ESP.getFreeHeap()/1024);
  Serial.println("Kb");
  #endif 

  clientId.toUpperCase();

  WiFi.hostname(clientId); //esp32
}
