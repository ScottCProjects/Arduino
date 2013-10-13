// Scott Camarena
// scprojects.wordpress.com
// November 2012
// Lang: Arduino C++
// ReboundBot controlled with 6Ch Remote
//=============================================================

#include "RCBot.h"

RCBot bot( DCMotor(3, 5), DCMotor(9, 10),
  HobbyRadioReceiver( 4, A0, A1, A2, A3 ) );
void setup()
{
  Serial.begin(9600);
  bot.setThrotAndTurn(2, 1);
  
}

void loop()
{
  bot.control();
  bot.printVals();
}
