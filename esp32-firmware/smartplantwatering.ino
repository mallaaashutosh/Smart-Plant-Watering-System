/******************** WIFI DETAILS ********************/
char ssid[] = ""; //include your wifi ssid
char pass[] = ""; //include your wifi password

/******************** MQTT DETAILS ********************/
#include <WiFi.h>
#include <PubSubClient.h> 
const char* mqtt_server = "broker.mqttdashboard.com";
const int mqtt_port = 1883;

/******************** PIN DEFINITIONS ********************/
#define DHTPIN     4
#define DHTTYPE    DHT22
#define SOIL_PIN   34          // YL-38 AO
#define RELAY_PIN  18          // Relay IN1 (Channel 1)

/******************** RELAY LOGIC ********************/
#define RELAY_ON   LOW         // Active-LOW relay
#define RELAY_OFF  HIGH

/******************** OBJECTS ********************/
#include <DHT.h>
DHT dht(DHTPIN, DHTTYPE);
WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;

/******************** VARIABLES ********************/
bool autoPump   = false;
const int soilThreshold = 40;  // %
const int hysteresis    = 5;   // prevents relay chatter

/******************** SENSOR + CONTROL ********************/
void readSensors() {
  /* -------- Soil Moisture -------- */
  int soilRaw = analogRead(SOIL_PIN);
  int soilPercent = map(soilRaw, 3000, 900, 0, 100);
  soilPercent = constrain(soilPercent, 0, 100);

  /* -------- Temperature & Humidity -------- */
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println(" DHT22 read failed");
    return;
  }

  /* -------- AUTO LOGIC ONLY -------- */
  if (soilPercent < soilThreshold - hysteresis) {
    autoPump = true;
  }
  else if (soilPercent > soilThreshold + hysteresis) {
    autoPump = false;
  }

  /* -------- RELAY CONTROL -------- */
  bool pumpRunning = autoPump;
  digitalWrite(RELAY_PIN, pumpRunning ? RELAY_ON : RELAY_OFF);

  /* -------- MQTT PUBLISH -------- */
  client.publish("plant/soil", String(soilPercent).c_str());
  client.publish("plant/temp", String(temperature, 1).c_str());
  client.publish("plant/hum", String(humidity, 1).c_str());
  client.publish("plant/pump_state", pumpRunning ? "ON" : "OFF");

  /* -------- SERIAL DEBUG -------- */
  Serial.println("----------- STATUS -----------");
  Serial.print("Soil Raw       : "); Serial.println(soilRaw);
  Serial.print("Soil %         : "); Serial.println(soilPercent);
  Serial.print("Temp           : "); Serial.print(temperature); Serial.println(" °C");
  Serial.print("Humidity       : "); Serial.print(humidity); Serial.println(" %");
  Serial.print("Auto Pump      : "); Serial.println(autoPump ? "ON" : "OFF");
  Serial.print("Pump Running   : "); Serial.println(pumpRunning ? "ON" : "OFF");
  Serial.print("Relay Pin      : "); Serial.println(digitalRead(RELAY_PIN));
  Serial.println("-------------------------------\n");
}

/******************** RECONNECT ********************/
void reconnect() {
  String clientId = "PlantESP_" + String(ESP.getEfuseMac(), HEX);
  
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection as [");
    Serial.print(clientId);
    Serial.print("]... ");

    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" → retry in 5s");
      delay(5000);
    }
  }
}

/******************** SETUP ********************/
void setup() {
  Serial.begin(9600);
  analogSetAttenuation(ADC_11db);
  
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, RELAY_OFF);   // Start OFF - important!

  dht.begin();

  // Connect WiFi
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("WiFi connected");

  // Setup MQTT
  client.setServer(mqtt_server, mqtt_port);
  
  Serial.println("Smart Plant Watering System Started (Auto mode only)");
}

/******************** LOOP ********************/
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Run sensor reading every 3 seconds
  if (millis() - lastMsg > 3000) {
    lastMsg = millis();
    readSensors();
  }
}
