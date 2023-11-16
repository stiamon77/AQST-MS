# AQST-MS
Occupancy\Motion sensor HLK-LD1115H-24G ESP32 or ESP8266 Arduino with integration to Home Assistant.

Buy me a coffee https://paypal.me/StanislavTimofeev       )

Main file AQST-MS_v2.ino

See the documentation for the sensor in the folder [Documentation HLK-LD1115H-24G ](https://github.com/stiamon77/AQST-MS/tree/main/Documentation%20HLK-LD1115H-24G)

and in https://h.hlktech.com/Mobile/download/fdetail/183.html

**Home Assistant integration**

![Image alt](https://github.com/stiamon77/AQST-MS/blob/main/image/AQST-MS_HA_img1.png)
![Image alt](https://github.com/stiamon77/AQST-MS/blob/main/image/AQST-MS_HA_img2.png)


**To get started we will need:**

Main board:

[**ESP32С3 SuperMini**](https://s.click.aliexpress.com/e/_mMy9m2s) This is a very small board with a powerful chip and WiFi on board, it is ideal for our project and I recommend using it, the following boards are also suitable for us:

or

[**ESP32-S2**](https://s.click.aliexpress.com/e/_mNCYsni)

or

[**ESP8266 D1 MINI V4.0.0**](https://s.click.aliexpress.com/e/_m0Oh844) I recommend this version of the board with ESP8266, it is more stable than options with an iron screen, I don’t know why)

and

[**1x USB-C PD Trigger**](https://s.click.aliexpress.com/e/_oFomIyB) allows you to connect any charger for phones or gadgets to our device

[**1x Capacitor 6.3V 470UF**](https://s.click.aliexpress.com/e/_Ex5wAbL) to ensure stable power supply to the device we need one capacitor (6.3V 470UF)

[**1x BH1750**](https://s.click.aliexpress.com/e/_mNCYsni) this is a light brightness sensor

[**1x HLK-LD1155H-24G**](https://s.click.aliexpress.com/e/_mOwmUMk) Microwave Radar Sensor Detection Motion Module

[**1x LED WS2812B**](https://s.click.aliexpress.com/e/_EQSz20l) if you are making several devices, it is more profitable to buy 1 meter of LED strip consisting of WS2812B diodes, it will be cheaper. The tape must be cut one LED at a time and used as indicated in the diagrams below.  If there is one device, then there is a [[WS2812B module](https://s.click.aliexpress.com/e/_mtHwtJO)](https://s.click.aliexpress.com/e/_mtHwtJO) with this LED. Choose the smallest one.

[**ABS Plastic Case**](https://s.click.aliexpress.com/e/_mNRPQqo) And of course, I specially selected the right case to accommodate all the electronic stuffing. See below how to place everything in it correctly and where to make the holes. You can choose a suitable color, black or white.

[**UV Bonding & Welding Glue**](https://s.click.aliexpress.com/e/_mKt8EUU) This is not necessary, but to make beautiful holes for the light brightness sensor and LED, this glue will be useful. It is transparent after drying.

[**HSS Straight Groove Step Drill**](https://s.click.aliexpress.com/e/_mqpy4ZI) This is also not necessary, but beautiful holes can be made using a step drill.

[**Motion Sensor Holder**](https://s.click.aliexpress.com/e/_m0g4aag) To attach our motion and presence sensor to a wall or ceiling, we will need a rotating mount. I liked this one, but something like this will do [**360 Rotatable Magnetic Holder**](https://s.click.aliexpress.com/e/_omawH59) it has its advantages and you can choose the color ) .

**Connecting the sensor to the power supply**:

Since our device requires constant power from a USB charger, you need at least 2a (look at the power supply, your old phone charger with USB will do). And since the device will hang on the wall, it is more convenient to make the power cable yourself; for this we need a wire (you can buy it at an electrician’s store) and contacts [**USB**](https://s.click.aliexpress.com/e/_ok0QxCx) and [**USB-C**](https://s.click.aliexpress.com/e/_mtt0R5a). See the connection diagram below.



**Connection diagrams:**

**ESP32-C3**

![Image alt](https://github.com/stiamon77/AQST-MS/blob/main/image/ESP32-C3.jpg)

**ESP32-S2**

![Image alt](https://github.com/stiamon77/AQST-MS/blob/main/image/ESP32-S2.jpg)

**ESP8266 D1 Mini V3-V4**

![Image alt](https://github.com/stiamon77/AQST-MS/blob/main/image/ESP8266%20D1%20Mini.jpg)
