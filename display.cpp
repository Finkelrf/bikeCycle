#include "display.h"

#define REFRESH_INTERVAL 2

static uint8_t* findLedSeg(char c);

struct displayState
{
	uint8_t currentlyOn;
	char info[NUMBER_OF_DISPLAYS];
};

uint8_t digitTable[11][8] = {  
	{ 0, 0, 0, 0, 0, 0, 1, 1 }, // = 0
	{ 1, 0, 0, 1, 1, 1, 1, 1 }, // = 1
	{ 0, 0, 1, 0, 0, 1, 0, 1 }, // = 2
	{ 0, 0, 0, 0, 1, 1, 0, 1 }, // = 3
	{ 1, 0, 0, 1, 1, 0, 0, 1 }, // = 4
	{ 0, 1, 0, 0, 1, 0, 0, 1 }, // = 5
	{ 0, 1, 0, 0, 0, 0, 0, 1 }, // = 6
	{ 0, 0, 0, 1, 1, 1, 1, 1 }, // = 7
	{ 0, 0, 0, 0, 0, 0, 0, 1 }, // = 8
	{ 0, 0, 0, 1, 1, 0, 0, 1 }, // = 9
	{ 1, 1, 1, 1, 1, 1, 0, 1 } // = -
};

struct displayState state;


uint8_t displayWriteDigit(int displayId, char toWrite){
	uint8_t* ptr = findLedSeg(toWrite);
	halResetDisplays();
	halTurnOnDisplay(displayId);

	int i;
	for (i = 0; i < 8; ++i)
	{
		if(ptr[i]==0){
			halTurnLedDisplayOn(i,displayId);
		}
	}
	return 1;

}

static uint8_t* findLedSeg(char c){
	switch(c){
		case '0':			
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			return digitTable[c - '0'];
			break;
		case '-':
			return digitTable[10];
			break;
	}
	return NULL;
}

void displayInit(){
	halInitDisplay(REFRESH_INTERVAL);
	state.currentlyOn = 0;
	int i;
	for (i = 0; i < NUMBER_OF_DISPLAYS; ++i)
	{
		state.info[i] = '0';
	}
}

void displayHandler(){
	if(halGetDisplayMultiplexFlag()){
		//shift display on
		state.currentlyOn = (state.currentlyOn + 1)%NUMBER_OF_DISPLAYS;
		displayWriteDigit(state.currentlyOn,state.info[state.currentlyOn]);
	}
	halClearDisplayMultiplexFlag();
}


void displaySetInfo(char* info){
	int i = 0;
	int done = 0;
	while(!done){
		if(info[i] == '\0' || i == 6){
			done = 1;
		}
		else{
			state.info[i] = info[i];
			i++;
		}
	}
}