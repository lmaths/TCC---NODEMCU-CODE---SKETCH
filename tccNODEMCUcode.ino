#include <LiquidCrystal.h>
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

LiquidCrystal lcd(16,5,4,0,2,14);

// Set these to run example.
#define FIREBASE_HOST "nodemcu-8c8b9.firebaseio.com"
#define FIREBASE_AUTH "NCto0QZk7wkMgd0mdhZKTCHihHFNqEcxd1Ra1M6f"
#define WIFI_SSID "matheuslima"
#define WIFI_PASSWORD ""



void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);

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
  
  

  
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
 
   Firebase.setString("lampada", "Matheus");
   Firebase.setString("nomeCachorro", "Lara");
   
  
}


void loop() {

lcd.clear();
lcd.setCursor(3, 0);

String name = Firebase.getString("lampada");
String teste = Firebase.getString("nomeCachorro");

Serial.print(name);
lcd.print(name);

 lcd.setCursor(3, 1);
  lcd.print(teste);
  delay(5000);


  for (int posicao = 0; posicao < 3; posicao++)
  {
    lcd.scrollDisplayLeft();
    delay(300);
  }
   
  //Rolagem para a direita
  for (int posicao = 0; posicao < 6; posicao++)
  {
    lcd.scrollDisplayRight();
    delay(300);
  }




  
  
}
