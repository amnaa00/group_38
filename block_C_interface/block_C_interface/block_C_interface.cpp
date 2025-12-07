#include <BluetoothSerial.h>
#include <Adafruit_GFX.h>
//#include <Adafruit_ST7735.h>
#include "block_A_movement.h"
#include "block_B_navigation.h"
#include "block_C_interface.h"
//#include <SPI.h>
// Define pins
// ----- OLED pins (already in your code) -----
/*#define TFT_CS   5
#define TFT_DC   2
#define TFT_RST  4
#define TFT_MOSI 23 // SDA
#define TFT_SCLK 18 // SCL
*/


const int OBSTACLE_LIMIT_CM = 3;   // stop if object is closer than this
BluetoothSerial BT;

// Create display object
//Adafruit_ST7735 display = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

bool obst = false;
bool manualMode = true;
int selectedDest = 0;

void initBluetooth() {
  BT.begin("MyRobot");
  Serial.println("Bluetooth ready");
}

/*void initOLED() {
  SPI.begin(TFT_SCLK, -1, TFT_MOSI, TFT_CS);
  // SCK, MISO, MOSI, SS
   // Initialise the ST7735 driver
 // display.initR(INITR_BLACKTAB); 
  display.initR(INITR_MINI160x80);

  display.setRotation(1);
  display.fillScreen(ST77XX_BLACK);

  display.setCursor(0, 0);
  display.setTextColor(ST77XX_WHITE);
  display.setTextSize(2);
  display.println("Robot Ready!");
}     */
  
bool Rintersection(){
  if (digitalRead(LINE_OUTER_RIGHT)==0&&digitalRead(LINE_OUTER_LEFT)==1) return(true);
  else return (false);
}
bool Lintersection() {
  if (digitalRead(LINE_OUTER_LEFT) == 0 && digitalRead(LINE_OUTER_RIGHT) == 1) {
    return true;
  } else {
    return false;
  }
}

bool isManualMode() {
  return manualMode;
}

int getSelectedDestination() {
  return selectedDest;
}
//bool isIntersection() {
  //uint8_t pattern = readLineSensors();
  //return (pattern == 0b1111);  // all sensors on line
//}
void goStraightOverIntersection() {
    moveForward();
    delay(300);
}

void followUntilIntersection() {
  while (true) {
    runLineFollow();
     //OBSTACLE_CHECK();
    if (Lintersection()) {
      stopMoving();
      delay(200);
      break;
    }
  }
}

void followBackToStart() {
   while (true) {
        
        uint8_t pattern = readLineSensors();
        if (!Rintersection()) {
           runLineFollow();
              }
          if (Rintersection()) {
               moveForward();
               delay(300);
              runLineFollow();
            }

        if (pattern == 0b0000) {
               //stopMoving();
               dropObject();
               break;
              // dropObject();
            }   // home reached
        }
    }



//void showStatusOnOLED(const char* msg) {
  // Clear the screen
//  display.fillScreen(ST77XX_BLACK);

  // First line - mode
  //display.setCursor(0, 0);
  //display.setTextSize(2);
  //display.setTextColor(ST77XX_YELLOW);
  //display.print("Mode: ");
  //if (manualMode) {
    //display.println("Manual");
  //} else {
    //display.println("Auto");
    
  //}

  

  // Second line - destination
  /*display.setTextColor(ST77XX_CYAN);
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

  
}v*/
void goToDestination(int dest) {
  int node = 0;
  bool reached = false;

  while (!reached) {
    // always allow user to break out into manual mode
    handleBluetooth();
    if (manualMode) {
      stopMoving();
      return;
    }

    followUntilIntersection();
    node++;
    //OBSTACLE_CHECK();

    if (dest == 1) {
      if (node == 1) {
        // first junction: turn left into branch
        turnLeft();
        delay(400);
        goStraightOverIntersection();
        runLineFollow();
        //OBSTACLE_CHECK();
        reached = true;
      }

    } else if (dest == 2) {
      if (node == 1) {
        // go straight at first junction
        goStraightOverIntersection();
      } else if (node == 2) {
        // second junction: turn left to destination
        turnLeft();
        delay(400);
        goStraightOverIntersection();
        runLineFollow();
        //OBSTACLE_CHECK();
        reached = true;
      }

    } else if (dest == 3) {
      if (node < 3) {
        goStraightOverIntersection();
      } else if (node == 3) {
        // third junction is destination
        runLineFollow();
        //OBSTACLE_CHECK();
        
        reached = true;
      }
    }
  }
}


void handleBluetooth() {
  if (!BT.available()) return;

  char cmd = BT.read();

  // mode switching
  if (cmd == 'M') {
    manualMode = true;
    return;
  }

  if (cmd == 'A') {
    manualMode = false;
    return;
  }

  // MANUAL MODE
  if (manualMode) {
  switch (cmd) {
    case 'F':
      moveForward();
      break;

    case 'B':
      moveBackward();
      break;

    case 'L':
      turnLeft();
      delay(400);
      moveForward();
      break;

    case 'R':
      turnRight();
      delay(400);
      moveForward();
      break;

    case 'S':
      stopMoving();
      break;

    case 'P':   // manual pick
      stopMoving();
      pickObject();
      break;

    case 'D':   // manual drop
      stopMoving();
      dropObject();
      break;

    case '1':   // optional: select destination while in manual
      selectedDest = 1;
      break;
    case '2':
      selectedDest = 2;
      break;
    case '3':
      selectedDest = 3;
      break;
  }
  return;
}


void handleAutoWithObstacle() {
  // Only relevant in AUTO mode
  if (manualMode) {
    return;  // do nothing if we are in manual mode
  }
   goToDestination(selectedDest);  
  long d = getDistanceCm(); // get distance block B

  // simple check: valid reading and closer than limit
 // bool obstacle = (d > 0 && d < OBSTACLE_LIMIT_CM);
    bool obstacle = false;
  if (!obstacle && d >= 2 && d <= 5) {    
    //showStatusOnOLED("Obstacle ahead!");
    stopMoving(); 
   pickObject();;   // close gripper

    obstacle = true;  // do not grab again

    // optional: freeze program after grabbing
    // while (true) { delay(1000); }
  
    // stop the robot and show warning
    //stopMoving();   // from blockA_movement.h

    //obst = true;
    //showStatusOnOLED("Obstacle ahead!");
     // pick it
      //  pickObject();

        delay(300);
        moveBackward();
        uint8_t pattern = readLineSensors();
        if (pattern == 0b0000)// need to be set to a specific pattern
        {stopMoving();
         turnLeft();
         followBackToStart(); }// continue following line
         if (pattern == 0b0000){
          dropObject();
         }
        obstacle = false;
  }

  } 
  //else { obst = false;
  //}
  
 
 