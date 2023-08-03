
/*
     FLipWare - AsTeRICS Foundation
     For more info please visit: http://www.asterics-academy.net

     Module: FlipWare.h  - main header file

        This firmware allows control of HID functions via FLipPad module and/or AT-commands
        For a description of the supported commands see: commands.h

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; See the GNU General Public License:
   http://www.gnu.org/licenses/gpl-3.0.en.html

*/


#ifndef _FLIPWARE_H_
#define _FLIPWARE_H_

#include <Arduino.h>
#include <Wire.h>
#include <EEPROM.h>
#include <string.h>
#include <stdint.h>
#include "commands.h"
#include "eeprom.h"
#include "buttons.h"
#include "bluetooth.h"
#include "hid_hal.h"

#define VERSION_STRING "v1.2"

//  V1.2: added check for MPRLS pressure sensor
//        changed back OLed Display orientation for longer cables
//        reduced maxSpeed

//  V1.1: changed orientation of Oled Display for shorter trackpad cable length

//  V1.0: first FlipPad firmware with support for cirque trackpads



// Optional Debug Output Control

// #define DEBUG_OUTPUT_FULL      // if full debug output is desired
// #define DEBUG_OUTPUT_BASIC     // if basic debug output is desired (for eeprom)

/**
   global constant definitions
*/
#define UPDATE_INTERVAL     5    // update interval for performing HID actions (in milliseconds)
#define DEFAULT_CLICK_TIME  8    // time for mouse click (loop iterations from press to release)

// Analog input pins (4FSRs + 1 pressure sensor, optional 1 hall sensor for FlipPad configuration)
#define PRESSURE_SENSOR_PIN A0
#define HALL_SENSOR_PIN     A1

// RAM buffers and memory constraints
#define WORKINGMEM_SIZE         300    // reserved RAM for working memory (command parser, IR-rec/play)
#define MAX_KEYSTRING_LEN (WORKINGMEM_SIZE-3)   // maximum length for AT command parameters
#define MAX_NAME_LEN  15               // maximum length for a slotname or ir name
#define MAX_KEYSTRINGBUFFER_LEN 500    // maximum length for all string parameters of one slot

// direction identifiers
#define DIR_E   1   // east
#define DIR_NE  2   // north-east
#define DIR_N   3   // north
#define DIR_NW  4   // north-west
#define DIR_W   5   // west
#define DIR_SW  6   // sout-west
#define DIR_S   7   // south
#define DIR_SE  8   // south-east

/**
   SlotSettings struct
   contains parameters for current slot
*/
struct SlotSettings {

  char slotName[MAX_NAME_LEN];   // slotname
  uint16_t keystringBufferLen;   
  
  uint8_t  padMode;  // alternative(0), mouse(1), joystick (2,3,4), pad(5) or alternative_sticky (6) mode
  uint8_t  ax;     // acceleration x
  uint8_t  ay;     // acceleration y
  int16_t  dx;     // deadzone x
  int16_t  dy;     // deadzone y
  uint16_t ms;     // maximum speed
  uint16_t ac;     // acceleration time
  uint16_t ts;     // threshold sip
  uint16_t tp;     // threshold puff
  uint8_t  ws;     // wheel stepsize
  uint16_t sp;     // threshold strong puff
  uint16_t ss;     // threshold strong sip
  uint8_t  gv;     // tap threshold time (0-100, 0=disable)
  uint8_t  rv;     // trackpad sensitivity 
  uint8_t  gh;     // tap-gesture threshold time (0-100, 0=disable)
  uint8_t  rh;     // TBD
  int16_t  cx;     // calib x
  int16_t  cy;     // calib y
  uint16_t ro;     // orientation (0,90,180,270)
  uint8_t  bt;     // bt-mode (0,1,2)
};

/**
   SensorData struct
   contains working data of sensors (raw and processed values)
*/
struct SensorData {
  int x, y, xRaw,yRaw;
  int pressure;
  float deadZone, force, forceRaw, angle;
  uint8_t dir;
  int8_t autoMoveX,autoMoveY;
  int up, down, left, right;
  uint8_t calib_now;
  int16_t  cx, cy;
  int xDriftComp, yDriftComp;
  int xLocalMax, yLocalMax;
  uint8_t ignorePressure;
};

/**
   extern declarations of functions and data structures 
   which can be accessed from different modules
*/
extern char moduleName[];
extern uint8_t actSlot;
extern uint8_t addonUpgrade;
extern struct SensorData sensorData;
extern struct SlotSettings slotSettings; 
extern const struct SlotSettings defaultSlotSettings;
extern uint8_t workingmem[WORKINGMEM_SIZE];            // working memory  (command parser, IR-rec/play)
extern char keystringBuffer[MAX_KEYSTRINGBUFFER_LEN];  // storage for all button string parameters of a slot

/**
   set the correct strcpy/strcmp functions for TeensyLC / ARM)
*/
#define strcpy_FM   strcpy
#define strcmp_FM   strcmp
typedef char* uint_farptr_t_FM;

#endif
