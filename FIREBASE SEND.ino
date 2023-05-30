#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
const char* WIFI_SSID = "------------";
const char* WIFI_PASSWORD = "-----------";
const char* FIREBASE_HOST = "------------";
const char* FIREBASE_AUTH = "----------------";
FirebaseData firebaseData;
void setup() {
  Serial.begin(115200);
  delay(100);
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
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void loop() {

  
}
