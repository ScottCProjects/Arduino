#include "Compass.h"
#include <Wire.h>

Compass compass;

void setup()
{
  Serial.begin(9600);
  Serial.println("Press to Calibrate Compass");
  while( !Serial.available() );
  Serial.flush();
  Serial.println("Calibrating...");
  compass.calibrate();
  Serial.println("Calibration done.");
  Serial.println("Press any key to set trueNorth");
  while( !Serial.available() );
  Serial.flush();
  compass.setTrueNorth();
  Serial.print("trueNorth = ");
  Serial.println( compass.trueNorth );
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
