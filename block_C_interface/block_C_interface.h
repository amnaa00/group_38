#pragma once

void initBluetooth();
void initOLED();
void handleBluetooth();
void handleAutoWithObstacle();
void showStatusOnOLED(const char* msg);
bool isManualMode();
int getSelectedDestination();
