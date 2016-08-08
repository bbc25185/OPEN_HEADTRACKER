//--------------------------------------------------------------------------------------
// Func: DebugOutput
// Desc: Outputs useful device/debug information to the serial port.
//--------------------------------------------------------------------------------------
void DebugOutput()
{
    Serial.println();  
    Serial.println();
    Serial.println();
    Serial.println("------ Debug info------");

    Serial.print("FW Version: ");
    Serial.println(FIRMWARE_VERSION_FLOAT, 2);
    
    Serial.print("tiltRollBeta: ");
    Serial.println(tiltRollBeta); 

    Serial.print("panBeta: ");
    Serial.println(panBeta); 
 
    Serial.print("gyroWeightTiltRoll: ");
    Serial.println(gyroWeightTiltRoll); 

    Serial.print("GyroWeightPan: ");
    Serial.println(GyroWeightPan); 

    Serial.print("servoPanCenter: ");
    Serial.println(servoPanCenter); 
 
    Serial.print("servoTiltCenter: ");
    Serial.println(servoTiltCenter); 

    Serial.print("servoRollCenter: ");
    Serial.println(servoRollCenter); 

    Serial.print("tiltFactor: ");
    Serial.println(tiltFactor); 

    Serial.print("panFactor: ");
    Serial.println(panFactor);  
 
    Serial.print("Gyro offset stored ");
    Serial.print(gyroOff[0]);
    Serial.print(",");   
    Serial.print(gyroOff[1]);
    Serial.print(",");      
    Serial.println(gyroOff[2]);    
 
    Serial.print("Mag offset stored ");
    Serial.print(magOffset[0]);
    Serial.print(",");   
    Serial.print(magOffset[1]);
    Serial.print(",");      
    Serial.println(magOffset[2]);
 
    Serial.print("Acc offset stored ");
    Serial.print(accOffset[0]);
    Serial.print(",");   
    Serial.print(accOffset[1]);
    Serial.print(",");      
    Serial.println(accOffset[2]);
 
    SensorInfoPrint();    
}

