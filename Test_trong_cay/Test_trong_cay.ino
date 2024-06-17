#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "DHT.h"
#include <string.h>
#include <FastLED.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define DHTTYPE DHT11   // DHT 11
#define DHTPIN 27

#define LIGHT_SENSOR_PIN 35
#define ANALOG_THREHOLD 300

#define LED_PIN 25
#define NUM_LEDS    50
#define BRIGHTNESS  80
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

#define soilWet 500   // Define max value we consider soil 'wet'
#define soilDry 750   // Define min value we consider soil 'dry'
#define sensorPin 34

DHT dht(DHTPIN, DHTTYPE);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  // put your setup code here, to run once:
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  pinMode(sensorPin, OUTPUT);
  Serial.begin(9600);
  dht.begin();

    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)){
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  display.clearDisplay();
}

void loop() {
  // put your main code here, to run repeatedly:
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  int moisture = analogRead(sensorPin);
  if (isnan(h) || isnan(t)) {
    delay(500);
    Serial.println("Failed to read from DHT sensor!\n");
    return;
  }
  if (isnan(moisture)) {
    delay(500);
    Serial.println("Failed to read from Soil Moisture sensor!\n");
    return;
  }
  int Light_analogValue = analogRead(LIGHT_SENSOR_PIN);
  if (Light_analogValue > ANALOG_THREHOLD ){
    //digitalWrite(LED,HIGH);
      for( int i = 0; i < NUM_LEDS; ++i) {
        leds[i] = CRGB::Orange;
      }
      // FastLED.show();
      // delay(300);
      // for( int i = 0; i < NUM_LEDS; ++i) {
      //   leds[i] = CRGB::Black;
      // }
      FastLED.show();
  }  
  else{
      for( int i = 0; i < NUM_LEDS; ++i) {
        leds[i] = CRGB::Black;
      }
      FastLED.show();
  }
  Serial.print("\n");
  Serial.print("Air Humid: " + String(h) + "%");
  Serial.print("\t");
  Serial.print("Temp:" + String(t) + " C");
  Serial.print("\t");
  Serial.println("Lux:" + String(Light_analogValue));
  Serial.print("\t");
  Serial.println("Soil Moisture:" + String(moisture));
  if (moisture < soilWet) {
      Serial.println("Status: Soil is too wet");
  } else if (moisture >= soilWet && moisture < soilDry) {
      Serial.println("Status: Soil moisture is perfect");
  } else {
      Serial.println("Status: Soil is too dry - time to water!");
  }
  delay(750);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(5,10);
  display.print("Air Humid: " + String(h) + "%");
  display.setCursor(5,20);
  display.print("Temp:" + String(t) + " C");
  display.setCursor(5,30);
  display.print("Lux:" + String(Light_analogValue));
  display.setCursor(5,40);
  display.print("Soil Moisture:" + String(moisture));
  display.display();
}

// //  This function returns the analog soil moisture measurement
// int readSensor() {
//     digitalWrite(sensorPin, HIGH);    // Turn the sensor ON
//     delay(10);                // Allow power to settle
//     int val = analogRead(sensorPin);    // Read the analog value form sensor
//     digitalWrite(sensorPin, LOW);        // Turn the sensor OFF
//     return val;                // Return analog moisture value
// }