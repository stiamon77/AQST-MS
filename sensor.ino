void startHA() {

  //set device's details (optional)
  device.setName(clientId.c_str());
  device.setSoftwareVersion("2.1.1");
  device.setManufacturer("AQST");
  device.setModel("AQST-MS v2");

  luxSensor.setIcon("mdi:brightness-7");
  luxSensor.setName("Illuminance");
  luxSensor.setUnitOfMeasurement("lx");

  signalSensor.setIcon("mdi:wifi");
  signalSensor.setName("Signal Strength");
  signalSensor.setUnitOfMeasurement("%");

  occupancySensor.setCurrentState(lastStateOccupancy);
  occupancySensor.setName("Occupancy sensor");
  occupancySensor.setDeviceClass("occupancy");
  occupancySensor.setIcon("mdi:home");

  motionSensor.setCurrentState(lastStateMotion);
  motionSensor.setName("Motion sensor");
  motionSensor.setDeviceClass("motion");
  motionSensor.setIcon("mdi:motion-sensor");
  
  device.enableSharedAvailability();  //This controls the availability of the entire device
  //  device.setAvailability(true);
  device.enableLastWill(); //Accessibility control by sensors, together with general control, do not work, for one thing (

  // optional properties
  buttonSave.setIcon("mdi:vector-point-edit");
  buttonSave.setName("Save settings");

  buttonDefault.setIcon("mdi:restart-off");
  buttonDefault.setName("Reset to default (settings only)");

  buttonRestart.setIcon("mdi:restart");
  buttonRestart.setName("Restart");

  switchledOn.setIcon("mdi:led-off");
  switchledOn.setName("Led On-Off");
  switchledOn.setRetain(true);

  // press callbacks
  buttonSave.onCommand(onButtonCommand);

  buttonDefault.onCommand(onButtonCommand);

  buttonRestart.onCommand(onButtonCommand);

  switchledOn.onCommand(onSwitchCommand);

  numTh1.onCommand(numTh1Com);
  numTh1.setIcon("mdi:cogs");
  numTh1.setName("Motion sensor (TH1)");
  numTh1.setMin(20); // can be float if precision is set via the constructor
  numTh1.setMax(1200); // can be float if precision is set via the constructor
  numTh1.setStep(10); // minimum step: 0.001f
  // numberTh1.setMode(HANumber::ModeBox);
  numTh1.setMode(HANumber::ModeSlider);
  numTh1.setRetain(true);

  numTh2.onCommand(numTh2Com);
  numTh2.setIcon("mdi:cogs");
  numTh2.setName("Occupancy sensor (TH2)");
  numTh2.setMin(50); // can be float if precision is set via the constructor
  numTh2.setMax(2500); // can be float if precision is set via the constructor
  numTh2.setStep(10); // minimum step: 0.001f
  // numberTh1.setMode(HANumber::ModeBox);
  numTh2.setMode(HANumber::ModeSlider);
  numTh2.setRetain(true);

  numledBright.onCommand(numledBrightCom);
  numledBright.setIcon("mdi:brightness-5");
  numledBright.setName("Led Bright");
  numledBright.setMin(10); // can be float if precision is set via the constructor
  numledBright.setMax(255); // can be float if precision is set via the constructor
  numledBright.setStep(5); // minimum step: 0.001f
  // numberTh1.setMode(HANumber::ModeBox);
  numledBright.setMode(HANumber::ModeSlider);
  numledBright.setRetain(true);

//Del  mqtt.begin(BROKER_ADDR, BROKER_USERNAME, BROKER_PASSWORD);
  mqtt.begin(mqtt_server, mqtt_user, mqtt_password);
}
