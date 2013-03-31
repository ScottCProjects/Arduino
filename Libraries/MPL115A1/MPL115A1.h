// Scott Camarena
//
// MPL115A1 Barometric Pressure Sensor
// - Arduino Class
//
// Based on MPL115A1 ATmega328 C code by Jim Lindblom at sparkfun.com
// 
//============================================================

#ifndef _MPL115A1_H_
#define _MPL115A1_H_

#include <Arduino.h>
#include <SPI.h>

#define PRESH	0x00	// 80
#define	PRESL	0x02	// 82
#define	TEMPH	0x04	// 84
#define	TEMPL	0x06	// 86

#define A0MSB	0x08	// 88
#define A0LSB	0x0A	// 8A
#define B1MSB	0x0C	// 8C
#define B1LSB	0x0E	// 8E
#define	B2MSB	0x10	// 90
#define B2LSB	0x12	// 92
#define C12MSB	0x14	// 94
#define	C12LSB	0x16	// 96
#define	C11MSB	0x18	// 98
#define C11LSB	0x1A	// 9A
#define C22MSB	0x1C	// 9C
#define C22LSB	0x1E	// 9E

// AVR Port macros
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif 

class MPL115A1
{
private:

	void init_SPI();
	void writeData(char data);
	char readData();
	char read(uint8_t address);
	void write(uint8_t address, char data);
	float calculatePressure();

public:
	

};


bool testMain()
{
	return 0;
}

#endif
