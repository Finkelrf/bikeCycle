#include "hal.h"

#define NUMBER_OF_BUSES 9
#define NUMBER_OF_DISPLAYS 2


int busPins[9] = {2,3,4,5,6,7,8,9,10};
/* 	
	first index ID do display
	second index (I(nput power),A,B,C,D,E,F,G,P(oint))
*/
int displayBusSeq[NUMBER_OF_DISPLAYS][NUMBER_OF_BUSES] = {{0,8,1,2,3,4,5,6,7},{1,0,8,2,3,4,5,6,7}};

void halTurnLedDisplayOn(uint8_t ledId,uint8_t displayId){

	/*
		Turn on Input power
		Turn on led (led is on with low logic level) 
	*/
	pinMode(busPins[displayBusSeq[displayId][ledId+1]],OUTPUT);
	digitalWrite(busPins[displayBusSeq[displayId][ledId+1]],LOW);
}


void halTurnOffDisplay(uint8_t displayId){
	pinMode(busPins[displayBusSeq[displayId][0]],INPUT);
}

void halResetDisplays(){
	int i;
	for (i = 0; i < NUMBER_OF_BUSES; ++i)
	{
		pinMode(busPins[i],INPUT);
	}
}

void halTurnOnDisplay(uint8_t displayId){
	pinMode(busPins[displayBusSeq[displayId][0]],OUTPUT);
	digitalWrite(busPins[displayBusSeq[displayId][0]],HIGH);
}

void halTurnSingleLedDisplayOn(char ledLetter,int displayId){
	//TODO convert ledLetter to upper case
	int ledLetterId = 99;
	//calculate index of letter
	switch(ledLetter){
		case 'I':
			ledLetterId = 0;
			break;
		case 'A':
			ledLetterId = 1;
			break;
		case 'B':
			ledLetterId = 2;
			break;
		case 'C':
			ledLetterId = 3;
			break;
		case 'D':
			ledLetterId = 4;
			break;
		case 'E':
			ledLetterId = 5;
			break;
		case 'F':
			ledLetterId = 6;
			break;
		case 'G':
			ledLetterId = 7;
			break;
		case 'P':
			ledLetterId = 8;
			break;
	}

	/*
		Turn on Input power
		Turn on led (led is on with low logic level) 
	*/
	pinMode(busPins[displayBusSeq[displayId][ledLetterId]],OUTPUT);
	digitalWrite(busPins[displayBusSeq[displayId][ledLetterId]],LOW);
}