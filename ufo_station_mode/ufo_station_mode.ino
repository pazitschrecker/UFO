#include <WebServer.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include "Freenove_WS2812_Lib_for_ESP32.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

//set up to connect to an existing network (e.g. mobile hotspot from laptop that will run the python code)
const char* ssid = "Pazit's iPhone";
const char* password = "zb7jix0talil";
WiFiUDP Udp;
unsigned int localUdpPort = 4210;  //  port to listen on
char incomingPacket[255];  // buffer for incoming packets

/* * * * Motion sensor + LED Ring setup * * * */
int sensorPin = 14; // the number of the infrared motion sensor pin
#define LEDS_COUNT  8
#define LEDS_PIN  2
#define CHANNEL   0

int planet = 0;
String messages[8][2] = {{"Our planet is", "small + mighty"}, {"Days are longer", " than years"}, {"We met", "Perseverance!"}, {"Dropped from", "King of planetae"}, {"From the most", "blinged-out planet"}, {"Please,", "no jokes"}, {"Our home is cold", "but we come in peace"}, {"We just want to", "be included"}};
Freenove_ESP32_WS2812 strip = Freenove_ESP32_WS2812(LEDS_COUNT, LEDS_PIN, CHANNEL, TYPE_GRB);

/* * * * Liquid Crystal I2C setup * * * */
// set the LCD number of columns and rows
int lcdColumns = 16;
int lcdRows = 2;

bool LCdisplayed = false;
bool sentLight = false;
bool sentPlanet = false;
int messagesSent = 0;

// set LCD address, number of columns and rows
// if you don't know your display address, run an I2C scanner sketch
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);

void setup()
{
  int status = WL_IDLE_STATUS;
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");
  lcd.init();
  lcd.backlight();

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    lcd.print(".");
  }

  LCdisplayed = false;
  sentLight = false;
  sentPlanet = false;
  messagesSent = 0;

  Serial.println("Connected to wifi");
  lcd.setCursor(0, 0);
  lcd.print("CONNECTED");
  lcd.noBacklight(); // turn off backlight
  Udp.begin(localUdpPort);
  Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), localUdpPort);

  setPlanet();
  strip.begin();
  strip.show();

 
  // we recv one packet from the remote so we can know its IP and port
  bool readPacket = false;
  while (!readPacket) {
    int packetSize = Udp.parsePacket();
    if (packetSize)
     {
      // receive incoming UDP packets
      Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
      int len = Udp.read(incomingPacket, 255);
      if (len > 0)
      {
        incomingPacket[len] = 0;
      }
      Serial.printf("UDP packet contents: %s\n", incomingPacket);
      readPacket = true;
    } 
  }
  lcd.clear();
}

void loop()
{

  delay(2000);
  // once we know where we got the inital packet from, send data back to that IP address and port

  bool readPacket = false;

  // once both displayed, wait for message the user has answered to come in
  while (!readPacket && sentLight && LCdisplayed && sentPlanet) {
    reset();
    /*
    int packetSize = Udp.parsePacket();
    if (packetSize)
    {
      Serial.printf("UDP packet contents: %s\n", incomingPacket);
      int len = Udp.read(incomingPacket, 255);
      readPacket = true;
      reset();
     }
     */
   }

   while (messagesSent < 6) {
    if (digitalRead(sensorPin) == 1) {
    setColors();
    strip.setBrightness(60);
    strip.show();
 
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.printf(String(1).c_str());
    Udp.endPacket();
    
    if (sentLight == false) {
      sentLight = true;
    }
    messagesSent++;
    delay(1000);
    }
  }

  while (messagesSent < 12) {
    if (!LCdisplayed) {
      lcd.setCursor(0, 0);
      lcd.print(messages[planet][0]);
      lcd.setCursor(0, 1);
      lcd.print(messages[planet][1]);
      lcd.backlight();
      LCdisplayed = true;
    }
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.printf(String(2).c_str());
    Udp.endPacket();
    messagesSent++;
    delay(1000);
  }

  /*int touch = touchRead(T0);
  if (touch < 25) {
    lcd.setCursor(0, 0);
    lcd.backlight();
    lcd.print(messages[planet]);

    
    
    if (!LCdisplayed) {
      LCdisplayed = true;
    }
  }*/

  String strPlanet = String(planet);
  String toSend = "P"+strPlanet;
  Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
  Udp.printf(toSend.c_str());
  Udp.endPacket();
  sentPlanet = true;
  
  delay(1000);
}

void reset() {
  LCdisplayed = false;
  sentLight = false;
  sentPlanet = false;
  messagesSent = 0;
  
  setPlanet();
  strip.setBrightness(0);
  strip.show();
 
  // we recv one packet from the remote so we can know its IP and port
  bool readPacket = false;
  while (!readPacket) {
    Serial.printf("Nothing Yet");
    int packetSize = Udp.parsePacket();
    if (packetSize)
     {
      // receive incoming UDP packets
      Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
      int len = Udp.read(incomingPacket, 255);
      if (len > 0)
      {
        incomingPacket[len] = 0;
      }
      Serial.printf("UDP packet contents: %s\n", incomingPacket);
      readPacket = true;
      lcd.noBacklight();
      lcd.clear();
    } 
  }
  delay(5000);
  loop();
}

// pick planet at random
void setPlanet() {
   int num = random(8);
   planet = num;
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
