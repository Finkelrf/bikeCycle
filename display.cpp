#include "display.h"

//#define HARDWARE_TIMER

#define DISPLAY_FREQ 100

static uint8_t* findLedSeg(char c);

struct displayState
{
	uint8_t currentlyOn;
	char info[NUMBER_OF_DISPLAYS];
	bool point[NUMBER_OF_DISPLAYS];
};

uint8_t digitTable[][8] = {  
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
	{ 1, 1, 1, 1, 1, 1, 1, 1 }, // = 
	{ 1, 0, 0, 1, 0, 0, 0, 1 },	// = k
	{ 0, 1, 0, 1, 0, 1, 1, 1 }, // = m
	{ 1, 1, 0, 1, 0, 0, 0, 1 }, // = h
	{ 0, 1, 0, 0, 1, 0, 0, 1 },	// = s
	{ 0, 0, 1, 1, 0, 0, 0, 1 },	// = p
	{ 0, 1, 1, 0, 0, 0, 0, 1 }, // = e
	{ 1, 0, 0, 0, 0, 1, 0, 1 }, // = d	
	{ 1, 1, 1, 0, 0, 1, 0, 1 },	// = c
	{ 1, 1, 0, 1, 1, 1, 1, 1 },	// = i
	{ 0, 1, 1, 1, 0, 0, 1, 1 },	// = t
	{ 0, 0, 0, 1, 0, 0, 0, 1 },	// = a
	{ 1, 1, 0, 1, 0, 1, 0, 1 },	// = n 
	{ 1, 1, 0, 0, 0, 1, 1, 1 },	// = u
	{ 1, 1, 1, 1, 0, 1, 0, 1 }
};

struct displayState state;


uint8_t displayWriteDigit(int displayId, char toWrite,bool point){
	uint8_t* ptr = findLedSeg(toWrite);
	halResetDisplays();
	halTurnOnDisplay(displayId);
	int i;
	for (i = 0; i < 7; ++i)
	{
		if(ptr[i]==0){
			halTurnLedDisplayOn(i,displayId);
		}
	}
	if(point){
		halTurnLedDisplayOn(7,displayId);
	}else{

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
			return digitTable[c - '0'];break;
		case '-':
			return digitTable[10];break;
		case ' ':
			return digitTable[11];break;
		case 'k':
			return digitTable[12];break;
		case 'm':
			return digitTable[13];break;
		case 'h':
			return digitTable[14];break;
		case 's':
			return digitTable[15];break;
		case 'p':
			return digitTable[16];break;
		case 'e':
			return digitTable[17];break;
		case 'd':
			return digitTable[18];break;
		case 'c':
			return digitTable[19];break;
		case 'i':
			return digitTable[20];break;
		case 't':
			return digitTable[21];break;
		case 'a':
			return digitTable[22];break;
		case 'n':
			return digitTable[23];break;
		case 'u':
			return digitTable[24];break;
		case 'r':
			return digitTable[25];break;
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
	displayWriteDigit(state.currentlyOn,state.info[state.currentlyOn],state.point[state.currentlyOn]);
	halClearDisplayMultiplexFlag();
}


void displaySetInfo(char* info){
	int done = 0;

	//get the lenght of the string
	int i;
	int arrayLen = 0;
	for(i=0;i<NUMBER_OF_DISPLAYS;i++){
		if(info[i] == '\0')
			break;
		else
			if(info[i] != '.' && info[i] != ',' )
				arrayLen++;
	}

	//clear all display info
	for (i=0; i < NUMBER_OF_DISPLAYS; ++i)
	{
		state.info[i] = ' ';
		state.point[i] = false; 
	}

	i=0;
	int iaux = i;

	while(!done){
		if(info[iaux] == '\0' || i == NUMBER_OF_DISPLAYS){
			done = 1;
		}else if(info[iaux] == '.' || info[iaux] == ','){
			state.point[i+(NUMBER_OF_DISPLAYS-arrayLen-1)] = true; 
			iaux++;
		}else{
			state.info[i+(NUMBER_OF_DISPLAYS-arrayLen)] = info[iaux];
			state.point[i+(NUMBER_OF_DISPLAYS-arrayLen)] = false; 
			i++;
			iaux++;
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