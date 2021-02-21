/******************************************************************************************************************
 *                                 ROTARY ENCODER FOR MENU
 *                            private library by Boris Emchenko
 * ****************************************************************************************************************
   some great ideas were taken from https://playground.arduino.cc/Main/RotaryEncoders/
   also many thanks to all folks from https://forum.arduino.cc
   
   Read a rotary encoder with interrupt
   it doesn't matter which encoder pin you use for A or B
   but pinA should support interrupts

   uses Arduino pull-ups on A & B channel outputs
   turning on the pull-ups saves having to hook up resistors
   to the A & B channel outputs
 * ****************************************************************************************************************
 * v 1.0 [2021/02/21]
 *  - first release
 * ****************************************************************************************************************
 */

#define DEBOUNCE_INTERVAL 2                         // debounce interval
#define SPEEDLIMIT_INTERVAL 100                     // speedlimit (if you move faster encoder will not count)

#include "Arduino.h"

class rotaryClass
{

public:

  int encoderPos;													// current encoder position
  
  rotaryClass();                                					// constructor
  rotaryClass(uint8_t pinA, uint8_t pinB);      					// constructor with encoder pin initialization

  void setInterrupt(uint8_t pinA, uint8_t pinB);  					// set pins after initialization (not need if using constructor with pin initialization)

  int checkEncoder();                             					// run this method in loop to check for encoder move events

private:
  uint8_t encoderPinA;
  uint8_t encoderPinB;
 
  unsigned long currenttime, lastIncTime;

  static volatile boolean rotatingA;
  static volatile unsigned long rotatingAStarts;

  static void ICACHE_RAM_ATTR doEncoderA(void);						// interrupt for channel A

};
