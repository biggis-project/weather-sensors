
#include "DigitalIO.h"
#include "RF24.h"

byte addr[][6] = {"tx>rx", "rx>tx"};

/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
RF24 radio(7,8);
/**********************************************************/

void setup() {
  Serial.begin(115200);
  radio.begin();

  // Set the PA Level low to prevent power supply related issues since this is a
  // getting_started sketch, and the likelihood of close proximity of the devices. RF24_PA_MAX is default.
  radio.setPALevel(RF24_PA_HIGH);
  radio.setAutoAck(true);
  radio.setChannel(108);
//  radio.setCRCLength(RF24_CRC_16);

  radio.openReadingPipe(1, addr[0]);
  radio.openWritingPipe(addr[1]);

  Serial.print("Channel: ");
  Serial.println(radio.getChannel());

  Serial.print("Payload size:");
  Serial.println(radio.getPayloadSize());

  // Start the radio listening for data
  radio.startListening();
  Serial.println("now listening");
}

void loop() {

  if( ! radio.available()) {
    return;
  }
 
  unsigned long got_time;
  radio.read( &got_time, sizeof(unsigned long) );

  Serial.print("received: ");
  Serial.println(got_time);
  Serial.flush();
  delay(50);

} // Loop


