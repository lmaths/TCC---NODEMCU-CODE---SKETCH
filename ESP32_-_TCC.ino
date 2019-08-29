
#include <Wire.h> //lib para conexao de portas i2c
#include <WiFi.h> //lib de wifi
#include "RTClib.h" //lib para modulo de relogio
#include <IOXhop_FirebaseESP32.h>
#include <LiquidCrystal_I2C.h>


// Set these to run example.
#define FIREBASE_HOST "nodemcu-8c8b9.firebaseio.com"
#define FIREBASE_AUTH "NCto0QZk7wkMgd0mdhZKTCHihHFNqEcxd1Ra1M6f"
#define WIFI_SSID "matheuslima"
#define WIFI_PASSWORD "M@theus123000"
#define TRIG_PIN 2
#define ECHO_PIN 15

RTC_DS3231 rtc;

int totalColumns = 16;
int totalRows = 2;

int horaAlimentacao[3], minutoAlimentacao[3], demosComida[3];




LiquidCrystal_I2C lcd(0x27, totalColumns, totalRows);

char daysOfTheWeek[7][12] = {"Domingo", "Segunda", "Terça", "Quarta", "Quinta", "Sexta", "Sábado"};





void setup() {

   horaAlimentacao[0] = 14; 
   minutoAlimentacao[0] = 27;
   demosComida [0] = 0;   

   horaAlimentacao[1] = 14;
   minutoAlimentacao[1]= 20;
   demosComida [1] = 0;
   horaAlimentacao[2] = 14;
   minutoAlimentacao[2] = 25;
    demosComida [2] = 0;

  
   Serial.begin(9600);
   lcd.init(); 
   lcd.backlight(); 
   

  delay(3000); 
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }

  
  // put your setup code here, to run once:
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");  
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");    
  Serial.println(WiFi.localIP());

delay (3000);


  Serial.print("iniciando firebase");
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
 
   demosComida[0] = 1;
    demosComida[1] = 1;
    demosComida[2] = 1;

}

void loop() {

  
  
      Firebase.setString("lampada", "Matheus");
   Firebase.setString("nomeCachorro", "Lara");
    DateTime now = rtc.now();

   int horaAtual = now.hour();
   int minutoAtual = now.minute();

   if(horaAtual == horaAlimentacao[0] && minutoAtual == minutoAlimentacao[0] && demosComida[0] == 0) {
    Serial.print("alimentou");
    demosComida[0] = 1;
    demosComida[1] = 1;
    demosComida[2] = 1;
   }
    
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(" (");
    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    Serial.print(") ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();


  if(horaAtual == 0 && minutoAtual == 0) { //reseta o verificador de alimentação a meia noite 
    for(byte i = 0; i <= demosComida[i]; i = i +1) {
      demosComida[i] = 0 ;

      Serial.print (demosComida[i]);
    }
  }
  


   
}
