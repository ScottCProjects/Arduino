// Scott Camarena
// scprojects.wordpress.com
// November 2012
// Lang: Arduino C++
// DC Motor Class
//=============================================================


#ifndef _DCMOTOR_H_
#define _DCMOTOR_H_

#include "Arduino.h"

class DCMotor
{
private:
	int pinA, pinB;
	int speed;

public:
	DCMotor( int pA, int pB );
	
	void on( char dir, byte pow = 255 );
	void on( int pow = 255 ); // Takes 'int' to handle negatives
	void off();
	void forward( byte pow = 255 );
	void back( byte pow = 255 );
	
	int getSpeed();
};


DCMotor::DCMotor( int pA, int pB )
: pinA(pA), pinB(pB), speed(0)
{
	pinMode( pinA, OUTPUT );
	pinMode( pinB, OUTPUT );
	digitalWrite( pinA, LOW );
	digitalWrite( pinB, LOW );
}

void DCMotor::on( char dir, byte pow )
{
	switch( dir )
	{
		case 'f':
			analogWrite( pinA, pow );
			digitalWrite( pinB, LOW );
			speed = pow;
			break;
		case 'b':
			digitalWrite( pinA, LOW );
			analogWrite( pinB, pow );
			speed = -pow;
	}
}
void DCMotor::on( int pow )
{
	if( pow > 0 ) on( 'f', pow );
	else on( 'b', -pow );
}

void DCMotor::off()
{
	digitalWrite( pinA, LOW );
	digitalWrite( pinB, LOW );
}

void DCMotor::forward( byte pow )
{
	on( 'f', pow );
}

void DCMotor::back( byte pow )
{
	on( 'b', pow );
}

int DCMotor::getSpeed()
{
	return speed;
}












#endif
