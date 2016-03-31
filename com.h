#ifndef COM_H	
#define COM_H

//#define BT_SOFTSERIAL
//#define COM_DEBUG

#include <Arduino.h>
#include "common.h"
#include "display.h"
#include "ledControl.h"
#ifdef BT_SOFTSERIAL
#include "SoftwareSerial.h"
#endif



void comInit();
bool comGetIfDataRcv();
uint8_t comGetCommandRcvFlag(); 
char comReadByte();
bool comGetExecCommFlag();
bool comInterpreteCmd();
void comSetExecFlag();
bool comInterpreteCmd();
bool comInterpreteCmdPreemptive();
bool comExecuteCmd();


#endif