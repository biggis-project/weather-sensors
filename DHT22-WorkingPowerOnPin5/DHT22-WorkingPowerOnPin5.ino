// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain

#define DHT_DEBUG

#include <VirtualWire.h>
#include <DHT.h>
#include <EEPROM.h>

#include "crc32.h"
#include "thermistor.h"

// pins
#define BUTTON_PIN 2
#define SENSOR_POWER_PIN 5              // power for DHT sensor
#define DHT_PIN 6                       // data pin for DHT sensor
#define RFTX_PIN 12                     // data pin for RF Transmission
#define THERMISTOR_PIN 0                // analog pin of the thermistor

//#define SENSOR_POWER_PIN 0              // power for DHT sensor
//#define RFTX_PIN 1                      // data pin for RF Transmission
//#define DHT_PIN 2                       // data pin for DHT sensor
//#define BUTTON_PIN 3
//#define THERMISTOR_PIN 5                // analog pin of the thermistor

// other config options
#define RF_BITRATE 2000                 // bits per second
#define DHT_TYPE DHT22                  // DHT 22  (AM2302)
#define SENSOR_POWER_HEATUP_DELAY 500   // wait for power stabilization

// EEPROM ADDRESSES
#define EE_SID_ADDR 0
#define EE_SMODE_ADDR EE_SID_ADDR + sizeof(sensor_id)

unsigned int sensor_id = 42;  // from EEPROM on address EE_SID_ADDR
unsigned int smode = 0;       // from EEPROM on address EE_SMODE_ADDR

DHT dht(DHT_PIN, DHT_TYPE);

void setup() {
  
  #ifdef DHT_DEBUG
    Serial.begin(9600);
  #endif
  
  // reading sensor id from EEPROM
  EEPROM.get(EE_SID_ADDR, sensor_id);
  DEBUG_PRINT("Hello, I'm sensor: ");
  DEBUG_PRINTLN(sensor_id);

  // reading smode from EEPROM
  EEPROM.get(EE_SMODE_ADDR, smode);
  DEBUG_PRINT("Current sending mode is: ");
  DEBUG_PRINTLN(smode);

  DEBUG_PRINT("Powering sensors from pin ");
  DEBUG_PRINTLN(SENSOR_POWER_PIN);
  pinMode(SENSOR_POWER_PIN, OUTPUT);

  dht.begin();

  DEBUG_PRINT("Setting up RF TX module on pin ");
  DEBUG_PRINT(RFTX_PIN);
  DEBUG_PRINT(" using bitrate ");
  DEBUG_PRINTLN(RF_BITRATE);
  
  vw_set_tx_pin(RFTX_PIN);
  vw_setup(RF_BITRATE);

  // interrupt handler that will be activated when the user presses a button
  //attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), on_button_press, RISING);
}

#define MILLISEC  1
#define SECOND    1000 * MILLISEC
#define MINUTE    60 * SECOND
#define HOUR      60 * MINUTE

#define SMODE_COUNT 4
const int smode_delays_ms[SMODE_COUNT] = { 2*SECOND, 1*MINUTE, 10*MINUTE, 1*HOUR };

// disty hack: reset function at address 0
void(* resetFunc) (void) = 0;

/**
 * Interrupt handler for the "smode" button.
 */
void on_button_press() {

  // cycle through sending modes
  smode = (smode + 1) % SMODE_COUNT;

  // write new smode value to EEPROM
  EEPROM.put(EE_SMODE_ADDR, smode);
  
  // now we can reset the device with a new smode value
  resetFunc();
}

void loop() {

  // turn off power from sensors to save battery
  digitalWrite(SENSOR_POWER_PIN, LOW);

  // Wait a few seconds between measurements.
  delay(2000);
  digitalWrite(SENSOR_POWER_PIN, HIGH);
  delayMicroseconds(1000 * SENSOR_POWER_HEATUP_DELAY); // wait for stabilisation

  // Now reading value from termistor
  const double temp = thermistor_to_temp(analogRead(THERMISTOR_PIN));
  DEBUG_PRINT("Thermistor: ");
  DEBUG_PRINT(temp);
  DEBUG_PRINT(" °C, ");

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  const float h = dht.readHumidity();
  const float t = dht.readTemperature();

//  // Check if any reads failed and exit early (to try again).
//  if (isnan(h) || isnan(t)) {
//    Serial.println("Failed to read from DHT sensor!");
//    return;
//  }

  DEBUG_PRINT("DHT-Temp: ");
  DEBUG_PRINT(t);
  DEBUG_PRINT(" °C, ");
  DEBUG_PRINT("DHT-Hum: ");
  DEBUG_PRINT(h);
  DEBUG_PRINTLN(" %");

//  // Now sending a the message through RF module
//  struct msg_t {
//    unsigned int sensor_id;
//    float temp_thermistor;
//    float temp_dht;
//    float humid_dht;
//  };
//
//  struct msg_t msg = {sensor_id, temp, t, h};

 
  char msg[32]; // this buffer is used for sending messages through the RF module
  sprintf(msg, "%u %i.%u %i.%u %i.%u",
    sensor_id,
    (int) temp, frac2(temp),
    (int) t, frac2(t),
    (int) h, frac2(h)
  );

  const unsigned long msgcrc = crc_string(msg);
  
  vw_send((uint8_t *)msg, strlen(msg));
  vw_send((uint8_t *)msgcrc, sizeof(msgcrc));
  vw_wait_tx(); // Wait until the whole message is gone
  
  DEBUG_PRINT("RF data sent: ");
  DEBUG_PRINTLN(msg);
  DEBUG_PRINT("CRC = ");
  DEBUG_PRINTLN(crc_string(msg));
}

unsigned int frac2(double x) {
  return abs((x - (int) x) * 100);
}

