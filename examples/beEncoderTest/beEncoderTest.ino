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

// rotaryClass encoderObj;                                               // need to use setInterrupt before use
// rotaryClass encoderObj(encoder0PinA, encoder0PinB);                     // without push button
rotaryClass encoderObj(encoder0PinA, encoder0PinB, encoderPushPin);

void setup() {
  Serial.begin(115200);

  //encoderObj.setInterrupt(encoder0PinA, encoder0PinB, encoderPushPin);  // needed if was used constructor without pin init
  encoderObj.debugOutput = true;                                          // some serial debug

  Serial.println("Start");
  
  Serial.println("CURRENT PARAMETERS: ");
  Serial.print("debugOutput: ");
  Serial.println(encoderObj.debugOutput);
  Serial.print("RotaryDebounceInterval: ");
  Serial.println(encoderObj.RotaryDebounceInterval);
  Serial.print("RotarySpeedLimit: ");
  Serial.println(encoderObj.RotarySpeedLimit);
  Serial.print("ButtonDebounceInterval: ");
  Serial.println(encoderObj.ButtonDebounceInterval);
  Serial.print("ButtonLongpressInterval: ");
  Serial.println(encoderObj.ButtonLongpressInterval);

}

void loop() {
  // chech encoder rotation
  int encoder = encoderObj.checkEncoder();
  if (encoder) {
     Serial.print("Current pos: ");
     Serial.print(encoderObj.encoderPos);
     Serial.print(" [");
     Serial.print(encoder);
     Serial.println("]");
  }

  // chech encoder buttion press
  if (encoderObj.checkButton()) {
    Serial.print ("Pushed: ");
    Serial.print ( encoderObj.button_short_press ? "short" : ( encoderObj.button_long_press ? "long" : ""));
    Serial.println();
  }
}
