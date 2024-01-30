void startLED(){
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
}

void testLED(){
  //LED
  leds[0].setHSV(0, 255, 255);  // HSV, 0-255
  FastLED.show();
  // strip.setLedColorData(0, 0, 0, 255);
  // strip.show();
  leds[0].setHSV(32, 255, 255);  // HSV, 0-255
  FastLED.show();
  delay(500);
  // strip.setLedColorData(0, 0, 255, 0);
  // strip.show();
  leds[0].setHSV(64, 255, 255);  // HSV, 0-255
  FastLED.show();
  delay(500);   
  // strip.setLedColorData(0, 255, 0, 0);
  // strip.show();
  leds[0].setHSV(96, 255, 255);  // HSV, 0-255
  FastLED.show();
  delay(500); 
  leds[0].setHSV(128, 255, 255);  // HSV, 0-255
  FastLED.show();
  delay(500);
  leds[0].setHSV(160, 255, 255);  // HSV, 0-255
  FastLED.show();
  delay(500);
  leds[0].setHSV(192, 255, 255);  // HSV, 0-255
  FastLED.show();
  delay(500);
  leds[0].setHSV(224, 255, 255);  // HSV, 0-255
  FastLED.show();
  delay(500);
  // strip.setLedColorData(0, 0, 0, 0);
  // strip.show();
  FastLED.clear();
  FastLED.show();
}

void redLed(){
  if (ledOn){
    if(lastStateMotionN || lastStateOccupancyN){
      leds[0].setHSV(255, 255, ledBright);  // HSV, 0-255
      FastLED.show();
    }else{ 
      leds[0].setHSV(96, 255, ledBright);  // HSV, 0-255
      FastLED.show();
    }
  }else{
    FastLED.clear();
    FastLED.show();
  }
}