// Scott Camarena
// scprojects.wordpress.com
// 2012
// Lang: Arduino C++
// 2 motor robot controlled by Hobby Remote (6 Channel)
//============================================================

#ifndef _RCBOT_H_
#define _RCBOT_H_

#include "Arduino.h"
#include "../sixChReceiver/sixChReceiver.h"
#include "../DCMotor/DCMotor.h"

// Define the channels used
#define THROTTLE 2
#define TURNING 1

class RCBot
{
private:
	DCMotor leftM, rightM;
	sixChReceiver rec;
	int throt, turn, leftPow, rightPow;

public:
	RCBot( DCMotor l, DCMotor r, sixChReceiver re )
	: leftM(l), rightM(r), rec(re), throt(0), turn(0),
		leftPow(0), rightPow(0) {}

	// One control cycle
	void control()
	{
		// Check the receiver channels
		throt = rec.check( THROTTLE );
		turn = rec.check( TURNING );
		
		// Adjust each motor to turn
		leftM.on( throt + turn );
		rightM.on( throt - turn );
	}
};
















#endif
