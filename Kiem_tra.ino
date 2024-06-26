#include <Adafruit_Fingerprint.h>
#include "AdafruitIO.h"
#include <config.h>
#include "AdafruitIO_WiFi.h"
#include <HardwareSerial.h>
#include <Adafruit_GFX.h>           //https://github.com/adafruit/Adafruit-GFX-Library
#include <Adafruit_SSD1306.h>       //https://github.com/adafruit/Adafruit_SSD1306
#include <SPI.h>
#include <Wire.h>
#include <map>
#include <iostream>
#include <string>
#include <Arduino.h>
#if (defined(__AVR__) || defined(ESP8266)) && !defined(__AVR_ATmega2560__)
// For UNO and others without hardware serial, we must use software serial...
// pin #2 is IN from sensor (GREEN wire)
// pin #3 is OUT from arduino  (WHITE wire)
// Set up the serial port to use softwareserial..
// SoftwareSerial mySerial(2, 3);
#else
// On Leonardo/M0/etc, others with hardware serial, use hardware serial!
// #0 is green wire, #1 is white
// #define mySerial Serial1
#endif

// Adafruit_Fingerprint finger = Adafruit_Fingerprint(&Serial2);
// SimpleTimer timer;
// WiFi parameters
#define WIFI_SSID       "wifi"
#define WIFI_PASS       "12345678"

// Adafruit IO parameters
#define IO_USERNAME  "tuan1707"
#define IO_KEY       "aio_MngQ41lyxFnjUVJfVHMqm83oWPx3"
// Define pins for LED or other status indicator
// #define LED_PIN         13  // Example: use onboard LED

// Initialize Adafruit IO
AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);

// Define Adafruit IO Feeds for status
AdafruitIO_Feed *statusFeed = io.feed("test");
AdafruitIO_Feed *idFeed = io.feed("test3");
AdafruitIO_Feed *optionFeed = io.feed("test4");
AdafruitIO_Feed *choiceFeed = io.feed("test5");
AdafruitIO_Feed *ListFeed = io.feed("test6");
AdafruitIO_Feed *Button = io.feed("test2");


