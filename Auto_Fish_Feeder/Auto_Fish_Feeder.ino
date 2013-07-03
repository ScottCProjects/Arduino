// Scott Camarena
//
// - Automatic Fish Feeder
//  Using a Servo and Timed Alarm
//=================================================
#include <Servo.h>
#include <Time.h>
#include <TimeAlarms.h>

#define TANK_LED_PIN 7
#define TOP_LED_PIN 8
#define SERVO_PIN 6
#define MIDPOINT 80
#define MAXDAYSFOOD 8


Servo feeder;

int dayNum;

// Degrees of rotation to drop each day's food.
// Rotation to the right for first 4 days,
//  and back to the left for last 4 days
int dayDrop[MAXDAYSFOOD] = {110, 130, 150, 170, 60, 40, 20, 0};

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

void resetFeeder()
{
  feeder.write(MIDPOINT);
  dayNum = 0;
}

// Fail state
void needsRefill()
{
  
  Serial.println("Out of food!");
  while(1)
  {
    blinkLED(TOP_LED_PIN, 10, 50);
  }
}
  


void feedFish()
{
  blinkLED(TANK_LED_PIN, 3, 50);
  Serial.print("Feeding fish - Day");
  Serial.println(dayNum+1);
  // Rotate the feeder to drop current day's food,
  //  then incriment the day.
  if( dayNum < MAXDAYSFOOD )
    feeder.write(dayDrop[dayNum++]);
  else needsRefill();
}

void setup()
{
  Serial.begin(9600);
  feeder.attach(SERVO_PIN);
  setTime(20,00,00,7,3,13); // set time to 8:00:00pm July 2 2013
  resetFeeder();
  // Feed fish once a day at 8am.
  Alarm.alarmRepeat( 8, 00, 00, feedFish );
}


void loop()
{
}
