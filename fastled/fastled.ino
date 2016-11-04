#define FASTLED_ESP8266_RAW_PIN_ORDER
// we need to include this before the FastLED import to define the mappings
// more info here https://github.com/FastLED/FastLED/wiki/ESP8266-notes

#include <FastLED.h>
#define NUM_LEDS 30 // Number of LEDs in strip

#define DATA_PIN 4
#define CLOCK_PIN 5

CRGB leds[NUM_LEDS];

void setup() { 

  delay( 3000 ); // power-up safety delay
  FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, BRG>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  pinMode(LED_BUILTIN, OUTPUT);
  // we put BRG here as the color scheme because nodemcu implements it that way... god knows why!!!
}

void loop() {
  fill_rainbow(leds, NUM_LEDS, millis());
  FastLED.show();
  delay(150); 
}
