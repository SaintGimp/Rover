//#include "SoftwareSerial.h"

#define LED_PIN 9
//SoftwareSerial mySerial(1, 0);

void setup() {
  //mySerial.begin(1200);
  Serial.begin(1200);  
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
//  digitalWrite(LED_PIN, HIGH);
//  delay(250);
//  digitalWrite(LED_PIN, LOW);
//  delay(250);
  
  if (Serial.available())
  {
    char code = Serial.read();
    if (code == 'J')
    {
      digitalWrite(LED_PIN, HIGH);
    }
    if (code == 'E')
    {
      digitalWrite(LED_PIN, LOW);
    }
  }
}
