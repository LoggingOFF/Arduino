const int openPin = 13;
const int closePin = 12;
const int lockIndicator = 11;
const int outsideOpenButton = 10;
const int insideOpenButton = 9;
const int lockButton = 8;
int lockedDoor = 0;
int motionStopped = 0;
int lastDirection = 0;
int autoOpen = 0;


int states[4];


// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(openPin, OUTPUT);
  pinMode(closePin, OUTPUT);
  pinMode(lockIndicator, OUTPUT);
  pinMode(outsideOpenButton, INPUT);
  pinMode(insideOpenButton, INPUT);
  pinMode(lockButton, INPUT);
  Serial.begin(9600);

  digitalWrite(openPin, LOW);
  digitalWrite(closePin, LOW);
  /*delay(2000);
  digitalWrite(openPin, HIGH);
  delay(1000);
  digitalWrite(openPin, LOW);
  delay(100);
  digitalWrite(openPin, HIGH);
  delay(1000);
  digitalWrite(openPin, LOW);
  digitalWrite(closePin, HIGH);
  delay(4000);
  digitalWrite(closePin, LOW);*/
  digitalWrite(lockIndicator, HIGH);
  Serial.print("Boot complete.");

}

int lockDoor() {
  states[3]=1;
  Serial.print("\nDoor locked");
  digitalWrite(lockIndicator, LOW);
  delay(1000);
  return 0;
}

int unlockDoor(){
  states[3]=0;
  Serial.print("\nDoor unlocked");
  digitalWrite(lockIndicator, HIGH);
  autoOpen = 1;
  delay(1000);
  return 0;
}

int changeDirection(){
  if(lastDirection == 1) lastDirection = 0;
  else if(lastDirection == 0) lastDirection = 1;
  return lastDirection;  
}


int driveDoors(int driveDirection){
  int loopCounter = 0;
  if(driveDirection == 1 || autoOpen == 1){
      while(digitalRead(outsideOpenButton) == 1 || digitalRead(insideOpenButton) == 1){
        delay(50);
        Serial.print("\nOpening");
        digitalWrite(openPin, HIGH);
        if(loopCounter % 10 == 0){
        Serial.print("\nWaiting");
          digitalWrite(openPin, LOW);
          delay(1);
          digitalWrite(openPin, HIGH);
        }
        loopCounter++;
        if(loopCounter > 100) break;
      }
        autoOpen = 0;
        digitalWrite(openPin, LOW);
      motionStopped=1;
  }else if(driveDirection == 0){
      while(digitalRead(outsideOpenButton) == 1 || digitalRead(insideOpenButton) == 1){
        delay(50);
        Serial.print("\nClosing");
        digitalWrite(closePin, HIGH);
        loopCounter++;
        if(loopCounter > 120) break;
      }
        digitalWrite(closePin, LOW);
      motionStopped=1;
  }
  return 0;  
}


int readPins() {
  states[0] = digitalRead(outsideOpenButton);
  states[1] = digitalRead(insideOpenButton);
  states[2] = digitalRead(lockButton);
  return 0;
}

// the loop function runs over and over again forever
void loop() {
  delay(50);
  readPins();
  //Serial.print("\nStatus matrix:");
  for(int i=0;i<4;i++){
    //Serial.print(states[i]);
    //Serial.print(" ");
  }
  if(states[3] == 1){ 
    if(states[2] == 1){
      unlockDoor();    
    } else Serial.print("\n Door locked, doing nothing.");
  }else if(states[0] == 1 || states[1] == 1){
    Serial.print("\n Starting to drive the doors!");
    driveDoors(changeDirection());
    Serial.print("\n Stopped driving doors!");  
  } else if(states[2] == 1){
    if(states[3] == 1) unlockDoor();
    else if(states[3] == 0) lockDoor(); 
  }
}
