#include "EEPROM.h"
#include "PinChangeInt.h"

//Calibrate - Time how long it takes to fill the can
#define CalibrateButton 30 //Atmega328p-pu pin xx

//Controller
#define BeerFiller1Button 31 //Atmega328p-pu pin xx - Only Filler #1
#define BeerFiller12Button 32 //Atmega328p-pu pin xx - Run both fillers
#define EmergencyShutDownButton A10 //Atmega328p-pu interrupt pin xx - Stop everything
//Arduino Uno/Nano/Mini: All pins are usable
//Arduino Mega: 10, 11, 12, 13, 50, 51, 52, 53, A8 (62), A9 (63), A10 (64),A11 (65), A12 (66), A13 (67), A14 (68), A15 (69)

//Filler 1
#define BeerValve1 22 //Atmega328p-pu pin xx
#define GasValve1 23 //Atmega328p-pu pin xx
//Filler 2
#define BeerValve2 24 //Atmega328p-pu pin xx
#define GasValve2 25 //Atmega328p-pu pin xx

//Define values
int CanFillUpTimeCalibrated = 0;
int CanFillUpTime = 0; // Count loops (to be able to store 255 max loops), multiply with CountDelayValueMs
int Calibratedms = 0;
int CountDelayValueMs = 500; //should not be lower than 250
int countButtonPress = 0;
int Co2PurgeBeforeDelay = 2500; //2,5 seconds
int Co2PurgeAfterCountLimit = 8; // on/off 8 times
int Co2PurgeAfterCount = 0; // Counter
long buttonTimer = 0;
long longPressTime = 1000;
boolean buttonActive = false;
boolean longPressActive = false;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Startup...");

  //CalibrateButton
  pinMode(CalibrateButton,INPUT);
  //Controller
  pinMode(BeerFiller1Button,INPUT);
  pinMode(BeerFiller12Button,INPUT);
  pinMode(EmergencyShutDownButton,INPUT);
  
  //Filler 1
  pinMode(BeerValve1,OUTPUT);
  pinMode(GasValve1,OUTPUT);
  //Filler 2
  pinMode(BeerValve2,OUTPUT);
  pinMode(GasValve2,OUTPUT);

  Serial.println("Startup complete!");
  Serial.println("Waiting for input!");
  Serial.println("..."); 

  attachPinChangeInterrupt(EmergencyShutDownButton, EmergencyShutDownButtonFunction, CHANGE);
}

//EEPROM Write, writeEEPROM(0, "Hello Arduino");
void writeEEPROM(int addrOffset, const String &strToWrite){
  byte len = strToWrite.length();
  EEPROM.update(addrOffset, len);
  for (int i = 0; i < len; i++){
    EEPROM.update(addrOffset + 1 + i, strToWrite[i]);
  }
}

//EEPROM Read, readEEPROM(0);
String readEEPROM(int addrOffset){
  int newStrLen = EEPROM.read(addrOffset);
  char data[newStrLen + 1];

  for (int i = 0; i < newStrLen; i++){
    data[i] = EEPROM.read(addrOffset + 1 + i);
  }
  data[newStrLen] = '\ 0'; // !!! NOTE !!! Remove the space between the slash "/" and "0" (I've added a space because otherwise there is a display bug)

  //Fix trailing scrap data
  String EEPROMData = String(data);
  int StringIndex = EEPROMData.length() - 1;
  EEPROMData.remove(StringIndex, 1); 
  return String(EEPROMData);
}

void loop() {
  // put your main code here, to run repeatedly:

    //Check if calibration button is pressed short or long
    if (digitalRead(CalibrateButton) == HIGH) {
      if (buttonActive == false) {
        buttonActive = true;
        buttonTimer = millis();
      }
      if ((millis() - buttonTimer > longPressTime) && (longPressActive == false)) {
        longPressActive = true;
        Serial.println("CalibrateButton pressed - Long");
        CalibrateButtonFunction();
        delay(250);        
      }else{
          Serial.println("CalibrateButton pressed - Short");
          CalibrateButtonPressFunction();
          delay(250);
      }
    } else {
      if (buttonActive == true) {
        if (longPressActive == true) {
          longPressActive = false;
        }
        buttonActive = false;
      }
    }  

  // BeerFiller1Button
  if (digitalRead(BeerFiller1Button) == HIGH) {
    Serial.println("BeerFiller1Button pressed");
    BeerFiller1ButtonFunction();
    delay(250);
  }

  // BeerFiller12Button
  if (digitalRead(BeerFiller12Button) == HIGH) {
    Serial.println("BeerFiller12Button pressed");
    BeerFiller12ButtonFunction();
    delay(25);
  }

  delay(100);
}

