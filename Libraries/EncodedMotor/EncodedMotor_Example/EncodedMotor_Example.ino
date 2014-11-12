// Scott Camarena
// scprojects.wordpress.com
// November 2014
//
// Encoded DC Motor Class Example
//=============================================================

#include "EncodedMotor.h"

#define L_ENC    2
#define L_PIN_A  5
#define L_PIN_B  6

#define R_ENC    3
#define R_PIN_A  7
#define R_PIN_B  8


EncodedMotor leftM( L_ENC, L_PIN_A, L_PIN_B );
EncodedMotor rightM( R_ENC, R_PIN_A, R_PIN_B );

// Functions to attach to interrupt
void leftISR()
{
  leftM.stepISR();
}
void rightISR()
{
  rightM.stepISR();
}

// Setup timer interrupt
ISR(TIMER1_COMPA_vect)
{
  Serial.print(".");
  leftM.timeISR();
  rightM.timeISR();
}

void setup()
{
  Serial.begin(9600);
  EncodedMotor::timerSetup();
  attachInterrupt(0, leftISR, CHANGE);
  attachInterrupt(1, rightISR, CHANGE);
  leftM.go(10000);
  rightM.go(5000);
}

void loop()
{
  Serial.print("+");
  delay(1000);
}
