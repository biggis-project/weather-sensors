#include "Arduino.h"
#include "thermistor.h"

/**
 * Inputs ADC Value from Thermistor and outputs Temperature in Celsius.
 * Utilizes the Steinhart-Hart Thermistor Equation:
 * Temperature in Kelvin = 1 / {A + B[ln(R)] + C[ln(R)]^3}
 * where A = 0.001129148, B = 0.000234125 and C = 8.76741E-08
 * We assume 10K thermistor
 * 
 * See http://en.wikipedia.org/wiki/Thermistor for explanation of formula
 */
double thermistor_to_temp(int RawADC) {
  double Temp;
  
  // "Temp" means "Temporary" on this line.
  Temp = log(((10240000 / RawADC) - 10000));
  Temp = 1 / (0.001129148 + (0.000234125 * Temp) + (0.0000000876741 * Temp * Temp * Temp));
  
   // Now it means both "Temporary" and "Temperature"
  Temp = Temp - 273.15; // Convert Kelvin to Celcius

  // Now it only means "Temperature"
  return Temp;
}

