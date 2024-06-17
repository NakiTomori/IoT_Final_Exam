#define LIGHT_SENSOR_PIN 26
#define LED 25
#define ANALOG_THREHOLD 500

void setup() {
  // put your setup code here, to run once:
  pinMode(LED, OUTPUT);
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  int analogValue = analogRead(LIGHT_SENSOR_PIN);
  Serial.println(analogValue);

  if (analogValue < ANALOG_THREHOLD ){
    //Serial.print(analogValue);
    digitalWrite(LED,HIGH);
  }  
  else
    digitalWrite(LED,LOW);
}
