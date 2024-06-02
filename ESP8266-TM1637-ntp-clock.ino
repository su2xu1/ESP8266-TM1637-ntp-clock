//
//   Based on https://steve.fi/Hardware/
//   https://github.com/BaiGeorgi/ESP8266-TM1637-ntp-clock.git
//   Build the code for 160MHz CPU frequency!
//

#include <ESP8266WiFi.h>  // WiFi & OTA updates
#include <ArduinoOTA.h>
#include "NTPClient.h"            // For dealing with NTP & the clock.
#include <TM1637Display.h>        // The display-interface
#include <WiFiManager.h>          // WiFi setup.
#include "debug.h"                // Debug messages over the serial console.
#define PROJECT_NAME "NTP-CLOCK"  //   OTA name.
#define TIME_ZONE +7              // The timezone - comment out to stay at GMT.
WiFiUDP ntpUDP;                   // NTP client, and UDP socket it uses.
NTPClient timeClient(ntpUDP);
#define CLK 12  // TM1637 CLK pin
#define DIO 13  // TM1637 DIO pin
#define JMP 15  // Jumper pin for DST
int jState = 0;

//#define ADVANCE_MIN 15 //15 min in advance
#define ADVANCE_MIN 0  //0 min in advance

// The amount of time (in milliseconds) between tests
#define TEST_DELAY 2000


TM1637Display display(CLK, DIO);

void on_before_ntp()  // Called just before the date/time is updated via NTP
{
  DEBUG_LOG("Updating date & time");
}

//
// Called just after the date/time is updated via NTP
//
void on_after_ntp() {
  DEBUG_LOG("Updated NTP client\n");
}

//
// This function is called when the device is powered-on.
//
void setup() {
  pinMode(JMP, INPUT_PULLUP);
  int jumperValue = digitalRead(JMP);
  /*  if (jumperValue == LOW){
       jState = 2; // If button pushed, turn LED on
   } else {
      jState = 3; // Otherwise, turn the LED off
   }
   */
  jState = 2;

  Serial.begin(115200);  // Enable our serial port.

  Serial.println("Start! tm1637.init()");
  //tm1637.init(); // initialize the display.




  const uint8_t SEG_DONE[] = {
    SEG_B | SEG_C | SEG_D | SEG_E | SEG_G,          // d
    SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,  // O
    SEG_C | SEG_E | SEG_G,                          // n
    SEG_A | SEG_D | SEG_E | SEG_F | SEG_G           // E
  };



  int k;
  uint8_t data[] = { 0xff, 0xff, 0xff, 0xff };
  uint8_t blank[] = { 0x00, 0x00, 0x00, 0x00 };
  display.setBrightness(0x0f);
  // All segments on
  display.setSegments(data);
  delay(TEST_DELAY);

  // Selectively set different digits
  data[0] = display.encodeDigit(0);
  data[1] = display.encodeDigit(1);
  data[2] = display.encodeDigit(2);
  data[3] = display.encodeDigit(3);
  display.setSegments(data);
  delay(TEST_DELAY);

  /*
  for(k = 3; k >= 0; k--) {
  display.setSegments(data, 1, k);
  delay(TEST_DELAY);
  }
  */

  display.clear();
  display.setSegments(data + 2, 2, 2);
  delay(TEST_DELAY);

  display.clear();
  display.setSegments(data + 2, 2, 1);
  delay(TEST_DELAY);

  display.clear();
  display.setSegments(data + 1, 3, 1);
  delay(TEST_DELAY);


  // Show decimal numbers with/without leading zeros
  display.showNumberDec(0, false);  // Expect: ___0
  delay(TEST_DELAY);
  display.showNumberDec(0, true);  // Expect: 0000
  delay(TEST_DELAY);
  display.showNumberDec(1, false);  // Expect: ___1
  delay(TEST_DELAY);
  display.showNumberDec(1, true);  // Expect: 0001
  delay(TEST_DELAY);
  display.showNumberDec(301, false);  // Expect: _301
  delay(TEST_DELAY);
  display.showNumberDec(301, true);  // Expect: 0301
  delay(TEST_DELAY);
  display.clear();
  display.showNumberDec(14, false, 2, 1);  // Expect: _14_
  delay(TEST_DELAY);
  display.clear();
  display.showNumberDec(4, true, 2, 2);  // Expect: 04__
  delay(TEST_DELAY);
  display.showNumberDec(-1, false);  // Expect: __-1
  delay(TEST_DELAY);
  display.showNumberDec(-12);  // Expect: _-12
  delay(TEST_DELAY);
  display.showNumberDec(-999);  // Expect: -999
  delay(TEST_DELAY);
  display.clear();
  display.showNumberDec(-5, false, 3, 0);  // Expect: _-5_
  delay(TEST_DELAY);
  display.showNumberHexEx(0xf1af);  // Expect: f1Af
  delay(TEST_DELAY);
  display.showNumberHexEx(0x2c);  // Expect: __2C
  delay(TEST_DELAY);
  display.showNumberHexEx(0xd1, 0, true);  // Expect: 00d1
  delay(TEST_DELAY);
  display.clear();
  display.showNumberHexEx(0xd1, 0, true, 2);  // Expect: d1__
  delay(TEST_DELAY);

  // Run through all the dots
  for (k = 0; k <= 4; k++) {
    display.showNumberDecEx(0, (0x80 >> k), true);
    delay(TEST_DELAY);
  }

  // Brightness Test
  for (k = 0; k < 4; k++)
    data[k] = 0xff;
  for (k = 0; k < 7; k++) {
    display.setBrightness(k);
    display.setSegments(data);
    delay(TEST_DELAY);
  }

  // On/Off test
  for (k = 0; k < 4; k++) {
    display.setBrightness(7, false);  // Turn off
    display.setSegments(data);
    delay(TEST_DELAY);
    display.setBrightness(7, true);  // Turn on
    display.setSegments(data);
    delay(TEST_DELAY);
  }


  // Done!
  display.setSegments(SEG_DONE);
  display.setBrightness(0);

  //FINE


  //tm1637.point(true); // Show ":" between the digits
  //tm1637.set(BRIGHT_3); //Set the intensity from 0 to 7

  WiFiManager wifiManager;  // Handle WiFi setup
  wifiManager.autoConnect(PROJECT_NAME);
  timeClient.begin();                          // Ensure our NTP-client is ready.
  //timeClient.on_before_update(on_before_ntp);  // Configure the callbacks.
  //timeClient.on_after_update(on_after_ntp);
  //timeClient.setTimeOffset(TIME_ZONE + jState * (60 * 60)); // Setup the timezone & update-interval.
  timeClient.setTimeOffset(9 * (60 * 60));  // Setup the timezone & update-interval.
  timeClient.setUpdateInterval(300 * 1000);


  //
  // The final step is to allow over the air updates
  //
  // This is documented here:
  //     https://randomnerdtutorials.com/esp8266-ota-updates-with-arduino-ide-over-the-air/
  //
  // Hostname defaults to esp8266-[ChipID]
  //

  ArduinoOTA.setHostname(PROJECT_NAME);

  ArduinoOTA.onStart([]() {
    DEBUG_LOG("OTA Start\n");
  });
  ArduinoOTA.onEnd([]() {
    DEBUG_LOG("OTA End\n");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    char buf[32];
    memset(buf, '\0', sizeof(buf));
    snprintf(buf, sizeof(buf) - 1, "Upgrade - %02u%%\n", (progress / (total / 100)));
    DEBUG_LOG(buf);
  });
  ArduinoOTA.onError([](ota_error_t error) {
    DEBUG_LOG("Error - ");

    if (error == OTA_AUTH_ERROR)
      DEBUG_LOG("Auth Failed\n");
    else if (error == OTA_BEGIN_ERROR)
      DEBUG_LOG("Begin Failed\n");
    else if (error == OTA_CONNECT_ERROR)
      DEBUG_LOG("Connect Failed\n");
    else if (error == OTA_RECEIVE_ERROR)
      DEBUG_LOG("Receive Failed\n");
    else if (error == OTA_END_ERROR)
      DEBUG_LOG("End Failed\n");
  });

  //
  // Ensure the OTA process is running & listening.
  //
  ArduinoOTA.begin();
}



