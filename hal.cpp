#include "hal.h"


bool multipleFlag = 0;
int busPins[9] = {2,3,4,5,6,7,8,9,10};
/* 	
	first index ID do display
	second index (I(nput power),A,B,C,D,E,F,G,P(oint))
*/
int displayBusSeq[NUMBER_OF_DISPLAYS][NUMBER_OF_BUSES] = {{0,8,1,2,3,4,5,6,7},{1,0,8,2,3,4,5,6,7},{2,0,1,8,3,4,5,6,7},{3,0,8,1,2,4,5,6,7},{4,3,2,1,0,8,5,6,7},{5,4,3,2,0,1,8,6,7}};

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


void halInitDisplay(uint16_t interval){
	//call Timer1 every 3ms tu update display
	Timer1.initialize(interval);         // initialize timer1, and set a 1/2 second period
	Timer1.attachInterrupt(callback);  // attaches callback() as a timer overflow interrupt
}

void callback(){
	multipleFlag = true;
}

bool halGetDisplayMultiplexFlag(){
	return multipleFlag;
}

void halClearDisplayMultiplexFlag(){
	multipleFlag = false;
}