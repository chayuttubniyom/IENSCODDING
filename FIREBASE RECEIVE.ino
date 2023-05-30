#include <ESP8266WiFi.h>
#include <DHT.h>
#include <FirebaseESP8266.h>

// Replace with your WiFi credentials
const char* WIFI_SSID = "IENS-WIFI03";
const char* WIFI_PASSWORD = "123456789";

// Replace with your Firebase project credentials
const char* FIREBASE_HOST = "https://iens-kku-default-rtdb.asia-southeast1.firebasedatabase.app/";
const char* FIREBASE_AUTH = "Efd2VG3PelOaczyRK3sSwWoZ4nxEIzbawk2Or9gw";

// DHT22 sensor configuration
const int BUILTIN_LED_PIN = LED_BUILTIN;
FirebaseData firebaseData;
void setup() {
  Serial.begin(9600);
  delay(100);

  // Connect to Wi-Fi
  Serial.println();
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected to: ");
  Serial.println(WIFI_SSID);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Initialize Firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  // Configure built-in LED
  pinMode(BUILTIN_LED_PIN, OUTPUT);
  digitalWrite(BUILTIN_LED_PIN, HIGH); // Turn off the LED initially
}
void loop() {
  // Get temperature and humidity data from Firebase
  if (Firebase.getFloat(firebaseData, "/temperature") && Firebase.getFloat(firebaseData, "/humidity")) {
    Firebase.getFloat(firebaseData, "/temperature");
    float temperature = firebaseData.floatData(); // Read temperature
    Firebase.getFloat(firebaseData, "/humidity");
    float humidity = firebaseData.floatData(); // Read humidity
  // Wait before fetching data again
  delay(1000); // Update interval: 1 seconds
}