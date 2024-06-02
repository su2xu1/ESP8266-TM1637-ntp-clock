## About this project  
This is a LED clock project using Arduino-compatible ESP8266 and LED driver TM1637.  

I've been using this clock for several years, and it continues to run. I'm satisfied with it. Despite its cheap construction, it synchronizes the time with NTP, so the time never drifts.

## Softawares  
On the software side,   
I used the Arduino IDE to implement WiFi management, NTP client, and LED driver functions on the ESP8266. The ESP8266 connects to WiFi, synchronizes time with an NTP server on the internet, and uses that time data to drive a 4-digit 7-segment blue LED (OSL40391) via the TM1637 to display the time.  

I got the idea for this NTP clock from [Steave Fi](https://steve.fi/hardware/d1-ntp-clock/). From him, I found the [source code published on GitHub by BaiGeorgi](https://github.com/BaiGeorgi/ESP8266-TM1637-ntp-clock.git) and reused the code.  

For the ESP8266, I used the [WiFiManager](https://www.arduino.cc/reference/en/libraries/wifimanager/) which is included in the official Arduino library, specifically [this one](https://www.ardu-badge.com/WiFiManager). It's excellent. Before using this, I had to hardcode the SSID and password for WiFi connection into the source code, so I couldn't publish reusable source code. However, this Manager can create a captive portal, allowing dynamic SSID and password settings via a smartphone, eliminating the need for hardcoding in the source.  

The [NTPClient_Generic library](https://www.arduino.cc/reference/en/libraries/ntpclient_generic/) is also great. It's lightweight and easy to use.


## Hardware  
The TM1637 is an interesting LED driver. You can find [samples that work on Arduino](https://github.com/avishorp/TM1637) on the internet, making it easy to handle.
You can also find it at [Akizuki Denshi](https://akizukidenshi.com/catalog/g/g113224/)! I purchased mine from [Aitendo](https://www.aitendo.com/product/12990).
  
I was particular about the color of the LEDs. I excluded red LEDs because they felt too retro. Blue! Blue LEDs are the color for which Nakamura won the Nobel Prize. It's the color of the 21st century!  
 I remembered using a clock with [Nixie tubes](https://ja.wikipedia.org/wiki/%E3%83%8B%E3%82%AD%E3%82%B7%E3%83%BC%E7%AE%A1) that glowed blue in the past, so I chose blue LEDs. For the blue LEDs, I chose the OSL40391 (anode common), available at [Akizuki Denshi](https://akizukidenshi.com/catalog/g/g109964/).


## Next step  
In reality, I created very little of this program from scratch. I gathered publicly available resources from the internet and combined them with collective wisdom. The power of the internet is truly remarkable in this regard. Oh, I hadn't yet published this project myself. I just [published it on GitHub](https://github.com/su2xu1/ESP8266-TM1637-ntp-clock).

The next step is to design a stylish case and a safe and reliable USB power supply. This part is quite challenging. 

This is probably the difference between a hobby and a business.




