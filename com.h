#ifndef COM_H
#define COM_H

#include <Arduino.h>

void comInit();
bool comGetIfDataRcv();
bool comGetCommandRcvFlag();
char comReadByte();


#endif