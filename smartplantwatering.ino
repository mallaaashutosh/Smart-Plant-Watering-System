/******************** BLYNK DETAILS ********************/
#define BLYNK_TEMPLATE_ID "TMPL64qbJ_snS"
#define BLYNK_TEMPLATE_NAME "Smart Plant Watering"
#define BLYNK_AUTH_TOKEN "H0fi1i7uPHiptx2jC0ohSxP7DmJYYbNq"

#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>

/******************** WIFI DETAILS ********************/
char ssid[] = "Informatics College";
char pass[] = "INf0RM@TICs2025";

/******************** PIN DEFINITIONS ********************/
#define DHTPIN     4
#define DHTTYPE    DHT22
#define SOIL_PIN   34          // YL-38 AO
#define RELAY_PIN  5           // Relay IN

#define RELAY_ON   LOW         // Active LOW relay
#define RELAY_OFF  HIGH

/******************** OBJECTS ********************/
DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

/******************** VARIABLES ********************/
bool manualPump = false;
int soilThreshold = 40;        // % below this → pump ON

/******************** BLYNK BUTTON ********************/
BLYNK_WRITE(V0) {              // Button widget (Switch)
  manualPump = param.asInt();
  Serial.print("Manual Pump: ");
  Serial.println(manualPump ? "ON" : "OFF");
}

/******************** SENSOR FUNCTION ********************/
void readSensors() {

  // -------- SOIL MOISTURE --------
  int soilRaw = analogRead(SOIL_PIN);
  Serial.print("Raw Soil Sensor: ");
  Serial.println(soilRaw);

  // Calibrated mapping for YL-69 + ESP32
  // Air ≈ 3000 | Wet soil ≈ 900
  int soilPercent = map(soilRaw, 3000, 900, 0, 100);
  soilPercent = constrain(soilPercent, 0, 100);

  // -------- DHT22 --------
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println(" Failed to read DHT22");
    return;
  }

  // -------- SEND TO BLYNK --------
  Blynk.virtualWrite(V1, soilPercent);   // Soil %
  Blynk.virtualWrite(V2, temperature);   // Temperature °C
  Blynk.virtualWrite(V3, humidity);      // Humidity %

  // -------- PUMP CONTROL --------
  if (manualPump) {
    digitalWrite(RELAY_PIN, RELAY_ON);
    Serial.println("Pump: MANUAL ON");
  }
  else if (soilPercent < soilThreshold) {
    digitalWrite(RELAY_PIN, RELAY_ON);
    Serial.println("Pump: AUTO ON (Dry Soil)");
  }
  else {
    digitalWrite(RELAY_PIN, RELAY_OFF);
    Serial.println("Pump: OFF");
  }

  // -------- DEBUG --------
  Serial.print("Soil Moisture: ");
  Serial.print(soilPercent);
  Serial.println(" %");

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");
  Serial.println("-----------------------------");
}

/******************** SETUP ********************/
void setup() {
  Serial.begin(9600);

  // IMPORTANT FOR ESP32 ADC
  analogSetAttenuation(ADC_11db);

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, RELAY_OFF);

  dht.begin();

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  timer.setInterval(3000L, readSensors);

  Serial.println("Smart Plant Watering System Started");
}

/******************** LOOP ********************/
void loop() {
  Blynk.run();
  timer.run();
}