// SimpleTimer timer;
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
// WidgetTerminal terminal(V0);
// Icon of Fingerprint
#define LOGO_HEIGHT   64
#define LOGO_WIDTH    128
static const unsigned char PROGMEM logo_bmp[] =
{
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0x83, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xf0, 0x00, 0x0f, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x80, 0x00, 0x03, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x7e, 0x03, 0xff, 0xc0, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x3f, 0xff, 0xf8, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x01, 0xf0, 0xff, 0xff, 0xfe, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x01, 0xe1, 0xff, 0x00, 0xff, 0x87, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x01, 0xc7, 0xf0, 0x00, 0x1f, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x01, 0x8f, 0xc0, 0x00, 0x03, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x03, 0xff, 0xc1, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x3e, 0x0f, 0xff, 0xf0, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0x3f, 0xff, 0xfc, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0xff, 0x00, 0xff, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0xf1, 0xf8, 0x00, 0x1f, 0x8f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x01, 0xe3, 0xf0, 0x18, 0x0f, 0xc7, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x03, 0xc7, 0xc1, 0xff, 0x83, 0xe3, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x03, 0xc7, 0x87, 0xff, 0xe1, 0xf1, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x07, 0x8f, 0x1f, 0xff, 0xf0, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x07, 0x1e, 0x3f, 0x01, 0xf8, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x07, 0x1e, 0x7e, 0x00, 0x7c, 0x79, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x78, 0x00, 0x3e, 0x3d, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0xf0, 0xff, 0x1f, 0x3d, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x79, 0xe1, 0xff, 0x8f, 0x1c, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0xf1, 0xe3, 0xff, 0xc7, 0x1e, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x01, 0xe3, 0xc7, 0xc3, 0xc7, 0x9e, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x07, 0xe3, 0x8f, 0x01, 0xe7, 0x9e, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x0f, 0xc7, 0x8f, 0x00, 0xe3, 0x9e, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x0f, 0x8f, 0x9e, 0x18, 0xf3, 0x9e, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x0e, 0x0f, 0x1e, 0x38, 0xf3, 0x9e, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x3c, 0x38, 0xf3, 0x9e, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x3c, 0x78, 0xf7, 0x9c, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0x78, 0x78, 0xe7, 0x9c, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x01, 0xf0, 0xf0, 0xf1, 0xe7, 0x9c, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x03, 0xe1, 0xf1, 0xe1, 0xe7, 0x3c, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x0f, 0xc3, 0xe3, 0xe3, 0xc7, 0x3c, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x0f, 0x87, 0xc3, 0xc3, 0xcf, 0x3c, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x0f, 0x0f, 0x87, 0x87, 0x8f, 0x3c, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x0f, 0x0f, 0x8f, 0x38, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x7e, 0x1f, 0x1f, 0x0f, 0x3c, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x3e, 0x1e, 0x0f, 0x3d, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x01, 0xf0, 0x7c, 0x3c, 0xcf, 0x3c, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x07, 0xe0, 0xf8, 0x79, 0xcf, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x0f, 0xc1, 0xf0, 0xf9, 0xcf, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x0f, 0x07, 0xe1, 0xf1, 0xc7, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x0e, 0x0f, 0xc3, 0xe3, 0xc7, 0x87, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x07, 0xc3, 0xc7, 0x87, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x3e, 0x1f, 0x83, 0xc3, 0xc3, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x3e, 0x01, 0xc3, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x01, 0xf8, 0x7c, 0x01, 0xc1, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x01, 0xf0, 0xf8, 0x01, 0xe0, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x01, 0xc3, 0xf3, 0xf8, 0xf0, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xe7, 0xfc, 0xf8, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xcf, 0xfe, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x1f, 0x1e, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x3e, 0x0f, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x0f, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
//---------------------------------------------------------------
#define Fingerprint_valid_width 64
#define Fingerprint_valid_height 64
static const unsigned char PROGMEM Fingerprint_valid_bts[] = {
  0x00, 0x00, 0x03, 0xfe, 0x00, 0x00, 0x00, 0x00
  , 0x00, 0x00, 0x1f, 0xff, 0xe0, 0x00, 0x00, 0x00
  , 0x00, 0x00, 0x7f, 0xff, 0xf8, 0x00, 0x00, 0x00
  , 0x00, 0x00, 0xfc, 0x00, 0xfe, 0x00, 0x00, 0x00
  , 0x00, 0x03, 0xe0, 0x00, 0x1f, 0x00, 0x00, 0x00
  , 0x00, 0x07, 0xc0, 0x00, 0x07, 0x80, 0x00, 0x00
  , 0x00, 0x0f, 0x80, 0x00, 0x03, 0xe0, 0x00, 0x00
  , 0x00, 0x0e, 0x03, 0xff, 0x01, 0xe0, 0x00, 0x00
  , 0x00, 0x1c, 0x1f, 0xff, 0xe0, 0xf0, 0x00, 0x00
  , 0x00, 0x3c, 0x3f, 0xff, 0xf0, 0x78, 0x00, 0x00
  , 0x00, 0x78, 0x7c, 0x00, 0xf8, 0x3c, 0x00, 0x00
  , 0x00, 0x70, 0xf0, 0x00, 0x3c, 0x1c, 0x00, 0x00
  , 0x00, 0xe1, 0xe0, 0x00, 0x1e, 0x1c, 0x00, 0x00
  , 0x00, 0xe1, 0xc0, 0x00, 0x0f, 0x0e, 0x00, 0x00
  , 0x00, 0xc3, 0x81, 0xfc, 0x07, 0x0e, 0x00, 0x00
  , 0x00, 0x03, 0x83, 0xff, 0x07, 0x8e, 0x00, 0x00
  , 0x00, 0x07, 0x07, 0x8f, 0x83, 0x87, 0x00, 0x00
  , 0x00, 0x0f, 0x0f, 0x03, 0xc3, 0x87, 0x00, 0x00
  , 0x00, 0x1e, 0x0e, 0x01, 0xc3, 0x87, 0x00, 0x00
  , 0x00, 0x3c, 0x1c, 0x00, 0xe1, 0x87, 0x00, 0x00
  , 0x00, 0xf8, 0x1c, 0x30, 0xe1, 0x87, 0x00, 0x00
  , 0x07, 0xf0, 0x38, 0x70, 0xe1, 0x86, 0x00, 0x00
  , 0x07, 0xc0, 0x78, 0x70, 0xe3, 0x8e, 0x00, 0x00
  , 0x02, 0x00, 0xf0, 0xf0, 0xe3, 0x8e, 0x00, 0x00
  , 0x00, 0x01, 0xe0, 0xe0, 0xe3, 0x8e, 0x00, 0x00
  , 0x00, 0x03, 0xc1, 0xe1, 0xc3, 0x8e, 0x00, 0x00
  , 0x00, 0x0f, 0x83, 0xc3, 0xc3, 0x8e, 0x00, 0x00
  , 0x00, 0x7f, 0x07, 0x83, 0x83, 0x0e, 0x00, 0x00
  , 0x07, 0xfc, 0x0f, 0x07, 0x83, 0x0e, 0x00, 0x00
  , 0x07, 0xf0, 0x1e, 0x0f, 0x03, 0x0e, 0x00, 0x00
  , 0x07, 0x80, 0x7c, 0x1e, 0x03, 0x07, 0x00, 0x00
  , 0x00, 0x00, 0xf8, 0x3c, 0x03, 0x87, 0x80, 0x00
  , 0x00, 0x03, 0xf0, 0x78, 0x03, 0x83, 0xc0, 0x00
  , 0x00, 0x1f, 0xc0, 0xf0, 0x02, 0x00, 0x00, 0x00
  , 0x00, 0xff, 0x01, 0xe1, 0xc0, 0x0c, 0x00, 0x00
  , 0x07, 0xfc, 0x03, 0xc3, 0xe1, 0xff, 0xc0, 0x00
  , 0x07, 0xe0, 0x0f, 0x87, 0xc7, 0xff, 0xf0, 0x00
  , 0x07, 0x00, 0x3f, 0x0f, 0x0f, 0xff, 0xfc, 0x00
  , 0x00, 0x00, 0x7c, 0x3e, 0x3f, 0xff, 0xfe, 0x00
  , 0x00, 0x03, 0xf8, 0x7c, 0x3f, 0xff, 0xff, 0x00
  , 0x00, 0x1f, 0xe0, 0xf0, 0x7f, 0xff, 0xff, 0x80
  , 0x00, 0xff, 0x83, 0xe0, 0xff, 0xff, 0xff, 0x80
  , 0x01, 0xfc, 0x07, 0xc1, 0xff, 0xff, 0xe3, 0xc0
  , 0x01, 0xe0, 0x1f, 0x01, 0xff, 0xff, 0xc3, 0xc0
  , 0x00, 0x00, 0xfe, 0x01, 0xff, 0xff, 0x87, 0xe0
  , 0x00, 0x03, 0xf8, 0x13, 0xff, 0xff, 0x0f, 0xe0
  , 0x00, 0x1f, 0xe0, 0x73, 0xff, 0xfe, 0x1f, 0xe0
  , 0x00, 0x7f, 0x81, 0xf3, 0xff, 0xfc, 0x1f, 0xe0
  , 0x00, 0xfc, 0x03, 0xe3, 0xef, 0xf8, 0x3f, 0xe0
  , 0x00, 0x60, 0x0f, 0xc3, 0xc7, 0xf0, 0x7f, 0xe0
  , 0x00, 0x00, 0x3f, 0x03, 0xc3, 0xe0, 0xff, 0xe0
  , 0x00, 0x00, 0xfc, 0x03, 0xc1, 0xc1, 0xff, 0xe0
  , 0x00, 0x07, 0xf0, 0x13, 0xe0, 0x83, 0xff, 0xe0
  , 0x00, 0x0f, 0xc0, 0x7b, 0xf8, 0x07, 0xff, 0xe0
  , 0x00, 0x0f, 0x01, 0xf9, 0xfc, 0x0f, 0xff, 0xc0
  , 0x00, 0x00, 0x07, 0xf1, 0xfe, 0x1f, 0xff, 0xc0
  , 0x00, 0x00, 0x1f, 0xc0, 0xff, 0x3f, 0xff, 0x80
  , 0x00, 0x00, 0x7e, 0x00, 0xff, 0xff, 0xff, 0x80
  , 0x00, 0x00, 0xfc, 0x00, 0x7f, 0xff, 0xff, 0x00
  , 0x00, 0x00, 0xf0, 0x1f, 0x3f, 0xff, 0xfe, 0x00
  , 0x00, 0x00, 0x00, 0x7f, 0x1f, 0xff, 0xfc, 0x00
  , 0x00, 0x00, 0x01, 0xff, 0x8f, 0xff, 0xf8, 0x00
  , 0x00, 0x00, 0x03, 0xe0, 0xe3, 0xff, 0xe0, 0x00
  , 0x00, 0x00, 0x01, 0x80, 0x00, 0x7f, 0x00, 0x00
};
//---------------------------------------------------------------
#define Fingerprint_invalid_width 64
#define Fingerprint_invalid_height 64
static const unsigned char PROGMEM Fingerprint_invalid_bts[] = {
  0x00, 0x00, 0x03, 0xfe, 0x00, 0x00, 0x00, 0x00
  , 0x00, 0x00, 0x1f, 0xff, 0xe0, 0x00, 0x00, 0x00
  , 0x00, 0x00, 0x7f, 0xff, 0xf8, 0x00, 0x00, 0x00
  , 0x00, 0x00, 0xfc, 0x00, 0xfe, 0x00, 0x00, 0x00
  , 0x00, 0x03, 0xe0, 0x00, 0x1f, 0x00, 0x00, 0x00
  , 0x00, 0x07, 0xc0, 0x00, 0x07, 0x80, 0x00, 0x00
  , 0x00, 0x0f, 0x80, 0x00, 0x03, 0xe0, 0x00, 0x00
  , 0x00, 0x0e, 0x03, 0xff, 0x01, 0xe0, 0x00, 0x00
  , 0x00, 0x1c, 0x1f, 0xff, 0xe0, 0xf0, 0x00, 0x00
  , 0x00, 0x3c, 0x3f, 0xff, 0xf0, 0x78, 0x00, 0x00
  , 0x00, 0x78, 0x7c, 0x00, 0xf8, 0x3c, 0x00, 0x00
  , 0x00, 0x70, 0xf0, 0x00, 0x3c, 0x1c, 0x00, 0x00
  , 0x00, 0xe1, 0xe0, 0x00, 0x1e, 0x1c, 0x00, 0x00
  , 0x00, 0xe1, 0xc0, 0x00, 0x0f, 0x0e, 0x00, 0x00
  , 0x00, 0xc3, 0x81, 0xfc, 0x07, 0x0e, 0x00, 0x00
  , 0x00, 0x03, 0x83, 0xff, 0x07, 0x8e, 0x00, 0x00
  , 0x00, 0x07, 0x07, 0x8f, 0x83, 0x87, 0x00, 0x00
  , 0x00, 0x0f, 0x0f, 0x03, 0xc3, 0x87, 0x00, 0x00
  , 0x00, 0x1e, 0x0e, 0x01, 0xc3, 0x87, 0x00, 0x00
  , 0x00, 0x3c, 0x1c, 0x00, 0xe1, 0x87, 0x00, 0x00
  , 0x00, 0xf8, 0x1c, 0x30, 0xe1, 0x87, 0x00, 0x00
  , 0x07, 0xf0, 0x38, 0x70, 0xe1, 0x86, 0x00, 0x00
  , 0x07, 0xc0, 0x78, 0x70, 0xe3, 0x8e, 0x00, 0x00
  , 0x02, 0x00, 0xf0, 0xf0, 0xe3, 0x8e, 0x00, 0x00
  , 0x00, 0x01, 0xe0, 0xe0, 0xe3, 0x8e, 0x00, 0x00
  , 0x00, 0x03, 0xc1, 0xe1, 0xc3, 0x8e, 0x00, 0x00
  , 0x00, 0x0f, 0x83, 0xc3, 0xc3, 0x8e, 0x00, 0x00
  , 0x00, 0x7f, 0x07, 0x83, 0x83, 0x0e, 0x00, 0x00
  , 0x07, 0xfc, 0x0f, 0x07, 0x83, 0x0e, 0x00, 0x00
  , 0x07, 0xf0, 0x1e, 0x0f, 0x03, 0x0e, 0x00, 0x00
  , 0x07, 0x80, 0x7c, 0x1e, 0x03, 0x07, 0x00, 0x00
  , 0x00, 0x00, 0xf8, 0x3c, 0x03, 0x87, 0x80, 0x00
  , 0x00, 0x03, 0xf0, 0x78, 0x03, 0x83, 0xc0, 0x00
  , 0x00, 0x1f, 0xc0, 0xf0, 0x02, 0x00, 0x00, 0x00
  , 0x00, 0xff, 0x01, 0xe1, 0xc0, 0x00, 0x00, 0x00
  , 0x07, 0xfc, 0x03, 0xc3, 0xe1, 0xff, 0xc0, 0x00
  , 0x07, 0xe0, 0x0f, 0x87, 0xc7, 0xff, 0xf0, 0x00
  , 0x07, 0x00, 0x3f, 0x0f, 0x0f, 0xff, 0xf8, 0x00
  , 0x00, 0x00, 0x7c, 0x3e, 0x1f, 0xff, 0xfe, 0x00
  , 0x00, 0x03, 0xf8, 0x7c, 0x3f, 0xff, 0xff, 0x00
  , 0x00, 0x1f, 0xe0, 0xf0, 0x7f, 0xff, 0xff, 0x00
  , 0x00, 0xff, 0x83, 0xe0, 0xfe, 0xff, 0xbf, 0x80
  , 0x01, 0xfc, 0x07, 0xc0, 0xfc, 0x7f, 0x1f, 0xc0
  , 0x01, 0xe0, 0x1f, 0x01, 0xf8, 0x3e, 0x0f, 0xc0
  , 0x00, 0x00, 0xfe, 0x01, 0xf8, 0x1c, 0x07, 0xe0
  , 0x00, 0x03, 0xf8, 0x13, 0xf8, 0x00, 0x0f, 0xe0
  , 0x00, 0x1f, 0xe0, 0x73, 0xfc, 0x00, 0x1f, 0xe0
  , 0x00, 0x7f, 0x81, 0xf3, 0xfe, 0x00, 0x3f, 0xe0
  , 0x00, 0xfc, 0x03, 0xe3, 0xff, 0x00, 0x7f, 0xe0
  , 0x00, 0x60, 0x0f, 0xc3, 0xff, 0x80, 0xff, 0xe0
  , 0x00, 0x00, 0x3f, 0x03, 0xff, 0x00, 0x7f, 0xe0
  , 0x00, 0x00, 0xfc, 0x03, 0xfe, 0x00, 0x3f, 0xe0
  , 0x00, 0x07, 0xf0, 0x13, 0xfc, 0x00, 0x1f, 0xe0
  , 0x00, 0x0f, 0xc0, 0x79, 0xf8, 0x08, 0x0f, 0xe0
  , 0x00, 0x0f, 0x01, 0xf9, 0xf8, 0x1c, 0x0f, 0xc0
  , 0x00, 0x00, 0x07, 0xf1, 0xfc, 0x3e, 0x1f, 0xc0
  , 0x00, 0x00, 0x1f, 0xc0, 0xfe, 0x7f, 0x3f, 0x80
  , 0x00, 0x00, 0x7e, 0x00, 0xff, 0xff, 0xff, 0x80
  , 0x00, 0x00, 0xfc, 0x00, 0x7f, 0xff, 0xff, 0x00
  , 0x00, 0x00, 0xf0, 0x1f, 0x3f, 0xff, 0xfe, 0x00
  , 0x00, 0x00, 0x00, 0x7f, 0x1f, 0xff, 0xfc, 0x00
  , 0x00, 0x00, 0x01, 0xff, 0x8f, 0xff, 0xf8, 0x00
  , 0x00, 0x00, 0x03, 0xe0, 0xe3, 0xff, 0xe0, 0x00
  , 0x00, 0x00, 0x01, 0x80, 0x00, 0x7f, 0x00, 0x00
};
//---------------------------------------------------------------
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&Serial2);
// DatabaseConnection db;

