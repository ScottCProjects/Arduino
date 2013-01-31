//
// Pushbutton
//
//===========================================================


#ifndef _PUSHBUTTON_H_
#define _PUSHBUTTON_H_

#include "Arduino.h"
#include "../ooPinChangeInt/ooPinChangeInt.h"
#define LIBCALL_OOPINCHANGEINT

#define debounceTime 400

class Pushbutton: public CallBackInterface
{
	public:
	int pin;
	volatile int count;
	volatile unsigned long intTime;

	Pushbutton( int p = 0 );
	volatile bool isPushed();
	void pause();
	int getCt();
	void cbmethod();
};



Pushbutton::Pushbutton( int p )
: pin(p), count(0), intTime(0)
{
	pinMode( pin, INPUT );
	digitalWrite( pin, HIGH );
	PCintPort::attachInterrupt( pin, this, FALLING );
}

volatile bool Pushbutton::isPushed()
{
	bool pushed = count != 0;
	count = 0;
	return pushed;
}

void Pushbutton::pause()
{
	count = 0;
	while( !count );
	count = 0;
}

int Pushbutton::getCt()
{
	delayMicroseconds(100);
	return count;
}

void Pushbutton::cbmethod()
{
	//Serial.print("in cbmethod: ");
	//Serial.println(freeMemory());
	if( millis() >= intTime + debounceTime )
	count++;
	intTime = millis();
	Serial.println("*Pushed*");
	//Serial.println("button interrupt");
}












#undef debounceTime
#endif
