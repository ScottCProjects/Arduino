// Scott Camarena
//
// - Automatic Fish Feeder
//  Using a Servo and Timed Alarm
//=================================================
#include <Servo.h>
#include <Time.h>
#include <TimeAlarms.h>


#define ST_HOUR 8
#define ST_MIN 45

#define TANK_LED_PIN 8
#define TOP_LED_PIN 13
#define SERVO_PIN 6
#define MIDPOINT 90
#define MAXDAYSFOOD 2


Servo feeder;

int dayNum;

// Degrees of rotation to drop each day's food.
// Rotation to the right for first 4 days,
//  and back to the left for last 4 days
int dayDrop[MAXDAYSFOOD] = {0, 180};

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
    blinkLED(TOP_LED_PIN, 10, 1000);
  }
}
  


void feedFish()
{
  blinkLED(TANK_LED_PIN, 3, 200);
  Serial.print("Feeding fish - Day");
  Serial.println(dayNum+1);
  // Rotate the feeder to drop current day's food,
  //  rotate back to mid,
  //  then incriment the day.
  if( dayNum < MAXDAYSFOOD )
  {
    feeder.write(dayDrop[dayNum++]);
    delay(200);
    feeder.write(MIDPOINT);
  }
    
  if( dayNum == MAXDAYSFOOD )
    needsRefill();
}

void setup()
{
  digitalWrite(TOP_LED_PIN, LOW);
  digitalWrite(9, LOW);
  digitalWrite(TANK_LED_PIN, LOW);
  Serial.begin(9600);
  feeder.attach(SERVO_PIN);
  setTime(ST_HOUR, ST_MIN, 00, 7, 3, 13); // set time to 8:00:00pm July 2 2013
  resetFeeder();
  // Feed fish once a day at 8am.
  //Alarm.alarmRepeat( 8, 00, 5, feedFish );
  //Alarm.alarmRepeat( 8, 00, 15, feedFish );
  //Alarm.alarmRepeat( 8, 00, 10, feedFish );
  Alarm.alarmOnce( ST_HOUR, ST_MIN, 10, feedFish );
  resetFeeder();
  Alarm.alarmOnce( dowFriday, 8, 00, 00, feedFish );
  Alarm.alarmOnce( dowSaturday, 8, 00, 00, feedFish );
}


void loop()
{
  digitalClockDisplay();
  Alarm.delay(1000);
}

void digitalClockDisplay()
{
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.println(); 
}

void printDigits(int digits)
{
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}
