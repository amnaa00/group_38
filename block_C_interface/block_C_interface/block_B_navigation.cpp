#include <Arduino.h>
#include <ESP32Servo.h>
#include "block_A_movement.h"

// ========= ULTRASONIC PINS =========
// Ball ultrasonic
#define TRIG_PIN        13
#define ECHO_PIN        12

// Front ultrasonic (obstacle)
#define FRONT_TRIG_PIN  27
#define FRONT_ECHO_PIN  26

// ========= SERVO PIN =========
#define GRIPPER_SERVO_PIN 5

Servo gripper;

void initBlockB() {
  // Ball ultrasonic
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Front ultrasonic
  pinMode(FRONT_TRIG_PIN, OUTPUT);
  pinMode(FRONT_ECHO_PIN, INPUT);

  // Gripper servo
  gripper.attach(GRIPPER_SERVO_PIN);
  gripper.write(90);    // neutral / closed
}

// ========= MOTOR PLACEHOLDER =========
//void stopMoving();

//void moveForward();



// ========= GRIPPER FUNCTIONS =========
void smoothOpen() {
  for (int angle = 90; angle >= 0; angle -= 2) {
    gripper.write(angle);
    delay(20);
  }
}

void smoothClose() {
  for (int angle = 0; angle <= 90; angle += 2) {
    gripper.write(angle);
    delay(20);
  }
}

long readUltrasonicCm(uint8_t trigPin, uint8_t echoPin) {
  digitalWrite(trigPin, LOW);
  delay(1);     

  digitalWrite(trigPin, HIGH);
  delay(1);     
  digitalWrite(trigPin, LOW);

 long duration = pulseIn(echoPin, HIGH, 30000);

  if (duration <= 0) return 300;

  long distance = duration * 0.034 / 2;
  if (distance > 300) distance = 300;

  return distance;
}

long getDistanceCm() {
  return readUltrasonicCm(TRIG_PIN, ECHO_PIN);
}

long getFrontDistanceCm() {
  return readUltrasonicCm(FRONT_TRIG_PIN, FRONT_ECHO_PIN);
}

void pickObject() {
  smoothOpen();
  d
  smoothClose();
}

void dropObject() {
  smoothOpen();
}
// ========= SETUP =========


// ========= LOOP =========
//void loop() {
  //static bool grabbed = false;

/*void OBSTACLE_CHECK(){
  long frontDist = getFrontDistanceCm();

  while (frontDist < 20) {
    stopMoving();
    delay(200);
    return;  // do not continue
  }

/*void OBSTACLE_CHECK() {
  // If no obstacle, just return
  if (getFrontDistanceCm() >= 20) return;

  // Obstacle present: stop and wait until it is gone
  stopMoving();

  while (getFrontDistanceCm() < 20) {
    stopMoving();

    // optionally allow manual escape:
          // so you can switch to manual if needed
    delay(50);
  }
}*/

  // When this function returns, the caller (followUntilIntersection)
  // will continue with runLineFollow() on the next loop iteration



  //moveMotorsForward();  // 

  // 2) BALL PICKUP CHECK
  //////long d = getBallDistanceCm();

  //if (!grabbed && d >= 2 && d <= 5) {
   ////// smoothOpen();
    //delay(200);
   // smoothClose();
   // grabbed = true;
  

 // delay(200);

