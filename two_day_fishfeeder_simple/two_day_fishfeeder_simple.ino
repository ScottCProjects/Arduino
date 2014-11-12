// Scott Camarena
//
// - Automatic Fish Feeder
//  Using a Servo and Delay
//=================================================
#include <Servo.h>

#define TOP_LED_PIN 13
#define SERVO_PIN 6
#define SPLASH_PIN 11

// Every 3 Days
#define TIME_BETWEEN 259200000
//#define TIME_BETWEEN 5000

Servo feeder, splasher;

void blinkLED(int pin, int times, int rate)
{
  for(int i = 0; i < times; ++i)
  {
    digitalWrite(pin, HIGH);
    delay(rate);
    digitalWrite(pin, LOW);
    delay(rate);
  }
}

void splash( Servo &v )
{
  for( int i = 0; i < 3; ++i )
  {
    v.write(180);
    delay(400);
    v.write(0);
    delay(400);
  }
  v.write(90);
}

void feed( Servo &v, int deg )
{
  v.write(deg);
  delay(400);
  v.write(90);
  delay(10);
}

void setup()
{
  digitalWrite(TOP_LED_PIN, LOW);
  feeder.attach(SERVO_PIN);
  feeder.write(90);
  splasher.attach(SPLASH_PIN);
  splasher.write(90);
  blinkLED(TOP_LED_PIN, 2, 50);
  delay(500);
  splash( splasher );
  feed( feeder, 0 );
}

void loop()
{
  delay(TIME_BETWEEN);
  splash( splasher );
  feed( feeder, 0 );
  delay(TIME_BETWEEN);
  splash( splasher );
  feed( feeder, 180 );
}
