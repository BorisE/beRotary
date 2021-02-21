#include "beRotary.h"

  volatile boolean rotaryClass::rotatingA;
  volatile unsigned long rotaryClass::rotatingAStarts;

  /* 
   * Constructor, simple form 
   */
  rotaryClass::rotaryClass() {
    rotatingA = false;
    rotatingAStarts = 0;
    
    encoderPos = 0;

    lastIncTime = 0;
    currenttime = millis();
  }

  /* 
   * Constructor, with pin initialization
   */
  rotaryClass::rotaryClass(uint8_t pinA, uint8_t pinB) {
    rotaryClass();
    setInterrupt(pinA, pinB);
  }


  void rotaryClass::setInterrupt(uint8_t pinA, uint8_t pinB) {
    encoderPinA = pinA;
    encoderPinB = pinB;

    pinMode(encoderPinA, INPUT);
    digitalWrite(encoderPinA, HIGH);       // turn on pull-up resistor
    
    pinMode(encoderPinB, INPUT);
    digitalWrite(encoderPinB, HIGH);       // turn on pull-up resistor

    attachInterrupt(digitalPinToInterrupt (encoderPinA), doEncoderA, CHANGE); 
  }

  int rotaryClass::checkEncoder()  {
    int ret = 0;
    if (rotatingA) {
      currenttime = millis();
      int dir = 0;
      if ( currenttime - rotatingAStarts > DEBOUNCE_INTERVAL) {
        // When signal changes we wait 2 milliseconds for it to
        // stabilise before reading 
        // (increase this value if there still bounce issues)
        if (digitalRead(encoderPinA) == digitalRead(encoderPinB)) {
          dir = 1;
        }
        else {
          dir = -1;
        }
        if (currenttime - lastIncTime > SPEEDLIMIT_INTERVAL) {
           encoderPos+=dir;
           lastIncTime = currenttime;
           ret = 1;
        }
        rotatingA = false; // Reset the flag back to false
      }
    } 
    return ret;
  }

  void ICACHE_RAM_ATTR rotaryClass::doEncoderA(void) {
    if (!rotatingA) rotatingAStarts = millis();
    rotatingA = true;
    // If a signal change (noise or otherwise) is detected
    // in the rotary encoder, the flag is set to true
  }
