#include "DHT.h"

#define DHTPIN 4
#define DHTTYPE DHT11

// auth

#define BLYNK_TEMPLATE_ID ""
#define BLYNK_TEMPLATE_NAME ""
#define BLYNK_AUTH_TOKEN ""

#include <Blynk.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>


// wifi
#define BLYNK_PRINT Serial
// ssid pass UNTUK WIFI
char ssid[] = "";
char pass[] = "";


const int pinRelay = 5; // d1
const int dinginPin = 13; //d7
const int normalPin = 12; //d6
const int panasPin = 14; //d5


String stateButton = "no";

DHT dht(DHTPIN, DHTTYPE);
void setup() {
  // put your setup code here, to run once:
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  Serial.begin(9600);
  dht.begin();

  pinMode(pinRelay, OUTPUT);
  pinMode(dinginPin, OUTPUT);
  pinMode(normalPin, OUTPUT);
  pinMode(panasPin, OUTPUT);
}

BLYNK_WRITE(V1) {
  int state = param.asInt();
  // Serial.println("V1 = " + state);
  // Serial.print(state);
  Serial.println(state);
  digitalWrite(pinRelay, state);
  delay(200);
  stateButton = stateButton == "no" ? "yes" : "no";
  delay(200);
  }

void loop() {
  Blynk.run();
  float suhu = dht.readTemperature();
  float kelembapan = dht.readHumidity();
  if (isnan(suhu)) {
    Serial.println(F("Gagal membaca sensor SUHU!"));
    return;
  }
  delay(200);
  // Serial.println(String("Suhu: ") + String(suhu) + String(" | Kelembapan: ") + String(kelembapan));
  if(suhu > 37){
    if (stateButton == "no"){
      digitalWrite(pinRelay, LOW);
      Blynk.virtualWrite(V1, 0);
    }

    digitalWrite(panasPin, HIGH);
    digitalWrite(normalPin, LOW);
    digitalWrite(dinginPin, LOW);

    Blynk.virtualWrite(V4, 1);
    Blynk.virtualWrite(V3, 0);
    Blynk.virtualWrite(V2, 0);

    // Serial.println(stateButton);
    Serial.println("suhu panas");
  }else if (suhu < 29){
    if (stateButton == "no"){
      digitalWrite(pinRelay, HIGH);
      Blynk.virtualWrite(V1, 1);
    }

    digitalWrite(panasPin, LOW);
    digitalWrite(normalPin, LOW);
    digitalWrite(dinginPin, HIGH);

    Blynk.virtualWrite(V4, 0);
    Blynk.virtualWrite(V3, 0);
    Blynk.virtualWrite(V2, 1);

    // Serial.println(stateButton);
    Serial.println("suhu dingin");
  }else{
    if(stateButton == "no"){
      digitalWrite(pinRelay, HIGH);
      Blynk.virtualWrite(V1, 1);
    }

    digitalWrite(panasPin, LOW);
    digitalWrite(normalPin, HIGH);
    digitalWrite(dinginPin, LOW);

    Blynk.virtualWrite(V4, 0);
    Blynk.virtualWrite(V3, 1);
    Blynk.virtualWrite(V2, 0);

    // Serial.println(stateButton);
    Serial.println("suhu normal");
  }
  // mengirim data ke blynk
  Blynk.virtualWrite(V7, suhu);
  Blynk.virtualWrite(V6, kelembapan);
}
