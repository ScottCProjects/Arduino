// Scott Camarena
// scprojects.wordpress.com
//
//
//Copyright 2014 Scott Camarena
//
//This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//
//	EncodedMotor.h
//		- Class for using an encoded DC motor
//
//============================================================

#ifndef _ENCODEDMOTOR_H_
#define _ENCODEDMOTOR_H_

#include "Arduino.h"

#define POW_INCREMENT 10
#define HERTZ 10

class EncodedMotor
{
private:
	int encPin1, encPin2, pinA, pinB;
	volatile int stepCount;
	volatile int stepsPerSec, expectedSteps;
	bool forward;
	byte power;

public:
	EncodedMotor( int ePin, int pA, int pB )
	: encPin1(ePin), encPin2(-1), pinA(pA), pinB(pB), power(0),
			stepCount(0), stepsPerSec(0), expectedSteps(0), forward(true)
	{
	}

	static void timerSetup()
	{
		cli();//stop interrupts
		//set timer1 interrupt at 1Hz
		TCCR1A = 0;// set entire TCCR1A register to 0
		TCCR1B = 0;// same for TCCR1B
		TCNT1  = 0;//initialize counter value to 0
		// set compare match register for 1hz increments
		OCR1A = 15624 / HERTZ;// = (16*10^6) / (1*1024) - 1 (must be <65536)
		// turn on CTC mode
		TCCR1B |= (1 << WGM12);
		// Set CS10 and CS12 bits for 1024 prescaler
		TCCR1B |= (1 << CS12) | (1 << CS10);  
		// enable timer compare interrupt
		TIMSK1 |= (1 << OCIE1A);
		sei();//allow interrupts
	}

//========================================================
//====================== Movement ========================
//========================================================

	void onForward( int stepsPSec )
	{
		setForward(true);
		setSpeed(stepsPSec);
	}
	void onBack( int stepsPSec )
	{
		setForward(false);
		setSpeed(stepsPSec);
	}

	void go( int stepsPSec )
	{
		setSpeed(stepsPSec);
	}

	void setForward( bool fb )
	{
		int temp;
		// If not already set to same
		if(forward != fb)
		{
			// Simply swap the pins
			temp = pinA;
			pinA = pinB;
			pinB = temp;
			forward = fb;
		}
	}

	void setSpeed( int stepsPSec )
	{
		stepsPerSec = stepsPSec;
		expectedSteps = stepsPSec / HERTZ;
		power = 200;
	}

	void off()
	{
		setSpeed(0);
		digitalWrite( pinA, LOW );
		digitalWrite( pinB, LOW );
	}

//========================================================
//====================== Interrupts ======================
//========================================================
	
	void stepISR()
	{
		++stepCount;
	}

	void timeISR()
	{
		// If less steps taken than expected turn on
		if( stepCount < expectedSteps && power < 245 )
		{
			//on();
			power += 10;
		}
		// If more than expected, turn off
		else if( stepCount > expectedSteps && power > 10 )
		{
			//off();
			power -= 10;
		}

		// Reset stepCount
		stepCount = 0;
Serial.println(power);
		analogWrite( pinA, power );
		digitalWrite( pinB, LOW );
	}
};


#undef POW_INCREMENT
#endif
