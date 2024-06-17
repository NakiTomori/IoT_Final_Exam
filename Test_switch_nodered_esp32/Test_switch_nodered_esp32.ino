#include <WiFi.h> 
#include <WiFiClient.h>
#include <PubSubClient.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <string.h>

#define rxPin 12
#define txPin 13 
SoftwareSerial mySerial(rxPin, txPin);

#define LIGHT_SENSOR_PIN 35
#define ANALOG_THREHOLD 500

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define DHTTYPE DHT11   // DHT 11
#define DHTPIN 27

#define soilWet 819   // Define max value we consider soil 'wet' ~ 80%
#define soilDry 2047   // Define min value we consider soil 'dry' ~ 50%
#define sensorPin 34

DHT dht(DHTPIN, DHTTYPE);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const char* ssid = "VJU Student";
const char* password = "studentVJU@2022";
const char* mqtt_server = "test.mosquitto.org";

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;

String Submqtt;

void setup() {
  // put your setup code here, to run once:
  pinMode(sensorPin, OUTPUT);

  Serial.begin(9600);
  mySerial.begin(115200);

  dht.begin();

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)){
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  display.clearDisplay();
}

void loop() {
  // put your main code here, to run repeatedly:
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;

    float t = dht.readTemperature();
    float h = dht.readHumidity();
    int Light_analogValue = analogRead(LIGHT_SENSOR_PIN);
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
    if (isnan(Light_analogValue)) {
      delay(500);
      Serial.println("Failed to read from Light sensor!\n");
      return;
    }
 
    String Light_status;
    if(Light_analogValue > ANALOG_THREHOLD ){
      client.subscribe("/TestTrongCay/LButton");
      float Sublight = Submqtt.toFloat();
      if(Sublight == 11){
        String light_data = String("1") + ";" + String("0");
        Serial.print(light_data);
        mySerial.println(light_data);
      }
      //digitalWrite(LED,HIGH);
      Light_status = String("Dark");
      Submqtt = "";
    }  
    else{
      client.subscribe("/TestTrongCay/LButton");
      float Sublight = Submqtt.toFloat();
      if(Sublight == 11){
        String light_data = String("1") + ";" + String("1");
        Serial.print(light_data);
        mySerial.println(light_data);
      }
      Light_status = String("Bright");
      Submqtt = "";
    }

    Serial.print("\n");
    Serial.print("Air Humid: " + String(h) + "%");
    Serial.print("\t");
    Serial.print("Temp:" + String(t) + " C");
    Serial.print("\t");
    Serial.println("Lux:" + String(Light_analogValue) + " " + Light_status);
    Serial.print("\t");
    Serial.println("Soil Moisture:" + String(100-(moisture/40.95)) + "%");

    String moi_status;
    if (moisture < soilWet) {
        Serial.println("Status: Soil is too wet");
        client.subscribe("/TestTrongCay/PButton");
        float Submoi = Submqtt.toFloat();
        if(Submoi == 1){
          String Moi_data = String("0") + ";" + String("0");
          Serial.println(Moi_data);
          mySerial.println(Moi_data);
        }
        moi_status = String("Wet");
        Submqtt = "";
    } else if (moisture >= soilWet && moisture < soilDry) {
        Serial.println("Status: Soil moisture is perfect");
        client.subscribe("/TestTrongCay/PButton");
        float Submoi = Submqtt.toFloat();
        if(Submoi == 1){
          String Moi_data = String("0") + ";" + String("1");
          Serial.println(Moi_data);
          mySerial.println(Moi_data);
        }
        moi_status = String("Perfect");
        Submqtt = "";
    } else {
        Serial.println("Status: Soil is too dry - time to water!");
        client.subscribe("/TestTrongCay/PButton");
        float Submoi = Submqtt.toFloat();
        if(Submoi == 1){
          String Moi_data = String("0") + ";" + String("2");
          Serial.println(Moi_data);
          mySerial.println(Moi_data);
        }
        moi_status = String("Dry");
        Submqtt = "";
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
    display.print("Lux:" + String(Light_analogValue) + " " + Light_status);
    display.setCursor(5,40);
    display.print("Soil Moisture:" + String(100-(moisture/40.95)) + "% " + moi_status);
    display.display();
 
    client.publish("/TestTrongCay/hum", String(h).c_str());
    client.publish("/TestTrongCay/temp", String(t).c_str()); 
    client.publish("/TestTrongCay/lux", String(Light_analogValue).c_str());
    client.publish("/TestTrongCay/lux_status", String(Light_status).c_str());
    client.publish("/TestTrongCay/soilmoi", String(100-(moisture/40.95)).c_str());
    client.publish("/TestTrongCay/soilmoi_status", String(moi_status).c_str());
  }
}

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Submqtt = Submqtt + (char)payload[i];
    Serial.print((char)payload[i]);
  }
}

void reconnect() {
 while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      Serial.println("Connected");
      client.publish("/TestTrongCay/Publish", "Welcome");
      client.subscribe("/TestTrongCay/Subscribe");
    }
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}