#include "intf.h"

static uint8_t* findLedSeg(char c);


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


uint8_t intfWriteDigit(int displayId, char toWrite){
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
}