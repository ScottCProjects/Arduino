#include "PushButton.h"


Pushbutton button(4);
void setup()
{
  Serial.begin(9600);
  Serial.println("START");
}

void loop()
{
  while( button.getCt() == 0 );
  Serial.println(button.count);
  button.count = 0;
}
