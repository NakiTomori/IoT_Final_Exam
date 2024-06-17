#include <SoftwareSerial.h>
#include <FastLED.h>
#define rxPin 8 
#define txPin 9 
SoftwareSerial mySerial(rxPin, txPin);

#define LED_PIN 6
#define NUM_LEDS    50
#define BRIGHTNESS  80
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

#define PUMP 7

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  mySerial.begin(115200);

  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);

  pinMode(PUMP, OUTPUT);
}

void loop() {
  float name, soil_moi_value, light_value;
  
  if(mySerial.available()>0){
    String data = mySerial.readStringUntil('\n');
    name = getValue(data,';',0).toFloat();
    if(name == 1){
      light_value = getValue(data,';',1).toFloat();
      if(light_value == 0){
        Serial.println("Name= Light Sensor; Light value="+String(light_value));
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
      else if(light_value == 1){
        Serial.println("Name= Light Sensor; Light value="+String(light_value));
        for( int i = 0; i < NUM_LEDS; ++i) {
          leds[i] = CRGB::Black;
        }
        FastLED.show();
      }

    }
    if(name == 0){
      soil_moi_value = getValue(data,';',1).toFloat();
      if(soil_moi_value == 2){
        Serial.println("Name= Soil Moisture; Soil_value="+String(soil_moi_value));
        PumpControl();
      }
    }
  }
}

String getValue(String data, char separator, int index){
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;
  for (int i = 0; i <= maxIndex && found <= index; i++) {
      if (data.charAt(i) == separator || i == maxIndex) {
          found++;
          strIndex[0] = strIndex[1] + 1;
          strIndex[1] = (i == maxIndex) ? i+1 : i;
      }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void PumpControl(){
  digitalWrite(PUMP, HIGH);
  delay(1000);
  digitalWrite(PUMP, LOW);
}