//--------------------------------------------------------------------------------------
// Func: GUI_CONTROL
// Desc: Used for communication with GUI
//--------------------------------------------------------------------------------------
void GUI_CONTROL()
{
      // All this is used for communication with GUI 
    //
    if (Serial.available())
    {
        if (string_started == 1)
        {
            // Read incoming byte
            serial_data[serial_index++] = Serial.read();
           
            // If string ends with "CH" it's channel configuration, that have been received.
            // String must always be 12 chars/bytes and ending with CH to be valid. 
            if (serial_index == 14 &&
                serial_data[serial_index-2] == 'C' &&
                serial_data[serial_index-1] == 'H' )
            {
                // To keep it simple, we will not let the channels be 0-initialized, but
                // start from 1 to match actual channels. 
                for (unsigned char i = 0; i < 13; i++)
                {
                    channel_mapping[i + 1] = serial_data[i] - 48;
                  
                    // Update the dedicated PPM-in -> PPM-out array for faster performance.
                    if ((serial_data[i] - 48) < 14)
                    {
                        PpmIn_PpmOut[serial_data[i]-48] = i + 1;
                    }
                }
               
                Serial.println("Channel mapping received");
               
               // Reset serial_index and serial_started
               serial_index = 0;
               string_started = 0;
            }
            
            // Configure headtracker
            else if (serial_data[serial_index-2] == 'H' &&
                     serial_data[serial_index-1] == 'E')
            {
                // HT parameters are passed in from the PC in this order:
                //
                // 0 tiltRollBeta      
                // 1 panBeta       
                // 2 gyroWeightTiltRoll    
                // 3 GyroWeightPan 
                // 4 tiltFactor        
                // 5 panFactor          
                // 6 rollFactor
                // 7 servoReverseMask
                // 8 servoPanCenter
                // 9 panMinPulse 
                // 10 panMaxPulse
                // 11 servoTiltCenter
                // 12 tiltMinPulse
                // 13 tiltMaxPulse
                // 14 servoRollCenter
                // 15 rollMinPulse
                // 16 rollMaxPulse
                // 17 htChannels[0]  // pan            
                // 18 htChannels[1]  // tilt 
                // 19 htChannels[2]  // roll         
             
                // Parameters from the PC client need to be scaled to match our local
                // expectations

                Serial.println("HT config received:");
           
                int valuesReceived[20] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
                int comma_index = 0;

                for (unsigned char k = 0; k < serial_index - 2; k++)
                {
                    // Looking for comma
                    if (serial_data[k] == 44)
                    {
                        comma_index++;
                    }
                    else
                    {
                        valuesReceived[comma_index] = valuesReceived[comma_index] * 10 + (serial_data[k] - 48);
                    }
             
#if (DEBUG)
                    Serial.print(serial_data[k]);
#endif
                }

#if (DEBUG)
                Serial.println();
                for (unsigned char k = 0; k < comma_index+1; k++)
                {
                    Serial.print(valuesReceived[k]); 
                    Serial.print(",");           
                }
                Serial.println();
#endif

                tiltRollBeta  = (float)valuesReceived[0] / 100;  
                panBeta       = (float)valuesReceived[1] / 100;
                gyroWeightTiltRoll = (float)valuesReceived[2] / 100;
                GyroWeightPan = (float)valuesReceived[3] / 100;
                tiltFactor    = (float)valuesReceived[4] / 10;         
                panFactor     = (float)valuesReceived[5] / 10;          
                rollFactor    = (float)valuesReceived[6] / 10;   

                servoReverseMask = (unsigned char)valuesReceived[7];

                tiltInverse = 1;
                rollInverse = 1;
                panInverse = 1;           
                
                if ((servoReverseMask & HT_PAN_REVERSE_BIT) != 0)
                {
                    panInverse = -1;
                }
                if ((servoReverseMask & HT_ROLL_REVERSE_BIT) != 0)
                {
                    rollInverse = -1; 
                }
                if ((servoReverseMask & HT_TILT_REVERSE_BIT) != 0)
                {
                    tiltInverse = -1;
                }

                servoPanCenter = valuesReceived[8];
                panMinPulse = valuesReceived[9];
                panMaxPulse = valuesReceived[10];         
         
                servoTiltCenter = valuesReceived[11];
                tiltMinPulse = valuesReceived[12];
                tiltMaxPulse = valuesReceived[13];         

                servoRollCenter = valuesReceived[14];
                rollMinPulse = valuesReceived[15];
                rollMaxPulse = valuesReceived[16];              
     
                htChannels[0] = valuesReceived[17];                   
                htChannels[1] = valuesReceived[18];              
                htChannels[2] = valuesReceived[19];                       

                Serial.println(htChannels[0]);
                Serial.println(htChannels[1]);
                Serial.println(htChannels[2]);                
        
                SaveSettings();

                serial_index = 0;
                string_started = 0;
            } // end configure headtracker
          
            // Debug info
            else if (serial_data[serial_index-5] == 'D' &&
                     serial_data[serial_index-4] == 'E' &&
                     serial_data[serial_index-3] == 'B' &&
                     serial_data[serial_index-2] == 'U' &&
                     serial_data[serial_index-1] == 'G')
            {  
                DebugOutput();
                serial_index = 0;
                string_started = 0; 
            }

            // Firmware version requested
            else if (serial_data[serial_index-4] == 'V' &&
                     serial_data[serial_index-3] == 'E' &&
                     serial_data[serial_index-2] == 'R' &&
                     serial_data[serial_index-1] == 'S')
            {
                Serial.print("FW: ");
                Serial.println(FIRMWARE_VERSION_FLOAT, 2);
                serial_index = 0;
                string_started = 0; 
            }
          
            // Start mag and accel data stream
            else if (serial_data[serial_index-4] == 'C' &&
                     serial_data[serial_index-3] == 'M' &&
                     serial_data[serial_index-2] == 'A' &&
                     serial_data[serial_index-1] == 'S')
            {  
                outputMagAcc = 1;
                outputMag = 0;
                outputAcc = 0;
                outputTrack = 0;
                serial_index = 0;
                string_started = 0;
            }        

            // Stop mag and accel data stream
            else if (serial_data[serial_index-4] == 'C' &&
                     serial_data[serial_index-3] == 'M' &&
                     serial_data[serial_index-2] == 'A' &&
                     serial_data[serial_index-1] == 'E')
            {  
                outputMagAcc = 0;
                outputMag = 0;
                outputTrack = 0;
                outputAcc = 0;
                serial_index = 0;
                string_started = 0;
            }        

            // Start magnetometer data stream
            else if (serial_data[serial_index-4] == 'C' &&
                     serial_data[serial_index-3] == 'A' &&
                     serial_data[serial_index-2] == 'S' &&
                     serial_data[serial_index-1] == 'T')
            {  
                outputMag = 1;
                outputMagAcc = 0;
                outputAcc = 0;
                outputTrack = 0;
                serial_index = 0;
                string_started = 0; 

            }        
          
            // Stop magnetometer data stream
            else if (serial_data[serial_index-4] == 'C' &&
                     serial_data[serial_index-3] == 'A' &&
                     serial_data[serial_index-2] == 'E' &&
                     serial_data[serial_index-1] == 'N')
            {  
                outputMag = 0;
                outputMagAcc = 0;
                outputAcc = 0;
                outputTrack = 0;
                serial_index = 0;
                string_started = 0; 
            }

            // Start accelerometer data stream
            else if (serial_data[serial_index-4] == 'G' &&
                     serial_data[serial_index-3] == 'R' &&
                     serial_data[serial_index-2] == 'A' &&
                     serial_data[serial_index-1] == 'V')
            {  
                outputAcc = 1;     
                outputMagAcc = 0;
                outputMag = 0;
                outputTrack = 0;
                serial_index = 0;
                string_started = 0; 
            }              
          
            // Stop accelerometer data stream
            else if (serial_data[serial_index-4] == 'G' &&
                     serial_data[serial_index-3] == 'R' &&
                     serial_data[serial_index-2] == 'E' &&
                     serial_data[serial_index-1] == 'N')
            {  
                outputAcc = 0;
                outputMag = 0;
                outputMagAcc = 0;
                outputTrack = 0;
                serial_index = 0;
                string_started = 0; 
            }

            // Start tracking data stream
            else if (serial_data[serial_index-4] == 'P' &&
                     serial_data[serial_index-3] == 'L' &&
                     serial_data[serial_index-2] == 'S' &&
                     serial_data[serial_index-1] == 'T')
            {  
                outputTrack = 1;
                outputMagAcc = 0;
                outputMag = 0;
                outputAcc = 0;
                serial_index = 0;
                string_started = 0; 
            }        

            // Stop tracking data stream          
            else if (serial_data[serial_index-4] == 'P' &&
                     serial_data[serial_index-3] == 'L' &&
                     serial_data[serial_index-2] == 'E' &&
                     serial_data[serial_index-1] == 'N')
            {  
                outputTrack = 0;
                outputMag = 0;
                outputAcc = 0;
                outputMagAcc = 0;
                serial_index = 0;
                string_started = 0; 
            }
          
            // Save RAM settings to EEPROM
            else if (serial_data[serial_index-4] == 'S' &&
                     serial_data[serial_index-3] == 'A' &&
                     serial_data[serial_index-2] == 'V' &&
                     serial_data[serial_index-1] == 'E')
            {  
                SaveSettings();     
                serial_index = 0;
                string_started = 0; 
            }          
          
            // Calibrate gyro
            else if (serial_data[serial_index-4] == 'C' &&
                     serial_data[serial_index-3] == 'A' &&
                     serial_data[serial_index-2] == 'L' &&
                     serial_data[serial_index-1] == 'G')
            { 
                SetGyroOffset();
                SaveSettings();
               
                Serial.print("Gyro offset measured:");
                Serial.print(gyroOff[0]);
                Serial.print(",");   
                Serial.print(gyroOff[1]);
                Serial.print(",");      
                Serial.println(gyroOff[2]);    
               
                serial_index = 0;
                string_started = 0; 
            }

            // Store magnetometer offset
            else if (serial_data[serial_index-3] == 'M' &&
                     serial_data[serial_index-2] == 'A' &&
                     serial_data[serial_index-1] == 'G')
            {
                Serial.println(serial_data);
                int valuesReceived[5] = {0,0,0,0,0};
                int comma_index = 0; 
              
                for (unsigned char k = 0; k < serial_index - 3; k++)
                {
                    // Looking for comma
                    if (serial_data[k] == 44)
                    {
                        comma_index++;
                    }
                    else
                    {
                        valuesReceived[comma_index] = valuesReceived[comma_index] * 10 + (serial_data[k] - 48);
                    }
                }
                
                // Y and Z are swapped on purpose.
                magOffset[0] = valuesReceived[0] - 2000;
                magOffset[1] = valuesReceived[2] - 2000;
                magOffset[2] = valuesReceived[1] - 2000;

                serial_index = 0;
                string_started = 0; 

                SaveMagData();                
            }

            // Store accelerometer offset
            else if (serial_data[serial_index-3] == 'A' &&
                     serial_data[serial_index-2] == 'C' &&
                     serial_data[serial_index-1] == 'C')
            {
                Serial.println(serial_data);
                int valuesReceived[5] = {0,0,0,0,0};
                int comma_index = 0; 
                for (unsigned char k = 0; k < serial_index - 3; k++)
                {
                    // Looking for comma
                    if (serial_data[k] == 44)
                    {
                        comma_index++;
                    }
                    else
                    {
                        valuesReceived[comma_index] = valuesReceived[comma_index] * 10 + (serial_data[k] - 48);
                    }              
                }

                accOffset[0] = valuesReceived[0] - 2000;
                accOffset[1] = valuesReceived[1] - 2000;
                accOffset[2] = valuesReceived[2] - 2000;
                
                serial_index = 0;
                string_started = 0; 

                SaveAccelData();                
            }

            // Retrieve settings
            else if (serial_data[serial_index-4] == 'G' &&
                     serial_data[serial_index-3] == 'S' &&
                     serial_data[serial_index-2] == 'E' &&
                     serial_data[serial_index-1] == 'T' )
            {
                // Get Settings. Scale our local values to
                // real-world values usable on the PC side.
                //
                Serial.print("$SET$"); // something recognizable in the stream

                Serial.print(tiltRollBeta * 100);
                Serial.print(",");   
                Serial.print(panBeta * 100);
                Serial.print(",");
                Serial.print(gyroWeightTiltRoll * 100);  
                Serial.print(",");
                Serial.print(GyroWeightPan * 100);
                Serial.print(",");
                Serial.print(tiltFactor * 10);
                Serial.print(",");
                Serial.print(panFactor * 10);
                Serial.print(",");
                Serial.print(rollFactor * 10);
                Serial.print(",");
                Serial.print(servoReverseMask);
                Serial.print(",");
                Serial.print(servoPanCenter);
                Serial.print(",");
                Serial.print(panMinPulse);
                Serial.print(",");
                Serial.print(panMaxPulse);
                Serial.print(",");
                Serial.print(servoTiltCenter);
                Serial.print(",");
                Serial.print(tiltMinPulse);
                Serial.print(",");
                Serial.print(tiltMaxPulse);
                Serial.print(",");
                Serial.print(servoRollCenter);
                Serial.print(",");
                Serial.print(rollMinPulse);
                Serial.print(",");
                Serial.print(rollMaxPulse);
                Serial.print(",");
                Serial.print(htChannels[0]);
                Serial.print(",");
                Serial.print(htChannels[1]);
                Serial.print(",");
                Serial.println(htChannels[2]);

                Serial.println("Settings Retrieved!");

                serial_index = 0;
                string_started = 0;
            }
            else if (serial_index > 100)
            {
                // If more than 100 bytes have been received, the string is not valid.
                // Reset and "try again" (wait for $ to indicate start of new string). 
                serial_index = 0;
                string_started = 0;
            }
        }
        else if (Serial.read() == '$')
        {
            string_started = 1;
        }
    } // serial port input
}
