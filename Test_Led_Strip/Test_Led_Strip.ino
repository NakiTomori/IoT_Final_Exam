#include <FastLED.h>

#define LIGHT_SENSOR_PIN 26

#define LED_PIN 25
#define NUM_LEDS    50
#define BRIGHTNESS  64
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

#define ANALOG_THREHOLD 300

void setup() {
  // put your setup code here, to run once:
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  int analogValue = analogRead(LIGHT_SENSOR_PIN);
  Serial.println(analogValue);

  if (analogValue > ANALOG_THREHOLD ){
    //digitalWrite(LED,HIGH);
      for( int i = 0; i < NUM_LEDS; ++i) {
        leds[i] = CRGB::Red;
      }
      FastLED.show();
  }  
  else{
      for( int i = 0; i < NUM_LEDS; ++i) {
        leds[i] = CRGB::Black;
      }
      FastLED.show();
  }
}
