#include <DHT22.h>
#include <SPI.h>
#include <MPL115A1.h>
#include <SensorSystem.h>
#include <SoftwareSerial.h>

String command;

void setup()
{
  randomSeed(analogRead(0));
  Serial.begin(9600);
  bpsensor.begin();
  Serial.println("Starting...");
  //SERIALBT.begin(9600);
  //SerialBT.println("Starting...");
}

void loop()
{
  // Read in command
  Serial.println("Listening for command...");
  command = readInCommand();
  //Serial.println("Received: " + command);
  

  // Execute command
  Serial.println("Executing: " + command);
  if( execCommand(command) != 0 )
    Serial.println("*Invalid Command" );
  delay(1000);
}
