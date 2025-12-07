#include "block_A_movement.h"
#include "block_B_navigation.h"
#include "block_C_interface.h"

void setup() {
   
   Serial.begin(115200);
   initMovement();
   initBluetooth();
   initBlockB(); 
   //initOLED();
}

void loop() {
   handleBluetooth();
   handleAutoWithObstacle();
  //if (!isManualMode() && !obst) {
     // if (getSelectedDestination() != 0) {
        //  goToDestination(getSelectedDestination()); // block_B_how to move to the selected destination.
//}}
 }
