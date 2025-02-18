// import libary dht11
#include "DHT.h"

const int ledPin = 14;
// pin sensor ping
const int triggerPin = 16;
const int echoPin = 5;
//pin dht11 sensor
#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

const int button = 2;

String state = "ping";

long duration;
int distance;

void setup(){
  Serial.begin(9600);

  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);


  pinMode(ledPin, OUTPUT);

  pinMode(button, INPUT_PULLUP);
  
  dht.begin();
}


void loop(){
  float suhu = dht.readTemperature();

  if (digitalRead(button) == 0){
    state = state == "ping" ? "suhu" : "ping";
    delay(200);
  }

  if (state == "ping"){
      // Serial.println(state);
      digitalWrite(triggerPin, LOW);
      delayMicroseconds(2);

      digitalWrite(triggerPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(triggerPin, LOW);

      duration = pulseIn(echoPin, HIGH);
      distance = duration * 0.034 / 2;
      delay(100);
        if (distance < 10){
          Serial.println(String("Jarak ") + String(distance) + String(" CM") + String(" Led ON"));
          digitalWrite(ledPin, HIGH); 

          delay(100);

          digitalWrite(ledPin, LOW); 

          delay(100); 
        }else{
          Serial.println(String("Jarak ") + String(distance) + String(" CM") + String(" Led OFF"));
        }
        digitalWrite(ledPin, LOW);
    }else{
      if(suhu > 35){
        Serial.println(String("Suhu: ") + String(suhu) + String(" C") + String(" Led ON"));
        digitalWrite(ledPin, HIGH); 

          delay(100); 

          digitalWrite(ledPin, LOW); 

          delay(100); 
      }else{
        Serial.println(String("Suhu: ") + String(suhu) + String(" C") + String(" Led OFF"));
      }
    }
}
