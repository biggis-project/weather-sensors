#include <VirtualWire.h>

const int led_pin = 13;    // PIN 13 has an integrated LED on Arduino Nano
const int receive_pin = 9;

void setup()
{
    delay(1000);
    Serial.begin(9600);	// Debugging only
    Serial.println("Setting up the receiver");
    pinMode(led_pin, OUTPUT);

    // Initialise the IO and ISR
    vw_set_rx_pin(receive_pin);
    vw_setup(2000);	 // Bits per sec

    vw_rx_start();       // Start the receiver PLL running

}

void loop()
{
    uint8_t buf[VW_MAX_MESSAGE_LEN];
    uint8_t buflen = sizeof(buf);

    if (vw_get_message(buf, &buflen)) // Non-blocking
    {
      	int i;
      
        digitalWrite(led_pin, HIGH); // Flash a light to show received good message
      	// Message with a good checksum received, dump it.
      	Serial.print("Got: ");

        char cbuf[buflen+1];
        cbuf[buflen] = '\0';
      	for (i = 0; i < buflen; i++) {
          cbuf[i] = (char) buf[i];
      	}
        Serial.print(cbuf);
      	Serial.println();
        digitalWrite(led_pin, LOW);
    }
}
