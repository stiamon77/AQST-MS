void startWIFI (){

  WiFi.begin(); // Mandatory
  delay(1000);

  #ifdef DEBUG
  String TEMP_Ssid = WiFi.SSID();
  String TEMP_Pass = WiFi.psk();
  #endif

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
  
  clientId = "AQST-MS-"+String(chipId, HEX);

  //init the WiFi connection
  #ifdef DEBUG
  Serial.println();
  Serial.println();
  Serial.print("INFO: Connecting to ");
  #endif

  WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP 

  #ifdef DEBUG
  Serial.println(WIFI_SSID);
  #endif

  Serial.setDebugOutput(false); 

  delay(3000);

  #ifdef DEBUG
  Serial.println(TEMP_Ssid);
  Serial.println(TEMP_Pass);
  #endif

  int var = 0;
  while (WiFi.status() != WL_CONNECTED && var < 100) {
    delay(500);

    #ifdef DEBUG
    Serial.print(".");
    #endif 

    var++;
  }

  pinMode(TRIGGER_PIN, INPUT_PULLUP);

  // Serial.println("local ip");
  // Serial.println(WiFi.localIP());

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

  // set Hostname

  wifiManager.setHostname(clientId.c_str());

  //clean FS, for testing
  //SPIFFS.format();

  //read configuration from FS json
  #ifdef DEBUG
  Serial.println("mounting FS...");
  #endif

  if (SPIFFS.begin()) {

    #ifdef DEBUG
    Serial.println("mounted file system");
    #endif

    if (SPIFFS.exists("/config.json")) {
      //file exists, reading and loading

      #ifdef DEBUG
      Serial.println("reading config file");
      #endif

      File configFile = SPIFFS.open("/config.json", "r");
      if (configFile) {

        #ifdef DEBUG
        Serial.println("opened config file");
        #endif

        size_t size = configFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);

        configFile.readBytes(buf.get(), size);

 #if defined(ARDUINOJSON_VERSION_MAJOR) && ARDUINOJSON_VERSION_MAJOR >= 6
        DynamicJsonDocument json(1024);
        auto deserializeError = deserializeJson(json, buf.get());

        #ifdef DEBUG
        serializeJson(json, Serial);
        #endif

        if ( ! deserializeError ) {
#else
        DynamicJsonBuffer jsonBuffer;
        JsonObject& json = jsonBuffer.parseObject(buf.get());

        #ifdef DEBUG
        json.printTo(Serial);
        #endif

        if (json.success()) {
#endif

          #ifdef DEBUG
          Serial.println("\nparsed json");
          #endif

          strcpy(mqtt_server, json["mqtt_server"]);
          strcpy(mqtt_port, json["mqtt_port"]);
          strcpy(mqtt_user, json["mqtt_user"]);
          strcpy(mqtt_password, json["mqtt_password"]);
        } else {

          #ifdef DEBUG
          Serial.println("failed to load json config");
          #endif

        }
        configFile.close();
      }
    }
  } else {

    #ifdef DEBUG
    Serial.println("failed to mount FS");
    #endif

  }
  //end read

}


