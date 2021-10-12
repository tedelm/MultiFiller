# Schematics

</br>

![img](https://github.com/tedelm/MultiFiller/blob/main/img/schematic.png)

</br>

## Buttons 
R1 = Resistor 10-50kOhm </br>
S = Signal, to arduino (eg. 30,31,32,A10)</br>

    //Calibrate - Time how long it takes to fill the can
    #define CalibrateButton 30 //Atmega328p-pu pin xx
    //Controller
    #define BeerFiller1Button 31 //Atmega328p-pu pin xx - Only Filler #1
    #define BeerFiller12Button 32 //Atmega328p-pu pin xx - Run both fillers
    #define EmergencyShutDownButton A10 //Atmega328p-pu interrupt pin xx - Stop everything
    //Arduino Uno/Nano/Mini: All pins are usable
    //Arduino Mega: 10, 11, 12, 13, 50, 51, 52, 53, A8 (62), A9 (63), A10 (64),A11 (65), A12 (66), A13 (67), A14 (68), A15 (69)

 </br>

![img](https://github.com/tedelm/MultiFiller/blob/main/img/button.png)

## Relay and valves 
V = Solenoid valve</br>
<- = Diode </br>
S = Signal, to arduino (22,23,24,25)</br>

    //Filler 1
    #define BeerValve1 22 //Atmega328p-pu pin xx
    #define GasValve1 23 //Atmega328p-pu pin xx
    //Filler 2
    #define BeerValve2 24 //Atmega328p-pu pin xx
    #define GasValve2 25 //Atmega328p-pu pin xx

</br>

![img](https://github.com/tedelm/MultiFiller/blob/main/img/valveRelay.png)

