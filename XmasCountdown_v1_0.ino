// XmasCountdown by OKLighting of Bloomington, IL
// "Keep it merry and bright!"
//
// RGB Panel GFX Demo example for 16x32 panel used to connect to 32x64 panel
// By Marc MERLIN <marc_soft@merlins.org>
// Contains code (c) Adafruit, license BSD
//
// This uses CountDown by Rob Tillaart: https://github.com/RobTillaart/CountDown
//
// *** DUE TO LIMITATIONS OF THE COUNTDOWN CALCULATION, minutes will not be displayed until **22 DAYS or so, or less**  Until that point, only days and hours are displayed.
// *** You can start this countdown with any combo of the input at CD.start - days, hours, minutes - in that order.
//
// Future Plans:
// TODO: Make custom screen for Christmas Day (count = 0)
// TODO: Add animation of Santa hat with snowflakes falling

//***************************************************************
// WILL NOT FIT on ARDUINO UNO -- requires a Mega, M0 or M4 board
//***************************************************************

#include <RGBmatrixPanel.h>
#include <Fonts/FreeMono12pt7b.h>
#include <Fonts/FreeMono9pt7b.h>
#include <CountDown.h>

CountDown CD;

// Most of the signal pins are configurable, but the CLK pin has some
// special constraints.  On 8-bit AVR boards it must be on PORTB...
// Pin 11 works on the Arduino Mega.  On 32-bit SAMD boards it must be
// on the same PORT as the RGB data pins (D2-D7)...
// Pin 8 works on the Adafruit Metro M0 or Arduino Zero,
// Pin A4 works on the Adafruit Metro M4 (if using the Adafruit RGB
// Matrix Shield, cut trace between CLK pads and run a wire to A4).

//#define CLK  8   // USE THIS ON ADAFRUIT METRO M0, etc.
//#define CLK A4 // USE THIS ON METRO M4 (not M0)

#define CLK 11 // *****USE THIS ON ARDUINO MEGA****
#define OE   9
#define LAT 10
#define A   A0
#define B   A1
#define C   A2
#define D   A3

// Enable double buffering
RGBmatrixPanel *matrix = new RGBmatrixPanel(A, B, C, D, CLK, LAT, OE, true, 64);

// Panel Matrix doesn't fully work like Neomatrix (which I originally
// wrote this demo for), so map a few calls to be compatible. The rest
// comes from Adafruit_GFX and works the same on both backends.
#define setBrightness(x) fillScreen(0) // no-op, no brightness on this board
#define clear()          fillScreen(0)
#define show()           swapBuffers(true)
#define Color(x,y,z)     Color444(x/16,y/16,z/16)

// Define matrix width and height.
#define mw 64
#define mh 32

// This could also be defined as matrix->color(255,0,0) but those defines
// are meant to work for Adafruit::GFX backends that are lacking color()
#define LED_BLACK           0

#define LED_RED_VERYLOW    (3 <<  11)
#define LED_RED_LOW        (7 <<  11)
#define LED_RED_MEDIUM     (15 << 11)
#define LED_RED_HIGH       (31 << 11)

#define LED_GREEN_VERYLOW  (1 <<  5)
#define LED_GREEN_LOW      (15 << 5)
#define LED_GREEN_MEDIUM   (31 << 5)
#define LED_GREEN_HIGH     (63 << 5)

#define LED_BLUE_VERYLOW     3
#define LED_BLUE_LOW         7
#define LED_BLUE_MEDIUM     15
#define LED_BLUE_HIGH       31

#define LED_ORANGE_VERYLOW (LED_RED_VERYLOW + LED_GREEN_LOW)
#define LED_ORANGE_LOW     (LED_RED_LOW     + LED_GREEN_LOW)
#define LED_ORANGE_MEDIUM  (LED_RED_MEDIUM  + LED_GREEN_MEDIUM)
#define LED_ORANGE_HIGH    (LED_RED_HIGH    + LED_GREEN_HIGH)

#define LED_PURPLE_VERYLOW (LED_RED_VERYLOW + LED_BLUE_VERYLOW)
#define LED_PURPLE_LOW     (LED_RED_LOW     + LED_BLUE_LOW)
#define LED_PURPLE_MEDIUM  (LED_RED_MEDIUM  + LED_BLUE_MEDIUM)
#define LED_PURPLE_HIGH    (LED_RED_HIGH    + LED_BLUE_HIGH)

