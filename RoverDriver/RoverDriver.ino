#include <Servo.h>
#include <NewPing.h>

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

// Ultrasonic sensor pins
#define SENSOR0_TRIGGER_PIN 3
#define SENSOR0_ECHO_PIN 4
#define SENSOR1_TRIGGER_PIN 5
#define SENSOR1_ECHO_PIN 6
#define MAX_DISTANCE 300 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

#define DEBUG_LED 9

Servo servo_left, servo_right;
NewPing sonar0(SENSOR0_TRIGGER_PIN, SENSOR0_ECHO_PIN, MAX_DISTANCE);
NewPing sonar1(SENSOR1_TRIGGER_PIN, SENSOR1_ECHO_PIN, MAX_DISTANCE);

int left_obstacle_distance;
int right_obstacle_distance;
bool automaticMode;
bool isRotating;

void setup() 
{
  // Attach servos and default to stopped mode
  servo_left.attach(SERVO_LEFT_PIN);
  servo_left.write(SERVO_LEFT_STOP);
  servo_right.attach(SERVO_RIGHT_PIN);
  servo_right.write(SERVO_RIGHT_STOP);
  
  // Set up IR receiver
  Serial.begin(1200);

  // Start in manual mode
  automaticMode = true;

  pinMode(DEBUG_LED, OUTPUT);
} 

void loop() 
{
  digitalWrite(DEBUG_LED, isRotating);
  
  receiveRemoteCommands();
  
  if (automaticMode)
  { 
    left_obstacle_distance = sonar0.ping_cm();
    delay(10);
    right_obstacle_distance = sonar1.ping_cm();
  
    if (!isRotating) {
      if (isClose(right_obstacle_distance) && !isClose(left_obstacle_distance)) {
        rotateLeft();
      }
      else if (isClose(left_obstacle_distance) && !isClose(right_obstacle_distance)) {
        rotateRight();
      }
    }
    else if (isFar(left_obstacle_distance) && isFar(right_obstacle_distance)) {
      moveForward();
    }
    
    delay(100);
  }
  else {
    stop();
  }
}

bool isClose(int distance) {
  return (distance < 25 && distance > 0);
}

bool isFar(int distance) {
  return (distance > 30);
}

void receiveRemoteCommands()
{
  if (Serial.available())
  {
    char code = Serial.read();
    Serial.flush();
    if (code == 'J')
    {
      automaticMode = true;
    }
    if (code == 'E')
    {
      automaticMode = false;
    }
  }
}

void moveForward()
{
  servo_left.write(SERVO_LEFT_FORWARD);
  servo_right.write(SERVO_RIGHT_FORWARD);
  isRotating = false;
}

void stop()
{
  servo_left.write(SERVO_LEFT_STOP);
  servo_right.write(SERVO_RIGHT_STOP);
  isRotating = false;
}

void rotateRight()
{
  servo_left.write(SERVO_LEFT_FORWARD);
  servo_right.write(SERVO_RIGHT_BACKWARD);
  isRotating = true;
}

void rotateLeft()
{
  servo_left.write(SERVO_LEFT_BACKWARD);
  servo_right.write(SERVO_RIGHT_FORWARD);
  isRotating = true;
}

