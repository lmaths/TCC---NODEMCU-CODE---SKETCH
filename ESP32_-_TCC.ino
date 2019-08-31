#include <ArduinoJson.h>


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

 // passar a quantidade do firebase

int releMotor = 15;
int demosComida1, demosComida2;


LiquidCrystal_I2C lcd(0x27, totalColumns, totalRows);

char daysOfTheWeek[7][12] = {"Domingo", "Segunda", "Terça", "Quarta", "Quinta", "Sexta", "Sábado"};





void setup() {
  pinMode(releMotor, OUTPUT); 
  digitalWrite(releMotor, HIGH);

    demosComida1 = 0;
    demosComida2 = 0;
  
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
  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
   rtc.adjust(DateTime(2019, 8, 30, 17, 25                    , 0));
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
 

}

void loop() {

   
  
   DateTime now = rtc.now();

   int horaAtual = now.hour();
   int minutoAtual = now.minute();
   
    delay(3000);
 //int quantidadeAlimentacao = Firebase.getInt("qntAlimentar");

 int horaAlimentacao1 = Firebase.getInt("/horaAlimentacao");
 int minutoAlimentacao1 = Firebase.getInt("/minutoAlimentacao");
 int horaAlimentacao2 = Firebase.getInt("/horaAlimentacaoDois");
 int minutoAlimentacao2 = Firebase.getInt("/minutoAlimentacaoDois");

 Serial.println(horaAlimentacao1);
 Serial.println(minutoAlimentacao1);
 Serial.println(horaAlimentacao2);
 Serial.println(minutoAlimentacao2);
 delay (1000);

 //Serial.println(quantidadeAlimentacao);
  //int horaAlimentacao[quantidadeAlimentacao], minutoAlimentacao[quantidadeAlimentacao], demosComida[quantidadeAlimentacao];

 /*int i = 0;
  while(i < quantidadeAlimentacao) { //setar os horarios e alimentar
  int teste = Firebase.get("/horarios/");
   Serial.println(teste);
    
    horaAlimentacao[i] = 2;
    Serial.println(horaAlimentacao[i]);
    
    i = i + 1;
  } */
   
  
    if(horaAtual == horaAlimentacao1 && minutoAtual == minutoAlimentacao1 && demosComida1 == 0) {

      digitalWrite(releMotor, LOW);
      delay(20000);
      digitalWrite(releMotor, HIGH);

      demosComida1 = 1;
    Serial.print("alimentou");
  
   }

   if(horaAtual == horaAlimentacao2 && minutoAtual == minutoAlimentacao2 && demosComida2 == 0) {
      digitalWrite(releMotor, LOW);
      delay(20000);
      digitalWrite(releMotor, HIGH);

      demosComida2 = 1;

       Serial.print("alimentou 2");
   }

    if (demosComida1 == 0 || demosComida1 == 1 && demosComida2 == 1 ){
      lcd.clear();
     
      
      int Hora = (now.hour());
      int Min = (now.minute());
      String horario = "Hora: ";
      String fim = "";

     
      String prox = "Prox: ";
      String fimAlimentacao = prox + horaAlimentacao1 + ":" + minutoAlimentacao1;
      
        fim = (horario + Hora + ":" + Min);
        lcd.setCursor(0,0);
        lcd.print(fim);
        lcd.setCursor(0,1);
        lcd.print(fimAlimentacao);
      
  
     
   
  
  
    Serial.print("Horário atual: ");
   
    Serial.print("Próxima alimentação: ");
    Serial.print(horaAlimentacao1);
    Serial.print("h:");
    Serial.print(minutoAlimentacao1);
    Serial.println("min");
    Serial.println("  ");
  }

  if (demosComida1 == 1 && demosComida2 == 0){
    
    lcd.print("Alimentação: " + horaAlimentacao2 + minutoAlimentacao2);
    
    int Hora2 = (now.hour());
      int Min2 = (now.minute());
      String horario2 = "Hora: ";
      String fim2 = "";

     
      String prox2 = "Prox: ";
      String fimAlimentacao2 = prox2 + horaAlimentacao2 + ":" + minutoAlimentacao2;
      
        fim2 = (horario2 + Hora2 + ":" + Min2);
        lcd.setCursor(0,0);
        lcd.print(fim2);
        lcd.setCursor(0,1);
        lcd.print(fimAlimentacao2);
      
    
    Serial.print("Próxima alimentação: ");
    Serial.print(horaAlimentacao2);
    Serial.print("h:");
    Serial.print(minutoAlimentacao2);
    Serial.println("min");
    Serial.println("  ");
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


    


 /* if(horaAtual == 0 && minutoAtual == 0) { //reseta o verificador de alimentação a meia noite 
    for(byte i = 0; i <= demosComida[i]; i = i +1) {
      demosComida[i] = 0 ;

      Serial.print (demosComida[i]);
    }
  } */

  if (horaAtual == 0 && minutoAtual == 0){
    demosComida1 = 0;
    demosComida2 = 0; 
  }

    delay (1000);



  

 


   
}
