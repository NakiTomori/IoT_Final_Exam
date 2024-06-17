#include <SoftwareSerial.h>
#define rxPin 8 
#define txPin 9 
SoftwareSerial mySerial(rxPin, txPin);

#define PUMP_LOW 7
#define PUMP_HIGH 6

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  mySerial.begin(115200);
  pinMode(PUMP_LOW, OUTPUT);
  pinMode(PUMP_HIGH, OUTPUT);

}

void loop() {
  float name, soil_moi_value;
  if(mySerial.available()>0){
    String data = mySerial.readStringUntil('\n');
    name = getValue(data,';',0).toFloat();
    if(name == 0){
      soil_moi_value = getValue(data,';',1).toFloat();
      if(soil_moi_value == 2){
        Serial.println("Name= Soil Moisture; Soil_value="+String(soil_moi_value));
        PumpControl();
      }
    }
  }
}

void PumpControl(){
  digitalWrite(PUMP_LOW, LOW);
  digitalWrite(PUMP_HIGH, HIGH);
  delay(10000);
  digitalWrite(PUMP_LOW, LOW);
  digitalWrite(PUMP_HIGH, LOW);
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