int relayPin = 23;
// int buzzerPin = 15;
String input;
uint8_t oldId;
uint8_t id;
uint8_t choice;
String name;
std::map<uint8_t, String> registeredUsers;
void sendMapDataToFeed() {
  String dataToSend;
  
  // Lặp qua các phần tử của map
  for (auto const& pair : registeredUsers) {
      dataToSend += "Name: " + String((pair.second)) + ", ID: " +  pair.first + "\n";
  }
  if(registeredUsers.empty()){
    dataToSend ="We do not have any users";
  }
  // Gửi dữ liệu lên feed
  ListFeed->save(dataToSend);
}
void handleMessage1(AdafruitIO_Data *data){
  // id = atoi((data->value()));
   input = (data->value());
 
}
void handleMessage2(AdafruitIO_Data *data){
  // id = atoi((data->value()));
    choice = atoi((data->value()));
 
}
void handleMessage3(AdafruitIO_Data *data){
    if(data->toPinLevel()==HIGH){
      digitalWrite(relayPin,LOW);
    }
    if(data->toPinLevel()==LOW){
      digitalWrite(relayPin,HIGH);
    }

}
void setup()
{
  choice = 0;
  name = "";
  id = 0;
  input = "-";
  Serial.begin(57600);
  Serial2.begin(115200);
  // db.open("database.db");
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }
  while (!Serial);
  delay(100);
  display.clearDisplay();
  display.drawBitmap(0, 0, logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.display();
  Serial.println("Fingerprint Door Lock");
  delay(3000);
  display.clearDisplay();
  // set the data rate for the sensor serial port
  finger.begin(57600);
  if (finger.verifyPassword())
  {
    Serial.println("Fingerprint Sensor Connected");
    display.clearDisplay();
    display.setTextSize(2);             // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE);        // Draw white text
    display.setCursor(25, 0);            // Start at top-left corner
    display.println(("Sensor"));
    display.setCursor(5, 35);
    display.println("Connected");
    display.display();
    delay(3000);
    // display.clearDisplay();
  }
  else
  {
    display.clearDisplay();
    display.setTextSize(2);             // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE);        // Draw white text
    display.setCursor(25, 0);            // Start at top-left corner
    display.println(("Sensor"));
    display.setCursor(5, 35);
    display.println("Not Found");
    display.display();
    Serial.println("Unable to find Sensor");
    delay(3000);
    Serial.println("Check Connections");
    while (1) {
      delay(1);
    }
  }
  Serial.print("Connecting to Adafruit IO");
  io.connect();
  display.clearDisplay();
   while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("Connected to Adafruit IO");
  // timer.setInterval(1000L, getFingerprintIDez);
  idFeed->get();
  idFeed->onMessage(handleMessage1);
  optionFeed->get();
  optionFeed->onMessage(handleMessage2);
  Button->get();
  Button->onMessage(handleMessage2);
  choiceFeed->save("Enter your option\n"
                 "Enter 1 for registering\n"
                 "Enter 2 for deleting registered fingerprint\n"
                 "Enter 3 to show the list of users");

  
}
void loop()                     // run over and over again
{ 
  io.run();
  delay(1000);
  choice = 0;
  name = "";
  id = 0;
  input = "-";
 
  while(choice==0){
     getFingerprintIDez();
     io.run();
  }
  if(choice == 1){
    RegisterFingerprint();
    choice = 0;
    id = 0;
    name="";
    input = "-";
  }
  if(choice == 2){
    deleteFingerprint();
    registeredUsers.erase(id);
    choice = 0;
    id = 0;
  }
  if(choice == 3){
    sendMapDataToFeed();
    choice = 0;
  }
  delay(1000);
 
  
  // getFingerprintIDez();
  // RegisterFingerprint();
}
void RegisterFingerprint()
{   io.run();
    Serial.println("Ready to enroll a fingerprint!");
    statusFeed->save("Ready to enroll a fingerprint!");
    delay(1000);
    Serial.println("Please type your name...");
    statusFeed->save("Please type your name...");
    while(input=="-"){
    io.run();
    }
    name = input;
    Serial.println(name);
    
    delay(1000);
    Serial.println("Please type in the ID # (from 1 to 127) you want to save this finger as...");
    statusFeed->save("Please type in the ID # (from 1 to 127) you want to save this finger as...");
 
  while(id==0){
    io.run();
    id = (input).toInt(); 
    // Serial.println(id);
  } 
  
  // oldId = id;
  

  
  Serial.println(id);

  if (id == 0) {
    delay(2000);// ID #0 not allowed, try again!
     return;
  }
  Serial.print("Enrolling ID #");
  statusFeed->save("Enrolling ID #"+String(id));
  // statusFeed->save(id);
  Serial.println(id);
  while (!  getFingerprintEnroll() );
  delay(2000);
  registeredUsers[id]=name;
}
uint8_t getFingerprintEnroll() 
{
  int p = -1;
  Serial.print("Waiting for valid finger to enroll as #"); Serial.println(id);
  statusFeed->save("Waiting for valid finger to enroll as #"+ String(id));
 while (p != FINGERPRINT_OK) {
  p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      statusFeed->save("Image taken");
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      // statusFeed->save(".");
      Serial.println(".");
      delay(2000);
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      statusFeed->save("Communication error");
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      statusFeed->save("Imaging error");
      Serial.println("Imaging error");
      break;
    default:
      statusFeed->save("Unknown error");
      Serial.println("Unknown error");
      break;
  }
  delay(2000);
}
delay(2000);
// OK success!
p = finger.image2Tz(1);
switch (p) {
  case FINGERPRINT_OK:
    statusFeed->save("Image converted");
    Serial.println("Image converted");
    break;
  case FINGERPRINT_IMAGEMESS:
    statusFeed->save("Image too messy");
    Serial.println("Image too messy");
    return p;
  case FINGERPRINT_PACKETRECIEVEERR:
    statusFeed->save("Communication error");
    Serial.println("Communication error");
    return p;
  case FINGERPRINT_FEATUREFAIL:
    statusFeed->save("Could not find fingerprint features");
    Serial.println("Could not find fingerprint features");
    return p;
  case FINGERPRINT_INVALIDIMAGE:
    statusFeed->save("Could not find fingerprint features");
    Serial.println("Could not find fingerprint features");
    return p;
  default:
    statusFeed->save("Unknown error");
    Serial.println("Unknown error");
    return p;
}
delay(2000);
statusFeed->save("Remove finger");
Serial.println("Remove finger");
delay(2000);
p = 0;
while (p != FINGERPRINT_NOFINGER) {
  p = finger.getImage();
}
statusFeed->save("ID " + String(id));
Serial.print("ID "); Serial.println(id);
delay(2000);
p = -1;
statusFeed->save("Place same finger again");
Serial.println("Place same finger again");

