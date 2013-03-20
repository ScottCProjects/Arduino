#include <DHT22.h>
//#include <SPI.h>
#include <SensorSystem.h>
//#include <SoftwareSerial.h>

/*
#define 

*/
String command;

void setup()
{
  randomSeed(analogRead(0));
  Serial.begin(9600);
  Serial.println("Starting...");
  //SerialBT.begin(9600);
}

void loop()
{
  // Read in command
  Serial.println("Listening for command...");
  command = readInCommand();
  Serial.println("Received: " + command);
  

  // Execute command
  Serial.println("Executing: " + command);
  if( execCommand(command) != 0 )
    Serial.println("*Invalid Command" );
  
}
