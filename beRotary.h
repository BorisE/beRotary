/******************************************************************************************************************
 *                                 ROTARY ENCODER FOR MENU
 *                            private library by Boris Emchenko
 * ****************************************************************************************************************
   Some great ideas were taken from https://playground.arduino.cc/Main/RotaryEncoders/
   also many thanks to all folks from https://forum.arduino.cc
   
   Read a rotary encoder with interrupt
   it doesn't matter which encoder pin you use for A or B
   but pinA should support interrupts

   uses Arduino pull-ups on A & B channel outputs
   turning on the pull-ups saves having to hook up resistors
   to the A & B channel outputs
 * ****************************************************************************************************************
 * v 2.0 [2021/02/21]
 *  - push button events (short and long)
 *  - some logic for encoder changed
 * v 1.0 [2021/02/21]
 *  - first release
 * ****************************************************************************************************************
 */

#define DEBOUNCE_INTERVAL 2                             // debounce interval
#define SPEEDLIMIT_INTERVAL 100                         // speedlimit (if you move faster encoder will not count)

#define BUTTON_LONGPRESS_DELAY  500                     // How long to wait before long press is fired
#define BUTTON_DEBOUNCE_DELAY   20                      // Debounce delay (how long to wait before short press is fired)


#include "Arduino.h"

class rotaryClass
{

public:

  int encoderPos;													// current encoder position
  
  boolean button_short_press;										// true if short press was fired
  boolean button_long_press;										// true if long press was fired
  
  rotaryClass();                                					// constructor
  rotaryClass(uint8_t pinA, uint8_t pinB);      					// constructor with encoder pin initialization
  rotaryClass(uint8_t pinA, uint8_t pinB, uint8_t pushPin);			// constructor with encoder and push button pin initialization

  void setInterrupt(uint8_t pinA, uint8_t pinB);  					// set pins after initialization (not need if using constructor with pin initialization)
  void setInterrupt(uint8_t pinA, uint8_t pinB, uint8_t pushPin);  	// set pins after initialization (not need if using constructor with pin initialization)

  int checkEncoder();                             					// run this method in loop to check for encoder move events
  int checkButton();												// run this method in loop to check for encoder push button events

private:
  uint8_t encoderPinA;												// stores channel A pin
  
  uint8_t encoderPinB;												// stores channel B pin
  
  static uint8_t encoderPushPin;									// stores push pin
																	// need to be static, because used in interrupt handler
 
  unsigned long lastIncTime;										// stores moment of last encoder position changed
																	// used to filter out fast moves

  static volatile boolean rotatingA;								// flag for marking rotating event on channel A
  static volatile unsigned long rotatingAStarts;					// saving millis of rotating event on channel A

  static void ICACHE_RAM_ATTR doEncoderA(void);						// interrupt for channel A
  
  static volatile boolean button_Active;							// flag for marking push button event
  static volatile boolean button_Release;							// flag for marking release button event
  static volatile unsigned long button_Timer;  						// saving millis for push button event
  static boolean button_longPressActive;							// flag for marking long push button event

  static void ICACHE_RAM_ATTR doEncoderPush(void);        			// interrupt for push button

};
