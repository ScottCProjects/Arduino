// Scott Camarena
// scprojects.wordpress.com
// October 2013
//
//
//Copyright 2013 Scott Camarena
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
// v1.0.1
// Multi Channel Radio Receiver Class
//    Based on FlySky FS-R6B 6 Channel Receiver
//
//============================================================

#ifndef _HOBBYRADIORECEIVER_H_
#define _HOBBYRADIORECEIVER_H_

#include "Arduino.h"

#define Byte uint8_t
#define ROUNDTO 5
#define DEFAULTMODE 0
#define DEADZONEAREA 10

// Valid Modes
#define CENTERDEADZONE 0
#define CENTER 1
#define FULL 2
#define TOGGLE 3
#define RAW 4

class HobbyRadioReceiver
{
private:
	Byte numChannels;
	Byte* channelPin;
	Byte* mode;

	// Keep space for temporary variables when
	//   polling receiver to speed up 'check'
	int checkVal;
	int checkRemain;

public:
	// Constructor allowing infinite number of channels (Actually 255)
	HobbyRadioReceiver( Byte num, ... );

	// Copy Constructor, DONT FORGET!
	// Because this is using dynamic allocation.
	//   I just spent 3 hours trying to figure out why my example
	//   code for this would work, but not the other code
	//   I used this class in. I forgot this constructor >:(
	HobbyRadioReceiver( const HobbyRadioReceiver& );

	// Destructor to free used memory on heap
	~HobbyRadioReceiver();

	int getNumChannels();
	
	// Sets mode of input given channel
	// Valid Modes:
	//  0: center-deadzone
	//	1: center
	//	2: full
	//	3: toggle
	//	4: raw
	void setMode( Byte ch, String m );

	// Returns raw input pulse between 1000-2000
	int checkRaw( Byte ch );

	// Returns constrained pulse 0 to 255 or -255 to 255
	//    based on set mode of input
	int checkExact( Byte ch );

	// Returns adjusted pulse
	//    rounding to the nearest 'ROUNDTO' for less noise
	int check( Byte ch );


};



HobbyRadioReceiver::HobbyRadioReceiver( Byte num, ... )
{
	numChannels = num;
	// Allocate arrays on the heap, freed in class destructor
	channelPin = (Byte*) malloc(num);
	mode = (Byte*) malloc(num);
	// Initialize the list of arguments and store in channelPin array
	va_list args;
	va_start( args, num );
	// Store arguments and initialize modes
	for( Byte i = 0; i < num; ++i )
	{
		channelPin[i] = (Byte) va_arg( args, int );
		mode[i] = DEFAULTMODE;
	}
	va_end(args);
}

HobbyRadioReceiver::HobbyRadioReceiver( const HobbyRadioReceiver& r )
{
	numChannels = r.numChannels;
	channelPin = (Byte*) malloc(numChannels);
	mode = (Byte*) malloc(numChannels);

	// Copy arrays over
	memcpy( channelPin, r.channelPin, numChannels );
	memcpy( mode, r.mode, numChannels );

}
	
HobbyRadioReceiver::~HobbyRadioReceiver()
{
	free(channelPin);
	free(mode);
}

int HobbyRadioReceiver::getNumChannels(){ return (int) numChannels; }

void HobbyRadioReceiver::setMode( Byte ch, String m )
{
	// Valid Modes:
	//  0: center-deadzone
	//	1: center
	//	2: full
	//	3: toggle
	//	4: raw

	if( m == "center-deadzone" ) mode[ch-1] = CENTERDEADZONE;
	else if( m == "center" ) mode[ch-1] = CENTER;
	else if( m == "full" ) mode[ch-1] = FULL;
	else if( m == "raw" ) mode[ch-1] = RAW;
}

int HobbyRadioReceiver::checkRaw( Byte ch )
{
	if( ch > numChannels ) return 0;
	return pulseIn( channelPin[ch-1], HIGH );
}

int HobbyRadioReceiver::checkExact( Byte ch )
{
	switch( mode[ch-1] )
	{
		// Centered with a deadzone
		case CENTERDEADZONE:
			checkVal = constrain( map( checkRaw(ch), 1000,
				2000, -255, 255 ), -255, 255 );
			if( checkVal < DEADZONEAREA && checkVal > -DEADZONEAREA )
				return 0;
			return checkVal;

		// Centered
		case CENTER:
			return constrain( map( checkRaw(ch), 1000,
				2000, -255, 255 ), -255, 255 );

		// Full length
		case FULL:
			return constrain( map( checkRaw(ch), 1000,
				2000, 0, 255 ), 0, 255 );

		// Toggle
		case TOGGLE:
			return constrain( map( checkRaw(ch), 1000,
				2000, 0, 1 ), 0, 1 );

		// Raw
		case RAW:
			return checkRaw(ch);
	}
}

int HobbyRadioReceiver::check( Byte ch )
{
	checkVal = checkExact(ch);
	checkRemain = checkVal % ROUNDTO;

	// Return value rounded up or down to nearest 'ROUNDTO'
	if( checkRemain < 5 )
		return checkVal - checkRemain;

	return checkVal + (ROUNDTO - checkRemain);
}



















#undef Byte
#endif
