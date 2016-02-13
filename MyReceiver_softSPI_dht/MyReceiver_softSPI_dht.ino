#include <nRF24L01p.h>
#include <UIPEthernet.h>
#include <ws_common.h>

#define RECV_LOOP_DELAY 5
#define SECONDS_SINCE_BOOT (.001 * millis())
#define GATEWAY_MAC_ADDRESS {0x00, 0x01, 0x02, 0x03, 0x04, 0x05}


nRF24L01p receiver(8,7); // CSN, CE

const char server[] = "research.ciant.cz";
const int gwid = 1;

EthernetClient client;

void setup(){
  
  Serial.begin(9600);

  delay(150);
  receiver.channel(WS_CHANNEL);
  receiver.RXaddress(WS_RXADDR);
  receiver.TXaddress(WS_TXADDR);
  receiver.init();

  const uint8_t mac[6] = GATEWAY_MAC_ADDRESS;
  
  Serial.println("DHCP request ...");
  Ethernet.begin(mac); // DHCP happens here

  Serial.print("IP/mask: ");
  Serial.print(Ethernet.localIP());
  Serial.print("/");
  Serial.println(Ethernet.subnetMask());
  Serial.print("GW: ");
  Serial.println(Ethernet.gatewayIP());
  Serial.print("NS: ");
  Serial.println(Ethernet.dnsServerIP());
}

void loop(){

  delay(RECV_LOOP_DELAY);
  if (!receiver.available()) {
    return;
  }

  ws_sensor_msg_t recv;
  
  receiver.read();
  receiver.rxPL(recv.sid);
  receiver.rxPL(recv.t);
  receiver.rxPL(recv.h);
  receiver.rxPL(recv.csum);

  // checksum must match
  const byte csum = WS_SENSOR_CSUM(recv);
  if (recv.csum != csum) {
    return;
  }

  // decode the incomming data
  const byte sensor_id = recv.sid;
  const float t = recv.t / 100.0;
  const float h = recv.h / 100.0;

  // debug info to the serial console
  Serial.print(SECONDS_SINCE_BOOT);
  Serial.print(" Sensor:");
  Serial.print(sensor_id);
  Serial.print(", t=");
  Serial.print(t);
  Serial.print(", h=");
  Serial.print(h);
  Serial.print(" ... ");

//  if (client.connect(server, 80)) {
//
//    // Make a HTTP request
//    client.print("GET /wsgw.php");
//    client.print("?ts="); client.print(SECONDS_SINCE_BOOT);
//    client.print("&sid="); client.print(sensor_id);
//    client.print("&gid="); client.print(gwid);
//    client.print("&d=");
//    client.print(t);
//    client.print(",");
//    client.print(h);
//    client.println(" HTTP/1.1");
//    
//    client.print("Host: ");
//    client.println(server);
//    
//    client.println("User-Agent: arduino");
//    client.println("Connection: close");
//    client.println();
//    
//  } else {
//    // if you didn't get a connection to the server
//    Serial.println("tcp timeout");
//    return;
//  }

  // at this point, we successfully transmitted the sensor data
  // to the REST API on a remote server
  Serial.println("ok");
  Serial.flush();
}

