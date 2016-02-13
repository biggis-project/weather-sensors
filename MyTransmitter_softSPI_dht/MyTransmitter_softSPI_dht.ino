#define DHT_DEBUG

#include "LowPower.h"
#include <nRF24L01p.h>
#include <DHT.h>
#include <ws_common.h>

#define SENSOR_POWER_PIN 9              // power for DHT sensor
#define DHT_PIN 3                       // data pin for DHT sensor
#define DHT_TYPE DHT22                  // DHT 22  (AM2302)
#define SENSOR_POWER_HEATUP_DELAY 2000  // wait for power stabilization (millis)
#define DEVICE_WATCHDOG_SLEEP_SECONDS 8 // use multiples of 8

const byte sensor_id = 2;

DHT dht(DHT_PIN, DHT_TYPE);

nRF24L01p transmitter(8,7); // CSN, CE

void setup(){

  delay(150);

  #ifdef DHT_DEBUG
    Serial.begin(115200);
  #endif

  DEBUG_PRINT("Hello, I'm sensor: ");
  DEBUG_PRINTLN(sensor_id);
  
  DEBUG_PRINT("Power-safe sleep mode in seconds: ");
  DEBUG_PRINTLN(DEVICE_WATCHDOG_SLEEP_SECONDS);

  DEBUG_PRINT("Powering sensors from pin ");
  DEBUG_PRINTLN(SENSOR_POWER_PIN);
  pinMode(SENSOR_POWER_PIN, OUTPUT);
  powerUpSensors();
  
  // DHT22 module
  dht.begin();

  // RF module
  DEBUG_PRINT("Setting up RF module ... ");
  transmitter.channel(WS_CHANNEL);
  transmitter.TXaddress(WS_TXADDR);
  transmitter.RXaddress(WS_RXADDR);
  transmitter.init();
  DEBUG_PRINTLN("done");
  
  DEBUG_PRINTLN("Now entering normal operation...");
}

void powerUpSensors() {
  DEBUG_PRINTLN("power-up sensors");
  Serial.flush();
  digitalWrite(SENSOR_POWER_PIN, HIGH);
  delay(SENSOR_POWER_HEATUP_DELAY);
}

void powerDownSensors() {
  DEBUG_PRINTLN("power-down sensors");
  digitalWrite(SENSOR_POWER_PIN, LOW);
}

void loop() {

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  const float h = dht.readHumidity();
  const float t = dht.readTemperature();

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
  Serial.flush();

  // Now sending a message through RF module
  transmitter.txPL( tosend.sid );
  transmitter.txPL( tosend.t );
  transmitter.txPL( tosend.h );
  transmitter.txPL( tosend.csum );

  // try to reach the station, but not too long
  int i;
  for (i = 10; i>0 && !transmitter.send(SLOW); --i) {
    DEBUG_PRINT(".");
    Serial.flush();
    delay(100);
  }
  transmitter.flushTX(); // TODO
  
  if (i == 0) {
    DEBUG_PRINTLN("timeout");
  } else {
    DEBUG_PRINTLN("sent");
  }
  Serial.flush();
  
  // turn off power from sensors to save battery
  powerDownSensors();

  // Wait a few seconds between measurements.
  for(int i = DEVICE_WATCHDOG_SLEEP_SECONDS / 8; i>0; --i) {
    DEBUG_PRINT("sleep 8s in cycle: ");
    DEBUG_PRINTLN(i);
    Serial.flush();
    // Enter power down state for 8 s with ADC and BOD module disabled
    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
  }
  powerUpSensors();
}

