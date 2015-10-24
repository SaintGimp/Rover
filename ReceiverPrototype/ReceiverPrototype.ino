#include "SoftwareSerial.h"

SoftwareSerial mySerial(1, 0);

void setup() {
  mySerial.begin(1200);
  pinMode(2, OUTPUT);
}

void loop() {
  if (mySerial.available())
  {
    char code = mySerial.read();
    if (code == 'J')
    {
      digitalWrite(2, HIGH);
    }
    if (code == 'E')
    {
      digitalWrite(2, LOW);
    }
  }
}
