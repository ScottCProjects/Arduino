//**************************************************************//
//  Name    : shiftOutCode, Hello World                                
//  Author  : Carlyn Maw,Tom Igoe, David A. Mellis 
//  Date    : 25 Oct, 2006    
//  Modified: 23 Mar 2010                                 
//  Version : 2.0                                             
//  Notes   : Code for using a 74HC595 Shift Register           //
//          : to count from 0 to 255                           
//
// Modified by Scott Camarena
//   Checks shift register outputs on Anolog pins and digital
//   pins 2 and 3. Instead of outputting to LEDs.
//****************************************************************

//Pin connected to ST_CP of 74HC595
int latchPin = 8;
//Pin connected to SH_CP of 74HC595
int clockPin = 12;
////Pin connected to DS of 74HC595
int dataPin = 11;



void setup() {
  Serial.begin(9600);
  //set pins to output so you can control the shift register
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  for(int i = 14; i < 20; ++i)
    pinMode(i, INPUT);
  Serial.println("Begin...");
}

void loop() {
  // count from 0 to 255 and display the number 
  // on the LEDs
  for (int numberToDisplay = 0; numberToDisplay < 256; numberToDisplay++) {
    // take the latchPin low so 
    // the LEDs don't change while you're sending in bits:
    digitalWrite(latchPin, LOW);
    // shift out the bits:
    shiftOut(dataPin, clockPin, LSBFIRST, numberToDisplay);  

    //take the latch pin high so the LEDs will light up:
    digitalWrite(latchPin, HIGH);
    // pause before next value:
    delay(200);
    readAndDisplayVals();
    delay(500);
  }
}

void readAndDisplayVals()
{
  Serial.print("Bit\t1  2  3  4  5  6  7  8\n\t");
  
  for(int i = 14; i < 20; ++i)
  {
    Serial.print(digitalRead(i));
    Serial.print("  ");
  }
  
  Serial.print(digitalRead(2));
  Serial.print("  ");
  Serial.println(digitalRead(3));
}
