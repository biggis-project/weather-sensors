#include <nRF24L01p.h>
#include <UIPEthernet.h>

nRF24L01p receiver(8,7); //CSN,CE

const char server[] = "research.ciant.cz";

EthernetClient client;

void setup(){
  
  delay(150);
  Serial.begin(115200);
  Serial.println("Serial initialized - Receiver");

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
    float t;
    float h;
    receiver.read();
    receiver.rxPL(t);
    receiver.rxPL(h);
    Serial.print(t);
    Serial.print(",");
    Serial.print(h);

    if (client.connect(server, 80)) {
      // Make a HTTP request:
      client.print("GET /wsgw.php");
      client.print("?ts="); client.print( 0.001 * millis() );
      client.print("&sid="); client.print(1);
      client.print("&gid="); client.print(1);
      client.print("&d=");
      client.print(t);
      client.print(",");
      client.print(h);
      client.println(" HTTP/1.1");
      
      client.print("Host: ");
      client.println(server);
      
      client.println("User-Agent: arduino-eth");
      client.println("Connection: close");
      client.println();
    }
    else {
      // if you didn't get a connection to the server:
      Serial.println("connection failed");
    }
    
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




  
