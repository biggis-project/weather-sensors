// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain

#include "VirtualWire.h"
#include "DHT.h"

#define POWERPIN 5 // power for DHT sensor
#define DHTPIN 6 // data pin for DHT sensor
#define RFTXPIN 12 // data pin for RF Transmission

#define DHTTYPE DHT22   // DHT 22  (AM2302)

char* sensor_id = "Sensor1";
const int transmit_pin = 12;

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  Serial.println("DHTxx test!");

  Serial.print("Powering DHT22 from pin ");
  Serial.println(POWERPIN);
  pinMode(POWERPIN, OUTPUT);

  vw_set_tx_pin(transmit_pin);
  vw_setup(2000); // Bits per sec

  dht.begin();

  Serial.println("Setting up RF TX module");
//  vw_set_tx_pin(RFTXPIN);
//  vw_setup(2000); // Bits per sec
}

void loop() {

  digitalWrite(POWERPIN, LOW);

  // Wait a few seconds between measurements.
  delay(2000);
  digitalWrite(POWERPIN, HIGH);
  delay(500); // wait for stabilisation

  // Now reading value from termistor
  double temp = Thermister(analogRead(0));
  Serial.print("Thermistor: ");
  Serial.print(temp);
  Serial.print(" °C, ");

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print("DHT-Temp: ");
  Serial.print(t);
  Serial.print(" °C, ");
  Serial.print("DHT-Hum: ");
  Serial.print(h);
  Serial.print(" %, ");
  Serial.print("Heat idx: ");
  Serial.print(hic);
  Serial.print(" °C");
  Serial.println();

  char msg[32];
  sprintf(msg, "%s %d.%d %d.%d %d.%d",
    sensor_id,
    (int) temp, frac2(temp),
    (int) t, frac2(t),
    (int) h, frac2(h)
  );
  vw_send((uint8_t *)msg, strlen(msg));
  vw_wait_tx(); // Wait until the whole message is gone
  Serial.println(msg);
}

int frac2(double x) {
  return abs((x - (int) x) * 100);
}

/**
 * Reads the voltage from a 10K thermistor and transforms
 * it into temparature (celsius).
 */
double Thermister(int RawADC) {
  double Temp;
  // See http://en.wikipedia.org/wiki/Thermistor for explanation of formula
  Temp = log(((10240000 / RawADC) - 10000));
  Temp = 1 / (0.001129148 + (0.000234125 * Temp) + (0.0000000876741 * Temp * Temp * Temp));
  Temp = Temp - 273.15;           // Convert Kelvin to Celcius
  return Temp;
}

