//https://github.com/stiamon77/AQST-MS

//Buy me a coffee Ihttps://paypal.me/StanislavTimofeev

#include <FS.h>                   
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager

#include <TimerMs.h>  //https://github.com/GyverLibs/TimerMs/tree/main

TimerMs tmr(3000, 1, 1);
TimerMs tmr1(500, 1, 1);

#if defined(ESP8266)

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>


#define msSerial Serial

#elif defined(ESP32)
#include <WiFi.h> 
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <SPIFFS.h>

#endif

#include <ArduinoJson.h>          //https://github.com/bblanchon/ArduinoJson

#include <ArduinoHA.h>  //https://github.com/dawidchyrzynski/arduino-home-assistant && https://github.com/knolleary/pubsubclient
/*
Icons for HA https://pictogrammers.com/library/mdi/
Device Class https://www.home-assistant.io/integrations/sensor
Binary Sensor https://www.home-assistant.io/integrations/binary_sensor/#device-class
https://dawidchyrzynski.github.io/arduino-home-assistant/index.html
*/

// LD1115H // https://h.hlktech.com/Mobile/download/fdetail/183.html

// select which pin will trigger the configuration portal when set to LOW
#define TRIGGER_PIN 0

//---LED---
#include <FastLED.h>

//indicate your LED connection pin
#if defined(ESP8266)
#define DATA_PIN    D3 //esp8266
#elif defined(ESP32)
#define DATA_PIN    10 //esp32
#endif

//#define CLK_PIN 4
#define LED_TYPE    WS2812B
#define COLOR_ORDER   GRB
#define NUM_LEDS    1
CRGB leds[NUM_LEDS];
#define BRIGHTNESS    255 //default led brightness

//---BH1750---
#include <Wire.h>
#include <BH1750.h>

uint16_t SensorLux; //Lux
BH1750 lightMeter;

#if defined(ESP32)
//---LD1115H---
#define RXp 16
#define TXp 17

// Timer callback functions
static void timer_callback1(void* arg1);
esp_timer_handle_t timer1;
volatile bool timerActive1 = false;

#endif

//---WiFi---
//Enter your WiFi network details, SSID and password
//del const char* WIFI_SSID = "1111";
//del const char* WIFI_PASSWORD = "2222";
String clientId;
//signal level
float SensorWiFi_RSSI = 0; 
WiFiClient espClient;
// ID client
uint32_t chipId;

//define your default values here, if there are different values in config.json, they are overwritten.
char mqtt_server[40];
char mqtt_port[10] = "1883";
char mqtt_user[34] = "mqtt";
char mqtt_password[34] = "mqtt";

//flag for saving data
bool shouldSaveConfig = false;

WiFiManager wifiManager;

//---MQTT---
//Enter your Home Assistant information
//del #define BROKER_ADDR     IPAddress(192,168,0,10)
//del #define BROKER_USERNAME     "mqtt" // replace with your credentials
//del #define BROKER_PASSWORD     "mqtt"

HADevice device;
HAMqtt mqtt(espClient, device);

//---Sensors---
bool lastStateOccupancy = false;
bool lastStateMotion = false;

bool lastStateMotionN = false;
bool lastStateOccupancyN = false;

bool th1Is = false;
bool th2Is = false;
bool movSnIs = false;
bool occSnIs = false;

bool butSave = false;
bool butDefault = false;

bool dataStart = true;

bool ledOn;
int ledBright;

int16_t numTh1Sens;
int16_t numTh2Sens;

String dataRadar;


//---Sensors HA---
HASensorNumber signalSensor("signal_strength1_1", HASensorNumber::PrecisionP0);

HASensorNumber luxSensor("illuminance_11", HASensorNumber::PrecisionP0);

HABinarySensor occupancySensor("occupancy_11");
HABinarySensor motionSensor("motion_11");

HAButton buttonSave("Save_11");
HAButton buttonDefault("Default_11");
HAButton buttonRestart("Restart_11");
HASwitch switchledOn("ledOn_11");

HANumber numTh1("Th1_11", HANumber::PrecisionP0);
HANumber numTh2("Th2_11", HANumber::PrecisionP0);
HANumber numledBright("LedBright_11", HANumber::PrecisionP0);

//----------------------------------------------------------
//#define DEBUG //DEBUG mod 
//----------------------------------------------------------

void saveConfigCallback () {
  #ifdef DEBUG
  Serial.println("Should save config");
  #endif

  shouldSaveConfig = true;
}

void setup() {

  #if defined(ESP8266)

  msSerial.begin(115200);

  tmr.setPeriodMode();
  tmr1.setPeriodMode();

  #elif defined(ESP32)

  timersStart();

  #endif

  startLED(); //start LED


  startWIFI(); //start WIFI

  testLED();  //test led, onli start

  startHA(); //Start HA MQTT

  updateFW(); //Over the air update

  //----------Start Sensors----------

  #if defined(ESP32)

  #ifdef DEBUG
  Serial.begin(115200);
  #endif

  Serial2.begin(115200, SERIAL_8N1, RXp, TXp);
  
  #endif
  //start BH1750
  Wire.begin();
  lightMeter.begin(); 

  delay(1000);

  redLed();

}

void loop() {

  checkButton();

  #if defined(ESP8266)

  MDNS.update();

  if (tmr.tick()){
    //---------Data Sensors---------
    dataBH1750();      

    //Checking the connection WiFi
    if (WiFi.status() != WL_CONNECTED) {

      #ifdef DEBUG
      Serial.print(millis());
      Serial.println("Reconnecting to WiFi...");
      #endif

      WiFi.disconnect();
      WiFi.reconnect();

    }      

    //---------Data Telemetry---------
    float SensorWiFi_RSSIS; //signal level
    String SensorUptimeS; //Uptime

    SensorWiFi_RSSIS = 2*(WiFi.RSSI()+100);

    if(SensorWiFi_RSSI != SensorWiFi_RSSIS){
      SensorWiFi_RSSI = SensorWiFi_RSSIS;  
      signalSensor.setValue(SensorWiFi_RSSI);
    }
  }

  #elif defined(ESP32)

  timersData();
  
  #endif
  
  mqtt.loop();
  
  dataLD1115H();

  ArduinoOTA.handle();

}

#if defined(ESP32)

//call back for timer
static void timer_callback1(void* arg){

  //---------Data Sensors---------
  dataBH1750();      

  //Checking the connection WiFi
  if (WiFi.status() != WL_CONNECTED) {

    #ifdef DEBUG
    Serial.print(millis());
    Serial.println("Reconnecting to WiFi...");
    #endif

    WiFi.disconnect();
    WiFi.reconnect();

  }      

  //---------Data Telemetry---------
  float SensorWiFi_RSSIS; //signal level
  String SensorUptimeS; //Uptime

  SensorWiFi_RSSIS = 2*(WiFi.RSSI()+100);

  if(SensorWiFi_RSSI != SensorWiFi_RSSIS){
    SensorWiFi_RSSI = SensorWiFi_RSSIS;  
    signalSensor.setValue(SensorWiFi_RSSI);
  }

  timerActive1 = true;
}
#endif
