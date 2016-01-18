// Notes:
// - eternate cable connected to docking station using green eth. wire
// - eth0 set as "Arduino shared" which is configured as "ipv4 shared to other computers"
// - the connection is routed through wifi connection
// - gets IP address from DHCP through wifi -> 10.42.0.?
#include <UIPEthernet.h>

const char server[] = "research.ciant.cz";

EthernetClient client;

void setup() {

  Serial.begin(9600);
  Serial.println("Starting WS");

  const uint8_t mac[6] = {0x00,0x01,0x02,0x03,0x04,0x05};

  Serial.print("SPI_SS:");
  Serial.println(SPI_SS);

  Serial.print("ENC28J60_CONTROL_CS:");
  Serial.println(ENC28J60_CONTROL_CS);


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

void loop() {
   
  if (client.connect(server, 80)) {
    Serial.println("connected");
    // Make a HTTP request:
    
    client.print("GET /wsgw.php");
    client.print("?ts="); client.print( 0.001 * millis() );
    client.print("&sid="); client.print(1);
    client.print("&gid="); client.print(1);
    client.print("&d=");
    client.print(20);
    client.print(",");
    client.print(70);
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

  delay(5000);
}
