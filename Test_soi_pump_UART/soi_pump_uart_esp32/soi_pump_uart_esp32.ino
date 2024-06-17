#include <SoftwareSerial.h>
#define rxPin 12
#define txPin 13 
SoftwareSerial mySerial(rxPin, txPin);

/* Change these values based on your calibration values */
#define soilWet 500   // Define max value we consider soil 'wet'
#define soilDry 750   // Define min value we consider soil 'dry'
// Sensor pins
#define sensorPin 34

void setup() {
    pinMode(sensorPin, OUTPUT);
    Serial.begin(9600);
    mySerial.begin(115200);
}
void loop() {
    //get the reading from the function below and print it
    int moisture = analogRead(sensorPin);
    Serial.print("Analog Output: ");
    Serial.println(moisture);
    // Determine status of our soil
    if (moisture < soilWet) {
        Serial.println("Status: Soil is too wet");
        String Moi_data = String("0") + ";" + String("0");
        mySerial.println(Moi_data);
    } else if (moisture >= soilWet && moisture < soilDry) {
        Serial.println("Status: Soil moisture is perfect");
        String Moi_data = String("0") + ";" + String("1");
        mySerial.println(Moi_data);
    } else {
        Serial.println("Status: Soil is too dry - time to water!");
        String Moi_data = String("0") + ";" + String("2");
        mySerial.println(Moi_data);
    }
    
    delay(1000);    // Take a reading every second for testing
            // Normally you should take reading perhaps once or twice a day
    Serial.println();
}
//  This function returns the analog soil moisture measurement
int readSensor() {
    digitalWrite(sensorPin, HIGH);    // Turn the sensor ON
    delay(10);                // Allow power to settle
    int val = analogRead(sensorPin);    // Read the analog value form sensor
    digitalWrite(sensorPin, LOW);        // Turn the sensor OFF
    return val;                // Return analog moisture value
}