// Scott Camarena
// October 2012
// Language: Arduino
// 6 Channel Radio Receiver Class
//============================================================

#ifndef _SIXCHRECEIVER_H_
#define _SIXCHRECEIVER_H_

#include "Arduino.h"

class sixChReceiver
{
private:
	int inCh[6];
	int mode[6];

public:

	sixChReceiver( int c1 = -1, int c2 = -1, int c3 = -1,
		int c4 = -1, int c5 = -1, int c6 = -1 );
	
	void setMode( int ch, int m );

	// Returns raw input pulse between 1000-2000
	int rawCheck( int ch );

	// Returns adjusted pulse based on settings
	int check( int ch );


};



sixChReceiver::sixChReceiver( int c1, int c2, int c3,
	int c4, int c5, int c6 )
{
	for( int i = 5; i >= 0; --i )
		mode[i] = 0;
	inCh[0] = c1;
	inCh[1] = c2;
	inCh[2] = c3;
	inCh[3] = c4;
	inCh[4] = c5;
	inCh[5] = c6;
	for( int i = 5; i >= 0; --i )
		if( inCh[i] != -1 ) pinMode( inCh[i], INPUT );
}

void sixChReceiver::setMode( int ch, int m )
{
	// Valid Modes:
	//	0: center
	//	1: full
	//	2: toggle
	//	3: raw
	if( ch >= 1 && ch <= 6 && m >= 0 && m <= 3 )
		mode[ch-1] = m;
}

int sixChReceiver::rawCheck( int ch )
{
	if( inCh[ch-1] != -1 )
		return pulseIn( inCh[ch-1], HIGH );
	return 0;
}

int sixChReceiver::check( int ch )
{
	switch( mode[ch-1] )
	{
		// Centered
		case 0:
			return constrain( map( rawCheck(ch), 1000,
				2000, -255, 255 ), -255, 255 );

		// Full length
		case 1:
			return constrain( map( rawCheck(ch), 1000,
				2000, 0, 255 ), 0, 255 );

		// Toggle
		case 2:
			return constrain( map( rawCheck(ch), 1000,
				2000, 0, 1 ), 0, 1 );

		// Raw
		case 3:
			return rawCheck(ch);
	}
}



















#endif
