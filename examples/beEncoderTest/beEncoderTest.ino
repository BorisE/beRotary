/* read a rotary encoder with interrupts
   Encoder hooked up with common to GROUND,
   encoderPinA to pin D3, encoderPinB to pin D4
   it doesn't matter which encoder pin you use for A or B
   but pinA should support interrupts
   pushPin should also support interrupts

   uses Arduino pull-ups on A & B channel outputs
   turning on the pull-ups saves having to hook up resistors
   to the A & B channel outputs
*/
#include <beRotary.h>

#define encoder0PinA    D3
#define encoder0PinB    D4
#define encoderPushPin  D8

// rotaryClass encoderObj();                                               // need to use setInterrupt before use
// rotaryClass encoderObj(encoder0PinA, encoder0PinB);                     // without push button
rotaryClass encoderObj(encoder0PinA, encoder0PinB, encoderPushPin);

void setup() {
  Serial.begin(115200);

  //encoderObj.setInterrupt(encoder0PinA, encoder0PinB, encoderPushPin);  // needed if was used constructor without pin init

  Serial.println("start");                // a personal quirk
}

void loop() {
  // chech encoder rotation
  if (encoderObj.checkEncoder()) {
     Serial.println(encoderObj.encoderPos);
  }

  // chech encoder buttion press
  if (encoderObj.checkButton()) {
    Serial.print ("Pushed: ");
    Serial.print ( encoderObj.button_short_press ? "short" : ( encoderObj.button_long_press ? "long" : ""));
    Serial.println();
  }
}
