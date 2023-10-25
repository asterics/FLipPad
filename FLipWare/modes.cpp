/*
     FLipWare - AsTeRICS Foundation
     For more info please visit: http://www.asterics-academy.net

     Module: modes.cpp: implementation of stick operation and special modes

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; See the GNU General Public License:
   http://www.gnu.org/licenses/gpl-3.0.en.html

*/

#include "FlipWare.h"
#include "modes.h"
#include "gpio.h"
#include "tone.h"
#include "utils.h"
#include "cirque.h"


/**
   static variables for mode handling
 * */
uint8_t strongSipPuffState = STRONG_MODE_IDLE;
uint8_t mouseMoveCount = 0;
unsigned long currentTime;
unsigned long previousTime = 0;

/**
   extern declaration of static variables
   which shall be accessed from other modules
*/
extern int resetPadDirectionStates;

/**
   forward declarations of module-internal functions
*/
void handleMovement(); 

int useAbsolutePadValues() {
  if ((slotSettings.padMode==PADMODE_PAD) || (slotSettings.padMode==PADMODE_ALTERNATIVE_STICKY) || (slotSettings.padMode==PADMODE_ALTERNATIVE))
    return(1);
  return(0);
}

void handleUserInteraction()
{
  static uint8_t pressureRising = 0, pressureFalling = 0;
  static int previousPressure = 512;
  static int waitStable = 0;
  static int checkPairing = 0;
  static uint8_t puffState = SIP_PUFF_STATE_IDLE, sipState = SIP_PUFF_STATE_IDLE;
  static uint8_t puffCount = 0, sipCount = 0;
  int strongDirThreshold;


  // check physical buttons 1,2 and 3
  for (int i = 0; i < NUMBER_OF_PHYSICAL_BUTTONS; i++) // update button press / release events
  handleButton(i, digitalRead(input_map[i]) == LOW ? 1 : 0);

  // check "long-press" of internal button unpairing all BT hosts
  if (digitalRead(input_map[0]) == LOW) {
    checkPairing++;
    if (checkPairing == 800) {   // approx 4 sec.
      makeTone(TONE_BT_PAIRING, 0);
      unpairAllBT();
      checkPairing = 0;
    }
  } else checkPairing = 0;
  
  
  // check sip/puff activities
  if (sensorData.pressure > previousPressure) pressureRising = 1; else pressureRising = 0;
  if (sensorData.pressure < previousPressure) pressureFalling = 1; else pressureFalling = 0;
  previousPressure = sensorData.pressure;

  if (slotSettings.padMode == PADMODE_ALTERNATIVE)
    strongDirThreshold = 0;
  else strongDirThreshold = STRONGMODE_MOUSE_JOYSTICK_THRESHOLD;

  // handle strong sip and puff actions
  switch (strongSipPuffState)  {

    case STRONG_MODE_IDLE:   // IDLE
      if (sensorData.pressure > slotSettings.sp) {
        strongSipPuffState = STRONG_MODE_ENTER_STRONGPUFF;
        makeTone(TONE_ENTER_STRONGPUFF, 0 );
      }
      if (sensorData.pressure < slotSettings.ss ) {
        strongSipPuffState = STRONG_MODE_ENTER_STRONGSIP;
        makeTone(TONE_ENTER_STRONGSIP, 0 );
      }
      break;

    case STRONG_MODE_ENTER_STRONGPUFF:     // puffed strong, wait for release
      if (sensorData.pressure < slotSettings.tp)
        waitStable++;
      else waitStable = 0;
      if (waitStable >= STRONGMODE_STABLETIME)
        strongSipPuffState = STRONG_MODE_STRONGPUFF_ACTIVE;
      break;

    case STRONG_MODE_STRONGPUFF_ACTIVE:    // strong puff mode active
        waitStable++;
        if (waitStable > STRONGMODE_EXIT_TIME) { // no stick movement occurred: perform strong puff action
          waitStable = 0;
          handlePress(STRONGPUFF_BUTTON);
          handleRelease(STRONGPUFF_BUTTON);
          strongSipPuffState = STRONG_MODE_RETURN_TO_IDLE;
        }
      break;

    case STRONG_MODE_ENTER_STRONGSIP:   // sipped strong, wait for release
      if (sensorData.pressure > slotSettings.ts)
        waitStable++;
      else waitStable = 0;
      if (waitStable >= STRONGMODE_STABLETIME)
        strongSipPuffState = STRONG_MODE_STRONGSIP_ACTIVE;
      break;

    case STRONG_MODE_STRONGSIP_ACTIVE:   // strong sip mode active
        waitStable++;
        if (waitStable > STRONGMODE_EXIT_TIME) {  // no stick movement occurred: perform strong sip action
          waitStable = 0;
          handlePress(STRONGSIP_BUTTON);
          handleRelease(STRONGSIP_BUTTON);
          strongSipPuffState = STRONG_MODE_RETURN_TO_IDLE;
        }
      break;

    case STRONG_MODE_RETURN_TO_IDLE:
      waitStable++;
      if (waitStable > STRONGMODE_IDLE_TIME)
      {
        waitStable = 0;
        strongSipPuffState = STRONG_MODE_IDLE;
        initDebouncers();
        puffState = 0; sipState = 0;
      }
      break;
    default: break;
  }

  if (strongSipPuffState == STRONG_MODE_IDLE)
  {
    //handle normal sip and puff actions
    switch (puffState)  {
      case SIP_PUFF_STATE_IDLE:
        if (sensorData.pressure > slotSettings.tp)   // handle single puff actions
        {
          makeTone(TONE_INDICATE_PUFF, 0);
          puffState = SIP_PUFF_STATE_STARTED; puffCount = 0;
        }
        break;

      case SIP_PUFF_STATE_STARTED:
        if (!pressureRising)
        {
          if (puffCount++ > SIP_PUFF_SETTLE_TIME)
          {
            puffCount = MIN_HOLD_TIME;
            handlePress(PUFF_BUTTON);
            puffState = SIP_PUFF_STATE_PRESSED;
          }
        } else if (puffCount) puffCount--;
        break;

      case SIP_PUFF_STATE_PRESSED:
        if (puffCount) puffCount--;
        if ((sensorData.pressure < slotSettings.tp) && (!puffCount)) {
          handleRelease(PUFF_BUTTON);
          puffState = 0;
        }
    }

    switch (sipState)  {
      case SIP_PUFF_STATE_IDLE:
        if (sensorData.pressure < slotSettings.ts)   // handle single sip actions
        {
          makeTone(TONE_INDICATE_SIP, 0);
          sipState = SIP_PUFF_STATE_STARTED; sipCount = 0;
        }
        break;

      case SIP_PUFF_STATE_STARTED:
        if (!pressureFalling)
        {
          if (sipCount++ > SIP_PUFF_SETTLE_TIME)
          {
            sipCount = MIN_HOLD_TIME;
            handlePress(SIP_BUTTON);
            sipState = SIP_PUFF_STATE_PRESSED;
          }
        } else if (sipCount) sipCount--;
        break;

      case SIP_PUFF_STATE_PRESSED:
        if (sipCount) sipCount--;
        if ((sensorData.pressure > slotSettings.ts) && (!sipCount)) {
          handleRelease(SIP_BUTTON);
          sipState = 0;
        }
    }

    // now handle pad movements!
    handleMovement();
  }
}

