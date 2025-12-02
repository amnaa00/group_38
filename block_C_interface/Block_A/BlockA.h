#ifndef BLOCKA_H 
#define BLOCKA_H

#include <Arduino.h>
#include <cstdint> 


// 1. PIN DEFINITIONS (ADJUST THESE TO YOUR WIRING!)


// --- MOTOR CONTROL PINS (TB6612FNG) ---
// Motor A (Left) - Uses MTRA_CH
#define MTRA_IN1  19 // AIN1 (Direction 1)
#define MTRA_IN2  18 // AIN2 (Direction 2)
#define MTRA_EN   17 // PWMA (Speed)

// Motor B (Right) - Uses MTRB_CH
#define MTRB_IN1 23 // BIN1 (Direction 1)
#define MTRB_IN2 22 // BIN2 (Direction 2)
#define MTRB_EN  21 // PWMB (Speed)



// --- 4 IR -SENSOR PINS (Digital Inputs) ---
//  We use 4 independent pins for the 4 sensors.
#define LINE_OUTER_LEFT   34 // OL
#define LINE_INNER_LEFT   35 // IL
#define LINE_CENTER 27
#define LINE_INNER_RIGHT  32 // IR
#define LINE_OUTER_RIGHT  33 // OR

// --- PWM CHANNEL & SPEED SETTINGS ---
#define MTRA_CH    0  // ESP32 PWM Channel 0
#define MTRB_CH    1  // ESP32 PWM Channel 1
#define PWM_FREQ       5000 
#define PWM_RESOLUTION 8    
#define BASE_SPEED     200  
#define MANUAL_SPEED   230
#define Turn_speed 150  


// 2. FUNCTION DECLARATIONS 


void initMovement();
void moveForward();
void moveBackward();
void turnLeft();
void turnRight();
void stopMoving();
uint8_t readLineSensors(); 
void runLineFollow(); 

#endif 
