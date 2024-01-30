void onButtonCommand(HAButton* sender){
    if (sender == &buttonSave){
      // button was clicked
      #ifdef DEBUG
      Serial.println("----------------Save----------------");
      #endif
      butSave = true;
      saveSetLD1115H();

    } else if (sender == &buttonRestart) {
      // button was clicked
      #ifdef DEBUG
      Serial.println("----------------Restart----------------");
      #endif
      ESP.restart();
    // } else if (sender == &buttonBuzzer) {
    //   // button was clicked
    //   #ifdef DEBUG
    //   Serial.println("----------------Buzzer----------------");
    //   #endif
    // } else if (sender == &buttonVibration) {
    //   // button was clicked
    //   #ifdef DEBUG
    //   Serial.println("----------------Vibration----------------");
    //   #endif

    } else if (sender == &buttonDefault) {
      // button was clicked
      #ifdef DEBUG
      Serial.println("----------------Default----------------");
      #endif  

      butDefault = true; 
      defSetLD1115H();     
    }
}

void numTh1Com(HANumeric numTh1, HANumber* numTh1Sender){
    if (!numTh1.isSet()) {
        // command was send by Home Assistant
    } else {
        numTh1Sens = numTh1.toInt16();
    }
    numTh1Sender->setState(numTh1Sens); // report the selected option back to the HA panel
}

void numTh2Com(HANumeric numTh2, HANumber* numTh2sender){
    if (!numTh2.isSet()) {
        // command was send by Home Assistant
    } else {
        numTh2Sens = numTh2.toInt16();
    }
    numTh2sender->setState(numTh2Sens); // report the selected option back to the HA panel
}

void numledBrightCom(HANumeric numledBright, HANumber* numledBrightsender){
    if (!numledBright.isSet()) {
        // command was send by Home Assistant
    } else {
        ledBright = numledBright.toInt16();
        redLed();
    }
    numledBrightsender->setState(ledBright); // report the selected option back to the HA panel
    
}

void onSwitchCommand(bool ledOnstate, HASwitch* ledOnsender){
    if (ledOnstate){
      ledOn = true;
      redLed();
    }else{
      ledOn = false;
      redLed();
    }
  ledOnsender->setState(ledOnstate); // report state back to the Home Assistant
}