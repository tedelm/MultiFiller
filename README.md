# MultiFiller
Modual Beercan filler </br></br>
MultiFiller is a OpenSource semi-auto beercan or bottle filler.</br></br>

![img](https://github.com/tedelm/MultiFiller/blob/main/img/mockup.png)

## In action Youtube video
[![Alt text](https://img.youtube.com/vi/LUoDoRMrJUI/0.jpg)](https://youtu.be/LUoDoRMrJUI)


## Navigation

* [Build](https://github.com/tedelm/MultiFiller/blob/main/Build/README.md)
* [Schematics](https://github.com/tedelm/MultiFiller/blob/main/Schematics/README.md)
* Videos 
* Parts

</br>

# How to use
button#1 = Calibration (Press and hold)</br>
button#2 = Filler #1 (Momentarily press)</br>
button#3 = Filler #1 and Filler #2 at same time (Momentarily press)</br>
button#4 = Emergency shutdown (Press and hold)</br>

## Calibration
Button#1

    - Press and hold button#1 (it will start to pour beer from Filler #1)
    This is to calibrate how long it takes to fill your can or bottle
    When the can/bottle is filled to desired levels, release the button#1

</br>

## Test fill / Filler #1 
Button#2

    - Momentarily press button#2
       Filler#1: Co2 will purge for 4 seconds
       Filler#1: Beer flows for x (calibrated seconds)
       Filler#1: Co2 will purge on/off rapidly 8 times

## Filler #1 and #2
Button#3

    - Momentarily press button#2
       Filler#1 and #2: Co2 will purge for 4 seconds
       Filler#1 and #2: Beer flows for x (calibrated seconds)
       Filler#1 and #2: Co2 will purge on/off rapidly 8 times

</br>

## Emergency shutdown
button#4

    - Emergency shutdown (Press and hold)

</br>

## Single filler

|Pcs | Name | Price | Price subtotal (sek) | Description	| link |
| -- | --------- | ---------- | ------------- | ----------- | ---- |
|2|12v solenoid valve - 1/4" (6,35mm)     |	65  |130    |1 gas, 1 beer  |	https://www.wish.com/product/5de4e26dd52cb3c890403e0f|
|4|Diode (1N4001 DO-41 50V 1A)        |	1.50  | 6    |Protect relay from solenoid kickback  |	https://www.electrokit.com/produkt/1n4001-do-41-50v-1a/|
|4|Resistor 0.25W 10-50kohm (10k)        |	1  | 4    |Pulldown resistor  |	https://www.electrokit.com/produkt/motstand-kolfilm-0-25w-10kohm-10k/|
|2|PE-slang 1/4" (4,3 x 6,35 mm) neutral|	19	|38		|slang	        |https://shop.humle.se/utrustning/slang/polyeten-pe/pe-slang-neutral-14-yd-per-meter|
|2|PE-slang 3/8" (6,7 x 9,5 mm) neutral |	19  |38		|slang	        |https://shop.humle.se/utrustning/slang/polyeten-pe/pe-slang-neutral-38-yd-per-meter|
|0|John Guest 3/8", Y                   |	39	|0		|Y-koppling	    |https://shop.humle.se/utrustning/kopplingar/push-in-kopplingar-jg/jg-38/john-guest-38-y|
|2|John Guest 3/8" - 1/4"               |	29	|58		|övergångs-koppling	|https://shop.humle.se/utrustning/kopplingar/push-in-kopplingar-jg/jg-38/john-guest-38-14-rak|
|2|silikonpropp 22/17 mm                |	9	|18		|propp	        |https://shop.humle.se/utrustning/jasning/tillbehor/proppar/silikonpropp-2217-mm|
|1|John Guest 3/8" - 1/4" FFL           |	39	|39	    |1 gas, 1 öl	öl/gas koppling	|https://shop.humle.se/utrustning/kopplingar/push-in-kopplingar-jg/jg-38/john-guest-38-14-ffl|
|1|PET-adapter (plast)                  |	69	|69		|gas-koppling	|https://shop.humle.se/utrustning/fattappning/servering/tillbehor-ovrigt/pet-adapter-plast|
|1|Gängad kulkoppling - 1/4" MFL - öl   |	89	|89		|öl-koppling	|https://shop.humle.se/utrustning/kopplingar/fatkopplingar/kulkopplingar/ol/gangad-kulkoppling-14-mfl-ol|
|2|Arduino 5v relay                     |	39	|78		|relä	|https://cdon.se/bygg-verktyg/rela-5-volt-2-kanals-optikt-isolerad-passar-arduino-p49744000|
|1|Arduino nano           |	40	|40		|micro controller	|https://www.banggood.com/Geekcreit-ATmega328P-Nano-V3-Controller-Board-Improved-Version-Module-Development-Board-p-940937.html?cur_warehouse=CN&ID=6300464&rmmds=search|
|3|Momentary press button               |	15	|45	|calibrate, filler1, filler 1 and 2	|https://www.electrokit.com/produkt/tryckknapp-1-pol-off-on-metall-bla-lodoron/|
|1| Patch cable ~5m                     |	99	|99	|Split and use as cabling	|https://www.electrokit.com/produkt/patchkabel-u-utp-cat6a-5m-gra/|



## Dual filler
Just add some more solenoid valves, relays and tubing

|Pcs | Name | Price | Price subtotal (sek) | Description	| link |
| -- | --------- | ---------- | ------------- | ----------- | ---- |
|4|12v solenoid valve - 1/4" (6,35mm)     |	65  |130    |2 gas, 2 beer  |	https://www.wish.com/product/5de4e26dd52cb3c890403e0f|
|4|Diode (1N4001 DO-41 50V 1A)        |	1.50  | 6    |Protect relay from solenoid kickback  |	https://www.electrokit.com/produkt/1n4001-do-41-50v-1a/|
|4|Resistor 0.25W 10-50kohm (10k)        |	1  | 4    |Pulldown resistor  |	https://www.electrokit.com/produkt/motstand-kolfilm-0-25w-10kohm-10k/|
|4|PE-slang 1/4" (4,3 x 6,35 mm) neutral|	19	|38		|slang	        |https://shop.humle.se/utrustning/slang/polyeten-pe/pe-slang-neutral-14-yd-per-meter|
|4|PE-slang 3/8" (6,7 x 9,5 mm) neutral |	19  |38		|slang	        |https://shop.humle.se/utrustning/slang/polyeten-pe/pe-slang-neutral-38-yd-per-meter|
|2|John Guest 3/8", Y                   |	39	|0		|Y-koppling	    |https://shop.humle.se/utrustning/kopplingar/push-in-kopplingar-jg/jg-38/john-guest-38-y|
|4|John Guest 3/8" - 1/4"               |	29	|58		|övergångs-koppling	|https://shop.humle.se/utrustning/kopplingar/push-in-kopplingar-jg/jg-38/john-guest-38-14-rak|
|4|silikonpropp 22/17 mm                |	9	|18		|propp	        |https://shop.humle.se/utrustning/jasning/tillbehor/proppar/silikonpropp-2217-mm|
|2|John Guest 3/8" - 1/4" FFL           |	39	|39	    |1 gas, 1 öl	öl/gas koppling	|https://shop.humle.se/utrustning/kopplingar/push-in-kopplingar-jg/jg-38/john-guest-38-14-ffl|
|2|PET-adapter (plast)                  |	69	|69		|gas-koppling	|https://shop.humle.se/utrustning/fattappning/servering/tillbehor-ovrigt/pet-adapter-plast|
|2|Gängad kulkoppling - 1/4" MFL - öl   |	89	|89		|öl-koppling	|https://shop.humle.se/utrustning/kopplingar/fatkopplingar/kulkopplingar/ol/gangad-kulkoppling-14-mfl-ol|
|1| 4in1 Arduino 5v relay                      |	50	|50		|relay	|https://www.banggood.com/Geekcreit-5V-4-Channel-Relay-Module-For-PIC-ARM-DSP-AVR-MSP430-Geekcreit-for-Arduino-products-that-work-with-official-Arduino-boards-p-87987.html?cur_warehouse=CN&rmmds=search|
|1|Arduino nano           |	40	|40		|micro controller	|https://www.banggood.com/Geekcreit-ATmega328P-Nano-V3-Controller-Board-Improved-Version-Module-Development-Board-p-940937.html?cur_warehouse=CN&ID=6300464&rmmds=search|
|4|Momentary press button                         |	15	|60	|calibrate, filler1, filler 1 and 2	|https://www.electrokit.com/produkt/tryckknapp-1-pol-off-on-metall-bla-lodoron/|
|1| Patch cable ~5m                     |	99	|99	|Split and use as cabling	|https://www.electrokit.com/produkt/patchkabel-u-utp-cat6a-5m-gra/|



![image](https://user-images.githubusercontent.com/26909079/136558059-81aed116-e799-4ba0-a810-7905bc2db499.png)
