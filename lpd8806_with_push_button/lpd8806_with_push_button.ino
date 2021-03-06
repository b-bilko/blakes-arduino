#include "LPD8806.h"
#include "SPI.h"

// Adapted from adafruit's awesome LPD8806 library, found here: https://github.com/adafruit/LPD8806 

/*****************************************************************************/

// Sequences
int currentSequence = 0;
int changeSequence = LOW;

// Number of RGB LEDs in strand:
int nLEDs = 18;
int dataPin  = 3;
int clockPin = 4;
int myButton = 0; // press button in pin D2, which is for some reason called 0 in interrupt land

long time = 0;
long debounce = 200;

// First parameter is the number of LEDs in the strand.  The LED strips
// are 32 LEDs per meter but you can extend or cut the strip.  Next two
// parameters are SPI data and clock pins:
LPD8806 strip = LPD8806(nLEDs, dataPin, clockPin);

// You can optionally use hardware SPI for faster writes, just leave out
// the data and clock pin parameters.  But this does limit use to very
// specific pins on the Arduino.  For "classic" Arduinos (Uno, Duemilanove,
// etc.), data = pin 11, clock = pin 13.  For Arduino Mega, data = pin 51,
// clock = pin 52.  For 32u4 Breakout Board+ and Teensy, data = pin B2,
// clock = pin B1.  For Leonardo, this can ONLY be done on the ICSP pins.
//LPD8806 strip = LPD8806(nLEDs);

void setup() {
  Serial.begin(9600); //logging
  attachInterrupt(myButton, stateChange, RISING);
  // Start up the LED strip
  strip.begin();
  // Update the strip, to start they are all 'off'
  strip.show();
}

void loop() {
  changeSequence = LOW;
  playSequence(currentSequence);
}

void stateChange() {
  Serial.println("state changed!");
  if (millis() - time > debounce) {
    changeSequence = HIGH;
    currentSequence += 1;
    time = millis();
  }
}

int playSequence(int current) {
  String logOne = "running sequence: ";
  String logStatement = logOne + current;
  Serial.println(logStatement);
  switch (current) {
     case 0:
       cycleColorChase(30);
       break;
       
     case 1:
       cycleColorChase(20);
       break;

     case 2:
       cycleColorChase(10);
       break;
       
     case 3: 
       cycleColorWipe(30);
       break;
      
     case 4: 
       cycleColorWipe(20);
       break;    
       
     case 5: 
       cycleColorWipe(10);
       break;    
       
     case 6:
       rainbowCycle(20);
       break;
       
     case 7:
       rainbowCycle(10);
       break;
       
     case 8:
       rainbowCycle(0);
       break;
       
     case 9:
       rainbow(0,354,20);
       break;
       
     case 10:
       rainbow(0,354,10);
       break;
      
     case 11:
       rainbow(0,354,0);
       break;

     case 12: 
       cycleColorStripe(20);
       break;

     case 13: 
       cycleColorStripe(10);
       break;
       
     case 14: 
       cycleColorStripe(0);
       break;
       
     case 15: 
       cycleColorStrobe(70);
       break;
       
     case 16: 
       cycleColorStrobe(30);
       break;    
       
     case 17: 
       cycleColorStrobe(20);
       break;
       
     case 18: 
       cycleColorStrobe(10);
       break;
       
     case 19:
       acceleratingColorChase();
       cycleColorStrobe(70);
       rainbowCycle(0);
       break;
       
     default:
       currentSequence = 0;
  }
}

// HIGHER LEVEL CLASSES-- none of these may loop because they do not check for button state
void acceleratingColorChase() {
   colorChase(strip.Color(  0,   0, 127), 40); // Blue
   colorChase(strip.Color(127,   0,   0), 35); // Red
   colorChase(strip.Color(127, 127,   0), 30); // Yellow
   colorChase(strip.Color(  0, 127,   0), 25); // Green
   colorChase(strip.Color(  0, 127, 127), 20); // Cyan
   colorChase(strip.Color(  0,   0, 127), 15); // Blue
   colorChase(strip.Color(127,   0, 127), 10); // Violet
   colorChase(strip.Color(  0,   0, 127), 8); // Blue
   colorChase(strip.Color(127,   0,   0), 6); // Red
   colorChase(strip.Color(127, 127,   0), 4); // Yellow
   colorChase(strip.Color(  0, 127,   0), 2); // Green
   colorChase(strip.Color(  0, 127, 127), 1); // Cyan
   colorChase(strip.Color(  0,   0, 127), 0); // Blue
   colorChase(strip.Color(127,   0, 127), 0); // Violet
}

void cycleColorChase(uint8_t wait) {
   colorChase(strip.Color(  0,   0, 127), wait); // Blue
   colorChase(strip.Color(127,   0,   0), wait); // Red
   colorChase(strip.Color(127, 127,   0), wait); // Yellow
   colorChase(strip.Color(  0, 127,   0), wait); // Green
   colorChase(strip.Color(  0, 127, 127), wait); // Cyan
   colorChase(strip.Color(  0,   0, 127), wait); // Blue
   colorChase(strip.Color(127,   0, 127), wait); // Violet
}

