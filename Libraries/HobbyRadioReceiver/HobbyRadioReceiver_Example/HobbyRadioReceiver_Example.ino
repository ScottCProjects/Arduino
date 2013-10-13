// Scott Camarena
// October 2013
//
// Hobby Radio Receiver Class Example
//=======================================================

#include "HobbyRadioReceiver.h"

// Specify the number of channels,
//   followed by the pins the channels are attached to
HobbyRadioReceiver rec( 4, A0, A1, A2, A3 );
int val;
void setup()
{
  Serial.begin(9600);
  Serial.print( "Num Channels: " );
  Serial.println(rec.getNumChannels());
}

void loop()
{
  val = rec.check(1);
  Serial.print( val );
  Serial.print( "\t" );
  val = rec.check(2);
  Serial.print( val );
  Serial.print( "\t" );
  val = rec.check(3);
  Serial.print( val );
  Serial.print( "\t" );
  val = rec.check(4);
  Serial.print( val );
  delay(50);
  Serial.println( "\n\n\n\n\n\n\n\n\n\n\n" );
}
