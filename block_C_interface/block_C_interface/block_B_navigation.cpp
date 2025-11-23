#include "block_B_navigation.h"
#include <Arduino.h>

Servo scanServo;
Servo gripperServo;

void initBlockB() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  scanServo.attach(SCAN_SERVO_PIN);
  scanServo.write(90);

  gripperServo.attach(GRIPPER_SERVO_PIN);
  gripperServo.write(90);
}

long getDistanceCm() {
  long duration;

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH, 30000);

  if (duration <= 0) return 300;

  long distance = duration * 0.0343 / 2;
  if (distance > 300) distance = 300;

  return distance;
}

int lookLeft() {
  scanServo.write(150);
  delay(450);
  int d = getDistanceCm();
  delay(50);
  scanServo.write(90);
  return d;
}

int lookRight() {
  scanServo.write(30);
  delay(450);
  int d = getDistanceCm();
  delay(50);
  scanServo.write(90);
  return d;
}

void smoothOpen() {
  for (int a = 90; a >= 0; a -= 3) {
    gripperServo.write(a);
    delay(15);
  }
}

void smoothClose() {
  for (int a = 0; a <= 90; a += 3) {
    gripperServo.write(a);
    delay(15);
  }
}

void pickObject() {
  smoothOpen();
  delay(200);
  smoothClose();
}

void dropObject() {
  smoothOpen();
}

bool obstacleAhead() {
  long d = getDistanceCm();
  return (d > 0 && d < 20);
}

String getObstacleDecision() {
  long front = getDistanceCm();

  if (front < 20) {

    int leftD  = lookLeft();
    int rightD = lookRight();

    if (leftD > rightD)
      return "TURN_LEFT";
    else
      return "TURN_RIGHT";
  }

  return "CLEAR";
}