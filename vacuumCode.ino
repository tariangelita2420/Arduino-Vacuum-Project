/*
Partner Project by Taria Reese and Darius Wilson
This cleaner is powered by two Arduino Unos for control and music.
The rover is equipped with sensors for navigation & obstacle avoidance, as well as making musical tones while cleaning :D
The communication between Arduinos ensures that the cleaning and musical tasks are in sync with synchronous serial communication.
This project is designed to make the user experience fun while they are cleaning with an alien theme and alien music!
*/
#include <AFMotor.h>  
#include <NewPing.h>
#include <Servo.h> 

#define TRIGGER_PIN A0 // Define pin for the ultrasonic sensor trigger
#define ECHO_PIN A1 // Define pin for the ultrasonic sensor echo
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters)
#define MOTOR_SPEED 190 // Maximum speed of DC motors
#define SPEED_OFFSET 20 // Speed offset for adjustments

// Initialize the ultrasonic sensor with the trigger and echo pins
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); 

// Initialize the motors with their respective pins
AF_DCMotor motorLeftFront(1, MOTOR12_1KHZ); 
AF_DCMotor motorRightFront(2, MOTOR12_1KHZ);
AF_DCMotor motorLeftRear(3, MOTOR34_1KHZ);
AF_DCMotor motorRightRear(4, MOTOR34_1KHZ);
Servo myservo; // Initialize servo object

boolean isMovingForward = false;
int measuredDistance = 100; // Stores the measured distance from the ultrasonic sensor
int currentSpeed = 0; // Variable to ramp up the speed

void setup() {
  myservo.attach(10); // Attach servo to pin 10
  myservo.write(115); // Center the servo
  delay(2000);
  for(int i = 0; i < 4; i++) { // Take multiple measurements for accuracy
    measuredDistance = readPing();
    delay(100);
  }
  // Initialize serial communication for debugging and communication with other arduino
  Serial.begin(9600);
  startTime = millis();
}

void loop() {
  int distanceRight = 0;
  int distanceLeft =  0;
  delay(40);
  
  // If an object is detected within 15 cm, execute avoidance maneuvers
  if(measuredDistance <= 15) {
    haltMovement(); // Stop the car
    delay(100);
    moveBackwards(); // Move backward for a short duration
    delay(300);
    haltMovement(); // Stop the car again
    delay(200);
    distanceRight = checkRight(); // Check distance to the right
    delay(200);
    distanceLeft = checkLeft(); // Check distance to the left
    delay(200);

    // Turn towards the direction with more space
    if(distanceRight >= distanceLeft) {
      rotateRight();
      haltMovement();
    } else {
      rotateLeft();
      haltMovement();
    }
  } else {
    moveForwards();
  }
  
  measuredDistance = readPing(); // Update the measured distance

  //for debugging
  if (millis() - startTime < 10000) {
    Serial.write('M');
  }
}

// Function definitions below

// Read the distance from the ultrasonic sensor
int readPing() { 
  delay(70);
  int cm = sonar.ping_cm();
  if(cm == 0) {
    cm = MAX_DISTANCE; // If no object detected, set distance to MAX_DISTANCE
  }
  return cm;
}

// Function to stop all motors
void haltMovement() {
  motorLeftFront.run(RELEASE); 
  motorRightFront.run(RELEASE);
  motorLeftRear.run(RELEASE);
  motorRightRear.run(RELEASE);
} 
  
// Function to move the car forward and gradually increase speed
void moveForwards() {
  if(!isMovingForward) {
    isMovingForward = true;
    setMotorsToMove(FORWARD);
    graduallyIncreaseSpeed();
  }
}

// Function to move the car backward and gradually increase speed
void moveBackwards() {
  isMovingForward = false;
  setMotorsToMove(BACKWARD);
  graduallyIncreaseSpeed();
}  

// Function to turn the car right
void rotateRight() {
  motorLeftFront.run(FORWARD);
  motorRightFront.run(FORWARD);
  motorLeftRear.run(BACKWARD);
  motorRightRear.run(BACKWARD);     
  delay(500);
  setMotorsToMove(FORWARD);      
} 
 
// Function to turn the car left
void rotateLeft() {
  motorLeftFront.run(BACKWARD);     
  motorRightFront.run(BACKWARD);  
  motorLeftRear.run(FORWARD);
  motorRightRear.run(FORWARD);   
  delay(500);
  setMotorsToMove(FORWARD);
}

// Function to set all motors to move in the same direction
void setMotorsToMove(int direction) {
  motorLeftFront.run(direction);     
  motorRightFront.run(direction);
  motorLeftRear.run(direction);
  motorRightRear.run(direction);
}

// Function to gradually increase the speed of the motors
void graduallyIncreaseSpeed() {
  for (currentSpeed = 0; currentSpeed < MOTOR_SPEED; currentSpeed += 2) {
    motorLeftFront.setSpeed(currentSpeed);
    motorRightFront.setSpeed(currentSpeed);
    motorLeftRear.setSpeed(currentSpeed);
    motorRightRear.setSpeed(currentSpeed);
    delay(5);
  }
}

// Function to check distance on the right
int checkRight() {
  myservo.write(50); // Turn servo to the right
  delay(500);
  int distance = readPing();
  myservo.write(115); // Reset servo position
  return distance;
}

// Function to check distance on the left
int checkLeft() {
  myservo.write(170); // Turn servo to the left
  delay(500);
  int distance = readPing();
  myservo.write(115); // Reset servo position
  return distance;
}
