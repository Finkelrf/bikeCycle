#include "display.h"

//#define HARDWARE_TIMER

#define DISPLAY_FREQ 100

static uint8_t* findLedSeg(char c);

struct displayState
{
	uint8_t currentlyOn;
	char info[NUMBER_OF_DISPLAYS];
};

uint8_t digitTable[12][8] = {  
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
	{ 1, 1, 1, 1, 1, 1, 0, 1 }, // = -
	{ 1, 1, 1, 1, 1, 1, 1, 1 } // = 
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
		case ' ':
			return digitTable[11];
			break;
	}
	return NULL;
}

void displayInit(){
	float period = 1000000/DISPLAY_FREQ/NUMBER_OF_DISPLAYS;
	halInitDisplay(period);
	state.currentlyOn = 0;
	int i;
	for (i = 0; i < NUMBER_OF_DISPLAYS; ++i)
	{
		state.info[i] = '0';
	}
}

void displayHandler(){
	//shift display on
	state.currentlyOn = (state.currentlyOn + 1)%NUMBER_OF_DISPLAYS;
#ifdef DEBUG
	if(state.currentlyOn == 2){
		delayCalcStart(0);			
	}else if(state.currentlyOn == 3){
		delayCalcStop(0);
	}
#endif
	displayWriteDigit(state.currentlyOn,state.info[state.currentlyOn]);
	halClearDisplayMultiplexFlag();
}


void displaySetInfo(char* info){
	int i = 0;
	int done = 0;
	while(!done){
		if(info[i] == '\0' || i == NUMBER_OF_DISPLAYS){
			done = 1;
			for (; i < NUMBER_OF_DISPLAYS; ++i)
			{
				state.info[i] = ' ';
			}
		}
		else{
			state.info[i] = info[i];
			i++;
		}
	}
}

bool displayUpdateFlag(){
#ifdef HARDWARE_TIMER
	return halGetDisplayMultiplexFlag();
#else
	if(!delayCalcIsRunning(1)){
		delayCalcGivePermissionToRun(1);
		delayCalcStart(1);
	}else{
		//period of display has passed
		if(delayCalcShowTimePassed(1)>=1000/DISPLAY_FREQ/NUMBER_OF_DISPLAYS){
			delayCalcStop(1);
			delayCalcGivePermissionToRun(1);
			delayCalcStart(1);
			return true;
		}else{
			return false;
		}
	}
	return false;
#endif
}