delay(2000);
while (p != FINGERPRINT_OK) {
  p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      statusFeed->save("Image taken");
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.print(".");
      // statusFeed->save(".");
      delay(3000);
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      statusFeed->save("Communication error");
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      statusFeed->save("Imaging error");
      Serial.println("Imaging error");
      break;
    default:
      statusFeed->save("Unknown error");
      Serial.println("Unknown error");
      break;
  }
  delay(1000);
}
delay(2000);
// OK success!
p = finger.image2Tz(2);
switch (p) {
  case FINGERPRINT_OK:
    statusFeed->save("Image converted");
    Serial.println("Image converted");
    break;
  case FINGERPRINT_IMAGEMESS:
    statusFeed->save("Image too messy");
    Serial.println("Image too messy");
    return p;
  case FINGERPRINT_PACKETRECIEVEERR:
    statusFeed->save("Communication error");
    Serial.println("Communication error");
    return p;
  case FINGERPRINT_FEATUREFAIL:
    statusFeed->save("Could not find fingerprint features");
    Serial.println("Could not find fingerprint features");
    return p;
  case FINGERPRINT_INVALIDIMAGE:
    statusFeed->save("Could not find fingerprint features");
    Serial.println("Could not find fingerprint features");
    return p;
  default:
    statusFeed->save("Unknown error");
    Serial.println("Unknown error");
    return p;
}
delay(2000);
// OK converted!
statusFeed->save("Creating model for #" + String(id));
Serial.print("Creating model for #");  Serial.println(id);
p = finger.createModel();
if (p == FINGERPRINT_OK) {
  statusFeed->save("Prints matched!");
  Serial.println("Prints matched!");
} else if (p == FINGERPRINT_PACKETRECIEVEERR) {
  statusFeed->save("Communication error");
  Serial.println("Communication error");
  return p;
} else if (p == FINGERPRINT_ENROLLMISMATCH) {
  statusFeed->save("Fingerprints did not match");
  Serial.println("Fingerprints did not match");
  return p;
} else {
  statusFeed->save("Unknown error");
  Serial.println("Unknown error");
  return p;
}
delay(2000);

