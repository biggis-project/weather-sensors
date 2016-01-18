/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the Uno and
  Leonardo, it is attached to digital pin 13. If you're unsure what
  pin the on-board LED is connected to on your Arduino model, check
  the documentation at http://www.arduino.cc

  This example code is in the public domain.

  modified 8 May 2014
  by Scott Fitzgerald
 */

#include "prescaler.h";

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin 13 as an output.
  pinMode(13, OUTPUT);

  const long p = CLOCK_PRESCALER_32;
  const long scale = (1 << p);
  setClockPrescaler(p);

  Serial.begin(9600 * scale);
  Serial.print("Blinking with prescaler: ");
  Serial.print(p);
  Serial.print(" which is scale ");
  Serial.println(scale);
}

// the loop function runs over and over again forever
void loop() {
  Serial.println("blink");
  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  trueDelayMicroseconds(500000);
  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
  trueDelayMicroseconds(500000);
}
