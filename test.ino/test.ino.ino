#include "SoftwareSerialTxOnly.h"

SoftwareSerialTxOnly mySerial(0, true);

void setup() {
  mySerial.begin(1200);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
}

void loop() {
  if (digitalRead(3) == LOW)
  {
    mySerial.write('J');
  }
  if (digitalRead(4) == LOW)
  {
    mySerial.write('E');
  }
}
