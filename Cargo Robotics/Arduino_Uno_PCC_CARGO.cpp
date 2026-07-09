#include <Servo.h>

/*
  Autonomous Cargo Robot
  Hardware assumed:
  - Arduino Uno/Nano
  - HC-SR04 ultrasonic sensor
  - L298N motor driver
  - 2 DC motors
  - Servo motor for cargo hold/release
*/

// =======================
// Pin setup
// =======================

// Ultrasonic sensor pins
const int TRIG_PIN = 12;
const int ECHO_PIN = 11;

// Servo pin
const int SERVO_PIN = 9;

// L298N motor driver pins
const int LEFT_EN  = 5;   // PWM pin
const int LEFT_IN1 = 2;
const int LEFT_IN2 = 4;

const int RIGHT_EN  = 6;  // PWM pin
const int RIGHT_IN1 = 7;
const int RIGHT_IN2 = 8;

// =======================
// Robot tuning values
// =======================

// Motor speeds: 0 to 255
const int BASE_SPEED = 170;
const int TURN_SPEED = 160;
const int REVERSE_SPEED = 150;

// Obstacle detection distance
const int OBSTACLE_DISTANCE_CM = 20;
const int CLEAR_DISTANCE_CM = 30;

// Maximum useful ultrasonic reading
const int MAX_DISTANCE_CM = 250;

// Adjust this after testing.
// This is how long the robot drives forward before reaching Point B.
const unsigned long TRAVEL_TIME_TO_B_MS = 15000;

// Servo angles
const int CARGO_HOLD_ANGLE = 30;       // Servo angle to hold cargo
const int CARGO_RELEASE_ANGLE = 110;   // Servo angle to release cargo

// Time for servo movement
const unsigned long SERVO_ACTION_TIME_MS = 1000;

// =======================
// Global variables
// =======================

Servo cargoServo;

enum RobotState {
  PICKUP_AT_A,
  DRIVE_TO_B,
  DROP_AT_B,
  FINISHED
};

RobotState currentState = PICKUP_AT_A;

unsigned long stateStartTime = 0;
unsigned long forwardDriveTime = 0;
unsigned long lastDriveTick = 0;

// =======================
// Setup
// =======================

void setup() {
  Serial.begin(9600);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(LEFT_EN, OUTPUT);
  pinMode(LEFT_IN1, OUTPUT);
  pinMode(LEFT_IN2, OUTPUT);

  pinMode(RIGHT_EN, OUTPUT);
  pinMode(RIGHT_IN1, OUTPUT);
  pinMode(RIGHT_IN2, OUTPUT);

  cargoServo.attach(SERVO_PIN);

  stopMotors();

  // Start with cargo released, then grab it at Point A
  cargoServo.write(CARGO_RELEASE_ANGLE);
  delay(500);

  stateStartTime = millis();

  Serial.println("Robot starting...");
}

// =======================
// Main loop
// =======================

void loop() {
  switch (currentState) {

    case PICKUP_AT_A:
      pickupCargo();
      break;

    case DRIVE_TO_B:
      driveToPointB();
      break;

    case DROP_AT_B:
      dropCargo();
      break;

    case FINISHED:
      stopMotors();
      break;
  }
}

// =======================
// State functions
// =======================

void pickupCargo() {
  stopMotors();

  cargoServo.write(CARGO_HOLD_ANGLE);

  if (millis() - stateStartTime >= SERVO_ACTION_TIME_MS) {
    Serial.println("Cargo secured. Driving to Point B.");

    currentState = DRIVE_TO_B;
    forwardDriveTime = 0;
    lastDriveTick = millis();
  }
}

void driveToPointB() {
  unsigned long now = millis();

  int distance = readDistanceMedianCM();

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  if (distance > 0 && distance <= OBSTACLE_DISTANCE_CM) {
    Serial.println("Obstacle detected. Avoiding...");
    stopMotors();
    avoidObstacle();

    // Reset timing after obstacle routine so blocked time is not counted
    lastDriveTick = millis();
  } 
  else {
    moveForward(BASE_SPEED);

    unsigned long newTime = millis();
    forwardDriveTime += newTime - lastDriveTick;
    lastDriveTick = newTime;
  }

  if (forwardDriveTime >= TRAVEL_TIME_TO_B_MS) {
    Serial.println("Point B reached. Dropping cargo.");

    stopMotors();
    currentState = DROP_AT_B;
    stateStartTime = millis();
  }
}

