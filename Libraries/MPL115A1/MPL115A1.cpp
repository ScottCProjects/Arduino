/*
 MPL115A1 Breakout board Sensor Library for Arduino
   created by NeoCat <neocat@neocat.jp>
*/

extern "C" {
  #include <math.h>
}
#include <WConstants.h>
#include <SPI.h>
#include "MPL115A1.h"

MPL115A1::MPL115A1(int csnPin, int shdnPin) : csnPin(csnPin), shdnPin(shdnPin)
{
}

void MPL115A1::begin()
{
  SPI.begin();
	
  pinMode(csnPin, OUTPUT);
  pinMode(shdnPin, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, INPUT);
  pinMode(13, OUTPUT);

  digitalWrite(csnPin, HIGH);
  digitalWrite(shdnPin, HIGH);
  bShutdown = 0;
  delay(1); // wait the device to be ready

  // read out coeddicients
  for (int i = 0x4; i < 0xf; i+=2) {
    int j = i/2 - 2; // c[] index
    c[j] = readw(i);
  }

  digitalWrite(csnPin, HIGH); // chip deselect
}

byte MPL115A1::read(byte addr)
{
  addr <<= 1;
  addr |= 0x80;

  digitalWrite(csnPin, LOW);
  SPI.transfer(addr);
  byte ret = SPI.transfer(0x55);
  digitalWrite(csnPin, HIGH);
  return ret;
}

word MPL115A1::readw(byte addr)
{
  word ret = read(addr);
  ret <<= 8;
  ret |= read(addr+1);
  /*
  Serial.print("reading ");
  Serial.print(addr, HEX);
  Serial.print(":");
  Serial.print(ret, HEX);
  Serial.print("\n");
  */

  return ret;
}

void MPL115A1::write(byte addr, byte data)
{
  addr <<= 1;
  addr &= 0x7f;
  digitalWrite(csnPin, LOW);
  SPI.transfer(addr);
  SPI.transfer(data);
  digitalWrite(csnPin, HIGH);
}

void MPL115A1::shutdown()
{
  digitalWrite(shdnPin, LOW);
  bShutdown = 1;
}

float MPL115A1::pressure()
{
  digitalWrite(shdnPin, HIGH);
  if (bShutdown) delay(1);
  bShutdown = 0;

  write(0x12, 0x00); // start AD conversions
  delay(5);
  short padc = readw(0x0) >> 6;
  short tadc = readw(0x2) >> 6;

  // compensation
  long a0 = c[0], b1 = c[1], b2 = c[2], c12 = c[3], c11 = c[4], c22 = c[5];
  long a1 = b1 + (c11*padc >> 14) + (c12*tadc >> 11);
  long a2 = (b2>>1) + (c22*tadc >> 17);
  float pcomp = a0/8.0 + (a1*padc + a2*tadc)/8192.0;

  /* pcomp = 0 : 500hPa  -  1023 : 1150hPa */
  float hPa = (1150-500)/1023.0*pcomp + 500;
  
  return hPa;
}
