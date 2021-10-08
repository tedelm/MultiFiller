#include "PinChangeInt.h"

///// Define pins ////

//Calibrate - Time how long it takes to fill the can
#define CalibrateButton x //Atmega328p-pu pin xx

//Controller
#define BeerFiller1Button x //Atmega328p-pu pin xx - Only Filler #1
#define BeerFiller12Button x //Atmega328p-pu pin xx - Run both fillers
#define EmergencyShutDownButton x //Atmega328p-pu pin xx - Stop everything

//Filler 1
#define BeerValve1 x //Atmega328p-pu pin xx
#define GasValve1 x //Atmega328p-pu pin xx
//Filler 2
#define BeerValve2 x //Atmega328p-pu pin xx
#define GasValve2 x //Atmega328p-pu pin xx

//Define values
int CanFillUpTimeCalibrated = 0;
int CanFillUpTime = 48000; // 48 seconds 
int countButtonPress = 0;
int Co2PurgeBeforeDelay = 2500; //2,5 seconds
int Co2PurgeAfterCountLimit = 8; // on/off 8 times
int Co2PurgeAfterCount = 0; // Counter

void setup() {
  // put your setup code here, to run once:

  //CalibrateButton
  pinMode(CalibrateButton,INPUT);
  //Controller
  pinMode(BeerFiller1Button,INPUT);
  pinMode(BeerFiller12Button,INPUT);
  pinMode(EmergencyShutDownButton,INPUT);
  
  //Filler 1
  pinMode(GasButton1,INPUT);
  pinMode(BeerValve1,OUTPUT);
  pinMode(GasValve1,OUTPUT);
  //Filler 2
  pinMode(GasButton2,INPUT);
  pinMode(BeerValve2,OUTPUT);
  pinMode(GasValve2,OUTPUT);

  //INTERRUPTS
  //Calibrate Beer filling time, only use beerValve1
  attachPinChangeInterrupt(CalibrateButton, CalibrateButtonFunction, CHANGE);
  //Run beerfiller #1
  attachPinChangeInterrupt(BeerFiller1Button, BeerFiller1ButtonFunction, CHANGE);
  //Run both beerfillers
  attachPinChangeInterrupt(BeerFiller12Button, BeerFiller12ButtonFunction, CHANGE);
  //Emergency ShutDown
  attachPinChangeInterrupt(EmergencyShutDownButton, EmergencyShutDownButtonFunction, CHANGE);
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(10);
}

//Calibrate the time it takes to fill a can
void CalibrateButtonFunction(){
    while(digitalRead(CalibrateButton) == HIGH){
      //Start filling beer
      digitalWrite(BeerValve1,HIGH);
      delay(250);
      CanFillUpTimeCalibrated++
    }
    //Stop filling beer
    digitalWrite(BeerValve1,LOW);
    //Set new time
    CanFillUpTime = CanFillUpTimeCalibrated * 250; //eg 4 * 250 = 1000
}

//GasValve1 - Co2 Co2 Purge Before filling
void GasValve1Co2PurgeBefore(){
   digitalWrite(GasValve1,HIGH);
   delay(Co2PurgeBeforeDelay);
   digitalWrite(GasValve1,LOW);
}

//GasValve2 - Co2 Co2 Purge Before filling
void GasValve2Co2PurgeBefore(){
   digitalWrite(GasValve2,HIGH);
   delay(Co2PurgeBeforeDelay);
   digitalWrite(GasValve2,LOW);
}

//GasValves - Co2 Co2 Purge Before filling
void GasValvesCo2PurgeBefore(){
    digitalWrite(GasValve1,HIGH);
    digitalWrite(GasValve2,HIGH);
    delay(Co2PurgeBeforeDelay);
    digitalWrite(GasValve1,LOW);
    digitalWrite(GasValve2,LOW);
}

//BeerValve1 - Fill can
void BeerValve1FillCan(){
   digitalWrite(BeerValve1,HIGH);
   delay(CanFillUpTime);
   digitalWrite(BeerValve1,LOW);
}

//BeerValve2 - Fill can
void BeerValve2FillCan(){
   digitalWrite(BeerValve2,HIGH);
   delay(CanFillUpTime);
   digitalWrite(BeerValve2,LOW);
}

//BeerValve both - Fill cans
void BeerValveFillCans(){
   digitalWrite(BeerValve1,HIGH);
   digitalWrite(BeerValve2,HIGH);
   delay(CanFillUpTime);
   digitalWrite(BeerValve1,LOW);
   digitalWrite(BeerValve2,LOW);
}



//GasValve1 - Co2 Co2 Purge after filling
void GasValve1Co2PurgeAfter(){
  while(Co2PurgeAfterCount >= Co2PurgeAfterCountLimit){
   digitalWrite(GasValve1,HIGH);
   delay(25);
   digitalWrite(GasValve1,LOW);
   delay(25);
   Co2PurgeAfterCount++
  }
  Co2PurgeAfterCount = 0;
  //Safety
  digitalWrite(GasValve1,LOW);
}

//GasValve2 - Co2 Co2 Purge after filling
void GasValve2Co2PurgeAfter(){
  while(Co2PurgeAfterCount >= Co2PurgeAfterCountLimit){
   digitalWrite(GasValve2,HIGH);
   delay(25);
   digitalWrite(GasValve2,LOW);
   delay(25);
   Co2PurgeAfterCount++
  }
  Co2PurgeAfterCount = 0;
  //Safety
  digitalWrite(GasValve2,LOW);
}

//GasValves - Co2 Co2 Purge after filling
void GasValvesCo2PurgeAfter(){
  while(Co2PurgeAfterCount >= Co2PurgeAfterCountLimit){
    digitalWrite(GasValve1,HIGH);
   digitalWrite(GasValve2,HIGH);
   delay(25);
   digitalWrite(GasValve1,LOW);
   digitalWrite(GasValve2,LOW);
   delay(25);
   Co2PurgeAfterCount++
  }
  Co2PurgeAfterCount = 0;
  //Safety
  digitalWrite(GasValve1,LOW);
  digitalWrite(GasValve2,LOW);
}

//Softstop - Just stop everything to be sure
void SoftStopFunction(){
    digitalWrite(GasValve1,LOW);
    digitalWrite(GasValve2,LOW);
    digitalWrite(BeerValve1,LOW);
    digitalWrite(BeerValve2,LOW);
}

//Emergency Shutdown
void EmergencyShutDownButtonFunction(){
  while(digitalRead(EmergencyShutDownButton) == HIGH){
    digitalWrite(GasValve1,LOW);
    digitalWrite(GasValve2,LOW);
    digitalWrite(BeerValve1,LOW);
    digitalWrite(BeerValve2,LOW);
  }
}


//Run first beerfiller
void BeerFiller1ButtonFunction(){
  GasValve1Co2PurgeBefore();
  delay(25);
  BeerValve1FillCan();
  delay(25);
  GasValve1Co2PurgeAfter();
  delay(25);
  SoftStopFunction();
}

//Run both fillers
void BeerFiller12ButtonFunction(){
  GasValvesCo2PurgeBefore();
  delay(25);
  BeerValveFillCans();
  delay(25);
  GasValvesCo2PurgeAfter();
  delay(25);
  SoftStopFunction();
}
