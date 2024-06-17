#include <SoftwareSerial.h>
#define rxPin 12
#define txPin 13 
SoftwareSerial mySerial(rxPin, txPin);

void setup() {
  Serial.begin(115200);
  mySerial.begin(9600);
}

void loop() {
  float t, h;
  if(mySerial.available()>0){
    String data = mySerial.readStringUntil('\n');
    t = getValue(data,';',0).toFloat();
    h = getValue(data,';',1).toFloat();
    Serial.println("T="+String(t)+"\tH="+String(h));
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
