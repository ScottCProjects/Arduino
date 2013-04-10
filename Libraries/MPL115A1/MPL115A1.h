/*
 MPL115A1 Breakout board Sensor Library for Arduino
   created by NeoCat <neocat@neocat.jp>
*/

#ifndef MPL115A1_h
#define MPL115A1_h

#include <SPI.h>

class MPL115A1 {
  byte  shdnPin, csnPin, bShutdown;
  short c[6]; //a0, b1, b2, c12, c11, c22 in datasheet

  byte read(byte addr);
  word readw(byte addr);
  void write(byte addr, byte data);

public:
  MPL115A1(int csnPin = 8, int shdnPin = 9);
  void  begin();
  void  shutdown();
  float pressure();     /* hPa */
};

#endif
