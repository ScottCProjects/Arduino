// Scott Camarena
// October 2012
// 6 Channel Radio Receiver Class Example
//=======================================================

#include "sixChReceiver.h"

sixChReceiver rec( 2, 3 );
int val;
void setup()
{
  Serial.begin(9600);
}

void loop()
{
  val = rec.check(1);
  //Serial.print( val );
  //Serial.print( "  " );
  //val = rec.check(2);
  Serial.println( val );
  delay(10);
}
