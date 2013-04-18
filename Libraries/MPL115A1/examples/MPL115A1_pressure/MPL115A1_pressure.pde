/*
 Sample Sketch for MPL115A1 Breakout board Sensor Library for Arduino
   created by NeoCat <neocat@neocat.jp>
 
 Circuit:
   MPL115A1 breakout board is connected to pins 8, 9, 11 - 13 as default:
   SHDN: pin 9 (default, configurable)
   CSN:  pin 8 (default, configurable)
   SDO/DOUT/MISO: pin 12
   SDI/DIN/MOSI:  pin 11
   SCK: pin 13
*/

#include <SPI.h>
#include <MPL115A1.h>

MPL115A1 sensor;

void setup() {
  Serial.begin(9600);
  sensor.begin();
}

void loop() {
  float hPa = sensor.pressure();
  Serial.print(hPa);
  Serial.print(" hPa\n");
  delay(5000);
}
