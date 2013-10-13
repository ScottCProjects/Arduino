// Scott Camarena
// October 2012
// Language: Arduino
//
// 6 Channel Radio Receiver Class
//    Using FlySky FS-R6B Receiver
//
//============================================================


// ***************************************************
// ***************************************************

// DEPRECIATED: Replaced with HobbyRadioReceiver Class

// ***************************************************
// ***************************************************


#ifndef _SIXCHRECEIVER_H_
#define _SIXCHRECEIVER_H_

#include "Arduino.h"

#define ROUNDTO 5
#define Byte uint8_t

class sixChReceiver
{
private:
	int channelPin[6];
	Byte mode[6];

public:
	// Default constructor to allow specification of less
	//   than 6 connected channels
	sixChReceiver( int c1 = -1, int c2 = -1, int c3 = -1,
		int c4 = -1, int c5 = -1, int c6 = -1 );
	
	// Sets mode of input given channel
	void setMode( Byte ch, Byte m );

	// Returns raw input pulse between 1000-2000
	int checkRaw( Byte ch );

	// Returns constrained pulse 0 to 255 or -255 to 255
	//    based on set mode of input
	int checkExact( Byte ch );

	// Returns adjusted pulse
	//    rounding to the nearest 'ROUNDTO' for less noise
	int check( Byte ch );


};



sixChReceiver::sixChReceiver( int c1, int c2, int c3,
	int c4, int c5, int c6 )
{
	// Set mode to "center" by default
	for( Byte i = 5; i >= 0; --i )
		mode[i] = 0;
	// Store into array.
	channelPin[0] = c1;
	channelPin[1] = c2;
	channelPin[2] = c3;
	channelPin[3] = c4;
	channelPin[4] = c5;
	channelPin[5] = c6;

	// For every connected channel, set Arduino pin to input.
	for( Byte i = 5; i >= 0; --i )
		if( channelPin[i] != -1 ) pinMode( channelPin[i], INPUT );
}

void sixChReceiver::setMode( Byte ch, Byte m )
{
	// Valid Modes:
	//	0: center
	//	1: full
	//	2: toggle
	//	3: raw
	if( ch >= 1 && ch <= 6 && m >= 0 && m <= 3 )
		mode[ch-1] = m;
}

int sixChReceiver::checkRaw( Byte ch )
{
	if( channelPin[ch-1] != -1 )
		return pulseIn( channelPin[ch-1], HIGH );
	return 0;
}

int sixChReceiver::checkExact( Byte ch )
{
	switch( mode[ch-1] )
	{
		// Centered
		case 0:
			return constrain( map( checkRaw(ch), 1000,
				2000, -255, 255 ), -255, 255 );

		// Full length
		case 1:
			return constrain( map( checkRaw(ch), 1000,
				2000, 0, 255 ), 0, 255 );

		// Toggle
		case 2:
			return constrain( map( checkRaw(ch), 1000,
				2000, 0, 1 ), 0, 1 );

		// Raw
		case 3:
			return checkRaw(ch);
	}
}

int sixChReceiver::check( Byte ch )
{
	int val = checkExact(ch);
	return val - (val % ROUNDTO);
}




















#endif
