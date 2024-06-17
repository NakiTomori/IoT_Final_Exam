#include <SoftwareSerial.h>
#define rxPin 8 
#define txPin 9 
SoftwareSerial mySerial(rxPin, txPin);

void setup() {
  Serial.begin(115200);
  mySerial.begin(9600);
}

void loop() {
  float t = random(30.0,38.0);
  float h = random(40, 90);
  String Data = String(t) + ";" + String(h);
  mySerial.println(Data);
  Serial.println(Data);
  delay(1000);
}
