#include <nRF24L01p.h>

nRF24L01p receiver(8,7); //CSN,CE

void setup(){
  
  delay(150);
  Serial.begin(9600);
  Serial.println("Serial initialized - Receiver");

  receiver.channel(90);
  receiver.RXaddress("wsdataR");
  receiver.TXaddress("wsdataT");
  receiver.init();

  Serial.println("I'm receiver");
}

int dots = 0;

void loop(){

  delay(10);

  if (!receiver.available()) {
    return;
  }
  
  if( ! receiver.read()) {
    return;
  }

  String s = String("");
  receiver.rxPL(s);
  if(s.length() > 0) {
    Serial.println(s);
  }
}




  
