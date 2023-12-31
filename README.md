# XmasCountdown

### By OK Lighting of Bloomington, IL

A Christmas Countdown program for Arduino boards and 32x64 RGB Matrix panel 

**Does not use an actual clock - you must set the amount of time remaining before Christmas Day**  

This is a standalone, no internet needed, no clock needed, countdown timer.  

Power to the board must be provided for the countdown to maintain its values - either via USB to the board, or external power adapter to the board.  Once you plug in the board, the countdown begins.  If power is lost or otherwise disconnected, the countdown values must be set to end the countdown at midnight, when Christmas Day begins.

The RGB matrix panel requires its own 5v power supply.  By which I mean - you can't power the board with USB, and expect the panel to work.  The panel also needs 5v power.  

This project was developed using an Arduino Mega board with a 32x64 RGB matrix panel.  

**Wiring your panel to your Arduino Mega board**  
You'll need some wires that are male on one end, female on the other.  

When looking at the pins on the panel input, 2 per row and working top to bottom, the wiring to the Arduino Mega board is as follows:  

**Panel-Board Panel-Board**   
R1-24,   G1-25  
B1-26,   GND-GND  
R2-27,   G2-28  
B2-29,   GND-GND  
A-A0,    B-A1  
C-A2,    D-A3  
CLK-11,  LAT-10  
OE-9,    GND-GND  

Again, this is for **Arduino Mega boards** (or equivalent - Elegoo etc).  

You will need to install the following in your Arduino IDE:    
RGB matrix Panel by Adafruit   
Countdown by Rob Tillaart  

From Adafruit-GFX-Library (comes with RGB matrix Panel):  
Fonts/FreeMono12pt7b.h  
Fonts/FreeMono9pt7b.h  
See: https://github.com/adafruit/Adafruit-GFX-Library/tree/master/Fonts

The heart of this Xmas Countdown comes from the amazing code of Rob Tillaart:  https://github.com/RobTillaart/CountDown
