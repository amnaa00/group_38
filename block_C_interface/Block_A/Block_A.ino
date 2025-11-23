#include "BlockA.h"

void setup() {
  Serial.begin(115200);
  initMovement(); // Function from cpp
}

void loop() {
  
  runLineFollow();
}
