#include <Arduino.h>
#include "block_A_movement.h" 
#include "driver/ledc.h" 

// 1. INITIALIZATION 


void initMovement() {
    // 1. Set Pin Modes: Direction and Standby pins are Outputs
    // Motor A (Right) Direction
    pinMode(MTRA_IN1, OUTPUT);
    pinMode(MTRA_IN2, OUTPUT);
    // Motor B (Left Direction
    pinMode(MTRB_IN1, OUTPUT);
    pinMode(MTRB_IN2, OUTPUT);
    pinMode(MTRB_EN, OUTPUT);
    pinMode(MTRA_EN, OUTPUT);
    
   

    // Set 4 sensor pins as inputs
    pinMode(LINE_OUTER_LEFT, INPUT_PULLDOWN);
    pinMode(LINE_INNER_LEFT, INPUT_PULLDOWN);
    pinMode(LINE_INNER_RIGHT, INPUT_PULLDOWN);
    pinMode(LINE_OUTER_RIGHT, INPUT_PULLDOWN); 

    
    stopMoving();

}


// 2. MOVEMENT FUNCTIONS

/*void moveForward() {
  digitalWrite(MTRA_IN1, LOW);
  digitalWrite(MTRA_IN2, HIGH);

  digitalWrite(MTRB_IN1, LOW);
  digitalWrite(MTRB_IN2, HIGH);

  ledcWrite(MTRA_EN,155);  // speed
  ledcWrite(MTRB_EN, 155+50);
}
void moveBackward() {
  digitalWrite(MTRA_IN1, HIGH);
  digitalWrite(MTRA_IN2,LOW);

  digitalWrite(MTRB_IN1, HIGH);
  digitalWrite(MTRB_IN2,LOW);

  ledcWrite(MTRA_EN, 155);
  ledcWrite(MTRB_EN, 155 + 62);

}

void turnLeft() {
  digitalWrite(MTRA_IN1, LOW);
  digitalWrite(MTRA_IN2, HIGH);

  digitalWrite(MTRB_IN1, HIGH);
  digitalWrite(MTRB_IN2, LOW);

  //ledcWrite(ENA, 200);
  //ledcWrite(ENB, 0);
}

void turnRight() {
  digitalWrite(MTRA_IN1, HIGH);
  digitalWrite(MTRA_IN2, LOW);

  digitalWrite(MTRB_IN1, LOW);
  digitalWrite(MTRB_IN2, HIGH);

 // ledcWrite(ENA, 0);
 // ledcWrite(ENB, 200);
}

void stopMoving() {
  digitalWrite(MTRA_IN1, LOW);
  digitalWrite(MTRA_IN2, LOW);

  digitalWrite(MTRB_IN1, LOW);
  digitalWrite(MTRB_IN2, LOW);

  ledcWrite(MTRA_EN, 0);
  ledcWrite(MTRB_EN,0);
}*/
void moveForward() {
  digitalWrite(MTRA_IN1, HIGH);
  digitalWrite(MTRA_IN2, LOW);

  digitalWrite(MTRB_IN1, LOW);
  digitalWrite(MTRB_IN2, HIGH);

  // speed + correction
  analogWrite(MTRA_EN, 155);       // right motor
  analogWrite(MTRB_EN, 155 + 50);  // left motor a bit faster
}

void moveBackward() {
  digitalWrite(MTRA_IN1, LOW);
  digitalWrite(MTRA_IN2, HIGH);

  digitalWrite(MTRB_IN1, HIGH);
  digitalWrite(MTRB_IN2, LOW);

  analogWrite(MTRA_EN, 155);
  analogWrite(MTRB_EN, 155 + 50);
}

void turnLeft() {
  digitalWrite(MTRA_IN1,HIGH);
  digitalWrite(MTRA_IN2, LOW);

  digitalWrite(MTRB_IN1, HIGH);
  digitalWrite(MTRB_IN2, LOW);

  analogWrite(MTRA_EN, 155);
  analogWrite(MTRB_EN, 155 + 50);
}

void turnRight() {
  digitalWrite(MTRA_IN1,LOW);
  digitalWrite(MTRA_IN2, HIGH);

  digitalWrite(MTRB_IN1, LOW);
  digitalWrite(MTRB_IN2, HIGH);

  analogWrite(MTRA_EN, 155);
  analogWrite(MTRB_EN, 155 + 50);
}

void stopMoving() {
  digitalWrite(MTRA_IN1, LOW);
  digitalWrite(MTRA_IN2, LOW);
  digitalWrite(MTRB_IN1, LOW);
  digitalWrite(MTRB_IN2, LOW);

  analogWrite(MTRA_EN, 0);
  analogWrite(MTRB_EN, 0);
}


// 3. SENSING FUNCTION (4-Bit Sensor Interface)


uint8_t readLineSensors() {
    int ol  = digitalRead(LINE_OUTER_LEFT);
    int il  = digitalRead(LINE_INNER_LEFT);
    int ir = digitalRead(LINE_INNER_RIGHT);
    int or_val = digitalRead(LINE_OUTER_RIGHT);

    // Combine into a 4-bit pattern: OL IL IR OR
    return (ol << 3) | (il << 2) | (ir << 1) | or_val;
}


// 4. AUTONOMOUS LINE FOLLOWING  


void runLineFollow() {

    analogWrite(MTRA_EN, 150); // Write The Duty Cycle 0 to 255 Enable Pin A for Motor1 Speed 
    analogWrite(MTRB_EN, 150); // Write The Duty Cycle 0 to 255 Enable Pin B for Motor2 Speed 
    delay(1500);  
        if((digitalRead(LINE_INNER_RIGHT) == 0)&&(digitalRead(LINE_INNER_LEFT) == 0)){moveForward();}//{forword();}   //if Right Sensor and Left Sensor are at White color then it will call forword function

        if((digitalRead(LINE_INNER_RIGHT) == 1)&&(digitalRead(LINE_INNER_LEFT) == 0)){turnRight();
            delay(250);}//{turnRight();} //if Right Sensor is Black and Left Sensor is White then it will call turn Right function
            if((digitalRead(LINE_INNER_RIGHT) == 0)&&(digitalRead(LINE_INNER_LEFT) == 1)){turnLeft();
            delay(250);}  //if Right Sensor is White and Left Sensor is Black then it will call turn Left function

        if((digitalRead(LINE_INNER_RIGHT) == 1)&&(digitalRead(LINE_INNER_LEFT) == 1)){stopMoving();} //if Right Sensor and Left Sensor are at Black color then it will call Stop function
}