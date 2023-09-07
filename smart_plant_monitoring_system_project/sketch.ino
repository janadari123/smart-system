#include <DHT.h>
#include <WiFi.h>
#include <FirebaseESP32.h>

#define DHT_PIN 2
#define DHT_TYPE DHT22
#define LDR_PIN 32
#define POT_PIN 33

DHT dht(DHT_PIN, DHT_TYPE);

#define FIREBASE_HOST "https://myprojectnew-da28b-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "AIzaSyBh3t-_3EO6jO8kJ1bxLDDFa_rS88olN-E"
#define WIFI_SSID "Wokwi-GUEST"
#define WIFI_PASS ""

FirebaseData firebaseData;

void setup() {
  Serial.begin(115200);
  dht.begin();
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);  // Initialize Firebase with credentials
}

void loop() {
  // Read sensor data
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  int lightIntensity = analogRead(LDR_PIN);
  lightIntensity = 2048 - lightIntensity;
  int soilMoisture = analogRead(POT_PIN);
  int soilMoisturePercentage = map(soilMoisture, 0, 2048, 0, 100);

  // Set data in Firebase
  FirebaseJson json;
  json.set("temperature", temperature);
  json.set("humidity", humidity);
  json.set("lightIntensity", lightIntensity);
  json.set("soilMoisture", soilMoisturePercentage);

  if (Firebase.setJSON(firebaseData, "/sensor_data", json)) {
    Serial.println("Sensor data sent to Firebase");
  } else {
    Serial.println("Error sending sensor data to Firebase: " + firebaseData.errorReason());
  }

  delay(2000); 
}
