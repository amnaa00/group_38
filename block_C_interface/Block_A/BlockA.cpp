#include <Arduino.h>
#include "BlockA.h" 
#include "driver/ledc.h" 

// 1. INITIALIZATION 


void initMovement() {
    // 1. Set Pin Modes: Direction and Standby pins are Outputs
    // Motor A (Left) Direction
    pinMode(MTRA_IN1, OUTPUT);
    pinMode(MTRA_IN2, OUTPUT);
    // Motor B (Right) Direction
    pinMode(MTRB_IN1, OUTPUT);
    pinMode(MTRB_IN2, OUTPUT);
    // Standby Pin
    pinMode(MTR_STBY_PIN, OUTPUT);
    
    // Set 4 sensor pins as inputs
    pinMode(LINE_OUTER_LEFT, INPUT_PULLDOWN);
    pinMode(LINE_INNER_LEFT, INPUT_PULLDOWN);
    pinMode(LINE_INNER_RIGHT, INPUT_PULLDOWN);
    pinMode(LINE_OUTER_RIGHT, INPUT_PULLDOWN); 

    // 2. Setup ESP32 PWM (LEDC) Channels
    //Motor A
    ledcAttach(MTRA_EN, PWM_FREQ, PWM_RESOLUTION);
    
    // Motor B (Right)
    ledcAttach(MTRB_EN, PWM_FREQ, PWM_RESOLUTION);

 
    
    // 3. Enable TB6612FNG Driver (Set STBY HIGH)
    digitalWrite(MTR_STBY_PIN, HIGH);
    
    stopMoving();
    
}


// 2. MOVEMENT FUNCTIONS


void stopMoving() {
    // Set both motor speeds to 0
    ledcWrite(MTRA_EN, 0);
    ledcWrite(MTRB_EN, 0);
    
    digitalWrite(MTRA_IN1, LOW);
    digitalWrite(MTRA_IN2, LOW);
    digitalWrite(MTRB_IN1, LOW);
    digitalWrite(MTRB_IN2, LOW);
}

void moveForward() {
    // Set direction pins: Motor A FWD, Motor B FWD
    digitalWrite(MTRA_IN1, HIGH);
    digitalWrite(MTRA_IN2, LOW);
    digitalWrite(MTRB_IN1, HIGH);
    digitalWrite(MTRB_IN2, LOW);
    
    // Set both motor speeds to the manual default
    ledcWrite(MTRA_EN, MANUAL_SPEED);
    ledcWrite(MTRB_EN, MANUAL_SPEED);
}

void moveBackward() {
    // Set direction pins: Motor A REV, Motor B REV
    digitalWrite(MTRA_IN1, LOW);
    digitalWrite(MTRA_IN2, HIGH);
    digitalWrite(MTRB_IN1, LOW);
    digitalWrite(MTRB_IN2, HIGH);

    // Set both motor speeds
    ledcWrite(MTRA_EN, MANUAL_SPEED);
    ledcWrite(MTRB_EN, MANUAL_SPEED);
}

void turnLeft() {
    // Pivot Left: Motor A Backward, Motor B Forward
    // Motor A (Left) -> Reverse
    digitalWrite(MTRA_IN1, LOW);
    digitalWrite(MTRA_IN2, HIGH); 
    // Motor B (Right) -> Forward
    digitalWrite(MTRB_IN1, HIGH);
    digitalWrite(MTRB_IN2, LOW);

    // Set both motor speeds to the manual default
    ledcWrite(MTRA_EN, MANUAL_SPEED);
    ledcWrite(MTRB_EN, MANUAL_SPEED);
}

void turnRight() {
    // Pivot Right: Motor A Forward, Motor B Backward
    
    digitalWrite(MTRA_IN1, HIGH);
    digitalWrite(MTRA_IN2, LOW); 
   
    digitalWrite(MTRB_IN1, LOW);
    digitalWrite(MTRB_IN2, HIGH);

    // Set both motor speeds to the manual default
    ledcWrite(MTRA_EN, MANUAL_SPEED); 
    ledcWrite(MTRB_EN, MANUAL_SPEED);
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
    uint8_t pattern = readLineSensors();
    int speed = BASE_SPEED; 
    
    // Forward Movement 
    digitalWrite(MTRA_IN1, HIGH);
    digitalWrite(MTRA_IN2, LOW);
    digitalWrite(MTRB_IN1, HIGH);
    digitalWrite(MTRB_IN2, LOW);

    //  Line Following Logic 
    switch (pattern) {
        
        // 1. PERFECT/NEAR-PERFECT CENTERED (OL IL IR OR) ---
        case 0b0110: // 0110: Inner two sensors are on line. Ideal position.
        case 0b0010: // 0010: Slightly left, IR on.
        case 0b0100: // 0100: Slightly right, IL on.
            ledcWrite(MTRA_EN, speed);
            ledcWrite(MTRB_EN, speed);
            break;

        //  2. GENTLE CORRECTION LEFT (Drifting Right) ---
        case 0b0011: // 0011: Inner Right and Outer Right are on line.
        case 0b0001: // 0001: Only Outer Right on line.
            ledcWrite(MTRA_EN, speed);        // Full speed Motor A (Left)
            ledcWrite(MTRB_EN, speed / 2);   // Half speed Motor B (Right) (Gentle correction)
            break;

        // 3. GENTLE CORRECTION RIGHT (Drifting Left) ---
        case 0b1100: // 1100: Outer Left and Inner Left are on line.
        case 0b1000: // 1000: Only Outer Left on line.
            ledcWrite(MTRA_EN, speed / 2);    // Half speed Motor A (Left)
            ledcWrite(MTRB_EN, speed);       // Full speed Motor B (Right) (Gentle correction)
            break;

        //  4. LOST LINE / SAFETY STOP ---
        case 0b0000: // 0000: Lost line 
            stopMoving();
            break;

        // 5. INTERSECTION / STOP LINE ---
        case 0b1111: // 1111: All sensors on. Intersection or Wide Stop Line.
           
            ledcWrite(MTRA_EN, speed / 3);
            ledcWrite(MTRB_EN, speed / 3);
            break;

        default: 
            // Any other rare pattern 
            stopMoving();
            break;
    }
}