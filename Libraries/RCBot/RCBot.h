// Scott Camarena
// scprojects.wordpress.com
// 2012
// Lang: Arduino C++
//
// 2 motor robot controlled by Hobby Remote (6 Channel)
//============================================================

#ifndef _RCBOT_H_
#define _RCBOT_H_

#include "Arduino.h"
#include "../HobbyRadioReceiver/HobbyRadioReceiver.h"
#include "../DCMotor/DCMotor.h"
#include "../TwoWheelBot/TwoWheelBot.h"

#define Byte uint8_t

// Define the channels used
class RCBot : public TwoWheelBot
{
private:
	Byte throtChan, turnChan;
	int throt, turn, leftPow, rightPow;

public:
	HobbyRadioReceiver rec;
	RCBot( DCMotor l, DCMotor r, HobbyRadioReceiver re )
	: TwoWheelBot(l, r), rec(re), throtChan(3), turnChan(1),
		throt(0), turn(0), leftPow(0), rightPow(0) {}
	
	// Set which channel is throttle and turn
	void setThrotAndTurn( Byte throtC, Byte turnC )
	{
		throtChan = throtC;
		turnChan = turnC;
	}

	// One control cycle
	void control()
	{
		// Check the receiver channels
		throt = rec.check( throtChan );
		turn = rec.check( turnChan );
		
		// Adjust each motor to turn
		leftM.on( throt + turn );
		rightM.on( throt - turn );
	}

	void printVals()
	{
		Serial.print("Throttle: ");
		Serial.println(throt);
		Serial.print("Turn: ");
		Serial.println(turn);
	}
};
















#endif
