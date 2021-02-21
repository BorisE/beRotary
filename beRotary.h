/******************************************************************************************************************
 *                                 ROTARY ENCODER FOR MENU
 *                            private library by Boris Emchenko
 * ****************************************************************************************************************
   read a rotary encoder with interrupt
   Encoder hooked up with common to GROUND,
   encoderPinA to pin D3, encoderPinB to pin D4 (D2 not working as intterupt so can)
   it doesn't matter which encoder pin you use for A or B
   but pinA should support interrupt

   uses Arduino pull-ups on A & B channel outputs
   turning on the pull-ups saves having to hook up resistors
   to the A & B channel outputs
 * ****************************************************************************************************************
 * v 1.0 [2021/02/21]
 *  - first release
 * 
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
  rotaryClass(uint8_t pinA, uint8_t pinB, uint8_t pushPin);			// constructor with encoder and push button pin initialization

  void setInterrupt(uint8_t pinA, uint8_t pinB);  					// set pins after initialization (not need if using constructor with pin initialization)
  void setInterrupt(uint8_t pinA, uint8_t pinB, uint8_t pushPin);  	// set pins after initialization (not need if using constructor with pin initialization)

  int checkEncoder();                             					// run this method in loop to check for encoder move events

private:
  uint8_t encoderPinA;
  uint8_t encoderPinB;
  uint8_t encoderPushPin;
 
  unsigned long currenttime, lastIncTime;

  static volatile boolean rotatingA;
  static volatile unsigned long rotatingAStarts;

  static void ICACHE_RAM_ATTR doEncoderA(void);						// interrupt for channel A

};
