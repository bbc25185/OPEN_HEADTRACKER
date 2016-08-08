//-----------------------------------------------------------------------------
// Original project by Dennis Frie - 2012 - Dennis.frie@gmail.com
// Discussion: http://www.rcgroups.com/forums/showthread.php?t=1677559
//
// Other contributors to this code:
//  Mark Mansur (Mangus on rcgroups)
//  
// Version history:
// - 0.01 - 0.08 - Dennis Frie - preliminary releases
// - 1.01 - April 2013 - Mark Mansur - code clean-up and refactoring, comments
//      added. Added pause functionality, added settings retrieval commands.
//      Minor optimizations.
// - 1.99 - August 2016 - WildWolf - Added Wireless Channel Trasmitter with a 433 Mhz Module
//-----------------------------------------------------------------------------

#include <Wire.h>
#include "config.h"
#include "sensors.h"
#include <EEPROM.h>
#include <FlexiTimer2.h>
#ifdef ENABLE_433_TX
  #include <VirtualWire.h>
#endif


// Local file variables
//
int frameNumber = 0;		    // Frame count since last debug serial output

char serial_data[101];          // Array for serial-data 
unsigned char serial_index = 0; // How many bytes have been received?
char string_started = 0;        // Only saves data if string starts with right byte
unsigned char channel_mapping[13];
char read_sensors = 0;
long channel_value[13] = {2100,2100,2100,2100,2100,2100,2100,2100,2100,2100,2100,2100,2100};
unsigned char PpmIn_PpmOut[13] = {0,1,2,3,4,5,6,7,8,9,10,11,12};

char outputMag = 0;             // Stream magnetometer data to host
char outputAcc = 0;             // Stream accelerometer data to host
char outputMagAcc = 0;          // Stream mag and accell data (for calibration on PC)
char outputTrack = 0;	        // Stream angle data to host

// Keep track of button press
char lastButtonState = 0;           // 0 is not pressed, 1 is pressed
unsigned long buttonDownTime = 0;   // the system time of the press
unsigned long LAST_READ_SENSOR = 0; // Last system time of readed sensor
char pauseToggled = 0;              // Used to make sure we toggle pause only once per hold
char ht_paused = 1;
volatile byte CHANNEL_PPM_SEND = 0; //Channel OutPut PPM Tracking
boolean SENDED_433 = LOW;


// External variables (defined in other files)
//
extern char resetValues;   
extern char tiltInverse;
extern char rollInverse;
extern char panInverse;

// Settings (Defined in sensors.cpp)
//
extern float tiltRollBeta;
extern float panBeta;
extern float gyroWeightTiltRoll;
extern float GyroWeightPan;
extern int servoPanCenter;
extern int servoTiltCenter;
extern int servoRollCenter;
extern int panMaxPulse;
extern int panMinPulse;
extern int tiltMaxPulse;
extern int tiltMinPulse;
extern int rollMaxPulse;
extern int rollMinPulse;
extern float panFactor;
extern float tiltFactor;  
extern float rollFactor;
extern unsigned char servoReverseMask;
extern unsigned char htChannels[];
extern float gyroOff[];
extern float magOffset[];
extern int accOffset[]; 
// End settings   

//--------------------------------------------------------------------------------------
// Func: setup
// Desc: Called by Arduino framework at initialization time. This sets up pins for I/O,
//       initializes sensors, etc.
//--------------------------------------------------------------------------------------
void setup()
{
    Serial.begin(SERIAL_BAUD);

    PREPARE_PIN();
    digitalWrite(ARDUINO_LED, HIGH);

#ifdef ENABLE_433_TX
    PREPARE_TX_MODULE();
#endif

    // Give it time to be noticed, then turn it off
    delay(200);
    digitalWrite(ARDUINO_LED, LOW);

#if FATSHARK_HT_MODULE
    digitalWrite(BUZZER, LOW);
#endif
 
    Wire.begin();               // Start I2C

    // If the device have just been programmed, write initial config/values to EEPROM:
    if (EEPROM.read(0) != 8)
    {
//#if (DEBUG)
        Serial.println("New board - saving default values!");
//#endif    
    
        InitSensors();
#if (ALWAYS_CAL_GYRO)    
        SetGyroOffset();
#endif     

        SaveSettings();
        SaveMagData();
        SaveAccelData();
    }
 
    GetSettings();                 // Get settings saved in EEPROM
    InitSensors();                // Initialize I2C sensors
    CalibrateMag();
    ResetCenter(); 
    PREPARE_PPM_OUT();
}

//--------------------------------------------------------------------------------------
// Func: loop
// Desc: Called by the Arduino framework once per frame. Represents main program loop.
//--------------------------------------------------------------------------------------
void loop()
{  
    //Check the pressing of the button to stop and start the hedtracker
    CHECK_BUTTON();

    //Check if it is time to read the sensor
    if ((read_sensors ==0) && ((millis() - LAST_READ_SENSOR) > (1000/UPDATE_RATE)))
    {
      LAST_READ_SENSOR = millis();
      read_sensors = 1;
    }
    
    // Communication with GUI
    GUI_CONTROL();


    // if "read_sensors" flag is set high, read sensors and update
    if (read_sensors == 1 && ht_paused == 0)
    {
        UpdateSensors();
        GyroCalc();
        AccelCalc();
        MagCalc();
        FilterSensorData();    
               
        // Only output this data every X frames.
        if (frameNumber++ >= SERIAL_OUTPUT_FRAME_INTERVAL)
        {
            if (outputTrack == 1)
            {
                trackerOutput();
            }
            else if (outputMagAcc == 1)
            {
                calMagAccOutput();
            }
            else if (outputMag == 1)
            {
                calMagOutput(); 
            }
            else if (outputAcc == 1)
            {
                calAccOutput();
            }
            frameNumber = 0; 
        }
      
        // Will first update read_sensors when everything is done.  
        read_sensors = 0;
    }

    #ifdef ENABLE_433_TX
      //If the PPM Out is in Sync time, Send the channel throught the 433 module... this mode sync the 433Tx with PPM
      if (CHANNEL_PPM_SEND == 0) {
        if (!SENDED_433) {
          SEND_TX();
          SENDED_433 = HIGH;
        }
      }
      else {SENDED_433 = LOW;}
          
    #endif

}
