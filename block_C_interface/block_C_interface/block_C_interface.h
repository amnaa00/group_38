#pragma once

extern bool obst;
void initBluetooth();
//void initOLED();
void handleBluetooth();
void handleAutoWithObstacle();
//void showStatusOnOLED(const char* msg);
bool isManualMode();
int getSelectedDestination();
bool isIntersection();
void goStraightOverIntersection();
bool Rintersection();
bool Lintersection();
void goToDestination(int dest);
