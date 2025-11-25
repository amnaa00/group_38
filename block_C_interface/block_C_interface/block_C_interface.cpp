#include <BluetoothSerial.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include "block_A_movement.h"
#include "block_B_navigation.h"
#include "block_C_interface.h"
// Define pins
// ----- OLED pins (already in your code) -----
#define TFT_CS   16
#define TFT_DC   2
#define TFT_RST  4

const int OBSTACLE_LIMIT_CM = 20;   // stop if object is closer than this
BluetoothSerial BT;

// Create display object
Adafruit_ST7735 display = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

bool obst = false;
bool manualMode = true;
int selectedDest = 0;

void initBluetooth() {
  BT.begin("MyRobot");
  Serial.println("Bluetooth ready");
}

void initOLED() {
   // Initialise the ST7735 driver
  display.initR(INITR_BLACKTAB);      
  display.fillScreen(ST77XX_BLACK);   // clear everything

  display.setRotation(1);             // optional, try 0..3 to match orientation
  display.setTextSize(1);             // small text
  display.setTextColor(ST77XX_WHITE); // white text on black background

  display.setCursor(0, 0);
  display.println("Robot Ready");
 

}


bool isManualMode() {
  return manualMode;
}

int getSelectedDestination() {
  return selectedDest;
}
bool isIntersection() {
  uint8_t pattern = readLineSensors();
  return (pattern == 0b1111);  // all sensors on line
}
void goStraightOverIntersection() {
    moveForward();
    delay(400);
}

void followUntilIntersection() {
  while (true) {
    runLineFollow();
    if (isIntersection()) {
      stopMoving();
      delay(200);
      break;
    }
  }
}

void followBackToStart() {
   while (true) {
        runLineFollow();
        uint8_t pattern = readLineSensors();
        if (isIntersection()) {
            moveForward();
            delay(300);
            runLineFollow();}
        if (pattern == 0b0000) {
               stopMoving();
               dropObject();
            }   // home reached
        }
    }



void showStatusOnOLED(const char* msg) {
  // Clear the screen
  display.fillScreen(ST77XX_BLACK);

  // First line - mode
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.setTextColor(ST77XX_YELLOW);
  display.print("Mode: ");
  if (manualMode) {
    display.println("Manual");
  } else {
    display.println("Auto");
    
  }

  

  // Second line - destination
  display.setTextColor(ST77XX_CYAN);
  display.print("Dest: ");
  if (selectedDest == 0) {
    display.println("None");
  } else {
    display.println(selectedDest);
  }

  // handling obstacle
  if (obst)
  {display.println("obstacle");
  }



  // Third line - custom message
  display.setTextColor(ST77XX_WHITE);
  display.setCursor(0, 32);
  display.println(msg);

  
}

void handleBluetooth() {
  if (!BT.available()) return;

  char cmd = BT.read();

  if (cmd == 'M') {
    manualMode = true;
    showStatusOnOLED("Manual Mode");
  }
  else if (cmd == 'A') {
    manualMode = false;
    showStatusOnOLED("Auto Mode");
  }
  else if (manualMode) {
    if (cmd == 'F') moveForward(); // block_A
    if (cmd == 'B') moveBackward(); // block_A
    if (cmd == 'L') turnLeft();   // block_A
    if (cmd == 'R') turnRight();   // block_A
    if (cmd == 'S') stopMoving();   // block_A
     if (cmd == 'P') {               // pick up object
              stopMoving();
              pickObject();         // from block_B_navigation
              showStatusOnOLED("Picked"); }  // oled
    if (cmd == 'D') {               // drop object
    stopMoving();
    dropObject();                 // from block_B_navigation
    showStatusOnOLED("Dropped");  // optional
  }
   }
 else {
    if (cmd == '1') selectedDest = 1;
    if (cmd == '2') selectedDest = 2;                        
    if (cmd == '3') selectedDest = 3;
    showStatusOnOLED("Going to destination");

  }
 }

 void goToDestination(int dest) {
  int node = 0;
  bool reached = false;

  while (!reached) {
    followUntilIntersection();  // stop at each junction
    node++;

    if (dest == 1) {
      if (node == 1) {
        // first junction: turn left into branch and then follow line a bit
        turnLeft();
        delay(400);    // enough to align with new branch
        moveForward();
        delay(300);
        runLineFollow();
        //stopMoving();
        reached = true;
      }
    }
    else if (dest == 2) {
      if (node == 1) {
        // go straight over first junction
        goStraightOverIntersection();
      } else if (node == 2) {
        turnLeft();
        delay(400);
        moveForward();
        delay(300);
        runLineFollow();
        //stopMoving();
        reached = true;
       
      }
    }
    else if (dest == 3) {
      if (node < 3) {
        goStraightOverIntersection();
      } else if (node == 3) {
       runLineFollow();// third junction is destination
        reached = true;
      }
    }
  }
 }



void handleAutoWithObstacle() {
  // Only relevant in AUTO mode
  if (manualMode) {
    return;  // do nothing if we are in manual mode
  }
   goToDestination(selectedDest);  
  long d = getDistanceCm(); // get distance block B

  // simple check: valid reading and closer than limit
  bool obstacle = (d > 0 && d < OBSTACLE_LIMIT_CM);

  if (obstacle) {
    // stop the robot and show warning
    stopMoving();   // from blockA_movement.h

    //obst = true;
    showStatusOnOLED("Obstacle ahead!");
     // pick it
        pickObject();

        delay(300);
        moveBackward();
        uint8_t pattern = readLineSensors();
        if (pattern == 0b0011)// need to be set to a specific pattern
         stopMoving();
         turnLeft();
         followBackToStart(); // continue following line
        obstacle = false;

  } 
  //else { obst = false;
  //}
  }
