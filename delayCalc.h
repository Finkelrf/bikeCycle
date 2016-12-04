#ifndef DELAYCALC_H
#define DELAYCALC_H

#include "Arduino.h"
#include "common.h"

#define DELAY_CALC_DISPLAY 0
#define DELAY_CALC_POINTER 4

void delayCalcInit();
void delayCalcGivePermissionToRun(uint8_t id);
void delayCalcStart(uint8_t id);
void delayCalcStop(uint8_t id);
long delayCalcGetDelay(uint8_t id);
long delayCalcShowTimePassed(uint8_t id);
bool delayCalcIsRunning(uint8_t id);

#endif