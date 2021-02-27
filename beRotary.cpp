/*
  beRotary.cpp - Library for ROTARY ENCODER FOR MENU
  Created by Boris Emchenko.
*/

#include "Arduino.h"
#include "beRotary.h"


  // mandatory for static fields
  volatile boolean rotaryClass::rotatingA;              
  volatile unsigned long rotaryClass::rotatingAStarts;  

  uint8_t rotaryClass::encoderPushPin;
  volatile boolean rotaryClass::button_Active;
  volatile boolean rotaryClass::button_Press;
  volatile boolean rotaryClass::button_Release;
  volatile unsigned long rotaryClass::button_Timer;  
  boolean rotaryClass::button_longPressActive;
  
  /* 
   * Constructor, simple form 
   */
  rotaryClass::rotaryClass() {
    init();    
  }

  /* 
   * Constructor -> encoder pin initialization
   */
  rotaryClass::rotaryClass(uint8_t pinA, uint8_t pinB) {
    init();    
    setInterrupt(pinA, pinB);
  }

  /* 
   * Constructor -> encoder pin & push pin initialization
   */
  rotaryClass::rotaryClass(uint8_t pinA, uint8_t pinB, uint8_t pushPin) {
    init();    
    setInterrupt(pinA, pinB, pushPin);
  }

  /*
   * Init variables for constructors
   */
  void rotaryClass::init(void) {
    debugOutput = false;
    
    rotatingA = false;
    rotatingAStarts = 0;
    
    encoderPos = 0;

    lastIncTime = 0;

    button_Active = false;
    button_Press = false;
    button_Release = false;
    button_longPressActive = false;
    button_short_press = false;
    button_long_press = false;

    button_Timer = 0;

    RotaryDebounceInterval  = ROTARY_DEBOUNCE_INTERVAL_DEFAULT;
    RotarySpeedLimit        = ROTARY_SPEEDLIMIT_INTERVAL_DEFAULT;
    ButtonDebounceInterval  = BUTTON_DEBOUNCE_DELAY_DEFAULT;
    ButtonLongpressInterval = BUTTON_LONGPRESS_DELAY_DEFAULT;
  }

  /* 
   * setInterrupt-> encoder pin
   */
  void rotaryClass::setInterrupt(uint8_t pinA, uint8_t pinB) {
    encoderPinA = pinA;
    encoderPinB = pinB;

    pinMode(encoderPinA, INPUT);
    digitalWrite(encoderPinA, HIGH);       // turn on pull-up resistor
    
    pinMode(encoderPinB, INPUT);
    digitalWrite(encoderPinB, HIGH);       // turn on pull-up resistor

    attachInterrupt(digitalPinToInterrupt (encoderPinA), doEncoderA, CHANGE); 
  }

  /* 
   * setInterrupt-> encoder pin & push pin
   */
  void rotaryClass::setInterrupt(uint8_t pinA, uint8_t pinB, uint8_t pushPin) {
    setInterrupt(pinA, pinB);
    
    encoderPushPin = pushPin;
    pinMode(encoderPushPin, INPUT);
    digitalWrite(encoderPushPin, HIGH);       // turn on pull-up resistor
    attachInterrupt(digitalPinToInterrupt (encoderPushPin), doEncoderPush, CHANGE); 
  }

  /* 
   * setParameters-> change defalut parameters
   */  
   void rotaryClass::setParameters(unsigned long  RotaryDebounce, unsigned long  SpeedLimit, unsigned long ButtonDebounce, unsigned long ButtonLongpress) {
      RotaryDebounceInterval = RotaryDebounce;
      RotarySpeedLimit = SpeedLimit;
      ButtonDebounceInterval = ButtonDebounce;
      ButtonLongpressInterval = ButtonLongpress;
  }

  /*
   *  checkEncoder
   *  poll this function in loop to get current encoder position
   *  returns:
   *  0 - no events
   *  1 - encoder increased
   *  -1 - encoder decreased
   */
  int rotaryClass::checkEncoder()  {
    int ret = 0;
    if (rotatingA) {
      int dir = 0;
      if ( millis() - rotatingAStarts > RotaryDebounceInterval) {
        // When signal changes we wait 2 milliseconds for it to
        // stabilise before reading 
        // (increase this value if there still bounce issues)
        if (digitalRead(encoderPinA) == digitalRead(encoderPinB)) {
          dir = 1;
        }
        else {
          dir = -1;
        }
        if (millis() - lastIncTime > RotarySpeedLimit) {
           encoderPos+=dir;
           lastIncTime = millis();
           ret = dir;
           if (debugOutput) Serial.print(F("<ENCODER POS: "));
           if (debugOutput) Serial.print(encoderPos);
           if (debugOutput) Serial.println(F(">"));
        }
        rotatingA = false; // Reset the flag back to false
      }
    } 
    return ret;
  }

  /*
   *  checkButton
   *  poll this function in loop to get push button status (short press | long press)
   *  returns:
   *  0 - no events
   *  1 - short press event
   *  2 - long press event
   */
  int rotaryClass::checkButton() {
      int ret = 0;
      
      if (button_Press) {
        //Если кнопка была только что нажата, очистить все предыдущие события
        button_long_press = false;
        button_short_press = false;
        button_longPressActive = false;
        button_Press = false;
      }
      
      if (button_Active) {
        //Все еще нажата? проверяем таймер - не пора ли уже переходить на длинное нажатие? 
        //(только если не был до этого флаг уже взведен)
        if ((millis() - button_Timer > ButtonLongpressInterval) && (button_longPressActive == false)) {
          button_longPressActive = true;
        
          /* ЗАПУСКАЕМ СОБЫТИЕ ПО ДЛИННОМУ НАЖАТИЮ */
          if (debugOutput) Serial.println(F("<BUTTON 1: long press>"));
          button_long_press = true;
          ret = 2;
        }
      }
      else if (button_Release)
      {
        //Если было длинной нажатие, то просто сбросим
        if (button_longPressActive == true) {
          button_longPressActive = false;
          button_Timer = millis();
        } else {
          //Debounce check
          if (millis() - button_Timer > ButtonDebounceInterval){
            /* ЗАПУСКАЕМ СОБЫТИЕ ПО КОРОТКОМУ НАЖАТИЮ */
            if (debugOutput) Serial.println(F("<BUTTON 1: short press>"));
            button_short_press = true;
            ret = 1;
          }else{
            /* это дребезг */
            if (debugOutput) Serial.println("дребезг");
          }
        }
        button_Release = false;
      }
      return ret;
  }

  /*
   * INTERRUPT FUNCTION - for rotating encoder, channel A
   */
  void ICACHE_RAM_ATTR rotaryClass::doEncoderA(void) {
    if (!rotatingA) rotatingAStarts = millis();
    rotatingA = true;
    // If a signal change (noise or otherwise) is detected
    // in the rotary encoder, the flag is set to true
  }

  
  /*
   * INTERRUPT FUNCTION - for push button
   */
  void ICACHE_RAM_ATTR rotaryClass::doEncoderPush(void) {
    //Если до этого не была нажата, ставим флаг "нажата" и запускаем отчет таймера
    if (!button_Active && digitalRead(encoderPushPin) == LOW) 
    {
      button_Timer = millis();
      button_Active = true;
      button_Press = true;
      //Serial.println("push");
    }
    else if (button_Active && digitalRead(encoderPushPin) == HIGH)
    {
      button_Release = true;
      button_Active = false;
    }
  }
