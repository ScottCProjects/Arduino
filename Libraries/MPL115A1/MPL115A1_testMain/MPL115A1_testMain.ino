// Scott Camarena
//
// Run MPL115A1 Test Code
// ===================================================
#include <SPI.h>
#include <MPL115A1.h>

void setup() {

}

void loop() {
  Serial.println( MPL115A1_testMain() );
  Serial.println("=================================");
  delay(2000);
}
