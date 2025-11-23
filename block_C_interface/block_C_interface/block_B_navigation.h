#include <Arduino.h>
#ifndef BLOCK_B_NAVIGATION_H
#define BLOCK_B_NAVIGATION_H

#include <ESP32Servo.h>

#define TRIG_PIN           14
#define ECHO_PIN           12

#define SCAN_SERVO_PIN      5
#define GRIPPER_SERVO_PIN  15

void initBlockB();
long getDistanceCm();

int lookLeft();
int lookRight();

void smoothOpen();
void smoothClose();

void pickObject();
void dropObject();

bool obstacleAhead();
String getObstacleDecision();

#endif