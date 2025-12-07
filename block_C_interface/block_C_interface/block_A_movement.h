#ifndef BLOCKA_H 
#define BLOCKA_H

#include <Arduino.h>
#include <cstdint> 


// 1. PIN DEFINITIONS (ADJUST THESE TO YOUR WIRING!)


// --- MOTOR CONTROL PINS 
// Motor A (Right) - Uses MTRA_CH
#define MTRA_IN1  19 // AIN1 (Direction 1)
#define MTRA_IN2  18 // AIN2 (Direction 2)
#define MTRA_EN   2 // PWMA (Speed)

// Motor B (Left) - Uses MTRB_CH
#define MTRB_IN1 23 // BIN1 (Direction 1)
#define MTRB_IN2 22 // BIN2 (Direction 2)
#define MTRB_EN  4 // PWMB (Speed)




// --- 4 IR -SENSOR PINS (Digital Inputs) ---
//  We use 4 independent pins for the 4 sensors.
#define LINE_OUTER_LEFT   35// OL
#define LINE_INNER_LEFT   34 // IL
#define LINE_INNER_RIGHT  33 // IR
#define LINE_OUTER_RIGHT  32 // OR

#define PWM_FREQ       5000 
#define PWM_RESOLUTION 8    
#define BASE_SPEED     155  
#define MANUAL_SPEED   180  


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