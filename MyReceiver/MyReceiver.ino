#include <SPI.h>
#include <nRF24L01p.h>
#include <UIPEthernet.h>

nRF24L01p receiver(8,7);//CSN,CE

uint8_t buf[32];
const char server[] = "research.ciant.cz";

EthernetClient client;

void setup(){
  
  delay(150);
  Serial.begin(9600);
  Serial.println("Serial initialized - Receiver");
  
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  receiver.channel(90);
  receiver.RXaddress("wsdataR");
  receiver.TXaddress("wsdataT");
  receiver.init();

  Serial.println("I'm receiver");

  const uint8_t mac[6] = {0x00,0x01,0x02,0x03,0x04,0x05};
  Serial.print("Contacting DHCP server ... ");
  Ethernet.begin(mac); // DHCP happens here
  Serial.println("done");

  Serial.print("localIP: ");
  Serial.println(Ethernet.localIP());
  Serial.print("subnetMask: ");
  Serial.println(Ethernet.subnetMask());
  Serial.print("gatewayIP: ");
  Serial.println(Ethernet.gatewayIP());
  Serial.print("dnsServerIP: ");
  Serial.println(Ethernet.dnsServerIP());
}

int dots = 0;

void loop(){
  while(receiver.available()) {
    String c;
    receiver.read();
    receiver.rxPL(c);
    Serial.print(c);
    dots = 0;
  }
  delay(10);
  if(dots == 0) {
    Serial.println();
  } else if(dots % 20 == 0) {
    Serial.print(".");
  }
  dots = (dots + 1) % 400;
}





