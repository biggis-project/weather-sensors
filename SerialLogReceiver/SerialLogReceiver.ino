#include <VirtualWire.h>

#define RF_MODULE_DIRECTLY_ON_BREADBOARD
#define RF_RX_PIN 3
#define LED_PIN 13
#define RF_BITRATE 2000

void setup()
{
    Serial.begin(9600);	// Debugging only
    Serial.println("Setting up the receiver");
    pinMode(LED_PIN, OUTPUT);

    // use a dedicated data pin for powering up
    Serial.println("Powering the RF module from +5V pin");
    Serial.print("RF receiving on pin ");
    Serial.println(RF_RX_PIN);
    
    // Initialise the IO and ISR
    vw_set_rx_pin(RF_RX_PIN);
    vw_setup(RF_BITRATE);

    vw_rx_start(); // Start the receiver PLL running
}

void loop()
{
    uint8_t buf[64];
    uint8_t buflen = sizeof(buf);

    // Non-blocking
    if (vw_get_message(buf, &buflen)) {

      // Message with a good checksum received, dump it.
      Serial.print("Got: ");
      
      char cbuf[buflen+1];
      cbuf[buflen] = '\0';
      for (int i = 0; i<buflen; i++) {
        cbuf[i] = (char) buf[i];
      }
      
      Serial.println(cbuf);
      
      // Flash a light to show received good message
      digitalWrite(LED_PIN, HIGH);
      delay(100);
      digitalWrite(LED_PIN, LOW);
      delay(100);
      digitalWrite(LED_PIN, HIGH);
      delay(300);
      digitalWrite(LED_PIN, LOW);
    }
}
