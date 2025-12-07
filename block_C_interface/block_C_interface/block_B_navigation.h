#// block_B_navigation.h
#ifndef BLOCK_B_NAVIGATION_H
#define BLOCK_B_NAVIGATION_H

#include <Arduino.h>


// Ball ultrasonic (near gripper)
#define TRIG_PIN        13
#define ECHO_PIN        12

// Front ultrasonic (obstacle)
#define FRONT_TRIG_PIN  27
#define FRONT_ECHO_PIN  26

// ========= SERVO PIN =========
#define GRIPPER_SERVO_PIN 5

// ========= INITIALISATION =========
void initBlockB();   

// ========= ULTRASONIC API =========
long getDistanceCm();
long getFrontDistanceCm();

// ========= GRIPPER API =========
void smoothOpen();
void smoothClose();

// ========= MOTOR CONTROL HOOKS =========
//void stopMotors();
//void moveMotorsForward();
//void stopMoving();
//void moveForward();
void OBSTACLE_CHECK();
void dropObject();
void pickObject();

#endif
