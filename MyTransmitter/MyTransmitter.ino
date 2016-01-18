#include <SPI.h>
#include <nRF24L01p.h>

nRF24L01p transmitter(8,7);//CSN,CE

void setup(){
  delay(150);
  Serial.begin(9600);
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  transmitter.channel(90);
  transmitter.TXaddress("wsdataT");
  transmitter.RXaddress("wsdataR");
  transmitter.init();
  Serial.println("I'm transmitter");
}

int i = 0;

void loop() {
  String msg = String(i);
  
  Serial.println(msg);
  transmitter.txPL(msg);
  transmitter.send(FAST);
  delay(3000);
  i++;
}

