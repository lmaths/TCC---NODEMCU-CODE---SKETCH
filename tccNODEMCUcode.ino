

#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

// Set these to run example.
#define FIREBASE_HOST "nodemcu-8c8b9.firebaseio.com"
#define FIREBASE_AUTH "NCto0QZk7wkMgd0mdhZKTCHihHFNqEcxd1Ra1M6f"
#define WIFI_SSID "TP-LINK_6642"
#define WIFI_PASSWORD "40462929"
int lampada = 0;

void setup() {
  Serial.begin(9600);

  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  pinMode(16, OUTPUT);
  digitalWrite(16, 0);
  
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
 
   Firebase.setInt("lampada", 0);
   
  
}


void loop() {
lampada = Firebase.getInt("lampada");
Firebase.setInt("Usuarios", lampada);

digitalWrite(16, Firebase.getInt("Usuarios"));



  
  
}
