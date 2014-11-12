// Create my own namespace to use instead of
//   the standard that the original functions are in
namespace mine
{
	// Keep track of the 20 pins on the Arduino,
	//   this will be the virtual replacement
	//   These are held as digital pins for now, meaning
	//   they only hold boolean HIGH and LOW.
	bool pinVals[20];
	for( int i = 0; i < 20; ++i )
		pinVals[i] = LOW; // aka "false"
	

	// Also keep track of values from pulseIn
	int pulseInVals[20];
	for( int i = 0; i < 20; ++i )
		pinVals[i] = 0;
	

	// Create functions to be overwritten

	void digitalWrite( int pin, bool hl )
	{
		Serial.print("Wrote pin: ");
		Serial.print(pin);
		if(hl) Serial.println(" HIGH");
		else Serial.println(" LOW");

		pinVals[pin] = hl;
		
		// Optionally call the real one at the end
		//std::digitalWrite(pin, hl);
	}

	bool digitalRead( int pin, bool hl )
	{
		Serial.print("Reading pin: ");
		Serial.print(pin);

		// Read our virtual pin instead of real one
		return pinVals[pin];
	}

	int pulseIn( int pin, bool hl )
	{
		return pulseInVals[pin];
	}

}

using namespace mine;

// Include file after "using namespace" in order to use that
//   namespace throughout the libraries code instead of standard.
//   This way we don't have to change anything in that code.

#include "HobbyRadioReceiver.h"
#include <ArduinoUnit.h>

// Test the "checkRaw" function
test(checkRaw)
{
	int pulseTestVal[] = {0, 900, 1500, 2000, 2500};

	// 1 channel receiver on Arduino pin 2
	HobbyRadioReceiver rec(1, 2);
	
	for( int i = 0; i < 5; ++i )
	{
		// Set the value of the pin to be read
		pulseInVals[2] = pulseTestVal[i];

		// Call function on channel 1
		int returned = rec.checkRaw(1);

		// Make sure it returned the correct value
		assertEqual( pulseTestVal[i], returned );
	}
}

// Test the "checkExact" function
test(checkExact)
{
	// Keep track of inputs and expected outputs for each loop
	int pulseTestVal[] = {0, 900, 1500, 2000, 2500};
	int returnVals[] = {-255, -255, 0, 255, 255};

	// 1 channel receiver on Arduino pin 2
	HobbyRadioReceiver rec(1, 2);

	// Make sure receiver is in "center-deadzone" mode
	rec.setMode(1, 0);
	
	// Test each expected value
	for( int i = 0; i < 5; ++i )
	{
		// Set the value of the pin to be read
		pulseInVals[2] = pulseTestVal[i];

		// Call function on channel 1
		int returned = rec.checkRaw(1);

		// Make sure it returned the correct value
		assertEqual( returnVals[i], returned );
	}
}

void setup
{
	Serial.begin(9600);


}

void loop
{
	// Test everything
	Test::run();
}












