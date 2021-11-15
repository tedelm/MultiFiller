#include <EEPROM.h>
#include "PinChangeInt.h"
#include <Wire.h>
#include <Adafruit_GFX.h> // Include Adafruit Graphics
#include <Adafruit_SSD1306.h> // Include Adafruit OLED libraries

//OLED Screen 128x64 I2C
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//Calibrate - Time how long it takes to fill the can
#define CalibrateButton 15 //Atmega328p-pu pin xx

//Controller
#define BeerFiller1Button 14 //Atmega328p-pu pin xx - Only Filler #1 
#define BeerFiller12Button 13 //Atmega328p-pu pin xx - Run both fillers
#define EmergencyShutDownButton 2 //Atmega328p-pu interrupt pin xx - Stop everything
//Arduino Nano Interrupt pin D2,D3
//Arduino Mega: 10, 11, 12, 13, 50, 51, 52, 53, A8 (62), A9 (63), A10 (64),A11 (65), A12 (66), A13 (67), A14 (68), A15 (69)

//Filler 1
#define BeerValve1 12 //Atmega328p-pu pin xx
#define GasValve1 11 //Atmega328p-pu pin xx
//Filler 2
#define BeerValve2 10 //Atmega328p-pu pin xx
#define GasValve2 9 //Atmega328p-pu pin xx

//Define values
float mfVersion = 0.24; // Firmware Version
int BothCanFillupComp = 1.20; // Fillup compensation for pressure loss 1.20 = 20% extra
int CanFillUpTimeCalibrated = 0;
int CanFillUpTime = 0; // Count loops (to be able to store 255 max loops), multiply with CountDelayValueMs
int Calibratedms = 0;
int CountDelayValueMs = 500; //should not be lower than 250
float pressedSeconds = millis();
int countButtonPress = 0;
int Co2PurgeBeforeDelay = 2500; //2,5 seconds
int Co2PurgeAfterCountLimit = 8; // on/off 8 times
int Co2PurgeAfterCount = 0; // Count
long buttonTimer = 0;
long longPressTime = 1000;
boolean buttonActive = false;
boolean longPressActive = false;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Startup...");
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }  
  delay(2000); // Pause for 2 seconds
  display.display();
  display.clearDisplay();

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
 
  //attachPinChangeInterrupt(EmergencyShutDownButton, EmergencyShutDownButtonFunction, CHANGE);
}

//OLED
void writeOLEDMain(){
  // Clear the display
  display.clearDisplay();
  //Set the font size
  display.setTextSize(3);
  //Set the color - always use white despite actual display color
  display.setTextColor(WHITE);
  //Set the cursor coordinates
  display.setCursor(0,0);
  display.print("Multi-");
  display.setCursor(0,20);
  display.print("filler");  
  display.setCursor(0,50); 
  display.setTextSize(1);
  display.print("Version 1.3 !!!");
  display.display(); 
  delay(2000);  
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
  writeOLEDMain();

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
    //write2LCD(0,0,"Multifiller " + String(mfVersion),0,1,"Calibrating...");

    unsigned long TimeRightNow = millis();
    
    while(digitalRead(CalibrateButton) == HIGH){
      Serial.print(".");
      //Start filling beer
      digitalWrite(BeerValve1,HIGH);
      pressedSeconds = (millis() - TimeRightNow) / 1000;
      Serial.print(String(pressedSeconds));

      //write2LCD(0,0,"Multifiller " + String(mfVersion),0,1,"Cal (s):" + String(pressedSeconds));
      delay(25);

    }
    //Stop filling beer
    digitalWrite(BeerValve1,LOW);
    
    Serial.println(" ");
    Serial.println("Saving to EEPROM... (Value in s):" + String(pressedSeconds));

    //Write to EEPROM
    writeEEPROM(0, String(pressedSeconds));
    //EEPROM.update(0, CanFillUpTime);
    Serial.println("Saved");

    String FetchedStrCanFillUpTimeMS = readEEPROM(0);
    Serial.println("Read from EEPROM: " + String(FetchedStrCanFillUpTimeMS));

    //write2LCD(0,0,"Multifiller " + String(mfVersion),0,1,"Cal (s):" + String(FetchedStrCanFillUpTimeMS));
}

