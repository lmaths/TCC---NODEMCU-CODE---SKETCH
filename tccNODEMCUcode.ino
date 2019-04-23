//
// Copyright 2015 Google Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

// FirebaseDemo_ESP8266 is a sample that demo the different functions
// of the FirebaseArduino API.

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
