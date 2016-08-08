//-----------------------------------------------------------------------------
// File: Config.h
// Desc: Compilation configuration file. Defines user-configurable settings
//       as well as project-wide development #defines for configuration.
//-----------------------------------------------------------------------------
#ifndef config_h
#define config_h

//-----------------------------------------------------------------------------
// These are things you can change before you compile, to enable/disable
// features.
//

// Button hold time for pause/unpause
#define BUTTON_HOLD_PAUSE_THRESH    1500    // 1.5 second button hold required to pause/unpause tracking.

// Fatshark headset headtracker module support. Set to 1 to enable.
// See: http://www.rcgroups.com/forums/showpost.php?p=23051198&postcount=573
#define FATSHARK_HT_MODULE 1

// 433 Tx Module wireless transmitter
// UnComment  to enable the Wireless channel transmitter 
#define ENABLE_433_TX
// A user generated ID for the 433 TX (random number)
#define TX_ID       12345         //Max Number: 32000


//
// -- End of user-configurable parameters.
//-----------------------------------------------------------------------------

//=============================================================================

//-----------------------------------------------------------------------------
// The user generally won't need to touch any of the following
//

// Firmware Version, e.g. X.YY
#define FIRMWARE_VERSION_FLOAT  1.04    // 2 decimal places

// Number of PPM channels out. 1 - 12 channels supported. 
#define NUMBER_OF_CHANNELS 8

// Define for extra debug serial info
#define DEBUG 0

// Output serial data to host evern X frames
#define SERIAL_OUTPUT_FRAME_INTERVAL    10

// Serial communication speed. 
#define SERIAL_BAUD 57600

// Sensor board update-rate. Not done yet. 
#define UPDATE_RATE 64

// Dead-time between each channel in the PPM-stream. 
#define DEAD_TIME 300

// Sets the frame-length .
#define FRAME_LENGTH 25000
// Set the end-frame Lenght in ms
#define END_FRAME 5000

// TOP (timer rollover) used for PPM pulse time measurement
#define TOP (5003 + NUMBER_OF_CHANNELS * 5000)

// Set to 0, stored gyro calibration is used. If 1, gyro is calibrated at powerup  
#define ALWAYS_CAL_GYRO 0

// Center/pause input button pin number
#define BUTTON_INPUT 11

// PPM Output PIN
#define PPM_PIN 9

// Arduino LED
#define  ARDUINO_LED 13

// Pin definition for LED and buzzer (Fatshark goggles)
#if FATSHARK_HT_MODULE
    #define  BUZZER      4
#endif

// Pin definition for the 433 Mhz TX Add On
#ifdef ENABLE_433_TX
    #define TX_433_PIN    12
    #define TX_433_BAUD   2000
#endif

#define PPM_IN_MIN 1000 // 0.5 ms
#define PPM_IN_MAX 4000 // 2 ms

// Settings stuff
//
#define HT_TILT_REVERSE_BIT     0x01
#define HT_ROLL_REVERSE_BIT     0x02
#define HT_PAN_REVERSE_BIT      0x04

#endif
