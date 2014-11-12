//#include <PinChangeInt.h>

#define SWITCH_SIG 0
#define SWITCH_HIGH 3
#define SWITCH_GRD 4

#define ENA 8
#define CSA 9
#define IN1 10
#define IN2 11
#define LOGIC_SUPPLY 12

int count = 1;
volatile int dir1 = HIGH;
volatile int dir2 = LOW;

void setup()
{
  Serial.begin(9600);
  // Init Pins
  pinMode(SWITCH_HIGH, OUTPUT);
  pinMode(SWITCH_GRD, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(CSA, INPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(LOGIC_SUPPLY, OUTPUT);
  
  digitalWrite(LOGIC_SUPPLY, HIGH);
  digitalWrite(SWITCH_HIGH, HIGH);
  digitalWrite(SWITCH_GRD, LOW);
  digitalWrite(ENA, HIGH);
  digitalWrite(CSA, LOW);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  
  //pinMode(SWITCH_SIG, INPUT);
  //digitalWrite(SWITCH_SIG, LOW);
  
  attachInterrupt(SWITCH_SIG, reverseMotor, CHANGE);
  Serial.println("Start!");
  digitalWrite(IN1, dir1);
  digitalWrite(IN2, dir2);
}

void loop()
{
  Serial.print('.');
  if(count > 30)
  {
    Serial.print("\n");
    count = 0;
  }
  digitalWrite(IN1, dir1);
  digitalWrite(IN2, dir2);
  count++;
  delay(100);
}

void reverseMotor()
{
  noInterrupts();
  Serial.print("REVERSE!\n");
  dir1 = 1 - dir1;
  dir2 = 1 - dir2;
  delay(10);
  interrupts();
}
