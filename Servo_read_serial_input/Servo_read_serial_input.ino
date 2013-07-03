#include <Servo.h>

Servo s1;

int pos = 0;
int input = 0;
int input_digit = 0;

void setup()
{
  s1.attach(6);
  Serial.begin(9600);
}


void loop()
{
  if(Serial.available())
  {
    input = 0;
    while(Serial.available())
    {
      input_digit = Serial.read();
      input = input*10 + (input_digit - 48);
      delay(5);
    }
    
    if( input >= 0 && input <= 180 )
    {
      pos = input;
      s1.write(pos);
    }
    Serial.println(s1.read());
  }
  else delay(5); // Just to slow down the loop.
}
