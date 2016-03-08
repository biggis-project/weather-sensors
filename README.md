# Weather sensors developed within BigGIS project

The aim is to create a cheap wireless temparature and humidity sensors that can send sensor readings to cloud through a gateway connected to home router.

## Development

[BigGIS]: biggis-project.eu
[Waffle]: https://waffle.io/biggis-project/biggis-project.github.io

To see issues from all repositories related to [BigGIS] project, visit [this Waffle Dashboard][Waffle]

## How to contribute
- fork, clone, edit, commit, push, create pull request
- Use Arduino IDE or something similar

## Required Arduino Libraries
- Adafruit DHT sensor library : https://github.com/adafruit/DHT-sensor-library
- UIPEthernet
- nRF24L01p_SoftSPI

## Required Hardware (Overview)

Sensor node:
- Arduino Pro Mini @3.3V (MCU with low power consumption)
- DHT22 Temperature/Humidity sensor (cheap and accurate)
  - 10K resistor
  - 100nF capacitor
- NRF24L01 + PA + LNA (RF module with built-in antenna)
  - 3.3uF-10uF capacitor (see also [NRF24L01-Howto])
- 10K resistor
- 100nF capacitor for DHT22

Gateway:
- Arduino Nano v3 @5V (MCU with USB-mini connector)
- ENC28J60 (ethernet module, which is not the same as original Arduino ethernet shield W5100)
- NRF24L01 + PA + LNA (rf module with built-in antenna)
  - 3.3uF to 10uF capacitor (see also [NRF24L01-Howto])

[NRF24L01-Howto]: https://arduino-info.wikispaces.com/Nrf24L01-2.4GHz-HowTo
