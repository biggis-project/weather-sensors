#define DHT_DEBUG

#include <nRF24L01p.h>
#include <DHT.h>

#define SENSOR_POWER_PIN 9              // power for DHT sensor
#define DHT_PIN 3                       // data pin for DHT sensor
#define DHT_TYPE DHT22                  // DHT 22  (AM2302)
#define SENSOR_POWER_HEATUP_DELAY 2000  // wait for power stabilization (millis)

unsigned int sensor_id = 42;  // from EEPROM on address EE_SID_ADDR
unsigned int smode = 0;       // from EEPROM on address EE_SMODE_ADDR

DHT dht(DHT_PIN, DHT_TYPE);


nRF24L01p transmitter(8,7);//CSN,CE

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

  DEBUG_PRINT("Setting up RF module on pin ");
  transmitter.channel(90);
  transmitter.TXaddress("wsdataT");
  transmitter.RXaddress("wsdataR");
  transmitter.init();
  Serial.println("I'm transmitter");
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
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  digitalWrite(SENSOR_POWER_PIN, LOW);

  DEBUG_PRINT("DHT-Temp: ");
  DEBUG_PRINT(t);
  DEBUG_PRINT(" °C, ");
  DEBUG_PRINT("DHT-Hum: ");
  DEBUG_PRINT(h);
  DEBUG_PRINTLN(" %");

  // Now sending a message through RF module
  transmitter.txPL( t );
  transmitter.txPL( h );
  while( ! transmitter.send(SLOW)) {
    DEBUG_PRINT(".");
    delay(100);
  }
  delay(3000);
}

