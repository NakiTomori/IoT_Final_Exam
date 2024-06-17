
 #include <FastLED.h>

 #define NUM_LEDS 20
 #define LED_PIN 25

 CRGB leds[NUM_LEDS];

 void setup () {
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(50);
  delay(2000);
 }

 void loop () {
  leds[0] = CRGB::Red;
  leds[1] = CRGB::Green;
  leds[2] = CRGB::Blue;
  FastLED.show();
 }