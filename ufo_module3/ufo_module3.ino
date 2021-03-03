#include <WebServer.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include "Freenove_WS2812_Lib_for_ESP32.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

/* * * * Server/wifi setup * * * */
#define CONSOLE_IP "192.168.1.2"
#define CONSOLE_PORT 4210

const char* ssid = "ESP32";
const char* password = "12345678";
WiFiUDP Udp;
IPAddress local_ip(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
WebServer server(80);

/* * * * Motion sensor + LED Ring setup * * * */
int sensorPin = 14; // the number of the infrared motion sensor pin
#define LEDS_COUNT  8
#define LEDS_PIN  2
#define CHANNEL   0

String planets[] = {"mercury", "venus", "mars", "jupiter", "saturn", "uranus", "neptune", "pluto"};
String planet = "mercury";

Freenove_ESP32_WS2812 strip = Freenove_ESP32_WS2812(LEDS_COUNT, LEDS_PIN, CHANNEL, TYPE_GRB);

/* * * * Liquid Crystal I2C setup * * * */
// set the LCD number of columns and rows
int lcdColumns = 16;
int lcdRows = 2;

// set LCD address, number of columns and rows
// if you don't know your display address, run an I2C scanner sketch
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);

void setup()
{
  Serial.begin(115200);
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  pinMode(sensorPin, INPUT);  // initialize the sensor pin as input
  setPlanet();
  server.begin();
  strip.begin();
  
  // initialize LCD
  lcd.init();
}

void loop()
{
  Udp.beginPacket(CONSOLE_IP, CONSOLE_PORT);
  // Just test touch pin - Touch0 is T0 which is on GPIO 4.
  Udp.printf(String(touchRead(T0)).c_str());
  Udp.endPacket();

  lcd.setCursor(0, 0);
  // turn on LCD backlight                      
  lcd.backlight();
  // print message
  lcd.print("Hello, World!");

  int sense = digitalRead(sensorPin);
  if (sense == 1) {
    setColors();
    strip.show();
  }
  delay(1000);
}

// pick planet at random
void setPlanet() {
   /*int num = random(8);
   planet = planets[num];
   */
   planet = planets[7];
}

// set LED colors to correspond to planet
void setColors() {
  if (planet.equals("mercury")) {
    for (int i = 0; i < LEDS_COUNT; i++) {
      strip.setLedColorData(i,127, 133, 143); // gray
    }
  }
  else if (planet.equals("venus")) {
    for (int i = 0; i < LEDS_COUNT; i++) {
      strip.setLedColorData(i,255, 255, 128); // yellow-white
    }
  }
  else if (planet.equals("mars")) {
    for (int i = 0; i < LEDS_COUNT; i++) {
      strip.setLedColorData(i,252, 105, 0); // red/orange
    }
  }
  else if (planet.equals("jupiter")) {
    Serial.println("Should be multicolored red");
    for (int i = 0; i < LEDS_COUNT; i++) {
      if (i%4 == 0) strip.setLedColorData(i,255, 219, 133); // orange1
      else if (i%4 == 1) strip.setLedColorData(i,255, 174, 0); // orange2
      else if (i%4 == 2) strip.setLedColorData(i,255, 0, 0); // red
      else strip.setLedColorData(i,255, 238, 0); // yellow
    }
  }
  else if (planet.equals("saturn")) {
    for (int i = 0; i < LEDS_COUNT; i++) {
      strip.setLedColorData(i,255, 236, 74); // yellow
    }
  }
  else if (planet.equals("uranus")) {
    for (int i = 0; i < LEDS_COUNT; i++) {
      strip.setLedColorData(4,0, 158, 129); // teal
    }
  }
  else if (planet.equals("neptune")) {
    for (int i = 0; i < LEDS_COUNT; i++) {
      strip.setLedColorData(i,0, 0, 255); // dark blue
    }
  }
  else { //pluto
    strip.setLedColorData(0,255, 0, 0); // red
    strip.setLedColorData(1,255, 174, 0); // orange
    strip.setLedColorData(2,255, 238, 0); // yellow
    strip.setLedColorData(3,0, 255, 0); // green
    strip.setLedColorData(4,0, 255, 255); // teal
    strip.setLedColorData(5,0, 0, 255); // blue
    strip.setLedColorData(6,255, 0, 255); // purple
    strip.setLedColorData(7,255, 0, 160); // pink
  }
}
