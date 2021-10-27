/*
YF‐ S201 Water Flow Sensor
Water Flow Sensor output processed to read in litres/hour
Adaptation Courtesy: www.hobbytronics.co.uk
*/
volatile int flow_frequency; // Measures flow sensor pulses
unsigned int l_hour; // Calculated litres/hour
unsigned char flowsensor = 2; // Sensor Input
unsigned long currentTime;
unsigned long cloopTime;
unsigned long read_pulses;
int Lpulses = 10000; // the amount of pulses for 1 liter of liquid
unsigned long liquid_ml; // liquid_ml = 1000/(1L_pulses / read_pulses); // ml / ((1L * pulses) / inputSignal )



void flow () // Interrupt function
{
   flow_frequency++;
   Serial.println(flow_frequency);
}
void setup()
{
   pinMode(flowsensor, INPUT);
   digitalWrite(flowsensor, HIGH); // Optional Internal Pull-Up
   Serial.begin(9600);
   //attachInterrupt(0, flow, RISING); // Setup Interrupt
   //sei(); // Enable interrupts
   currentTime = millis();
   cloopTime = currentTime;
}
void loop ()
{
int aa = 0;
boolean flowing = true;

    while(flowing == true){

      delay(25);
      //read flowsensor
      read_pulses += digitalRead(flowsensor);
      Serial.println("Read pulses: " + String(read_pulses));

      aa++;
      if(aa < 200){
        flowing = false;
          liquid_ml = 1000/(Lpulses / read_pulses);
          Serial.println("Read liquid:" + String(liquid_ml));
      }
    }


    //Serial.println("Read pulses: " + String(read_pulses));
    //Flowsensor
    liquid_ml = 1000/(Lpulses / read_pulses);
    Serial.println("Read liquid:" + String(liquid_ml));

}
