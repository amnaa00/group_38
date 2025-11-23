#include "block_C_interface.h"
void setup() {
   Serial.begin(115200);
   initBluetooth();
   initOLED();
}

void loop() {
  void handleBluetooth();
  void handleAutoWithObstacle();
  //if (!isManualMode() && !obst) {
     // if (getSelectedDestination() != 0) {
        //  goToDestination(getSelectedDestination()); // block_B_how to move to the selected destination.
//}}
 }