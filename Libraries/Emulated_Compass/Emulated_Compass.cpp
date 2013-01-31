#include "WProgram.h"
#include <Wire.h>
#include "Compass.h"


Compass::Compass()
{
  HMCAdrs = 0x42;
}

void Compass::setup()
{
  count = 0;
  slaveAdrs = HMCAdrs >> 1;
  Wire.begin();
}

double Compass::req_read()
{/*
  // Send a "A" command to the HMC6352
  // This requests the current heading data
  Wire.beginTransmission(slaveAdrs);
  Wire.send("A"); // The "Get Data" command
  Wire.endTransmission();

  // The HMC6352 needs at least a 70us (microsecond) delay
  // after this command.  Using 10ms just makes it safe
  delay(10);

  // Read the 2 heading bytes, MSB first
  // The resulting 16bit word is the compass heading in 10th's of a degree
  // For example: a heading of 1345 would be 134.5 degrees
  Wire.requestFrom(slaveAdrs, 2);
  i = 0;
  while(Wire.available() && i < 2)
  {
    headingData[i] = Wire.receive();
    i++;
  }

  // Put the MSB and LSB together
  headingVal = headingData[0]*256 + headingData[1];
*/
if(count > 50 && count < 70) headingVal = 450;
else if(count > 70 && count < 100) headingVal = 500;
else if(count > 100 && count < 120) headingVal = 600;
else if(count > 120 && count < 140) headingVal = 700;
else if(count > 140 && count < 160) headingVal = 900;
else if(count > 160 && count < 180) headingVal = 180;
else if(count > 180 && count < 200) headingVal = 300;
else headingVal = 100;
  return (static_cast<double>(headingVal)/10.0);
}

void Compass::request()
{
  Wire.beginTransmission(slaveAdrs);
  Wire.send("A"); // The "Get Data" command
  Wire.endTransmission();

  //Get the current time
  req_time = millis();
}

double Compass::read()
{/*
  //Delay if less than 10 milliseconds has passed since the request
  // or the time has been reset
  while(millis() < req_time + 10 && millis() > req_time) //*************
  {}

  //Then read
  Wire.requestFrom(slaveAdrs, 2);
  i = 0;
  while(Wire.available() && i < 2)
  {
    headingData[i] = Wire.receive();
    i++;
  }

  // Put the MSB and LSB together
  headingVal = headingData[0]*256 + headingData[1];
*/
if(count > 50 && count < 70) headingVal = 450;
else if(count > 70 && count < 100) headingVal = 500;
else if(count > 100 && count < 120) headingVal = 600;
else if(count > 120 && count < 140) headingVal = 700;
else if(count > 140 && count < 160) headingVal = 900;
else if(count > 160 && count < 180) headingVal = 1800;
else if(count > 180 && count < 200) headingVal = 3000;
else headingVal = 100;
  return (static_cast<double>(headingVal)/10.0);
}

char Compass::direction()
{
  if((headingVal > 3150 && headingVal <= 3600) ||
     (headingVal > 0 && headingVal <= 450)) return 'N';
  if(headingVal > 450 && headingVal <= 1350) return 'E';
  if(headingVal > 1350 && headingVal <= 2250) return 'S';
  if(headingVal > 2250 && headingVal <= 3150) return 'W';
}

void Compass::serial_print()
{
  Serial.print("Current Heading: ");
  // The whole number part of the heading
  Serial.print(int (headingVal / 10));
  Serial.print(".");
  //The fractional part
  Serial.print(int (headingVal % 10));
  Serial.println(" degrees");
}