void checkButton(){

  // check for button press
  if ( digitalRead(TRIGGER_PIN) == LOW ) {
    // poor mans debounce/press-hold, code not ideal for production
    delay(50);
    if( digitalRead(TRIGGER_PIN) == LOW ){

      #ifdef DEBUG
      Serial.println("Button Pressed");
      #endif

      // still holding button for 3000 ms, reset settings, code not ideaa for production
      delay(3000); // reset delay hold

      if( digitalRead(TRIGGER_PIN) == LOW ){

        #ifdef DEBUG
        Serial.println("Button Held");
        Serial.println("Erasing Config, restarting");
        #endif

        wifiManager.resetSettings();
        ESP.restart();
      }
      
      // start portal w delay
      #ifdef DEBUG
      Serial.println("Starting config portal");
      #endif

      wifiManager.setConfigPortalTimeout(300);

      //clean FS, for testing
      //SPIFFS.format();

      //read configuration from FS json
      #ifdef DEBUG
      Serial.println("mounting FS...");
      #endif

      if (SPIFFS.begin()) {

        #ifdef DEBUG
        Serial.println("mounted file system");
        #endif

        if (SPIFFS.exists("/config.json")) {
          //file exists, reading and loading

          #ifdef DEBUG
          Serial.println("reading config file");
          #endif

          File configFile = SPIFFS.open("/config.json", "r");
          if (configFile) {

            #ifdef DEBUG
            Serial.println("opened config file");
            #endif

            size_t size = configFile.size();
            // Allocate a buffer to store contents of the file.
            std::unique_ptr<char[]> buf(new char[size]);

            configFile.readBytes(buf.get(), size);

    #if defined(ARDUINOJSON_VERSION_MAJOR) && ARDUINOJSON_VERSION_MAJOR >= 6
            DynamicJsonDocument json(1024);
            auto deserializeError = deserializeJson(json, buf.get());

            #ifdef DEBUG
            serializeJson(json, Serial);
            #endif

            if ( ! deserializeError ) {
    #else
            DynamicJsonBuffer jsonBuffer;
            JsonObject& json = jsonBuffer.parseObject(buf.get());
            
            #ifdef DEBUG
            json.printTo(Serial);
            #endif

            if (json.success()) {
    #endif
              
              #ifdef DEBUG
              Serial.println("\nparsed json");
              #endif

              strcpy(mqtt_server, json["mqtt_server"]);
              strcpy(mqtt_port, json["mqtt_port"]);
              strcpy(mqtt_user, json["mqtt_user"]);
              strcpy(mqtt_password, json["mqtt_password"]);
            } else {
              Serial.println("failed to load json config");
            }
            configFile.close();
          }
        }
      } else {

        #ifdef DEBUG
        Serial.println("failed to mount FS");
        #endif

      }
      //end read

      // The extra parameters to be configured (can be either global or just in the setup)
      // After connecting, parameter.getValue() will get you the configured value
      // id/name placeholder/prompt default length
      WiFiManagerParameter custom_mqtt_server("server", "mqtt server", mqtt_server, 40);
      WiFiManagerParameter custom_mqtt_port("port", "mqtt port", mqtt_port, 10);
      WiFiManagerParameter custom_mqtt_user("user", "mqtt user", mqtt_user, 32);
      WiFiManagerParameter custom_mqtt_password("password", "Mqtt password", mqtt_password, 32);

      //set config save notify callback
      wifiManager.setSaveConfigCallback(saveConfigCallback);

      //add all your parameters here
      wifiManager.addParameter(&custom_mqtt_server);
      wifiManager.addParameter(&custom_mqtt_port);
      wifiManager.addParameter(&custom_mqtt_user);
      wifiManager.addParameter(&custom_mqtt_password);

      //reset settings - for testing
      //wifiManager.resetSettings();

      //set minimu quality of signal so it ignores AP's under that quality
      //defaults to 8%
      //wifiManager.setMinimumSignalQuality();

      //sets timeout until configuration portal gets turned off
      //useful to make it all retry or go to sleep
      //in seconds
      //wifiManager.setTimeout(120);

      if (!wifiManager.startConfigPortal((clientId+"-CONFIG").c_str())) {

        #ifdef DEBUG
        Serial.println("failed to connect or hit timeout");
        #endif

        delay(3000);
        // ESP.restart();
      } else {

        //if you get here you have connected to the WiFi
        #ifdef DEBUG
        Serial.println("connected...yeey :)");
        Serial.println("local ip");
        Serial.println(WiFi.localIP());
        #endif

      }

      //if you get here you have connected to the WiFi
      #ifdef DEBUG
      Serial.println("connected...yeey :)");
      #endif

      //read updated parameters
      strcpy(mqtt_server, custom_mqtt_server.getValue());
      strcpy(mqtt_port, custom_mqtt_port.getValue());
      strcpy(mqtt_user, custom_mqtt_user.getValue());
      strcpy(mqtt_password, custom_mqtt_password.getValue());

      #ifdef DEBUG
      Serial.println("The values in the file are: ");
      Serial.println("\tmqtt_server : " + String(mqtt_server));
      Serial.println("\tmqtt_port : " + String(mqtt_port));
      Serial.println("\tmqtt_user : " + String(mqtt_user));
      Serial.println("\tmqtt_password : " + String(mqtt_password));
      #endif

      //save the custom parameters to FS
      if (shouldSaveConfig) {

        #ifdef DEBUG
        Serial.println("2saving config");
        #endif

        #if defined(ARDUINOJSON_VERSION_MAJOR) && ARDUINOJSON_VERSION_MAJOR >= 6
        DynamicJsonDocument json(1024);
        #else
        DynamicJsonBuffer jsonBuffer;
        JsonObject& json = jsonBuffer.createObject();
        #endif
        json["mqtt_server"] = mqtt_server;
              json["mqtt_port"] = mqtt_port;
              json["mqtt_user"] = mqtt_user;
              json["mqtt_password"] = mqtt_password;

        File configFile = SPIFFS.open("/config.json", "w");
        if (!configFile) {

          #ifdef DEBUG
          Serial.println("failed to open config file for writing");
          #endif

        }

        #if defined(ARDUINOJSON_VERSION_MAJOR) && ARDUINOJSON_VERSION_MAJOR >= 6

        #ifdef DEBUG
        serializeJson(json, Serial);
        #endif 

        serializeJson(json, configFile);
        #else

        #ifdef DEBUG
        json.printTo(Serial);
        #endif 

        json.printTo(configFile);
        #endif
        configFile.close();
        //end save
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

    ESP.restart();

    }
  }
}











//DEL
// void startWIFI (){

//   byte mac[6];
//   WiFi.macAddress(mac);
  
//   #if defined(ESP8266)
  
//   chipId = ESP.getChipId();

//   #elif defined(ESP32)

//   for(int i=0; i<17; i=i+8) {
// 	chipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
// 	}

//   #endif

//   device.setUniqueId(mac, sizeof(mac));
  
//   clientId = "AQST-MS-"+String(chipId, HEX);

//   //init the WiFi connection
//   #ifdef DEBUG
//   Serial.println();
//   Serial.println();
//   Serial.print("INFO: Connecting to ");
//   #endif

//   WiFi.mode(WIFI_STA);

//   #ifdef DEBUG
//   Serial.println(WIFI_SSID);
//   #endif 

//   WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
 
//   while (WiFi.status() != WL_CONNECTED) {
//     delay(500);
//     #ifdef DEBUG
//     Serial.print(".");
//     #endif 
//   }
  
//   #ifdef DEBUG
//   Serial.println("");
//   Serial.println("INFO: WiFi connected");
//   Serial.print("INFO: IP address: ");
//   Serial.println(WiFi.localIP());

//   Serial.print(". Signal strength: ");
//   Serial.print(2*(WiFi.RSSI()+100));
//   Serial.println("%");

//   Serial.println();
//   Serial.print("MAC: ");
//   Serial.println(WiFi.macAddress());    

      
//   Serial.print("Free Heap size: ");
//   Serial.print(ESP.getFreeHeap()/1024);
//   Serial.println("Kb");
//   #endif 

//   clientId.toUpperCase();

//   WiFi.hostname(clientId); //esp32
// }