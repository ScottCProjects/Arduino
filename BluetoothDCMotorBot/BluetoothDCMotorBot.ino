// Scott Camarena
// scprojects.wordpress.com
//
//=============================================================

#include "DCMotor.h"

DCMotor leftM( 6, 5 );
DCMotor rightM( 9, 10 );
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
//char[10] input;
int motorPower[2] = {0,0};

void parseCommand( String input, int& out1, int& out2 )
{
  out1 = input.substring( 1, 4 ).toInt();
  if(out1 > 255)
    out1 = 0;
  if( input.charAt(0) == '-' )
    out1 = -out1;
  out2 = input.substring( 5, 8 ).toInt();
  if(out2 > 255)
    out2 = 0;
  if( input.charAt(4) == '-' )
    out2 = -out2;
}


//========================================================
//========================================================
//========================================================

void setup()
{
  Serial.begin(9600);
  Serial.println("Start...");
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
  leftM.off();
  rightM.off();
}

void loop()
{
  // print the string when a newline arrives:
  while( Serial.available() ) {
    // get the new byte:
    char inChar = (char)Serial.read(); 
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '.') {
      stringComplete = true;
    } 
  }
  
  if (stringComplete) {
    //input = inputString.toCharArray();
    Serial.print(">");
    Serial.println(inputString);
    parseCommand(inputString, motorPower[0], motorPower[1]);
    leftM.on(motorPower[0]);
    rightM.on(motorPower[1]);
    
    Serial.print("Motor1: ");
    Serial.println(leftM.getSpeed());
    Serial.print("Motor2: ");
    Serial.println(rightM.getSpeed());
    stringComplete = false;
    inputString = "";
  }
}
