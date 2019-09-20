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


int sensorPoteAgua = 2;
int sensorReservatorioUm = 4;
int sensorReservatorioDois = 5;



void encherPoteAgua(){

  boolean estado = digitalRead(sensorPoteAgua);
  Serial.println("Estado Sensor");
  Serial.println(estado);

  if(estado == false) {
    
   digitalWrite(releMotor, LOW);
   delay(3000);
    digitalWrite(releMotor, HIGH);
  

  } else {
     digitalWrite(releMotor, HIGH);
  }
 
 

  Firebase.set("/estadoPote", estado);

  
  
}
  

void setup() {
  pinMode(releMotor, OUTPUT); 
  digitalWrite(releMotor, HIGH);
  pinMode(sensorPoteAgua, INPUT);
  pinMode(sensorReservatorioUm, INPUT);
   pinMode(sensorReservatorioDois, INPUT);
  

    demosComida1 = 0;
    demosComida2 = 0;
  
   Serial.begin(9600);
   lcd.init(); 
   lcd.backlight(); 
   

 
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
    // following line sets the RTC to the date & time this sketch was compiled
  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
  // rtc.adjust(DateTime(2019, 9, 01, 10, 27, 0));
  }

  
  // put your setup code here, to run once:
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");  
    delay(500);
  }


  Serial.print("iniciando firebase");
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
 

}

void loop() {

   encherPoteAgua();

  
   DateTime now = rtc.now();

   int horaAtual = now.hour();
   int minutoAtual = now.minute();
   

 int horaAlimentacao1 = Firebase.getInt("/horaAlimentacao1");
 int minutoAlimentacao1 = Firebase.getInt("/minAlimentacao");
 int horaAlimentacao2 = Firebase.getInt("/horaAlimentacao2");
 int minutoAlimentacao2 = Firebase.getInt("/minAlimentacao2");
 bool qtdAlimentacao = Firebase.getBool("/qtdAlim");

 
  int estadoSensorBaixo = digitalRead(sensorReservatorioUm);
  int estadoSensorCima = digitalRead(sensorReservatorioDois);
  
  if(estadoSensorBaixo == 0 && estadoSensorCima == 1) {
    Firebase.setInt("/reservatorio", 0);
  } else if (estadoSensorBaixo == 1 && estadoSensorCima == 1) {
    Firebase.setInt("/reservatorio", 1);
  } else if (estadoSensorBaixo == 1 && estadoSensorCima == 0) {
    Firebase.setInt("/reservatorio", 2);
  }

  Serial.println(estadoSensorBaixo);
   Serial.println(estadoSensorCima);
 
 

 if(horaAtual == horaAlimentacao1 && minutoAtual == minutoAlimentacao1 && demosComida1 == 0) {
     /* digitalWrite(releMotor, LOW);
      delay(100000);
      digitalWrite(releMotor, HIGH); 
      
      passar outro rele*/

      demosComida1 = 1;
    Serial.print("alimentou");
  
   }

   if(horaAtual == horaAlimentacao2 && minutoAtual == minutoAlimentacao2 && demosComida2 == 0 && qtdAlimentacao == 1) {
      /* digitalWrite(releMotor, LOW);
      delay(100000);
      digitalWrite(releMotor, HIGH);

      demosComida2 = 1;

       Serial.print("alimentou 2");
 
        passar outro rele*/
   }

   if(demosComida1 == 0 || demosComida1 == 1 && qtdAlimentacao == 0) {
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
      
  }

  if (demosComida1 == 1 && demosComida2 == 0 && qtdAlimentacao == 1){
    
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
      
  }


  if (horaAtual == 0 && minutoAtual == 0){
    demosComida1 = 0;
    demosComida2 = 0; 
  }

  



  

 


   
}