/**
   @name getStickAccelFactor
   @brief calculates acceleration for mouse pointer movements in stick mode
          according to sensordata and acceleration settings 
   @return float value of current acceleration factor
*/
float getStickAccelFactor() {
  static float accelFactor=0;
  static int xo = 0, yo = 0;
  static float accelMaxForce = 0, lastAngle = 0;

  if (sensorData.force == 0) {
    accelFactor = 0;
    accelMaxForce = 0;
    lastAngle = 0;
  }
  else {
    if (sensorData.force > accelMaxForce) accelMaxForce = sensorData.force;
    if (sensorData.force > accelMaxForce * 0.8f) {
      if (accelFactor < 1.0f)
        accelFactor += ((float)slotSettings.ac / 5000000.0f);
    }
    else if (accelMaxForce > 0) accelMaxForce *= 0.99f;

    if (sensorData.force < accelMaxForce * 0.6f)  accelFactor *= 0.995f;
    if (sensorData.force < accelMaxForce * 0.4f)  accelFactor *= 0.99f;

    float dampingFactor = fabsf(sensorData.x - xo) + fabsf(sensorData.y - yo);
    accelFactor *= (1.0f - dampingFactor / 1000.0f);
    lastAngle = sensorData.angle;
    xo = sensorData.x; yo = sensorData.y;
  }
  return(accelFactor);
}


/**
   @name getPadAccelFactor
   @brief calculates acceleration for mouse pointer movements in pad mode
          according to sensordata and acceleration settings 
   @return float value of current acceleration factor
*/
float getPadAccelFactor() {
  static float accelFactor=0;
  static float distance=0;

  if ((sensorData.padState==CIRQUE_STATE_LIFTOFF) || (sensorData.padState==CIRQUE_STATE_HOVERING)) {
    accelFactor = 0;
    distance=0;
  }
  else {
    distance += (fabsf(sensorData.x) + fabsf(sensorData.y)) / 700000;
    accelFactor = 0.0005+distance;
    if (accelFactor>0.01) accelFactor=0.01;
  }
  return(accelFactor);
}

/**
   @name acceleratedMouseMove
   @brief performs accelerated mouse pointer movement
   @param accelFactor current acceleration factor
   @return none
*/
void acceleratedMouseMove(float accelFactor, uint8_t use_maxSpeed) {
  static float accumXpos = 0;
  static float accumYpos = 0;

  float moveValX = sensorData.x * (float)slotSettings.ax * accelFactor;
  float moveValY = sensorData.y * (float)slotSettings.ay * accelFactor;

  if (use_maxSpeed) { 
    float actSpeed =  __ieee754_sqrtf (moveValX * moveValX + moveValY * moveValY);
    float max_speed = (float)slotSettings.ms / 10.0f;
    if (actSpeed > max_speed) {
      moveValX *= (max_speed / actSpeed);
      moveValY *= (max_speed / actSpeed);
    }
  }

/*
  if ((sensorData.x!=0) && (sensorData.y!=0)) {
    Serial.print ("x=");Serial.print (sensorData.x);  Serial.print ("  y=");Serial.print (sensorData.y);
    Serial.print (" / moveValX=");Serial.print (moveValX);  Serial.print ("  moveValY=");Serial.print (moveValY);
    Serial.println(" ");
  }
*/

  accumXpos += moveValX;
  accumYpos += moveValY;

  int xMove = (int)accumXpos;
  int yMove = (int)accumYpos;

  if ((xMove != 0) || (yMove != 0)) {
    mouseMove(xMove, yMove);
  }

  accumXpos -= xMove;
  accumYpos -= yMove;
}

