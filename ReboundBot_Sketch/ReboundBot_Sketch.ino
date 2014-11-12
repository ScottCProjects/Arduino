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
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);               // wait for a second
  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);  
  bot.setThrotAndTurn(2, 1);
  
}

void loop()
{
  bot.control();
  bot.printVals();
}
