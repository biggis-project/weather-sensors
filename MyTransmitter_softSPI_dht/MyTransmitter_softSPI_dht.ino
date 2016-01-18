#define DHT_DEBUG

#include <nRF24L01p.h>
#include <DHT.h>
#include <ws_common.h>

#define SENSOR_POWER_PIN 9              // power for DHT sensor
#define DHT_PIN 3                       // data pin for DHT sensor
#define DHT_TYPE DHT22                  // DHT 22  (AM2302)
#define SENSOR_POWER_HEATUP_DELAY 2000  // wait for power stabilization (millis)

const byte sensor_id = 2;

DHT dht(DHT_PIN, DHT_TYPE);

nRF24L01p transmitter(8,7); // CSN, CE

void setup(){

  delay(150);

  #ifdef DHT_DEBUG
    Serial.begin(9600);
  #endif

  DEBUG_PRINT("Hello, I'm sensor: ");
  DEBUG_PRINTLN(sensor_id);
  
  DEBUG_PRINT("Powering sensors from pin ");
  DEBUG_PRINTLN(SENSOR_POWER_PIN);
  pinMode(SENSOR_POWER_PIN, OUTPUT);

  dht.begin();

  DEBUG_PRINT("Setting up RF module ... ");
  transmitter.channel(WS_CHANNEL);
  transmitter.TXaddress(WS_TXADDR);
  transmitter.RXaddress(WS_RXADDR);
  transmitter.init();
  DEBUG_PRINTLN("done");
}

void loop() {
  
  // turn off power from sensors to save battery
  digitalWrite(SENSOR_POWER_PIN, LOW);

  // Wait a few seconds between measurements.
  delay(2000);
  digitalWrite(SENSOR_POWER_PIN, HIGH);
  delay(SENSOR_POWER_HEATUP_DELAY);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  const float h = dht.readHumidity();
  const float t = dht.readTemperature();

  digitalWrite(SENSOR_POWER_PIN, LOW);

  ws_sensor_msg_t tosend = {
    sensor_id,
    t * 100,
    h * 100
  };
  
  tosend.csum = WS_SENSOR_CSUM(tosend);
  
  DEBUG_PRINT("DHT-Temp: ");
  DEBUG_PRINT(tosend.t);
  DEBUG_PRINT(" °C, ");
  DEBUG_PRINT("DHT-Hum: ");
  DEBUG_PRINT(tosend.h);
  DEBUG_PRINT(" % ");
//  DEBUG_PRINT("csum: ");
//  DEBUG_PRINT(tosend.csum);
//  DEBUG_PRINT(" ");

  // Now sending a message through RF module
  transmitter.txPL( tosend.sid );
  transmitter.txPL( tosend.t );
  transmitter.txPL( tosend.h );
  transmitter.txPL( tosend.csum );

  // try to reach the station, but not too long
  int i;
  for (i = 10; i>0 && !transmitter.send(SLOW); --i) {
    DEBUG_PRINT(".");
    delay(100);
  }
  
  if (i == 0) {
    DEBUG_PRINTLN("timeout");
  } else {
    DEBUG_PRINTLN("sent");
  }
  
  delay(3000); // later, we will use watchdog to sleep
}