void cycleColorStripe(uint8_t wait) {
   colorStripe(strip.Color(127, 0,   127), strip.Color(0, 127,   0), wait);
   colorStripe(strip.Color(0, 127,   0), strip.Color(127, 0,   127), wait);
   colorStripe(strip.Color(127, 127,   0), strip.Color(0, 0,   127), wait);
   colorStripe(strip.Color(0, 0,   127), strip.Color(127, 127,   0), wait);
   colorStripe(strip.Color(0, 127,   127), strip.Color(127, 0,   0), wait);
   colorStripe(strip.Color(127, 0,   0), strip.Color(0, 127,   127), wait);
}

void cycleColorStrobe(uint8_t wait){
   colorStrobe(strip.Color(  0,   0, 127), wait); // Blue
   colorStrobe(strip.Color(127,   0,   0), wait); // Red
   colorStrobe(strip.Color(127, 127,   0), wait); // Yellow
   colorStrobe(strip.Color(  0, 127,   0), wait); // Green
   colorStrobe(strip.Color(  0, 127, 127), wait); // Cyan
   colorStrobe(strip.Color(  0,   0, 127), wait); // Blue
   colorStrobe(strip.Color(127,   0, 127), wait); // Violet
}

void cycleColorWipe(uint8_t wait){
   colorWipe(strip.Color(  0,   0, 127), wait); // Blue
   colorWipe(strip.Color(127,   0,   0), wait); // Red
   colorWipe(strip.Color(127, 127,   0), wait); // Yellow
   colorWipe(strip.Color(  0, 127,   0), wait); // Green
   colorWipe(strip.Color(  0, 127, 127), wait); // Cyan
   colorWipe(strip.Color(  0,   0, 127), wait); // Blue
   colorWipe(strip.Color(127,   0, 127), wait); // Violet
}

void rainbow(int start, int finish, uint8_t wait) {
  innerRainbow(start,finish,1,wait); 
  innerRainbow(start,finish,0,wait);
}

// LOWER LEVEL CLASSES-- each of these should have a check in every loop that returns 
// if the interrupts's stateChange() method was called since the last check
void colorStrobe(uint32_t c, uint8_t wait){
  int i;
  for (i=0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
  }
  strip.show();
  if (changeSequence == HIGH) { return; }
  delay(wait);
  for (i=0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, 0);
  }
  strip.show();
  if (changeSequence == HIGH) { return; }
  delay(wait);
}

// Fill the dots progressively along the strip.
void colorWipe(uint32_t c, uint8_t wait) {
  int i;
  for (i=0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    if (changeSequence == HIGH) { return; }
    delay(wait);
  }
}

// Fill 2 colors of dots, alternating along the strip.
void colorStripe(uint32_t c1, uint32_t c2, uint8_t wait) {
  int i;
  for (i=0; i < strip.numPixels(); i++) {
    if (i % 2 == 0){
      strip.setPixelColor(i, c1);
    }
    else {
      strip.setPixelColor(i, c2);
    } 
    strip.show();
    if (changeSequence == HIGH) { return; }
    delay(wait);
  }
}

// Chase one dot down the full strip.
void colorChase(uint32_t c, uint8_t wait) {
  int i;

  // Start by turning all pixels off:
  for(i=0; i<strip.numPixels(); i++) strip.setPixelColor(i, 0);

  // Then display one pixel at a time:
  for(i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c); // Set new pixel 'on'
    strip.show();              // Refresh LED states
    strip.setPixelColor(i, 0); // Erase pixel, but don't refresh!
    if (changeSequence == HIGH) { return; }
    delay(wait);
  }
  strip.show(); // Refresh to turn off last pixel
}

void innerRainbow(int start, int finish, int upordown, uint8_t wait) {
  int i, j;
  for (j=start; j < finish; j++) {     // 3 cycles of all 384 colors in the wheel
    for (i=0; i < strip.numPixels(); i++) {
      if (upordown == 1){
        strip.setPixelColor(i, Wheel( (i + j) % 384));
      }
      else {
        strip.setPixelColor(i, Wheel( (i + (finish - j + start)) % 384));
      }
    }
    strip.show();   // write all the pixels out
    if (changeSequence == HIGH) { return; }
    delay(wait);
  }
}

// Slightly different, this one makes the rainbow wheel equally distributed 
// along the chain
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;
  for (j=0; j < 384 * 5; j++) {     // 5 cycles of all 384 colors in the wheel
    for (i=0; i < strip.numPixels(); i++) {
      // tricky math! we use each pixel as a fraction of the full 384-color wheel
      // (thats the i / strip.numPixels() part)
      // Then add in j which makes the colors go around per pixel
      // the % 384 is to make the wheel cycle around
      strip.setPixelColor(i, Wheel( ((i * 384 / strip.numPixels()) + j) % 384) );
    }  
    strip.show();   // write all the pixels out
    if (changeSequence == HIGH) { return; }
    delay(wait);
  }
}

/* Helper functions */

//Input a value 0 to 384 to get a color value.
//The colours are a transition r - g -b - back to r

uint32_t Wheel(uint16_t WheelPos) {
  byte r, g, b;
  switch(WheelPos / 128)
  {
    case 0:
      r = 127 - WheelPos % 128;   //Red down
      g = WheelPos % 128;      // Green up
      b = 0;                  //blue off
      break; 
    case 1:
      r = 0;                  //red off
      g = 127 - WheelPos % 128;  //green down
      b = WheelPos % 128;      //blue up
      break; 
    case 2:
      r = WheelPos % 128;      //red up
      g = 0;                  //green off
      b = 127 - WheelPos % 128;  //blue down 
      break; 
  }
  return(strip.Color(r,g,b));
}
