// Scott Camarena
// 2010
// Compass Class
//==============================================================
#ifndef COMPASS_H_
#define COMPASS_H_

#include "Arduino.h"
#include "../Wire/Wire.h"
//#include "../Wire/Wire.cpp"

typedef unsigned char byte;

class Compass
{
	private:
		int HMCAdrs;
		int slaveAdrs;
		byte headingData[2];
		int i;

		//For delay
		int req_time;

	public:
		int trueNorth;
		int heading;
		int count;

		//**Constructor, declares all variables needed
		Compass();

		//**Setup for setup function, initializes addresses & starts "Wire"
		void setup();

		//**Data functions for loop
		int req_read();

		//The compass needs at least 70 microseconds delay after request
		// command, so if we do them separately, we can do something else
		// in that time instead of just delaying and doing nothing
		//Doing them separately can save time if we
		void request();
		int read();

		void calibrate();
		void setTrueNorth();
		void convertToReal( int& val );
		char direction();
		void serial_print();
};


//Delay between request / read in MICROseconds (us)
//	Minimum = 70us
#define rrDelay 200

Compass::Compass()
	: HMCAdrs(0x42), count(0), trueNorth(0)
{
	slaveAdrs = HMCAdrs >> 1;
	Wire.begin();
}

void Compass::calibrate()
{
	Wire.beginTransmission(slaveAdrs);
	Wire.write("O"); // Update Bridge Offsets
	Wire.endTransmission();
	delay(rrDelay);
	Wire.beginTransmission(slaveAdrs);
	Wire.write("C"); // Calibration Mode
	Wire.endTransmission();
	delay(6000);
	Wire.beginTransmission(slaveAdrs);
	Wire.write("E"); // Exit Calibration Mode
	Wire.endTransmission();
}
	

void Compass::setup()
{
}

void Compass::setTrueNorth()
{
	trueNorth = req_read();
}

void Compass::convertToReal( int& val )
{
	val -= trueNorth;
	if( val < 0 )
		val += 3600;
}

int Compass::req_read()
{
	// Send a "A" command to the HMC6352
	// This requests the current heading data
	Wire.beginTransmission(slaveAdrs);
	Wire.write("A"); // The "Get Data" command
	Wire.endTransmission();

	// The HMC6352 needs at least a 70us (microsecond) delay
	// after this command.
	delayMicroseconds(rrDelay);

	// Read the 2 heading bytes, MSB first
	// The resulting 16bit word is the compass heading in 10th's of a degree
	// For example: a heading of 1345 would be 134.5 degrees
	Wire.requestFrom(slaveAdrs, 2);
	i = 0;
	while(Wire.available() && i < 2)
	{
		headingData[i] = Wire.read();
		i++;
	}

	// Put the MSB and LSB together
	heading = headingData[0]*256 + headingData[1];
	convertToReal( heading );
	return heading;
}

void Compass::request()
{
	Wire.beginTransmission(slaveAdrs);
	Wire.write("A"); // The "Get Data" command
	Wire.endTransmission();

	//Get the current time
	req_time = micros();
}

int Compass::read()
{
	//Delay if less than 10 milliseconds has passed since the request
	// or the time has been reset
	while(micros() < req_time + rrDelay && micros() > req_time) //*************
	{}

	//Then read
	Wire.requestFrom(slaveAdrs, 2);
	i = 0;
	while(Wire.available() && i < 2)
	{
		headingData[i] = Wire.read();
		i++;
	}

	// Put the MSB and LSB together
	heading = headingData[0]*256 + headingData[1];

	convertToReal( heading );
	return heading;
}

char Compass::direction()
{
	if((heading > 3150 && heading <= 3600) ||
		(heading > 0 && heading <= 450)) return 'N';
	if(heading > 450 && heading <= 1350) return 'E';
	if(heading > 1350 && heading <= 2250) return 'S';
	if(heading > 2250 && heading <= 3150) return 'W';
}

void Compass::serial_print()
{
	Serial.print("Current Heading: ");
	// The whole number part of the heading
	Serial.print(int (heading / 10));
	Serial.print(".");
	//The fractional part
	Serial.print(int (heading % 10));
	Serial.println(" degrees");
}

#endif
