#include <EEPROM.h>
#include <avr/sleep.h>

// ==================
// Change the ID here
// ==================
   #define NEWID 151
// ==================

#define EE_SID_ADDR 0
unsigned int sensor_id;  // from EEPROM on address EE_SID_ADDR

void setup() {
  Serial.begin(9600);

  // reading sensor id from EEPROM
  EEPROM.get(EE_SID_ADDR, sensor_id);

  Serial.println("Enter new sensor ID > ");
  const long new_sensor_id = NEWID;

  Serial.print("Entered sensor ID: ");
  Serial.println(new_sensor_id);
  
  if(sensor_id != new_sensor_id) {
    EEPROM.put(EE_SID_ADDR, new_sensor_id);
    Serial.println("EEPROM updated");
  } else {
    Serial.println("same ID, EEPROM not updated");
  }
  Serial.println("done.");
  Serial.println("now powering off in 2 seconds");
  delay(2000);
  
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable(); 
  cli();
  sleep_bod_disable();
  sei();
  sleep_cpu();
}

/** 
 *  This function should never be executed,
 *  since the CPU is turned off in the setup() function.
 */
void loop() {
  Serial.println("tick");
  delay(1000);
}
