#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include "hal.h"



void displayInit();
uint8_t displayWriteDigit(int displayId, char toWrite);
void displaySetInfo(char* info);
void displayHandler();




#endif