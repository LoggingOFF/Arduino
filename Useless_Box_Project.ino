#include <Servo.h>
Servo myservo;

const int switchPin = 2;
const int servoPin = 9;
const int sensorPin = 3;
const int irritationFreq = 5000;
const int XmovPin = 6;
const int YmovPin = 7;
const int XmovPinreverse = 4;
const int YmovPinreverse = 5;

int pos = 0;
int switchState = 0;
int state = 0;
int irritationCounter = 0;
int proximity = 1;
int movDir = 0;
unsigned long previoustime = millis();

void setup() {
  pinMode(switchPin, INPUT);
  pinMode(sensorPin, INPUT);
  
  pinMode(XmovPin, OUTPUT);
  pinMode(YmovPin, OUTPUT);
  pinMode(XmovPinreverse, OUTPUT);
  pinMode(YmovPinreverse, OUTPUT);
  pinMode(servoPin, OUTPUT);
  
  myservo.attach(servoPin);
  Serial.begin(9600);
}

void check_update() {
  if (switchState == 1) {
    delay(500);
    for (pos = 70; pos <= 90; pos += 5){
      myservo.write(pos);
      delay(15);
      Serial.println(pos);
    }
    for (pos = pos; pos >= 0; pos -= 5) {
      myservo.write(pos);
      delay(15);
      Serial.println(pos);
    }
  }
}

void loop() {
  proximity = digitalRead(sensorPin);
  switchState = digitalRead(switchPin);
  Serial.println(irritationCounter);
  Serial.println("start");
  Serial.println(proximity);
  long timeSinceOn = millis() - previoustime;

  // if irritation is over 10, ir reads count as switch hits
  if (irritationCounter >= 10) {
    if(proximity == 0) {
      irritationCounter += 1;
      movDir += 1;
      if(movDir > 4) {
        movDir = 1;
      }
      switch(movDIr){
        case(1):
          digitalWrite(XmovPin, HIGH);
          Serial.println("moving X positive");
          break;
        case(2):
          digitalWrite(YmovPin, HIGH);
          Serial.println("moving Y positive");
          break;
        case(3):
          digitalWrite(XmovPin, HIGH);
          digitalWrite(XmovPinreverse, HIGH);
          Serial.println("moving X negative");
          break;
        case(4):
          digitalWrite(YmovPin, HIGH);
          digitalWrite(YmovPinreverse, HIGH);
          Serial.println("moving Y negative");
          break;
      }

      delay(500);
      digitalWrite(XmovPin, LOW);
      digitalWrite(YmovPin, LOW);
      digitalWrite(XmovPinreverse, LOW);
      digitalWrite(YmovPinreverse, LOW);
      Serial.println("movement done");
      
      Serial.println("Irritation");
      Serial.println(irritationCounter);
    
      while(proximity == 0) {
        pos = 90;
        myservo.write(pos);
        proximity = digitalRead(sensorPin);
        delay(500);
        Serial.println("Don't even think about it");
        previoustime = millis();
    }}
  }

  if (timeSinceOn > irritationFreq) {
    if(irritationCounter > 0) {
      irritationCounter -= 1;
    }
  }
  
  if   (switchState == 1) {
    previoustime = millis();
    
    if(timeSinceOn <= irritationFreq) {
      irritationCounter += 1;
      Serial.println("Irritation");
      Serial.println(irritationCounter);
    } 
    
    for (pos = pos; pos <= 90; pos += 5){
      myservo.write(pos);
      delay(15);
    }
    
    Serial.println("Switch hit");

    if (irritationCounter >= 5) {
      for (pos = pos; pos >= 50; pos -= 5){
        myservo.write(pos);
        delay(15);
      }
      Serial.println("irritated + mid position");
      delay(150);
    }
  }
  delay(250);
  
  if (switchState == 0){
    for (pos = pos; pos >= 0; pos -= 5) {
      myservo.write(pos);
      delay(15);
    }
    Serial.println("at rest");
  }
  delay(500);
}
