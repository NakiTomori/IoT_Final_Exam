#include <WiFi.h> 
#include <WiFiClient.h>
#include <PubSubClient.h>
#include <string.h>
#include <DHT.h>


#define DHTTYPE DHT11   // DHT 11
#define DHTPIN 33
DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "ASUS_60";
const char* password = "";
const char* mqtt_server = "test.mosquitto.org";

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;


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
     client.publish("/TestIOT/Publish", "Welcome");
     client.subscribe("/TestIOT/Subscribe");
   } else {
     Serial.print("failed, rc=");
     Serial.print(client.state());
     Serial.println(" try again in 5 seconds");
     delay(5000);
   }}
}

void setup() {    
 Serial.begin(9600);
 dht.begin();
 setup_wifi();
 client.setServer(mqtt_server, 1883);
 client.setCallback(callback);
}

void loop() {
 if (!client.connected()) {
   reconnect();
 }
 client.loop();


 unsigned long now = millis();
 if (now - lastMsg > 2000) { 
  lastMsg = now;

  String t = String(dht.readTemperature());
  client.publish("/TestIOT/temp", t.c_str()); 
  String h = String(dht.readHumidity());
  client.publish("/TestIOT/hum", h.c_str());
  

  Serial.print("\n");
  Serial.print("Humidity: " + String(h) + "%");
  Serial.print("\t");
  Serial.print("Temperature:" + String(t) + " C");
  
 }
}
