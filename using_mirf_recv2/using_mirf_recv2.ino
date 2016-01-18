#include <UIPEthernet.h>
#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>

#define STATIC 0 

uint8_t buf[32];
const char server[] = "research.ciant.cz";

EthernetClient client;


void setup() {
  Serial.begin(9600);
  Serial.println("Serial initialized - Receiver");
  
  Mirf.cePin = 8;
  Mirf.csnPin = 7;
  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();
  Mirf.setRADDR((byte *)"TX_01");
  Mirf.payload = 1;
  Mirf.channel = 90;
  Mirf.config();
    
  Serial.println("NRF libraries initialised");
  
  const uint8_t mac[6] = {0x00,0x01,0x02,0x03,0x04,0x05};
  Serial.print("Contacting DHCP server ... ");
//  Ethernet.begin(mac); // DHCP happens here
//  Serial.println("done");
//
//  Serial.print("localIP: ");
//  Serial.println(Ethernet.localIP());
//  Serial.print("subnetMask: ");
//  Serial.println(Ethernet.subnetMask());
//  Serial.print("gatewayIP: ");
//  Serial.println(Ethernet.gatewayIP());
//  Serial.print("dnsServerIP: ");
//  Serial.println(Ethernet.dnsServerIP());
  
}

int dots = 0;

void loop() {
  byte c;

  // is there any data pending? 
  if( Mirf.dataReady() )
  {
     // well, get it
     Mirf.getData(&c);

    // ... and write it out to the PC
    Serial.print(c);
  }
  
  delay(10);
  
  if(dots == 0) {
    Serial.println();
  } else if(dots % 8 == 0) {
    Serial.print(".");
  }
  dots = (dots + 1) % 400;

}