//Calibrate the time it takes to fill a can
void CalibrateButtonFunction(){
    int CanFillUpTimeCalibrated = 0;
    Serial.println("Calibrating");
    while(digitalRead(CalibrateButton) == HIGH){
      Serial.print(".");
      //Start filling beer
      digitalWrite(BeerValve1,HIGH);
      delay(CountDelayValueMs);
      CanFillUpTimeCalibrated++;
    }
    //Stop filling beer
    digitalWrite(BeerValve1,LOW);
    
    //Set new time
    //int CanFillUpTime = CanFillUpTimeCalibrated; //CanFillUpTimeCalibrated * 500 = 0,5 second (1 * 0,5)
    int StrCanFillUpTimeMS = CountDelayValueMs * CanFillUpTimeCalibrated;
    Serial.println(" ");
    Serial.println("Saving to EEPROM... (Value in ms):" + String(StrCanFillUpTimeMS));

    //Write to EEPROM
    writeEEPROM(0, String(StrCanFillUpTimeMS));
    //EEPROM.update(0, CanFillUpTime);
    Serial.println("Saved");

    String FetchedStrCanFillUpTimeMS = readEEPROM(0);
    Serial.println("Read from EEPROM: " + String(FetchedStrCanFillUpTimeMS));
    
}

//Calibrate the time it takes to fill a can
void CalibrateButtonPressFunction(){
    String FetchedStrCanFillUpTimeMS = readEEPROM(0);
    int CanFillUpTime = FetchedStrCanFillUpTimeMS.toInt() + 500; //Add 0,5 seconds per press
    //Write to EEPROM
    writeEEPROM(0, String(CanFillUpTime));
    Serial.println("Saved (ms):" + String(CanFillUpTime));   
}

//GasValve1 - Co2 Co2 Purge Before filling
void GasValve1Co2PurgeBefore(){
  Serial.println("GasValve1Co2PurgeBefore");
   digitalWrite(GasValve1,HIGH);
   delay(Co2PurgeBeforeDelay);
   digitalWrite(GasValve1,LOW);
}

//GasValve2 - Co2 Co2 Purge Before filling
void GasValve2Co2PurgeBefore(){
  Serial.println("GasValve2Co2PurgeBefore");
   digitalWrite(GasValve2,HIGH);
   delay(Co2PurgeBeforeDelay);
   digitalWrite(GasValve2,LOW);
}

//GasValves - Co2 Co2 Purge Before filling
void GasValvesCo2PurgeBefore(){
  Serial.println("GasValvesCo2PurgeBefore");
    digitalWrite(GasValve1,HIGH);
    digitalWrite(GasValve2,HIGH);
    delay(Co2PurgeBeforeDelay);
    digitalWrite(GasValve1,LOW);
    digitalWrite(GasValve2,LOW);
}

//BeerValve1 - Fill can
void BeerValve1FillCan(){

    //Read from EEPROM
    String FetchedStrCanFillUpTimeMS = readEEPROM(0);
    int CanFillUpTime = FetchedStrCanFillUpTimeMS.toInt();
    Serial.println("Read from EEPROM: " + String(CanFillUpTime));
    //int CanFillUpTime = EEPROM.read(0);
    Serial.println("Start - Beer Valve 1 Fill Can (CountDelayValueMs * " + String(CanFillUpTime) +")");
    digitalWrite(BeerValve1,HIGH);
   
   delay(CanFillUpTime);
   digitalWrite(BeerValve1,LOW);
   Serial.println("End - Beer Valve 1 Fill Can (ms): " + String(CanFillUpTime));
}

//BeerValve2 - Fill can
void BeerValve2FillCan(){
  Serial.println("BeerValve2FillCan");
    //Read from EEPROM
    String FetchedStrCanFillUpTimeMS = readEEPROM(0);
    int CanFillUpTime = FetchedStrCanFillUpTimeMS.toInt();
    Serial.println("Read from EEPROM: " + String(CanFillUpTime));
    //int CanFillUpTime = EEPROM.read(0);
    Serial.println("Start - Beer Valve 1 Fill Can (CountDelayValueMs * " + String(CanFillUpTime) +")");
    digitalWrite(BeerValve2,HIGH);
   
   delay(CanFillUpTime);
   digitalWrite(BeerValve2,LOW);
   Serial.println("End - Beer Valve 1 Fill Can (ms): " + String(CanFillUpTime));

}