void dropCargo() {
  stopMotors();

  cargoServo.write(CARGO_RELEASE_ANGLE);

  if (millis() - stateStartTime >= SERVO_ACTION_TIME_MS) {
    Serial.println("Cargo delivered. Robot stopped.");
    currentState = FINISHED;
  }
}

// =======================
// Obstacle avoidance logic
// =======================

void avoidObstacle() {
  stopMotors();
  delay(150);

  // Back away from obstacle
  moveBackward(REVERSE_SPEED);
  delay(450);

  stopMotors();
  delay(150);

  // Try turning right
  turnRight(TURN_SPEED);
  delay(450);

  stopMotors();
  delay(150);

  int rightCheck = readDistanceMedianCM();

  if (rightCheck >= CLEAR_DISTANCE_CM) {
    Serial.println("Path clear after turning right.");
    return;
  }

  // If right side is blocked, turn left from current position
  turnLeft(TURN_SPEED);
  delay(900);

  stopMotors();
  delay(150);

  int leftCheck = readDistanceMedianCM();

  if (leftCheck >= CLEAR_DISTANCE_CM) {
    Serial.println("Path clear after turning left.");
    return;
  }

  // If both directions are blocked, reverse and rotate more
  Serial.println("Both sides blocked. Reversing and rotating.");

  moveBackward(REVERSE_SPEED);
  delay(500);

  turnRight(TURN_SPEED);
  delay(900);

  stopMotors();
  delay(150);
}

// =======================
// Ultrasonic sensor functions
// =======================

int readDistanceCM() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Timeout prevents the robot from freezing if no echo is received
  long duration = pulseIn(ECHO_PIN, HIGH, 30000);

  if (duration == 0) {
    return MAX_DISTANCE_CM + 1;
  }

  int distance = duration / 58;

  if (distance > MAX_DISTANCE_CM) {
    distance = MAX_DISTANCE_CM + 1;
  }

  return distance;
}

int readDistanceMedianCM() {
  const int samples = 5;
  int readings[samples];

  for (int i = 0; i < samples; i++) {
    readings[i] = readDistanceCM();
    delay(10);
  }

  // Sort readings using a simple bubble sort
  for (int i = 0; i < samples - 1; i++) {
    for (int j = 0; j < samples - i - 1; j++) {
      if (readings[j] > readings[j + 1]) {
        int temp = readings[j];
        readings[j] = readings[j + 1];
        readings[j + 1] = temp;
      }
    }
  }

  // Return median value
  return readings[samples / 2];
}

// =======================
// Motor control functions
// =======================

void setMotorSpeed(int leftSpeed, int rightSpeed) {
  leftSpeed = constrain(leftSpeed, -255, 255);
  rightSpeed = constrain(rightSpeed, -255, 255);

  // Left motor direction
  if (leftSpeed > 0) {
    digitalWrite(LEFT_IN1, HIGH);
    digitalWrite(LEFT_IN2, LOW);
  } 
  else if (leftSpeed < 0) {
    digitalWrite(LEFT_IN1, LOW);
    digitalWrite(LEFT_IN2, HIGH);
  } 
  else {
    digitalWrite(LEFT_IN1, LOW);
    digitalWrite(LEFT_IN2, LOW);
  }

  // Right motor direction
  if (rightSpeed > 0) {
    digitalWrite(RIGHT_IN1, HIGH);
    digitalWrite(RIGHT_IN2, LOW);
  } 
  else if (rightSpeed < 0) {
    digitalWrite(RIGHT_IN1, LOW);
    digitalWrite(RIGHT_IN2, HIGH);
  } 
  else {
    digitalWrite(RIGHT_IN1, LOW);
    digitalWrite(RIGHT_IN2, LOW);
  }

  analogWrite(LEFT_EN, abs(leftSpeed));
  analogWrite(RIGHT_EN, abs(rightSpeed));
}

void moveForward(int speedValue) {
  setMotorSpeed(speedValue, speedValue);
}

void moveBackward(int speedValue) {
  setMotorSpeed(-speedValue, -speedValue);
}

void turnLeft(int speedValue) {
  setMotorSpeed(-speedValue, speedValue);
}

void turnRight(int speedValue) {
  setMotorSpeed(speedValue, -speedValue);
}

void stopMotors() {
  setMotorSpeed(0, 0);
}