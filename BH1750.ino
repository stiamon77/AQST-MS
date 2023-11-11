void dataBH1750(){
  //receiving data from the sensor BH1750
  int SensorLuxS;
  SensorLuxS = lightMeter.readLightLevel();

  if(SensorLux != SensorLuxS){
    SensorLux = SensorLuxS;  
    luxSensor.setValue(SensorLux);
  }

  #ifdef DEBUG
  Serial.println(String(SensorLux) + " lx");
  #endif

  //luxSensor.setValue(lux);
  //start of counting interval
  //device.setAvailability(!device.isAvailable());

  if (lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {

    #ifdef DEBUG
    Serial.println(F("BH1750 Advanced begin"));
    #endif

    luxSensor.setAvailability(true); //online
  } else {

    #ifdef DEBUG
    Serial.println(F("Error initialising BH1750"));
    #endif

    luxSensor.setAvailability(false); //offline
  }
}