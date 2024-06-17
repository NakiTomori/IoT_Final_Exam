/* Change these values based on your calibration values */
#define soilWet 500   // Define max value we consider soil 'wet'
#define soilDry 750   // Define min value we consider soil 'dry'
// Sensor pins
#define sensorPin 34
void setup() {
    pinMode(sensorPin, OUTPUT);
    Serial.begin(9600);
}
void loop() {
    //get the reading from the function below and print it
    int moisture = analogRead(sensorPin);
    Serial.print("Analog Output: ");
    Serial.println(moisture);
    // Determine status of our soil
    if (moisture < soilWet) {
        Serial.println("Status: Soil is too wet");
    } else if (moisture >= soilWet && moisture < soilDry) {
        Serial.println("Status: Soil moisture is perfect");
    } else {
        Serial.println("Status: Soil is too dry - time to water!");
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