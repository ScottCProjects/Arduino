#include "Compass.h"
#include <Wire.h>

Compass compass;

void setup()
{
  Serial.begin(9600);
  compass.setup();
  Serial.println("Compass set up.\n\n");
}

void loop()
{
  compass.request();
  Serial.println("Data requested.");

  compass.read();
  Serial.println("Data read.");
  
  compass.serial_print();
  Serial.print("Direction: ");
  
  Serial.println(compass.direction());
  delay(1000);
  
  compass.req_read();
  Serial.println("Data requested and read.");
  
  compass.serial_print();
  Serial.print("Direction: ");
  
  Serial.println(compass.direction());
  Serial.print("\n\n");
  delay(1000);
}
