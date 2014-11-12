// Scott Camarena
// scprojects.wordpress.com
// November 2012
// Lang: Arduino C++
// DC Motor Class Example
//=============================================================

#include "DCMotor.h"

DCMotor motor1( 3, 5 );
DCMotor motor2( 9, 10 );
int mspeed = 0;    // motor speed
int fadeAmount = 5;    // how many points to adjust speed by
void setup()
{
  Serial.begin(9600);
  Serial.println("Start...");
}

void loop()
{
  // Speed up
  Serial.println("Speed up.");
  while(mspeed < 255)
  {
    motor1.on(mspeed);
    motor2.on(mspeed);
    // change the brightness for next time through the loop:
    mspeed = mspeed + fadeAmount;
    delay(30);
  }
  delay(2000);
  
  // Slow down
  Serial.println("Slow down.");
  while(mspeed > 0)
  {
    motor1.on(mspeed);
    motor2.on(mspeed);
    // change the brightness for next time through the loop:
    mspeed = mspeed - fadeAmount;
    delay(30);
  }
  motor1.off();
  motor2.off();
  delay(500);
  
  // Speed up backwards
  Serial.println("Speed up backward.");
  while(mspeed < 255)
  {
    motor1.back(mspeed);
    motor2.back(mspeed);
    // change the brightness for next time through the loop:
    mspeed = mspeed + fadeAmount;
    delay(30);
  }
  delay(2000);
  
  // Slow down
  Serial.println("Slow down back.");
  while(mspeed > 0)
  {
    motor1.back(mspeed);
    motor2.back(mspeed);
    // change the brightness for next time through the loop:
    mspeed = mspeed - fadeAmount;
    delay(30);
  }
  motor1.off();
  motor2.off();
  delay(1000);
}