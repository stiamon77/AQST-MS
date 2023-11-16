# AQST-MS
Occupancy\Motion sensor HLK-LD1115H-24G ESP32 or ESP8266 Arduino with integration to Home Assistant.

Buy me a coffee https://paypal.me/StanislavTimofeev       )

**Main file AQST-MS_v2.ino**

See the documentation for the sensor in the folder [Documentation HLK-LD1115H-24G ](https://github.com/stiamon77/AQST-MS/tree/main/Documentation%20HLK-LD1115H-24G)

and in https://h.hlktech.com/Mobile/download/fdetail/183.html

## **Home Assistant integration**

![Image alt](https://github.com/stiamon77/AQST-MS/blob/main/image/AQST-MS_HA_img1.png)
![Image alt](https://github.com/stiamon77/AQST-MS/blob/main/image/AQST-MS_HA_img2.png)


# **To get started we will need:**

**Main board:**

[**ESP32С3 SuperMini**](https://s.click.aliexpress.com/e/_mMy9m2s) This is a very small board with a powerful chip and WiFi on board, it is ideal for our project and I recommend using it, the following boards are also suitable for us:

or

[**ESP32-S2**](https://s.click.aliexpress.com/e/_mNCYsni)

or

[**ESP8266 D1 MINI V4.0.0**](https://s.click.aliexpress.com/e/_m0Oh844) I recommend this version of the board with ESP8266, it is more stable than options with an iron screen, I don’t know why)

**and**

[**1x USB-C PD Trigger**](https://s.click.aliexpress.com/e/_oFomIyB) allows you to connect any charger for phones or gadgets to our device

[**1x Capacitor 6.3V 470UF**](https://s.click.aliexpress.com/e/_Ex5wAbL) to ensure stable power supply to the device we need one capacitor (6.3V 470UF)

[**1x BH1750**](https://s.click.aliexpress.com/e/_mNCYsni) this is a light brightness sensor

[**1x HLK-LD1155H-24G**](https://s.click.aliexpress.com/e/_mOwmUMk) Microwave Radar Sensor Detection Motion Module

[**1x LED WS2812B**](https://s.click.aliexpress.com/e/_EQSz20l) if you are making several devices, it is more profitable to buy 1 meter of LED strip consisting of WS2812B diodes, it will be cheaper. The tape must be cut one LED at a time and used as indicated in the diagrams below.  If there is one device, then there is a [WS2812B module](https://s.click.aliexpress.com/e/_mtHwtJO) with this LED. Choose the smallest one.

[**ABS Plastic Case**](https://s.click.aliexpress.com/e/_mNRPQqo) And of course, I specially selected the right case to accommodate all the electronic stuffing. See below how to place everything in it correctly and where to make the holes. You can choose a suitable color, black or white.

**!When installing the sensor into the housing, remember that the distance from the antenna on the front side of the sensor to the housing wall must be at least 5-6mm.!**

[**UV Bonding & Welding Glue**](https://s.click.aliexpress.com/e/_mKt8EUU) This is not necessary, but to make beautiful holes for the light brightness sensor and LED, this glue will be useful. It is transparent after drying.

[**HSS Straight Groove Step Drill**](https://s.click.aliexpress.com/e/_mqpy4ZI) This is also not necessary, but beautiful holes can be made using a step drill.

[**Motion Sensor Holder**](https://s.click.aliexpress.com/e/_m0g4aag) To attach our motion and presence sensor to a wall or ceiling, we will need a rotating mount. I liked this one, but something like this will do [**360 Rotatable Magnetic Holder**](https://s.click.aliexpress.com/e/_omawH59) it has its advantages and you can choose the color ) .

## **Connecting the sensor to the power supply**:

Since our device requires constant power from a USB charger, you need at least 2a (look at the power supply, your old phone charger with USB will do). And since the device will hang on the wall, it is more convenient to make the power cable yourself; for this we need a wire (you can buy it at an electrician’s store) and contacts [**USB**](https://s.click.aliexpress.com/e/_ok0QxCx) and [**USB-C**](https://s.click.aliexpress.com/e/_mtt0R5a). See the connection diagram below.

***
   

# **Connection to Home Assistant and setup instructions:**

## **Connection:**

In the code you only need to change a few fields

`const char* WIFI_SSID = "1111";` 

Instead of **1111**, enter the name of your WiFi network, please note that ESP32 and ESP8266 operate on a 2.4 hertz network, a 5 hertz network is not suitable for them. This is usually not a problem and WiFi routers support two bands at the same time. Make sure you are running at 2.4 hertz

`const char* WIFI_PASSWORD = "2222";`

Instead of **2222**, enter the password of your WiFi network

`#define BROKER_ADDR     IPAddress(**192,168,0,10**)`

Instead of **192,168,0,10** , enter the IP address of your Home Assistant

`#define BROKER_USERNAME     "mqtt"`

`#define BROKER_PASSWORD     "mqtt"`

If you changed mqtt settings, login and password, change this data in the code. If not, then this is the default login and password; you do not need to change them.

If you changed the Led connection port, then change it in the code

For ESP8266

`#if defined(ESP8266)`

`#define DATA_PIN    0 //esp8266`

And for ESP32

`#if defined(ESP32)`

`#define DATA_PIN    10 //esp32`

Once enabled, the sensor will automatically connect to HA and appear in MQTT devices

## **Setting:**

Our sensor is equipped with an LED to indicate detection, if it detects the presence or movement of a person, the LED lights up red, otherwise it turns green.
With the **“Led On-Off”** setting you can turn the indication LED on or off.

**"Led Bright"** allows you to adjust its brightness.

**“Reset to default (settings only)”** allows you to reset the settings of the LD1115H sensor to factory settings, after clicking this button, click the **“Save settings”** button.

**“Save settings”** After setting the sensor sensitivity, press this button to save the data into the sensor memory.

**“Motion sensor (TH1)”** is a motion detection sensitivity setting. The factory setting is **120**, this is the maximum sensitivity (about 4-6 meters), if you increase it the sensitivity will decrease, this is necessary if your room is small and you need to avoid detecting movement behind the wall.

Changing data below 110 is not recommended; there will be many false positives.

**“Occupancy sensor (TH2)”** is the sensitivity of presence detection. When a person is in a room and does not move. Works normally within a radius of 2-3 meters. The factory value of 250 increases the sensitivity and detection range, but may also increase the possibility of false alarms.

You need to configure the parameters to suit your conditions. Find a balance between sensitivity and eliminating false positives.

***
 
 
### **You can ask your questions or suggestions [here](https://github.com/stiamon77/AQST-MS/discussions)**

***

## **Recommendations for placing sensors in a room:**

The sensors must be located at a distance of more than about 0.5 meters from each other.

The sensors should not face each other. Their rays should not intersect.

Do not place sensors near outlets or other electrical appliances.

Do not point the sensors at a fan or air conditioner; shaking surfaces may interfere with the sensor's operation.

It is optimal to place the sensor on the wall, at a distance of 80-100cm from the floor. Or on the ceiling.

If a person sits quietly with their back to the sensor, the sensitivity effect will be reduced. Because when the back is turned to the radar, the chest or abdomen subject to vibrations are not visible to the sensor.




# **Connection diagrams:**

### **ESP32-C3**

![Image alt](https://github.com/stiamon77/AQST-MS/blob/main/image/ESP32-C3.jpg)

### **ESP32-S2**

![Image alt](https://github.com/stiamon77/AQST-MS/blob/main/image/ESP32-S2.jpg)

### **ESP8266 D1 Mini V3-V4**

![Image alt](https://github.com/stiamon77/AQST-MS/blob/main/image/ESP8266%20D1%20Mini.jpg)
