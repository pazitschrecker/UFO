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
int wifiNum = 0;

char incomingPacket[255]; 

/* * * * Motion sensor + LED Ring setup * * * */
int sensorPin = 14; // the number of the infrared motion sensor pin
#define LEDS_COUNT  8
#define LEDS_PIN  2
#define CHANNEL   0

int planet = 0;
String messages[] = {"", "", "We met Perseverance and wanted to say hi!", "", "", "Please, no jokes", "", "We feel left out"};

Freenove_ESP32_WS2812 strip = Freenove_ESP32_WS2812(LEDS_COUNT, LEDS_PIN, CHANNEL, TYPE_GRB);

/* * * * Liquid Crystal I2C setup * * * */
// set the LCD number of columns and rows
int lcdColumns = 16;
int lcdRows = 2;

// set LCD address, number of columns and rows
// if you don't know your display address, run an I2C scanner sketch
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);

/* * * * Variables to keep track of what has been set and sent over wifi * * * */
int sentPlanet = 0;
int sentWifiNum = 0;
int sentIntro = 0;
int sentLight = 0;
int sentClue = 0;
int packetSize = 0;
int LCdisplay = 0;
bool startReceived = false;

void setup()
{
  Serial.begin(9600);
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  pinMode(sensorPin, INPUT);  // initialize the sensor pin as input
  
  setPlanet();
  server.begin();

  packetSize = 0;
  
  lcd.init();

  Serial.printf("HELP");

  Udp.begin(CONSOLE_PORT);

  
  
  while (!startReceived) {
    Serial.printf(planet, '\n');
    packetSize = Udp.parsePacket();
    if (packetSize) {
      Serial.printf("PACKETS");
      Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
      startReceived = true;
    }
  }

  strip.begin();
  strip.setBrightness(0);
}

void loop() {

  Serial.printf("In loop");
  Udp.beginPacket(CONSOLE_IP, CONSOLE_PORT);
  
  // Just test touch pin - Touch0 is T0 which is on GPIO 4.
  //Udp.printf(String(touchRead(T0)).c_str());

  if (digitalRead(sensorPin) == 1) {
    setColors();
    strip.setBrightness(20);
    strip.show();
    Udp.printf(String(1).c_str());
  }

  else if (touchRead(T0) < 28) {
    lcd.setCursor(0, 0);
    lcd.backlight();
    lcd.print(messages[planet]);
    Udp.printf(String(2).c_str());
    LCdisplay = 1;
  }
  
  else {
    String strPlanet = String(planet);
    String toSend = "P"+strPlanet;
    Udp.printf(toSend.c_str());
  }
  
  Udp.endPacket();
  delay(2000);
}

void setWifiNum() {
  int num = random(3);
}

// pick planet at random
void setPlanet() {
   /*int num = random(8);
   planet = num;
   */
   planet = 7;
}

// set LED colors to correspond to planet
void setColors() {
  // mercury
  if (planet == 0) {
    for (int i = 0; i < LEDS_COUNT; i++) {
      strip.setLedColorData(i,127, 133, 143); // gray
    }
  }
  // venus
  else if (planet == 1) {
    for (int i = 0; i < LEDS_COUNT; i++) {
      strip.setLedColorData(i,255, 255, 128); // yellow-white
    }
  }
  // mars
  else if (planet == 2) {
    for (int i = 0; i < LEDS_COUNT; i++) {
      strip.setLedColorData(i,252, 105, 0); // red/orange
    }
  }
  // jupiter
  else if (planet == 3) {
    Serial.println("Should be multicolored red");
    for (int i = 0; i < LEDS_COUNT; i++) {
      if (i%4 == 0) strip.setLedColorData(i,255, 219, 133); // orange1
      else if (i%4 == 1) strip.setLedColorData(i,255, 174, 0); // orange2
      else if (i%4 == 2) strip.setLedColorData(i,255, 0, 0); // red
      else strip.setLedColorData(i,255, 238, 0); // yellow
    }
  }
  // saturn
  else if (planet == 4) {
    for (int i = 0; i < LEDS_COUNT; i++) {
      strip.setLedColorData(i,255, 236, 74); // yellow
    }
  }
  // uranus
  else if (planet == 5) {
    for (int i = 0; i < LEDS_COUNT; i++) {
      strip.setLedColorData(4,0, 158, 129); // teal
    }
  }
  // neptune
  else if (planet == 6) {
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
