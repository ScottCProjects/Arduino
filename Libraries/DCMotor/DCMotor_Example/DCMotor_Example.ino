// Scott Camarena
// scprojects.wordpress.com
// November 2012
// Lang: Arduino C++
// DC Motor Class
//=============================================================

#include "DCMotor.h"

DCMotor motor1( 2, 3 );
void setup()
{
  
}

void loop()
{
  motor1.on();
  delay(100);
  motor1.off();
  delay(100);
  motor1.on( 'b', 100 );
  delay(100);
  motor1.off();
}