#define LED_CYAN_VERYLOW   (LED_GREEN_VERYLOW + LED_BLUE_VERYLOW)
#define LED_CYAN_LOW       (LED_GREEN_LOW     + LED_BLUE_LOW)
#define LED_CYAN_MEDIUM    (LED_GREEN_MEDIUM  + LED_BLUE_MEDIUM)
#define LED_CYAN_HIGH      (LED_GREEN_HIGH    + LED_BLUE_HIGH)

#define LED_WHITE_VERYLOW  (LED_RED_VERYLOW + LED_GREEN_VERYLOW + LED_BLUE_VERYLOW)
#define LED_WHITE_LOW      (LED_RED_LOW     + LED_GREEN_LOW     + LED_BLUE_LOW)
#define LED_WHITE_MEDIUM   (LED_RED_MEDIUM  + LED_GREEN_MEDIUM  + LED_BLUE_MEDIUM)
#define LED_WHITE_HIGH     (LED_RED_HIGH    + LED_GREEN_HIGH    + LED_BLUE_HIGH)

uint16_t get_daysleft() {
  //This returns remaining time in days - divide by 1440 minutes in a day.
  return CD.remaining() / 1440;
};

uint16_t get_hoursleft() {
  //This returns remaining time in hours - divide by 60 minutes in an hour.
  return CD.remaining() / 60;
};

uint16_t get_minutesleft() {
  //This returns remaining time in minutes.
  return CD.remaining();
};

void display_XmasCountdown() {
  int minutes = get_minutesleft();
  
  //showRemainingCount(minutes); //for troubleshooting any remaining time calculations, briefly show the minutes remaining.  Uncomment if needed.

  matrix->clear();
  matrix->setTextWrap(false);  // don't wrap text so it scrolls nicely
  matrix->setTextSize(1);
  matrix->setRotation(0);

  if (minutes == 0) {
      //countdown is done - Merry Christmas!
        for (int8_t x=7; x>=-42; x--) {
          matrix->clear();
          matrix->setCursor(x,0);
          matrix->setTextColor(LED_GREEN_LOW);
          matrix->print("No more waiting!");
          if (mh>11) {
              matrix->setCursor(-20-x,mh-7);
              matrix->setTextColor(LED_RED_LOW);
              matrix->print("It's Christmas!");
          }
          matrix->show();
          delay(75);
      };
  } else {
    //sometimes minutes end up a large negative result when calculating days over 22 days - just show regular wording.  Day and Hour calculations are still correct, we won't show minutes until they are correct.
    showChristmasCountdown();
    //display HO HO HO dropping from top of panel vertically
    display_hohoho();
  }

    //set rotation back to left > right
    matrix->setRotation(0);

    //Display Days until Christmas
    displayDays();
    resetFontClearMatrix();

    //Display Hours until Christmas
    displayHours();
    resetFontClearMatrix();

    //Display Minutes until Christmas
    displayMinutes();
    resetFontClearMatrix();

    if (minutes == 0) {
      flashMerryXmas1();
      resetFontClearMatrix();
      flashMerryXmas2();
      resetFontClearMatrix();
      flashMerryXmas1();
      resetFontClearMatrix();
      flashMerryXmas2();
      resetFontClearMatrix();
      flashMerryXmas1();
      resetFontClearMatrix();
      flashMerryXmas2();
      resetFontClearMatrix();
      flashMerryXmas1();
      resetFontClearMatrix();
      flashMerryXmas2();
    }
    resetFontClearMatrix();
    displayMerryXmas();
    resetFontClearMatrix();
};

//Show Christmas Countdown
void showChristmasCountdown() {
  for (int8_t x=7; x>=-42; x--) {
            matrix->clear();
            matrix->setCursor(x,0);
            matrix->setTextColor(LED_GREEN_LOW);
            matrix->print("Christmas");
            if (mh>11) {
                matrix->setCursor(-20-x,mh-7);
                matrix->setTextColor(LED_RED_LOW);
                matrix->print("Countdown");
            }
            matrix->show();
            delay(50);
        };
}

// Briefly display the minutes count to troubleshoot any issues.
 void showRemainingCount(int minutes) {
    matrix->setTextSize(1);
    matrix->setTextColor(LED_RED_LOW);
    matrix->setCursor(1,3);
    matrix->println(minutes);
    matrix->show();
    delay(1000);
}

//This will make HO HO HO appear from the top of the screen to the bottom, vertically dropping.
void display_hohoho() {
  matrix->setRotation(3);
    matrix->setTextColor(LED_RED_LOW);
    for (int8_t x=7; x>=-45; x--) {
        matrix->clear();
        matrix->setCursor(x,mw/2-4);
        matrix->print("HO ");
        matrix->setTextColor(LED_WHITE_LOW);
        matrix->print("HO ");
        matrix->setTextColor(LED_RED_LOW);
        matrix->print("HO");
        matrix->show();
        delay(25);
        matrix->clear();
    }
}

