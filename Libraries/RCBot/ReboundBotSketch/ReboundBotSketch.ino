// Scott Camarena
// scprojects.wordpress.com
// November 2012
// Lang: Arduino C++
// ReboundBot controlled with 6Ch Remote
//=============================================================

#include "RCBot.h"


RCBot bot( DCMotor(3, 9), DCMotor(10, 11),
  sixChReceiver(2, 4) );
void setup()
{
}

void loop()
{
  bot.control();
}
