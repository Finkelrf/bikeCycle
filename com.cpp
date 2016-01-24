#include "com.h"

#define BUFFER_MAX_SIZE 30
#define FINAL_BYTE '\n'


char buffer[BUFFER_MAX_SIZE] = {'\0'};
uint16_t writeBuffPtr = 0;
bool comCommandRcvFlag = false;

uint16_t readBuffPtr = 0;

void comInit(){
	Serial.begin(9600);
  	Serial.println("Bike Cycle 1.0");
}

bool comGetIfDataRcv(){
	if(Serial.available())
		return true;
	else 
		return false;
}

char comReadByte(){
	buffer[writeBuffPtr] = (char)Serial.read();
	writeBuffPtr++;
	if(writeBuffPtr >= BUFFER_MAX_SIZE)
		writeBuffPtr = 0;
	//check if the command is fully received
	if(buffer[writeBuffPtr]==FINAL_BYTE)
		comCommandRcvFlag = true;
	return buffer[writeBuffPtr];
} 

bool comGetCommandRcvFlag(){
	return comCommandRcvFlag;
 }