void resetFontClearMatrix() {
   //set font back to default
    matrix->setFont();
    //clear the display
    matrix->clear();
}

void displayDays() {   
    //figure out days left
    int days = get_daysleft();
    if (days > 0) {
      matrix->setTextColor(LED_RED_LOW);
      alignNumbers(days);
      matrix->println(days);
      //reset size - so "day" or "days" is smaller than the number of days
      matrix->setTextSize(1);
      matrix->setTextColor(LED_GREEN_LOW);
      if (days == 1) {
        matrix->setCursor(25,22);
          matrix->println("day");
      } else {
          matrix->setCursor(21,21);
          matrix->println("days");
      }
      
      matrix->show();
      delay(2000);
      displayTilXMASLook1();
    } 
}

void displayHours() {
  //figure out hours left
  int hours = get_hoursleft();
  if (hours > 0) {
    matrix->setTextColor(LED_RED_LOW);
    alignNumbers(hours);
    matrix->println(hours);
    matrix->setTextSize(1);
     matrix->setTextColor(LED_GREEN_LOW);
    if (hours == 1) {
      matrix->setCursor(22,22);
      matrix->println("hour");
    } else {
      matrix->setCursor(18,21);
      matrix->println("hours");
    }
    
    matrix->show();
    delay(2000);
    displayTilXMASLook1();
  }
}

void displayMinutes() {
  //figure out minutes left
  int minutes = get_minutesleft();
  if (minutes > 0) {
    matrix->setTextColor(LED_RED_LOW);
    alignNumbers(minutes);
    matrix->println(minutes);
    
    matrix->setTextSize(1);
     matrix->setTextColor(LED_GREEN_LOW);
    if (minutes == 1) {
      matrix->setCursor(15,22);
      matrix->println("minute");
    } else {
      matrix->setCursor(12,21);
      matrix->println("minutes");
    }
     
    matrix->show();
    delay(2000);
    displayTilXMASLook1();
  }
}

void alignNumbers(int count) {
 //adjust cursor for proper centering of what is sent in
  matrix->setTextSize(2);
  
  if (count < 100000) {
    matrix->setCursor(1,3);
  }

  if (count < 10000) {
    matrix->setCursor(9,3);
  }

  if (count < 1000) {
    matrix->setCursor(16,3);
  }

  if (count < 100) {
    matrix->setCursor(22,3);
  }

  if (count < 10) {
    matrix->setCursor(28,3);
  }

  if (count == 1) {
    //if we're at 1 day, 1 hour, or 1 minute left, change the number to be red
      matrix->setTextColor(LED_RED_LOW);
  }
}

void displayXMAS() {
  matrix->clear();
    //show 'til centered above XMAS
    matrix->setCursor(1,2);
    matrix->setTextSize(2);
    matrix->setTextColor(LED_ORANGE_LOW);
    matrix->println("til");
    //show XMAS big and centered on a 32x64
    matrix->setCursor(4,17);
    matrix->setTextSize(1);
    //Change the font and get fancy
    matrix->setFont(&FreeMono12pt7b);
    matrix->setTextColor(LED_RED_LOW);
    matrix->print("X");
    matrix->setTextColor(LED_GREEN_LOW);
    matrix->print("M");
    matrix->setTextColor(LED_RED_LOW);
    matrix->print("A");
    matrix->setTextColor(LED_GREEN_LOW);
    matrix->print("S");
    matrix->show();
    delay(3000);
}

void displayTilXMASLook1() {
  matrix->clear();
    //show 'til centered above XMAS
    matrix->setCursor(5,6);
    matrix->setTextSize(1);
    matrix->setTextColor(LED_ORANGE_LOW);
    matrix->println("til");
    //show XMAS big and centered on a 32x64
    matrix->setCursor(4,21);
    matrix->setTextSize(1);
    //Change the font and get fancy
    matrix->setFont(&FreeMono12pt7b);
    matrix->setTextColor(LED_RED_LOW);
    matrix->print("X");
    matrix->setTextColor(LED_GREEN_LOW);
    matrix->print("M");
    matrix->setTextColor(LED_RED_LOW);
    matrix->print("A");
    matrix->setTextColor(LED_GREEN_LOW);
    matrix->print("S");
    matrix->show();
    delay(3000);
}

