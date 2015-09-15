// transmitter.pde
//
// Simple example of how to use VirtualWire to transmit messages
// Implements a simplex (one-way) transmitter with an TX-C1 module
//
// See VirtualWire.h for detailed API docs
// Author: Mike McCauley (mikem@airspayce.com)
// Copyright (C) 2008 Mike McCauley
// $Id: transmitter.pde,v 1.3 2009/03/30 00:07:24 mikem Exp $

#include <VirtualWire.h>

#define POWERPIN 5 // power for DHT sensor


const int led_pin = 13;
const int transmit_pin = 12;

void setup()
{
  pinMode(POWERPIN, OUTPUT);
  digitalWrite(POWERPIN, HIGH);
  

    // Initialise the IO and ISR
    vw_set_tx_pin(transmit_pin);
    vw_setup(2000); // Bits per sec
    pinMode(led_pin, OUTPUT);

  Serial.begin(9600);
  Serial.println("RF transmitter demo");
    
}

void loop()
{
  double temp = Thermistor(analogRead(0));

  char msg[32];
  int dtemp = temp;
  int ftemp = abs( (temp - dtemp) * 100);
  sprintf(msg, "T=%d.%d", dtemp, ftemp );
  Serial.println(msg);

  digitalWrite(led_pin, HIGH); // Flash a light to show transmitting
  vw_send((uint8_t *)msg, strlen(msg));
  vw_wait_tx(); // Wait until the whole message is gone
  digitalWrite(led_pin, LOW);
  delay(1000);
}

double Thermistor(int RawADC) {
  double Temp;
  // See http://en.wikipedia.org/wiki/Thermistor for explanation of formula
  Temp = log(((10240000 / RawADC) - 10000));
  Temp = 1 / (0.001129148 + (0.000234125 * Temp) + (0.0000000876741 * Temp * Temp * Temp));
  Temp = Temp - 273.15;           // Convert Kelvin to Celcius
  return Temp;
}

