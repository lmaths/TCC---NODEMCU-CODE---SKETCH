

#include <ArduinoJson.h>


#include <Wire.h> //lib para conexao de portas i2c
#include <WiFi.h> //lib de wifi
#include "RTClib.h" //lib para modulo de relogio
#include <IOXhop_FirebaseESP32.h>
#include <LiquidCrystal_I2C.h>
#include <Ultrasonic.h>



// Set these to run example.
#define FIREBASE_HOST "nodemcu-8c8b9.firebaseio.com"
#define FIREBASE_AUTH "NCto0QZk7wkMgd0mdhZKTCHihHFNqEcxd1Ra1M6f"
#define WIFI_SSID "matheuslima"
#define WIFI_PASSWORD "M@theus123000"
#define TRIG_PIN 2
#define ECHO_PIN 15
const int portaLDR = 35;

RTC_DS3231 rtc;

int totalColumns = 16;
int totalRows = 2;
#define DOUT 14                                              // Pin connected to HX711 data output pin
#define CLK  12





 // passar a quantidade do firebase

int releMotor = 15;
int rele = 5;
int demosComida1, demosComida2;


LiquidCrystal_I2C lcd(0x27, totalColumns, totalRows);


char daysOfTheWeek[7][12] = {"Domingo", "Segunda", "Terça", "Quarta", "Quinta", "Sexta", "Sábado"};


int sensorPoteAgua = 2;
int sensorBaixo = 4;
int sensorCima = 33;
int leitura;



void encherPoteAgua(){

  boolean estado = digitalRead(sensorPoteAgua);
  Serial.println(estado);
  while(estado == false) {
    Serial.print("enchendo a agua");
    digitalWrite(releMotor, HIGH);
    estado = digitalRead(sensorPoteAgua);
  } 
    Serial.println("desliga relé");
     digitalWrite(releMotor, LOW);
  Firebase.set("poteAgua", estado);
  
}
  

void setup() {
 
  pinMode(releMotor, OUTPUT);
  pinMode(rele, OUTPUT);
  digitalWrite(rele, LOW); 
  digitalWrite(releMotor, LOW);
  pinMode(sensorPoteAgua, INPUT);
  pinMode(sensorBaixo, INPUT);
  pinMode(sensorCima, INPUT);
  pinMode(portaLDR, INPUT);
 
 

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

 
  
  int sensor = digitalRead(sensorBaixo);
  int segundoSensor = digitalRead(sensorCima);

  if (sensor == 0 && segundoSensor == 0) {
    Firebase.setInt("/reservatorio", 0);
  } else if (sensor == 1 && segundoSensor == 0) {
    Firebase.setInt("/reservatorio", 1);
  }else if(sensor == 1 && segundoSensor == 1) {
    Firebase.setInt("/reservatorio", 2);
  }

 
  
 

 if(horaAtual == horaAlimentacao1 && minutoAtual == minutoAlimentacao1 && demosComida1 == 0) {
  delay(2000);
     leitura = analogRead(portaLDR);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Verificando...");
   delay(5000);
           while(leitura > 3000) {
            lcd.setCursor(0,1);
            lcd.print("Alimentando :)");
            digitalWrite(rele, HIGH);
            delay(12000);
            digitalWrite(rele, LOW);
            delay(5000);
            leitura = analogRead(portaLDR);
           
           }
         lcd.setCursor(0,1);
         lcd.print("Pote cheio :(");
         digitalWrite(rele, LOW);
      demosComida1 = 1;

      delay(2000);
      lcd.clear();
   }

   if(horaAtual == horaAlimentacao2 && minutoAtual == minutoAlimentacao2 && demosComida2 == 0 && qtdAlimentacao == 1) {
      delay(2000);
       leitura = analogRead(portaLDR);
      Serial.println(leitura);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Verificando...");
       delay(5000);
      while(leitura > 3000) {
        lcd.setCursor(0,1);
        lcd.print("Alimentando :)");
        digitalWrite(rele, HIGH);
        delay(12000);
        digitalWrite(rele, LOW);
        delay(5000);
        leitura = analogRead(portaLDR);
 
      }
          lcd.setCursor(0,1);
          lcd.print("Pote cheio");
      digitalWrite(rele, LOW);
      demosComida2 = 1;
    delay(2000);
    lcd.clear();
 
        
   }

   if(demosComida1 == 0 || demosComida1 == 1 && qtdAlimentacao == 0) {
    
     
      
      int Hora = (now.hour());
      int Min = (now.minute());
      String horario = "Hora: ";
      String fim = "";

     
      String prox = "Prox: ";
      String fimAlimentacao = prox + horaAlimentacao1 + ":" + minutoAlimentacao1;
      
        fim = (horario + Hora + ":" + Min);
         lcd.clear();
        lcd.setCursor(0,0);
        lcd.print(fim);
        lcd.setCursor(0,1);
        lcd.print(fimAlimentacao);
      
  
   }

    if (demosComida1 == 0 || demosComida1 == 1 && demosComida2 == 1 ){
      
      int Hora = (now.hour());
      int Min = (now.minute());
      String horario = "Hora: ";
      String fim = "";

     
      String prox = "Prox: ";
      String fimAlimentacao = prox + horaAlimentacao1 + ":" + minutoAlimentacao1;
      
        fim = (horario + Hora + ":" + Min);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print(fim);
        lcd.setCursor(0,1);
        lcd.print(fimAlimentacao);
      
  }

  if (demosComida1 == 1 && demosComida2 == 0 && qtdAlimentacao == 1){
    lcd.clear();
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
