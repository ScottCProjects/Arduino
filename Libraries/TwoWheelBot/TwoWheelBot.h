// Scott Camarena
// scprojects.wordpress.com
// October 2013
// Lang: Arduino C++
//
// 2 wheeled robot using DC motors
//   Meant to be used as an abstract class for future robot classes
//============================================================

#ifndef _TWOWHEELBOT_H_
#define _TWOWHEELBOT_H_

#include "Arduino.h"
#include "../DCMotor/DCMotor.h"


class TwoWheelBot
{
protected:

public:
	DCMotor leftM, rightM;

	// Constructor passing pin numbers
	TwoWheelBot( int leftM_for, int leftM_back,
						int rightM_for, int rightM_back )
	: leftM( DCMotor(leftM_for, leftM_back) ),
		rightM( DCMotor(rightM_for, rightM_back) ) {}

	// Constructor passing motor objects
	TwoWheelBot( DCMotor l, DCMotor r )
	: leftM(l), rightM(r) {}

};















#endif
