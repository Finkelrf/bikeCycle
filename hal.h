#ifndef HAL_H
#define HAL_H

#include <Arduino.h>

void halTurnLedDisplayOn(uint8_t ledId,uint8_t displayId);
void halTurnOffDisplay(uint8_t displayId);
void halResetDisplays();
void halTurnOnDisplay(uint8_t displayId);
void halTurnSingleLedDisplayOn(char ledLetter,int displayId);





#endif