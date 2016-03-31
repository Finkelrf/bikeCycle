#ifndef LEDCONTROL_H
#define LEDCONTROL_H

#include "common.h"
#include "display.h"

#define MAX_LED_BRIGHTNESS 63	
#define NUMBER_OF_LEDS 3



void ledCtrlInit();
void ledCtrlShowNavStraight();
void ledCtrlShowNavLeft(int dist);
void ledCtrlShowNavRight(int dist);
void ledCtrlShowNavRDB1(int dist);
void ledCtrlShowNavRDB2(int dist);
void ledCtrlShowNavRDB3(int dist);
void ledCtrlShowNavRDB4(int dist);
void ledCtrlShowNavBack();
void ledCtrlSetBright(int percentage);
void TurnOffAllLeds();

#endif