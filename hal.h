#ifndef HAL_H
#define HAL_H

#include <Arduino.h>
#include "TimerOne.h"

#define NUMBER_OF_BUSES 9
#define NUMBER_OF_DISPLAYS 2

void halTurnLedDisplayOn(uint8_t ledId,uint8_t displayId);
void halTurnOffDisplay(uint8_t displayId);
void halResetDisplays();
void halTurnOnDisplay(uint8_t displayId);
void halTurnSingleLedDisplayOn(char ledLetter,int displayId);

void halInitDisplay();
uint8_t halGetDisplayMultiplexFlag();
void halClearDisplayMultiplexFlag();
void halInitDisplay(uint16_t interval);
void callback();



#endif