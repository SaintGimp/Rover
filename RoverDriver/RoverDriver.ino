#include <Adafruit_SoftServo.h>
#include <NewPing.h>
#include <SoftwareSerialRxOnly.h>

// Servo control lines (orange)
#define SERVO_LEFT_PIN 2   
#define SERVO_RIGHT_PIN 0

// IR Receiver
#define RECEIVER_PIN 4

// Values that completely stop the servos
#define SERVO_LEFT_STOP 90
#define SERVO_RIGHT_STOP 85

// Values that move the rover forward in a straight line
#define SERVO_LEFT_FORWARD 180
#define SERVO_RIGHT_FORWARD 30
#define SERVO_LEFT_BACKWARD 0
#define SERVO_RIGHT_BACKWARD 180


// Ultrasonic sensor pins
#define SENSOR_TRIGGER_PIN 3
#define SENSOR_ECHO_PIN 1
#define MAX_DISTANCE 300 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

Adafruit_SoftServo servo_left, servo_right;
NewPing sonar(SENSOR_TRIGGER_PIN, SENSOR_ECHO_PIN, MAX_DISTANCE);
SoftwareSerialRxOnly receiver(RECEIVER_PIN);

int obstacle_distance;
bool automaticMode;

void setup() 
{
  // Set up the interrupt that will refresh the servo for us automagically
  OCR0A = 0xAF;            // any number is OK
  TIMSK |= _BV(OCIE0A);    // Turn on the compare interrupt (below!)
  
  // Attach servos and default to stopped mode
  servo_left.attach(SERVO_LEFT_PIN);
  servo_left.write(SERVO_LEFT_STOP);
  servo_right.attach(SERVO_RIGHT_PIN);
  servo_right.write(SERVO_RIGHT_STOP);
  
  // Set up IR receiver
  receiver.begin(1200);

  // Start in manual mode
  automaticMode = false;
} 

void loop() 
{
  receiveRemoteCommands();
  
  if (automaticMode)
  { 
    obstacle_distance = sonar.ping_cm();
  
    if (obstacle_distance < 25 && obstacle_distance > 0) {
      rotate();
    }
    else if (obstacle_distance > 50) {
      moveForward();
    }
    
    delay(100);
  }
}

void receiveRemoteCommands()
{
  if (receiver.available())
  {
    char code = receiver.read();
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
}

void stop()
{
  servo_left.write(SERVO_LEFT_STOP);
  servo_right.write(SERVO_RIGHT_STOP);
}

void rotate()
{
  servo_left.write(SERVO_LEFT_FORWARD);
  servo_right.write(SERVO_RIGHT_BACKWARD);
}

// We'll take advantage of the built in millis() timer that goes off
// to keep track of time, and refresh the servo every 20 milliseconds
volatile uint8_t counter = 0;
SIGNAL(TIMER0_COMPA_vect) {
  // this gets called every 2 milliseconds
  counter += 2;
  // every 20 milliseconds, refresh the servos!
  if (counter >= 20) {
    counter = 0;
    servo_left.refresh();
    servo_right.refresh();
  }
}
