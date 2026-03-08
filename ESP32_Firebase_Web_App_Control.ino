
#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <Arduino_JSON.h>

// --------------------- USER CONFIG ---------------------
#define WIFI_SSID     "........."
#define WIFI_PASSWORD ".........."

#define API_KEY       "..........."
#define DATABASE_URL  "..........."
#define USER_EMAIL    "............"
#define USER_PASS     "............"
// --------------------------------------------------------

// Define Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// GPIO map
const int GPIO1 = 12;//Vert
const int GPIO2 = 13;//Orange
const int GPIO3 = 14;//Rouge

unsigned long sendDataPrevMillis = 0;

void setup() {
  // Init GPIO pins
  pinMode(GPIO1, OUTPUT); digitalWrite(GPIO1, LOW);
  pinMode(GPIO2, OUTPUT); digitalWrite(GPIO2, LOW);
  pinMode(GPIO3, OUTPUT); digitalWrite(GPIO3, LOW);

  Serial.begin(115200);
  delay(300);

  // Connect WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: "); Serial.println(WiFi.localIP());

  // Configure Firebase
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASS;

  Firebase.begin(&config, &auth);
  Firebase.reconnectNetwork(true);

  fbdo.setBSSLBufferSize(4096, 1024); // SSL buffer
  fbdo.setResponseSize(2048);

  Serial.println("Firebase initialized...");
}

void loop() {
  if (Firebase.ready() && (millis() - sendDataPrevMillis > 1000 || sendDataPrevMillis == 0)) {
    sendDataPrevMillis = millis();

    int value;

    // Read GPIO1
    if (Firebase.RTDB.getInt(&fbdo, "/gpio1", &value)) {
      digitalWrite(GPIO1, value ? HIGH : LOW);
    } else {
      Serial.println(fbdo.errorReason().c_str());
    }

    // Read GPIO2
    if (Firebase.RTDB.getInt(&fbdo, "/gpio2", &value)) {
      digitalWrite(GPIO2, value ? HIGH : LOW);
    } else {
      Serial.println(fbdo.errorReason().c_str());
    }

    // Read GPIO3
    if (Firebase.RTDB.getInt(&fbdo, "/gpio3", &value)) {
      digitalWrite(GPIO3, value ? HIGH : LOW);
    } else {
      Serial.println(fbdo.errorReason().c_str());
    }
  }

}
