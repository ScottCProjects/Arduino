// Scott Camarena
//
// MPL115A1 Barometric Pressure Sensor
// - Arduino Class
//
// Based on sample SCP1000 code from arduino.cc
// http://arduino.cc/en/Tutorial/BarometricPressureSensor
// 
//============================================================

#ifndef _MPL115A1_H_
#define _MPL115A1_H_

#include <Arduino.h>
#include <SPI.h>

#define byte uint8_t
//Sensor's memory register addresses:
#define PRESSURE = 0x1F;      //3 most significant bits of pressure
#define PRESSURE_LSB = 0x20;  //16 least significant bits of pressure
#define TEMPERATURE = 0x21;   //16 bit temperature reading
#define READ = 0b11111100;     // SCP1000's read command
#define WRITE = 0b00000010;   // SCP1000's write command

class MPL115A1
{
private:
	byte sdn, csn, sdo, sdi, sck;

	/*
	void writeData(char data);
	char readData();
	char read(uint8_t address);
	void write(uint8_t address, char data);
	float calculatePressure();
	*/

public:
	// No Default constructor, must define pins
	MPL115A1( byte sdnP, byte csnP,
			byte sdoP, byte sdiP, byte sckP )
	: sdn(sdnP), csn(csnP), sdo(sdoP), sdi(sdiP), sck(sckP)
	{}

};


int MPL115A1_testMain()
{
	/*
	SDN: pin 9
	CSN: pin 8
	SDI: pin 11
	SDO: pin 12
	SCK: pin 13
	*/
	Serial.begin(9600);
	MPL115A1 bp( 9, 8, 12, 11, 13 );
	
	return 0;
}




















#endif
