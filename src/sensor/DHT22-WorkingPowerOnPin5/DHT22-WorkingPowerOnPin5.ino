// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain

#define DHT_DEBUG

#include "VirtualWire.h"
#include "DHT.h"

// pins
#define BUTTONPIN 2
#define SENSOR_POWER_PIN 5              // power for DHT sensor
#define DHTPIN 6                        // data pin for DHT sensor
#define RFTX_PIN 12                     // data pin for RF Transmission
#define THERMISTOR_PIN 0                // analog pin of the thermistor

// other config options
#define RFTX_RATE 2000                  // bits per second
#define DHTTYPE DHT22                   // DHT 22  (AM2302)
#define SENSOR_POWER_HEATUP_DELAY 500   // wait for power stabilization

char* sensor_id = "Sensor1"; // TODO: this will be later loaded from EEPROM

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  
  #ifdef DHT_DEBUG
    Serial.begin(9600);
  #endif
  
  DEBUG_PRINT("Hello, I'm sensor: ");
  DEBUG_PRINTLN(sensor_id);

  DEBUG_PRINT("Powering sensors from pin ");
  DEBUG_PRINTLN(SENSOR_POWER_PIN);
  pinMode(SENSOR_POWER_PIN, OUTPUT);

  dht.begin();

  DEBUG_PRINT("Setting up RF TX module on pin ");
  DEBUG_PRINT(RFTX_PIN);
  DEBUG_PRINT(" using bitrate ");
  DEBUG_PRINTLN(RFTX_RATE);
  
  vw_set_tx_pin(RFTX_PIN);
  vw_setup(RFTX_RATE);
}

void loop() {

  digitalWrite(SENSOR_POWER_PIN, LOW);

  // Wait a few seconds between measurements.
  delay(2000);
  digitalWrite(SENSOR_POWER_PIN, HIGH);
  delay(SENSOR_POWER_HEATUP_DELAY); // wait for stabilisation

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

  // Now sending a the message through RF module
  char msg[32]; // this buffer is used for sending messages through the RF module
  sprintf(msg, "%s %d.%d %d.%d %d.%d",
    sensor_id,
    (int) temp, frac2(temp),
    (int) t, frac2(t),
    (int) h, frac2(h)
  );
  vw_send((uint8_t *)msg, strlen(msg));
  vw_wait_tx(); // Wait until the whole message is gone
  
  DEBUG_PRINT("RF data sent: ");
  DEBUG_PRINTLN(msg);
}

int frac2(double x) {
  return abs((x - (int) x) * 100);
}

/**
 * Reads the voltage from a 10K thermistor and transforms
 * it into temparature (celsius).
 */
double thermistor_to_temp(int RawADC) {
  double Temp;
  // See http://en.wikipedia.org/wiki/Thermistor for explanation of formula
  Temp = log(((10240000 / RawADC) - 10000));
  Temp = 1 / (0.001129148 + (0.000234125 * Temp) + (0.0000000876741 * Temp * Temp * Temp));
  Temp = Temp - 273.15;           // Convert Kelvin to Celcius
  return Temp;
}

