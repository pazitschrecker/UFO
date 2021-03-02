#include "Freenove_WS2812_Lib_for_ESP32.h"

int sensorPin = 14; // the number of the infrared motion sensor pin
#define LEDS_COUNT  8
#define LEDS_PIN  2
#define CHANNEL   0

Freenove_ESP32_WS2812 strip = Freenove_ESP32_WS2812(LEDS_COUNT, LEDS_PIN, CHANNEL, TYPE_GRB);

String planets[] = {"mercury", "venus", "mars", "jupiter", "saturn", "uranus", "neptune", "pluto"};
String planet = "mercury";

void setup() {
  pinMode(sensorPin, INPUT);  // initialize the sensor pin as input
  setPlanet();
  Serial.begin(9600);
  Serial.println("planet: "+ planet);
  strip.begin();
}

void loop() {
  // Turn on or off LED according to Infrared Motion Sensor
  int sense = digitalRead(sensorPin);
  Serial.println(sense);
  /*if (sense == 1) {
    Serial.println("should light up");
    setColors();
    strip.show();
  }
  else {
    for (int i = 0; i < LEDS_COUNT; i++) {
      strip.setLedColorData(i,0,0,0);
    }
    strip.show();
  }
  */
  setColors();
  strip.show();
  delay(1000);              // wait for a second
}

void setPlanet() {
   /*int num = random(8);
   planet = planets[num];
   */
   planet = planets[7];
}

void setColors() {
  if (planet.equals("mercury")) {
    for (int i = 0; i < LEDS_COUNT; i++) {
      strip.setLedColorData(i,127, 133, 143); // gray
    }
  }
  else if (planet.equals("venus")) {
    for (int i = 0; i < LEDS_COUNT; i++) {
      strip.setLedColorData(i,255, 255, 255); // white
    }
  }
  else if (planet.equals("mars")) {
    for (int i = 0; i < LEDS_COUNT; i++) {
      strip.setLedColorData(i,232, 142, 81); // red/orange
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
      strip.setLedColorData(i,235, 220, 91); // yellow
    }
  }
  else if (planet.equals("uranus")) {
    for (int i = 0; i < LEDS_COUNT; i++) {
      strip.setLedColorData(i,0, 247, 255); // light teal
    }
  }
  else if (planet.equals("neptune")) {
    for (int i = 0; i < LEDS_COUNT; i++) {
      strip.setLedColorData(i,0, 115, 255); // dark blue
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