//
// This function is called continously, and is responsible
// for flashing the ":", and otherwise updating the display.
//
// We rely on the background NTP-updates to actually make sure
// that that works.
//



void loop() {
  static char buf[10] = { '\0' };
  static char prev[10] = { '\0' };
  static long last_read = 0;
  static bool flash = true;

  //
  // Resync the clock?
  //
  timeClient.update();
      //Serial.println("timeClient.update() has done!");

  //
  // Handle any pending over the air updates.
  //
  ArduinoOTA.handle();

  //
  // Get the current hour/min
  //
  int cur_hour = timeClient.getHours();
  int cur_min = timeClient.getMinutes();

  int HH = cur_hour;
  int MM = cur_min;
  int MM2 = MM + ADVANCE_MIN;
  //
  //Adjust hour
  //
  if (MM2 >= 60) {
    MM2 -= 60;
    HH += 1;
  }
  if (HH >= 24) {
    HH -= 24;
  }


  //
  // Format them in a useful way.
  //
  //12 Hour mode!!!
  //if (HH > 12) {
  //  HH -= 12;
  //}
  // Handle hour 0 (midnight) being shown as 12.
  //else if (HH == 0) {
  //  HH += 12;
  //}
  //12 Hour mode!!!
  sprintf(buf, "%02d%02d", HH, MM2);
  //
  // If the current "hourmin" is different to
  // that we displayed last loop ..
  //
  if (strcmp(buf, prev) != 0) {
    // Update the display
    //display.display(0, buf[0] - '0');
    //display.display(1, buf[1] - '0');
    //display.display(2, buf[2] - '0');
    //display.display(3, buf[3] - '0');
    // And cache it
    strcpy(prev, buf);
    int HHMM = HH * 100 + MM2;
    display.showNumberDec(HHMM, true);  // Expect: HHMM
    Serial.println(HHMM);
  }


  //
  // The preceeding piece of code would
  // have ensured the display only updated
  // when the hour/min changed.
  //
  // However note that we nuke the cached
  // value every half-second - solely so we can
  // blink the ":".
  //
  //  Sigh

  long now = millis();

  if ((last_read == 0) || (abs(now - last_read) > 500)) {
    // Invert the "show :" flag
    flash = !flash;

    // Apply it.
    //display.point(flash);

    //
    // Note that the ":" won't redraw unless/until you update.
    // So we'll force that to happen by removing the cached
    // value here.
    //
    memset(prev, '\0', sizeof(prev));
    last_read = now;
  }
}
