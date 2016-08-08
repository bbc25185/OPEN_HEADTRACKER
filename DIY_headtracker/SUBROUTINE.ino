//********************************  SETUP the PIN  ****************************************
void PREPARE_PIN()
{
  // Set button pin to input:
    pinMode(BUTTON_INPUT,INPUT_PULLUP);

    pinMode(PPM_PIN,OUTPUT);
    digitalWrite(2,HIGH);
    digitalWrite(3,HIGH); 

    pinMode(ARDUINO_LED,OUTPUT);    // Arduino LED
    
#if FATSHARK_HT_MODULE
    pinMode(BUZZER,OUTPUT);         // Buzzer
    digitalWrite(BUZZER, HIGH);
#endif

    // Set all other pins to input, for safety.
    pinMode(0,INPUT);
    pinMode(1,INPUT);
    pinMode(2,INPUT);
    pinMode(3,INPUT);
    pinMode(6,INPUT);
    pinMode(7,INPUT);  
    pinMode(8,INPUT);    

    digitalWrite(0,LOW); // pull-down resistor
    digitalWrite(1,LOW);
    digitalWrite(2,HIGH);
    digitalWrite(3,HIGH);  

}


//******************************************  Check if button is pressed  *******************************
void CHECK_BUTTON()
{
  // Check input button for reset/pause request
  char buttonPressed = (digitalRead(BUTTON_INPUT) == 0);
    
  if ( buttonPressed && lastButtonState == 0)
    {
        resetValues = 1; 
        buttonDownTime = millis();
        lastButtonState = 1;
    }
    
    if ( buttonPressed )
    {
        if ( !pauseToggled && ((millis() - buttonDownTime) > BUTTON_HOLD_PAUSE_THRESH) )
        {
            // Pause/unpause
            ht_paused = !ht_paused;
            resetValues = 1;
            pauseToggled = 1;
        }
    }
    else
    {
        pauseToggled = 0;
        lastButtonState = 0;
    }
  
}

void PREPARE_PPM_OUT()
{
  FlexiTimer2::set(FRAME_LENGTH, 1.0/1000000, OUT_PPM); // call every 500 1ms "ticks"
  FlexiTimer2::start();
}


//*******************************  Subroutine called by Timer2 Interrupt  ***********************************
void OUT_PPM()
{
  static boolean output = HIGH;
  if (output == HIGH){                    //-----------------------Signal is HIGH... LOW 300ms
    digitalWrite (PPM_PIN, LOW);
    output = LOW;
    CHANNEL_PPM_SEND++;
    FlexiTimer2::set(DEAD_TIME, 1.0/1000000, OUT_PPM); //Call before 300ms
    FlexiTimer2::start();
  }
  else {                                  //-----------------------Signal is LOW... Channel OUT
    digitalWrite (PPM_PIN, HIGH);
    output = HIGH;
    if (CHANNEL_PPM_SEND <= NUMBER_OF_CHANNELS) {               //-------- Channel send
      unsigned int CHANNEL_TIME = (channel_value[CHANNEL_PPM_SEND]>>1) - DEAD_TIME;
      FlexiTimer2::set(CHANNEL_TIME, 1.0/1000000, OUT_PPM); // call every 500 1ms "ticks"
      FlexiTimer2::start();
    }
    else {                                                      //--------- End frame send
      FlexiTimer2::set(END_FRAME, 1.0/1000000, OUT_PPM); // call every 500 1ms "ticks"
      FlexiTimer2::start();
      CHANNEL_PPM_SEND = 0;
    }
  }  
}
