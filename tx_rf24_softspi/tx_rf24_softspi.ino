

#include "RF24.h"
#include "DigitalIO.h"

RF24 radio(7,8);

byte addr[][6] = {"tx>rx", "rx>tx"};

void setup() {
  Serial.begin(115200);

  radio.begin();

  // Set the PA Level low to prevent power supply related issues since this is a
  // getting_started sketch, and the likelihood of close proximity of the devices. RF24_PA_MAX is default.
  radio.setPALevel(RF24_PA_LOW);
  radio.setAutoAck(true);
  radio.setChannel(108);
//  radio.setCRCLength(RF24_CRC_16);

  radio.openWritingPipe(addr[0]);
  radio.openReadingPipe(1, addr[1]);

  Serial.print("Channel: ");
  Serial.println(radio.getChannel());

  Serial.print("Payload size:");
  Serial.println(radio.getPayloadSize());
  
}

void loop() {
  
  unsigned long start_time = millis();

  Serial.print("Now sending .. ");
  Serial.println(start_time);

  if (!radio.write( &start_time, sizeof(unsigned long) )){
    Serial.println("failed");
  }
  
  // Try again 1s later
  delay(1000);

} // Loop

