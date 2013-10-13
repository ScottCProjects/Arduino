// Scott Camarena
// scprojects.wordpress.com
// November 2012
// Lang: Arduino C++
//   TwoWheelBot Example
//=============================================================

#include <TwoWheelBot.h>

TwoWheelBot bot( DCMotor( 3, 5 ), DCMotor( 9, 10 ) );
int mspeed = 0;    // motor speed
int fadeAmount = 5;    // how many points to adjust speed by
void setup()
{
  
}

void loop()
{
  // Speed up
  while(mspeed < 255)
  {
    bot.leftM.on(mspeed);
    bot.rightM.back(mspeed);
    // change the brightness for next time through the loop:
    mspeed = mspeed + fadeAmount;
    delay(30);
  }
  delay(100);
  
  // Slow down
  while(mspeed > 0)
  {
    bot.leftM.on(mspeed);
    bot.rightM.back(mspeed);
    // change the brightness for next time through the loop:
    mspeed = mspeed - fadeAmount;
    delay(30);
  }
  bot.leftM.off();
  delay(50);
  
  // Speed up backwards
  while(mspeed < 255)
  {
    bot.leftM.back(mspeed);
    bot.rightM.on(mspeed);
    // change the brightness for next time through the loop:
    mspeed = mspeed + fadeAmount;
    delay(30);
  }
  delay(100);
  
  // Slow down
  while(mspeed > 0)
  {
    bot.leftM.back(mspeed);
    bot.rightM.on(mspeed);
    // change the brightness for next time through the loop:
    mspeed = mspeed - fadeAmount;
    delay(30);
  }
  bot.leftM.off();
  delay(100);
}