//Calibrate the time it takes to fill a can
void CalibrateButtonPressFunction(){
    String FetchedStrCanFillUpTimeMS = readEEPROM(0);
    int CanFillUpTime = FetchedStrCanFillUpTimeMS.toInt() + 1; //Add 1 seconds per press
    //Write to EEPROM
    writeEEPROM(0, String(CanFillUpTime));
    Serial.println("Saved (s):" + String(CanFillUpTime)); 
    ////write2LCD(0,1, "Cal (s):" + String(CanFillUpTime));  
    //write2LCD(0,0,"Multifiller " + String(mfVersion),0,1,"Cal (s):" + String(CanFillUpTime));
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
    int CanFillUpTime = FetchedStrCanFillUpTimeMS.toInt() * 1000;
    Serial.println("Read from EEPROM: " + String(CanFillUpTime));
    //int CanFillUpTime = EEPROM.read(0);
    Serial.println("Start - Beer Valve 1 Fill Can (CountDelayValueMs * " + String(CanFillUpTime) +")");
    //write2LCD(0,0,"Filling up",0,1,"One Can (s):"+ String(FetchedStrCanFillUpTimeMS));

    digitalWrite(BeerValve1,HIGH);
    
    unsigned long TimeRightNow = millis();
    unsigned long currentFillupSeconds;
    int CanFillUpTimeSec = CanFillUpTime / 1000;
    boolean FillupComplete = false;
    
    while(FillupComplete == false){
      
      int currentFillupSeconds = (millis() - TimeRightNow)/1000;
      Serial.println(String(currentFillupSeconds)+ " " + String(CanFillUpTimeSec));
      //write2LCD(0,0,"Fillup (s):",0,1,String(CanFillUpTimeSec)+"/"+String(currentFillupSeconds));
      delay(25);

      if(int(currentFillupSeconds) >= int(CanFillUpTimeSec)){
        Serial.println("Fillup complete!");
        FillupComplete = true;
      }
      
    }
    
    //delay(CanFillUpTime);
    digitalWrite(BeerValve1,LOW);

   TimeRightNow = 0;
   currentFillupSeconds = 0;
   FillupComplete = false;
   
   Serial.println("End - Beer Valve 1 Fill Can (s): " + String(CanFillUpTimeSec));
   //write2LCD(0,0,"Done filling",0,1,"Time (s): " + String(CanFillUpTimeSec));
}

//BeerValve2 - Fill can
void BeerValve2FillCan(){
  Serial.println("BeerValve2FillCan");
    //Read from EEPROM
    String FetchedStrCanFillUpTimeMS = readEEPROM(0);
    int CanFillUpTime = FetchedStrCanFillUpTimeMS.toInt() * 1000;
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
    int CanFillUpTime = FetchedStrCanFillUpTimeMS.toInt() * 1000;
    Serial.println("Read from EEPROM: " + String(CanFillUpTime));
    //int CanFillUpTime = EEPROM.read(0);
    Serial.println("Start - Beer Valve 1 Fill Can (CountDelayValueMs * " + String(CanFillUpTime) +")");
    ////write2LCD(0,0,"Filling up",0,1,"Two Cans (s):"+ String(FetchedStrCanFillUpTimeMS));
    digitalWrite(BeerValve1,HIGH);
    digitalWrite(BeerValve2,HIGH);

    unsigned long TimeRightNow = millis();
    unsigned long currentFillupSeconds; 
    int CanFillUpTimeSec = (CanFillUpTime / 1000) * BothCanFillupComp; //Add compensation for both cans being filled
    boolean FillupComplete = false;
    
    while(FillupComplete == false){
      
      int currentFillupSeconds = (millis() - TimeRightNow)/1000;
      Serial.println(String(currentFillupSeconds)+ " " + String(CanFillUpTimeSec));
      //write2LCD(0,0,"Fillup (s):",0,1,String(CanFillUpTimeSec)+"/"+String(currentFillupSeconds));
      delay(25);

      if(int(currentFillupSeconds) >= int(CanFillUpTimeSec)){
        Serial.println("Fillup complete!");
        FillupComplete = true;
      }
      
    }    
  
   //delay(CanFillUpTime);
   digitalWrite(BeerValve1,LOW);
   digitalWrite(BeerValve2,LOW);

   TimeRightNow = 0;
   currentFillupSeconds = 0;
   FillupComplete = false;
   
   Serial.println("End - Beer Valve 1/2 Fill Can (s): " + String(CanFillUpTimeSec));
   //write2LCD(0,0,"Done filling",0,1,"Time: " + String(CanFillUpTimeSec));
   
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
  //write2LCD(0,0,"Co2 Purge",0,1,"first can");
  GasValve1Co2PurgeBefore();
  delay(25);
  //write2LCD(0,0,"Start fillup",0,1,"...");
  BeerValve1FillCan();
  delay(25);
  GasValve1Co2PurgeAfter();
  delay(25);
  SoftStopFunction();
}

//Run both fillers
void BeerFiller12ButtonFunction(){
  Serial.println("BeerFiller 1-2 Button Function");
  //write2LCD(0,0,"Co2 Purge",0,1,"both cans");
  GasValvesCo2PurgeBefore();
  delay(25);
  //write2LCD(0,0,"Start fillup",0,1,"...");
  BeerValveFillCans();
  delay(25);
  GasValvesCo2PurgeAfter();
  delay(25);
  SoftStopFunction();
}
