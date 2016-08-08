
//************************ Sub to prepare the PIN of the TX Module  *************
void PREPARE_TX_MODULE()
{
    pinMode (TX_433_PIN, OUTPUT);
    digitalWrite (TX_433_PIN, LOW);
    vw_set_tx_pin (TX_433_PIN);
    vw_set_ptt_inverted(true); // Required for DR3100
    vw_setup(TX_433_BAUD);   // Bits per sec
}

//*************************  Sub to send the PACKET  ***************************
void SEND_TX()
{
  // 0 e 1 char = ID
  // 2 char = PAN
  // 3 char = TILT
  // 4 char = ROLL
    char MESSAGE[5];
    MESSAGE[0] = char(TX_ID & 0x00FF);
    MESSAGE[1] = char((TX_ID & 0xFF00)>>8);
    unsigned int TEMP_CHANNEL = channel_value[htChannels[0]] - 1000;   // Use the PAN Channel
    MESSAGE[2] = char(TEMP_CHANNEL>>2);
    TEMP_CHANNEL = channel_value[htChannels[1]] - 1000;   // Use the TILT Channel
    MESSAGE[3] = char(TEMP_CHANNEL>>2);
    TEMP_CHANNEL = channel_value[htChannels[2]] - 1000;   // Use the ROLL Channel
    MESSAGE[4] = char(TEMP_CHANNEL>>2);
    vw_send((uint8_t *)MESSAGE, strlen(MESSAGE));                         //180 ms e il tempo di trasmissione di un pacchetto dati
    vw_wait_tx();
    //Serial.write (MESSAGE);
}
