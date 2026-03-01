// ARDUINO OBSTACLE AVOIDING CAR
#include <AFMotor.h>
#include <NewPing.h>
#include <Servo.h>

#define TRIG_PIN A0
#define ECHO_PIN A1
#define MAX_DISTANCE 200
#define MAX_SPEED 190
#define SERVO_MIRRORED false  // set to true if servo is mounted the other way around

NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);

AF_DCMotor motor1(1, MOTOR12_1KHZ);
AF_DCMotor motor2(2, MOTOR12_1KHZ);
AF_DCMotor motor3(3, MOTOR34_1KHZ);
AF_DCMotor motor4(4, MOTOR34_1KHZ);
Servo myservo;

boolean goesForward = false;
int distance = 100;
int speedSet = 0;

// Servo positions — flipped automatically if SERVO_MIRRORED is true
int servoCenter = 115;
int servoRight   = SERVO_MIRRORED ? 170 : 50;
int servoLeft    = SERVO_MIRRORED ? 50  : 170;

void setup() {
  myservo.attach(10);
  myservo.write(servoCenter);
  delay(2000);

  // Take a few readings to stabilize the sensor on startup
  for (int i = 0; i < 4; i++) {
    distance = readPing();
    delay(100);
  }
}

void loop() {
  delay(40);

  if (distance <= 15) {
    moveStop();
    delay(100);
    moveBackward();
    delay(300);
    moveStop();
    delay(200);

    int distanceR = lookRight();
    delay(200);
    int distanceL = lookLeft();
    delay(200);

    if (distanceR >= distanceL) {
      turnRight();
    } else {
      turnLeft();
    }
    moveStop();

  } else {
    moveForward();
  }

  distance = readPing();
}

int lookRight() {
  myservo.write(servoRight);
  delay(500);
  int d = readPing();
  delay(100);
  myservo.write(servoCenter);
  return d;
}

int lookLeft() {
  myservo.write(servoLeft);
  delay(500);
  int d = readPing();
  delay(100);                  // fixed: delay is now before return
  myservo.write(servoCenter);
  return d;
}

int readPing() {
  delay(70);
  int cm = sonar.ping_cm();
  if (cm == 0) {
    cm = 250;  // 0 means out of range, treat as clear path
  }
  return cm;
}

void moveStop() {
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
}

void moveForward() {
  if (!goesForward) {
    goesForward = true;
    motor1.run(FORWARD);
    motor2.run(FORWARD);
    motor3.run(FORWARD);
    motor4.run(FORWARD);
    for (speedSet = 0; speedSet < MAX_SPEED; speedSet += 2) {
      motor1.setSpeed(speedSet);
      motor2.setSpeed(speedSet);
      motor3.setSpeed(speedSet);
      motor4.setSpeed(speedSet);
      delay(5);
    }
  }
}

void moveBackward() {
  goesForward = false;
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
  for (speedSet = 0; speedSet < MAX_SPEED; speedSet += 2) {
    motor1.setSpeed(speedSet);
    motor2.setSpeed(speedSet);
    motor3.setSpeed(speedSet);
    motor4.setSpeed(speedSet);
    delay(5);
  }
}

void turnRight() {
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
  delay(500);
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
}

void turnLeft() {
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
  delay(500);
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
}