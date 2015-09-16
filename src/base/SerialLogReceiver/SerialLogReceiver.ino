#include <VirtualWire.h>

#define RF_RX_PIN 2
#define LED_PIN 13
#define RF_POWER_PIN 4
#define RF_BITRATE 2000

void setup()
{
    Serial.begin(9600);	// Debugging only
    Serial.println("Setting up the receiver");
    pinMode(LED_PIN, OUTPUT);

    // Initialise the IO and ISR
    pinMode(RF_POWER_PIN, OUTPUT); // power for the RF module
    digitalWrite(RF_POWER_PIN, HIGH);
    
    vw_set_rx_pin(RF_RX_PIN);
    vw_setup(RF_BITRATE);

    vw_rx_start(); // Start the receiver PLL running
}

void loop()
{
    uint8_t buf[VW_MAX_MESSAGE_LEN];
    uint8_t buflen = sizeof(buf);

    // Non-blocking
    if (vw_get_message(buf, &buflen)) {
      digitalWrite(LED_PIN, HIGH); // Flash a light to show received good message
        // Message with a good checksum received, dump it.
        Serial.print("Got: ");
        
        char cbuf[buflen+1];
        cbuf[buflen] = '\0';
        for (int i = 0; i<buflen; i++) {
          cbuf[i] = (char) buf[i];
        }
        
        Serial.println(cbuf);
        delay(500);
        
      digitalWrite(LED_PIN, LOW);
    }
}
