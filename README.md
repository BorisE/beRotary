# beRotary
/******************************************************************************************************************
 *                                 ROTARY ENCODER FOR MENU
 *                            private library by Boris Emchenko
 * ****************************************************************************************************************
   some great ideas were taken from https://playground.arduino.cc/Main/RotaryEncoders/#Example12
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
