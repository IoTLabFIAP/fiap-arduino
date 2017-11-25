#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

#define FIREBASE_HOST "fiapiot-5888e.firebaseio.com"
#define FIREBASE_AUTH "CzS5ZbGRhyqoX6XTPcwTXIC5gjOpjGdRjN7cdOg4"
#define WIFI_SSID "labiotfiap"
#define WIFI_PASSWORD "telefonica"
int vermelhoPINO = 4;
int verdePINO = 13;
int amareloPINO = 5;
int presencaPINO = 16;
int contador =0;
int SensorDeLuminosidade = A0;
float valorpot;


int luminosidade = 0; //Armazena o valor lido do pino analogico do sensor de luminosidade
int intesidadeLux = 0;

void setup() {

  Serial.begin(9600);

 pinMode(SensorDeLuminosidade, INPUT);

  pinMode(vermelhoPINO, OUTPUT);
  pinMode(verdePINO, OUTPUT);
  pinMode(amareloPINO, OUTPUT);
  pinMode(presencaPINO, INPUT);
  
  digitalWrite(vermelhoPINO, LOW);
  digitalWrite(verdePINO, LOW);
  digitalWrite(amareloPINO, LOW);
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("conectando...");
   
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("conectado a rede: ");
  Serial.println(WiFi.localIP());
   
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  delay(1000);
  
  Firebase.set("ledVermelho", 0);
  
  if (Firebase.failed()) {
    Serial.print(">>> ERROR AO CRIAR LEDFIAP >>> ");
    Serial.println(Firebase.error());
  }
   Firebase.set("ledAmarelo", 0);
   Firebase.set("ledVerde", 0);
}

void loop() {
  
  int vermelho = Firebase.getInt("ledVermelho");
  if (Firebase.failed()) {
    Serial.print(">>> ERRO AO OBTER LED vermelho >>> ");
    Serial.println(Firebase.error());
  } else {
    digitalWrite(vermelhoPINO, vermelho);
    Serial.println(vermelho);
  }


  int amarelo = Firebase.getInt("ledAmarelo");
  if (Firebase.failed()) {
    Serial.print(">>> ERRO AO OBTER LED amarelo >>> ");
    Serial.println(Firebase.error());
  } else {
    digitalWrite(amareloPINO, amarelo);
    Serial.println(amarelo);
  }

    int verde = Firebase.getInt("ledVerde");
  if (Firebase.failed()) {
    Serial.print(">>> ERRO AO OBTER LED verde >>> ");
    Serial.println(Firebase.error());
  } else {
    digitalWrite(verdePINO, verde);
    Serial.println(verde);
  }

  
  //Le e armazena o valor do sensor de luminosidade
  luminosidade = analogRead(luminosidade);
  //Envia as informacoes para o serial monitor
  
  Serial.print(" - Valor analogico : ");
  Serial.println(luminosidade);
 
    // Le o valor - analogico - do LDR  
  valorpot = analogRead(SensorDeLuminosidade);  

  



     if (contador == 10)
     {
     int presenca = digitalRead(presencaPINO);  //bloco sensor de presença
     Firebase.push("presenca", presenca);
    
     // Converte o valor lido do LDR
  intesidadeLux = map(valorpot, 0, 1023, 0, 255); 
  Serial.print("Valor lido do LDR : ");   

  // Mostra o valor lido do LDR no monitor serial  
  Serial.print(luminosidade);  
  Serial.print(" = Luminosidade : ");  

  // Mostra o valor da luminosidade no monitor serial  
  Serial.println(intesidadeLux);

     Firebase.push("luminosidade", intesidadeLux);
     contador = 0;
     
     }



 
  
  delay(50);
  contador++; // contador para o sensor ler a cada 1 segundo
}
