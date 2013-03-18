//#include <DHT22.h>
//#include <SPI.h>
#include <SensorSystem.h>
//#include <SoftwareSerial.h>

/*
#define 

*/
char command;

void setup()
{
  Serial.begin(28800);
  //SerialBT.begin(9600);
}

void loop()
{
  // Read in command
  command = readInCommand();

  // Execute command
  if( execCommand(command) != 0 )
    Serial.println("*Invalid Command" );
  
}