void displayMerryXmas() {
    matrix->clear();
    //show XMAS big and centered on a 32x64
    matrix->setCursor(4,3);
    matrix->setTextSize(1);
    //Change the font and get fancy
    matrix->setFont(&FreeMono9pt7b);
    matrix->setTextColor(LED_RED_LOW);
    matrix->println("Merry");
    matrix->setTextColor(LED_GREEN_LOW);
    matrix->setCursor(6,19);
    matrix->print("Xmas");
    matrix->setTextColor(LED_ORANGE_LOW);
    matrix->setCursor(14,31);
    matrix->print("2023");
    matrix->show();
    delay(10000);
}

void flashMerryXmas1() {
    matrix->clear();
    //show XMAS big and centered on a 32x64
    matrix->setCursor(4,3);
    matrix->setTextSize(1);
    //Change the font and get fancy
    matrix->setFont(&FreeMono9pt7b);
    matrix->setTextColor(LED_GREEN_LOW);
    matrix->println("Merry");
    matrix->setTextColor(LED_RED_LOW);
    matrix->setCursor(6,19);
    matrix->print("Xmas");
    matrix->setTextColor(LED_ORANGE_LOW);
    matrix->setCursor(14,31);
    matrix->print("2023");
    matrix->show();
    delay(100);
}

void flashMerryXmas2() {
    matrix->clear();
    //show XMAS big and centered on a 32x64
    matrix->setCursor(4,3);
    matrix->setTextSize(1);
    //Change the font and get fancy
    matrix->setFont(&FreeMono9pt7b);
    matrix->setTextColor(LED_RED_LOW);
    matrix->println("Merry");
    matrix->setTextColor(LED_GREEN_LOW);
    matrix->setCursor(6,19);
    matrix->print("Xmas");
    matrix->setTextColor(LED_ORANGE_LOW);
    matrix->setCursor(14,31);
    matrix->print("2023");
    matrix->show();
    delay(100);
}

void loop() {
    // clear the screen after X bitmaps have been displayed and we
    // loop back to the top left corner
    // 8x8 => 1, 16x8 => 2, 17x9 => 6

    matrix->clear();
    display_XmasCountdown();
}

void setup() {
    Serial.begin(115200);
    // This is for setting how many days to Christmas.  Must be accurate at start.  
    //
    // *** DUE TO LIMITATIONS OF THE COUNTDOWN CALCUATION, Max Days value is 22 DAYS.  Anything more produces odd results from minutes - they turn negative.  If you want more time, multiply the days by 24 to get hours total, and use the hours input.
    // *** The earliest you can start this countdown is December 3rd if you want it to finish on Christmas Day.
    //
    // Input format:  start(days, hours, minutes) > getRemaining function will return time left in MINUTES
    //
    // Continuous power is required to maintain the countdown!  
    //
    // Unplugging will restart to this time, and remaining time must be adjusted if that happens.
    // You can combine days/hours/minutes to start - they will all however return "getRemaining" in minutes.
    //
    // Note that for any numbers of days greater than 22 (whether entered by days, hours, or minutes), the minutes value will NOT appear on the countdown until about 22 days left until Christmas.  This is due to some bug in the minutes value returned as a negative number for a time.
    // 600 hours = 25 days = start it at midnight December 1st.
    CD.start(2,8,51);

    matrix->begin();
    matrix->setTextWrap(false);
    matrix->setBrightness(BRIGHTNESS);
    // Test red/green/white LEDs if desired - uncomment as needed. 
    //matrix->fillScreen(LED_WHITE_LOW);
    //matrix->show();
    //delay(100);
    //matrix->clear();
    //matrix->fillScreen(LED_RED_LOW);
    //matrix->show();
    //delay(100);
        //matrix->clear();
    //matrix->fillScreen(LED_GREEN_LOW);
    //matrix->show();
    //delay(100);
    //
    // ************************************************************************************************
    // ** Below is our splash screen.  Remove it if you like, it will not show during looping.       **
    // ** Appears only on start up or restart.                                                       **
    // ************************************************************************************************
    matrix->clear();
    matrix->setTextColor(LED_WHITE_HIGH);
    matrix->print("OK");
    matrix->setTextColor(LED_ORANGE_MEDIUM);
    matrix->println("Lighting");
    matrix->setTextColor(LED_RED_LOW);
    matrix->println("Xmas");
    matrix->setTextColor(LED_GREEN_LOW);
    matrix->println("Countdown");
    matrix->setTextColor(LED_WHITE_LOW);
    matrix->println("v1.0");
    matrix->show();
    delay(2000);
    matrix->clear();
}

// vim:sts=4:sw=4