//BeerValve both - Fill cans
void BeerValveFillCans(){
    //Read from EEPROM
    String FetchedStrCanFillUpTimeMS = readEEPROM(0);
    int CanFillUpTime = FetchedStrCanFillUpTimeMS.toInt();
    Serial.println("Read from EEPROM: " + String(CanFillUpTime));
    //int CanFillUpTime = EEPROM.read(0);
    Serial.println("Start - Beer Valve 1 Fill Can (CountDelayValueMs * " + String(CanFillUpTime) +")");
    digitalWrite(BeerValve1,HIGH);
    digitalWrite(BeerValve2,HIGH);
  
   delay(CanFillUpTime);
   digitalWrite(BeerValve1,LOW);
   digitalWrite(BeerValve2,LOW);
   Serial.println("End - Beer Valve 1 Fill Can (ms): " + String(CanFillUpTime));
   
}

//GasValve1 - Co2 Co2 Purge after filling
void GasValve1Co2PurgeAfter(){
  Serial.println("GasValve1Co2PurgeAfter");
  while(Co2PurgeAfterCount <= Co2PurgeAfterCountLimit){
   Serial.print("+");
   digitalWrite(GasValve1,HIGH);
   delay(25);
   digitalWrite(GasValve1,LOW);
   delay(25);
   Co2PurgeAfterCount++;
  }
  Serial.println("+");
  Co2PurgeAfterCount = 0;
  //Safety
  digitalWrite(GasValve1,LOW);
}

//GasValve2 - Co2 Co2 Purge after filling
void GasValve2Co2PurgeAfter(){
  Serial.println("GasValve2Co2PurgeAfter");
  while(Co2PurgeAfterCount <= Co2PurgeAfterCountLimit){
    Serial.print("+");
   digitalWrite(GasValve2,HIGH);
   delay(25);
   digitalWrite(GasValve2,LOW);
   delay(25);
   Co2PurgeAfterCount++;
  }
  Serial.println("+");
  Co2PurgeAfterCount = 0;
  //Safety
  digitalWrite(GasValve2,LOW);
}

//GasValves - Co2 Co2 Purge after filling
void GasValvesCo2PurgeAfter(){
  Serial.println("GasValvesCo2PurgeAfter");
  while(Co2PurgeAfterCount <= Co2PurgeAfterCountLimit){
    Serial.print("+");
    digitalWrite(GasValve1,HIGH);
   digitalWrite(GasValve2,HIGH);
   delay(25);
   digitalWrite(GasValve1,LOW);
   digitalWrite(GasValve2,LOW);
   delay(25);
   Co2PurgeAfterCount++;
   }
   Serial.println("+");
  Co2PurgeAfterCount = 0;
  //Safety
  digitalWrite(GasValve1,LOW);
  digitalWrite(GasValve2,LOW);
}

//Softstop - Just stop everything to be sure
void SoftStopFunction(){
  Serial.println("SoftStopFunction");
    digitalWrite(GasValve1,LOW);
    digitalWrite(GasValve2,LOW);
    digitalWrite(BeerValve1,LOW);
    digitalWrite(BeerValve2,LOW);
}

//Emergency Shutdown
void EmergencyShutDownButtonFunction(){
  Serial.println("EmergencyShutDownButtonFunction");
  while(digitalRead(EmergencyShutDownButton) == HIGH){
    Serial.print("+");
    digitalWrite(GasValve1,LOW);
    digitalWrite(GasValve2,LOW);
    digitalWrite(BeerValve1,LOW);
    digitalWrite(BeerValve2,LOW);
    delay(100);
  }
    digitalWrite(GasValve1,LOW);
    digitalWrite(GasValve2,LOW);
    digitalWrite(BeerValve1,LOW);
    digitalWrite(BeerValve2,LOW);
    Serial.println("Going into softstop");
    SoftStopFunction();
}


//Run first beerfiller
void BeerFiller1ButtonFunction(){
  Serial.println("BeerFiller 1 Button Function");
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
  Serial.println("BeerFiller 1-2 Button Function");
  GasValvesCo2PurgeBefore();
  delay(25);
  BeerValveFillCans();
  delay(25);
  GasValvesCo2PurgeAfter();
  delay(25);
  SoftStopFunction();
}
