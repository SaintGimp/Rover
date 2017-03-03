#include <Servo.h>

// Servo control lines (orange)
#define SERVO_LEFT_PIN 8   
#define SERVO_RIGHT_PIN 10

// Values that completely stop the servos
#define SERVO_LEFT_STOP 85
#define SERVO_RIGHT_STOP 90

// Values that move the rover forward in a straight line
#define SERVO_LEFT_FORWARD 180
#define SERVO_RIGHT_FORWARD 0
#define SERVO_LEFT_BACKWARD 0
#define SERVO_RIGHT_BACKWARD 180

#define DEBUG_LED 9

Servo servo_left, servo_right;

long lastLeftButtonTime, lastRightButtonTime;

void setup() 
{
  // Attach servos and default to stopped mode
  servo_left.attach(SERVO_LEFT_PIN);
  servo_left.write(SERVO_LEFT_STOP);
  servo_right.attach(SERVO_RIGHT_PIN);
  servo_right.write(SERVO_RIGHT_STOP);
  
  // Set up IR receiver
  Serial.begin(1200);

  pinMode(DEBUG_LED, OUTPUT);
} 

void loop() 
{
  if (Serial.available())
  {
    char code = Serial.read();
    if (code == 'J')
    {
      servo_left.write(SERVO_LEFT_FORWARD);
      lastLeftButtonTime = millis();
    }
    if (code == 'E')
    {
  servo_right.write(SERVO_RIGHT_FORWARD);
      lastRightButtonTime = millis();
    }
  }

  if (millis() - lastLeftButtonTime > 100)
  {
    servo_left.write(SERVO_LEFT_STOP);
  }
  if (millis() - lastRightButtonTime > 100)
  {
    servo_right.write(SERVO_RIGHT_STOP);
  }

//  if (Serial.available())
//  {
//    char code = Serial.read();
//    Serial.flush();
//    if (code == 'J')
//    {
//      moveForward();
//    }
//    if (code == 'E')
//    {
//      stop();
//    }
//  }
}

void moveForward()
{
  servo_left.write(SERVO_LEFT_FORWARD);
  servo_right.write(SERVO_RIGHT_FORWARD);
}

void stop()
{
  servo_left.write(SERVO_LEFT_STOP);
  servo_right.write(SERVO_RIGHT_STOP);
}

void rotateRight()
{
  servo_left.write(SERVO_LEFT_FORWARD);
  servo_right.write(SERVO_RIGHT_BACKWARD);
}

void rotateLeft()
{
  servo_left.write(SERVO_LEFT_BACKWARD);
  servo_right.write(SERVO_RIGHT_FORWARD);
}