Serial.print("ID "); Serial.println(id);
statusFeed->save("ID " + String(id));
delay(2000);
p = finger.storeModel(id);
if (p == FINGERPRINT_OK) {
  statusFeed->save("Stored!");
  Serial.println("Stored!");
} else if (p == FINGERPRINT_PACKETRECIEVEERR) {
  statusFeed->save("Communication error");
  Serial.println("Communication error");
  return p;
} else if (p == FINGERPRINT_BADLOCATION) {
  statusFeed->save("Could not store in that location");
  Serial.println("Could not store in that location");
  return p;
} else if (p == FINGERPRINT_FLASHERR) {
  statusFeed->save("Error writing to flash");
  Serial.println("Error writing to flash");
  return p;
} else {
  statusFeed->save("Unknown error");
  Serial.println("Unknown error");
  return p;
}

  delay(2000);
  // id = 0;
  return true;
  
}
// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez()
{
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)
  {
    display.clearDisplay();
    display.drawBitmap(0, 0, logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
    display.display();
    Serial.println("Waiting For Valid Finger");
    statusFeed->save("Waiting For Valid Finger");
    delay(2000);
    return -1;
  }
  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)
  {
    display.clearDisplay();
    display.setTextSize(2);             // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE);        // Draw white text
    display.setCursor(0, 0);            // Start at top-left corner
    display.println(("Messy  Image"));
    statusFeed->save(("Messy  Image"));
    display.setCursor(0, 35);
    display.println("Try Again");
     statusFeed->save("Try Again");
    display.display();
    Serial.println("Messy Image Try Again");
    statusFeed->save("Messy Image Try Again");
    delay(1000);
    display.clearDisplay();
    return -1;
  }
  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  {
    display.clearDisplay();
    display.setTextSize(2);             // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE);        // Draw white text
    display.drawBitmap(34, 0, Fingerprint_invalid_bts, Fingerprint_invalid_width, Fingerprint_invalid_height, 1);
    display.display();
    Serial.println("Not Valid Finger");
    statusFeed->save("Not Valid Finger");
    // digitalWrite(buzzerPin, HIGH);
    delay(3000);
    display.clearDisplay();
    // Blynk.virtualWrite(V0, "Access Denied, Invalid Fingerprint" );
    // digitalWrite(buzzerPin, LOW);
    return -1;
  }
  // found a match!
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setTextSize(2);
  display.drawBitmap(34, 0, Fingerprint_valid_bts, Fingerprint_valid_width, Fingerprint_valid_height, 1);
  display.display();
  digitalWrite(relayPin, LOW);
  delay(1000);
  display.clearDisplay();
  display.setCursor(40, 0);            // Start at top-left corner
  display.println(("Door"));
  display.setCursor(15, 20);            // Start at top-left corner
  display.println(("Unlocked"));
  display.setCursor(20, 40);
  display.println("Welcome");
  display.display();
  statusFeed->save("Door Unlocked Welcome");
  // Blynk.virtualWrite(V0, "Access Granted, Door Unlocked" );
  delay(3000);
  display.clearDisplay();
  display.setTextSize(2);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(20, 20);            // Start at top-left corner
  display.println(("Closing"));
  display.setCursor(20, 40);            // Start at top-left corner
  display.println(("the Door"));
  display.display();
  statusFeed->save("Closing the door...");
  digitalWrite(relayPin, HIGH);
  delay(3000);
  display.clearDisplay();
  Serial.println("Door Unlocked Welcome");
  return finger.fingerID;
}
uint8_t deleteFingerprint() {
  Serial.println("Please type in the ID # (from 1 to 127) you want to delete...");
  statusFeed->save("Please type in the ID # (from 1 to 127) you want to delete...");
  while(id==0){
    io.run();
    id = (input).toInt(); 
  }
  delay(2000);
  statusFeed->save("Deleting ID #"+String(id));
  delay(2000);
  Serial.print("Deleting ID #");
  Serial.println(id);
  uint8_t p = -1;

  p = finger.deleteModel(id);

  if (p == FINGERPRINT_OK) {
    statusFeed->save("Deleted!");
    Serial.println("Deleted!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    statusFeed->save("Communication error");
    Serial.println("Communication error");
  } else if (p == FINGERPRINT_BADLOCATION) {
    statusFeed->save("Could not delete in that location");
    Serial.println("Could not delete in that location");
  } else if (p == FINGERPRINT_FLASHERR) {
    statusFeed->save("Error writing to flash");
    Serial.println("Error writing to flash");
  } else {
    Serial.print("Unknown error: 0x"); Serial.println(p, HEX);
  }

  return p;
}