/**
   @name scaleJoystickAxis
   @brief scales/crops coordinate values to joystick coordinates (0-1023, centered around 512)
   @param val x/y coordinate value to be scaled
   @return integer value for joystick coordinate
*/
int scaleJoystickAxis (float val) {
  int axis = 512 + (int) (val / 50);
  if (axis < 0) axis = 0; 
  else if (axis > 1023) axis = 1023;
  return (axis);
}

/**
   @name handleMovement
   @brief performs an action or movement according to the current sensorData and mode of operation
   @return none
*/
void handleMovement() 
{
  static int upState=0,downState=0,leftState=0,rightState=0;
  
  if ((sensorData.autoMoveX != 0) || (sensorData.autoMoveY != 0)) // handle movement induced by button actions
  {
    if (mouseMoveCount++ % 4 == 0)
      mouseMove(sensorData.autoMoveX, sensorData.autoMoveY);
  }

  switch (slotSettings.padMode) {  

    case PADMODE_MOUSE:   // handle mouse stick mode
      acceleratedMouseMove(getStickAccelFactor(), 1);
      break; 
     
    case PADMODE_PAD:   // handle mouse pad mode 
      acceleratedMouseMove(getPadAccelFactor(), 0);
      break;

    case PADMODE_ALTERNATIVE:  // handle alternative actions (non-sticky)
      if (sensorData.forceRaw>sensorData.deadZone) {
       switch (sensorData.dir) {   // non-sticky direction keys
          case DIR_E: leftState=1; rightState=upState=downState=0;break;
          case DIR_NE: upState=leftState=1; rightState=downState=0; break;
          case DIR_N: upState=1;  downState=leftState=rightState=0; break;
          case DIR_NW: upState=rightState=1; leftState=downState=0; break;
          case DIR_W: rightState=1; leftState=upState=downState=0; break;
          case DIR_SW: downState=rightState=1; upState=leftState=0; break;
          case DIR_S: downState=1; upState=leftState=rightState=0;break;
          case DIR_SE: downState=leftState=1; upState=rightState=0;break;        
        }
      }
      if (resetPadDirectionStates) {
        upState=downState=leftState=rightState=resetPadDirectionStates=0;
      }
      
      handleButton(UP_BUTTON,  upState);
      handleButton(DOWN_BUTTON,  downState);
      handleButton(LEFT_BUTTON,  leftState);
      handleButton(RIGHT_BUTTON,  rightState); 
    break;
      
    case PADMODE_ALTERNATIVE_STICKY:    // handle alternative actions (sticky mode)
      if (sensorData.forceRaw>sensorData.deadZone) {
        switch (sensorData.dir) {   // sticky direction keys
          case DIR_E: leftState=1; rightState=0; break;
          case DIR_NE: upState=leftState=1; rightState=downState=0; break;
          case DIR_N: upState=1;  downState=0; break;
          case DIR_NW: upState=rightState=1; leftState=downState=0; break;
          case DIR_W: rightState=1; leftState=0; break;
          case DIR_SW: downState=rightState=1; upState=leftState=0; break;
          case DIR_S: downState=1; upState=0;break;
          case DIR_SE: downState=leftState=1; upState=rightState=0;break;        
        } 
      }
      if (resetPadDirectionStates) {
        upState=downState=leftState=rightState=resetPadDirectionStates=0;
      }
      
      handleButton(UP_BUTTON,  upState);
      handleButton(DOWN_BUTTON,  downState);
      handleButton(LEFT_BUTTON,  leftState);
      handleButton(RIGHT_BUTTON,  rightState);    
      break; 

    case PADMODE_JOYSTICK_XY:
      Joystick.X (scaleJoystickAxis((float)sensorData.x * slotSettings.ax));
      Joystick.Y (scaleJoystickAxis((float)sensorData.y * slotSettings.ay));
      break;

    case PADMODE_JOYSTICK_ZR:
      Joystick.Z (scaleJoystickAxis((float)sensorData.x * slotSettings.ax));
      Joystick.Zrotate (scaleJoystickAxis((float)sensorData.y * slotSettings.ay));
      break;

    case PADMODE_JOYSTICK_SLIDERS:
      Joystick.sliderLeft (scaleJoystickAxis((float)sensorData.x * slotSettings.ax));
      Joystick.sliderRight (scaleJoystickAxis((float)sensorData.y * slotSettings.ay));
      break;
  }
}
