#include "delayCalc.h"

#define NUMBER_OF_CALCULATORS 5

struct info{
	unsigned long start;
	unsigned long stop;	
	bool permissionToRun;
	bool running;
};

struct info calcInfo[NUMBER_OF_CALCULATORS];
void delayCalcInit(){
	int i;
	for (i = 0; i < NUMBER_OF_CALCULATORS; ++i)
	{
		calcInfo[i].start = 0;
		calcInfo[i].stop = 0;
		calcInfo[i].permissionToRun = false;
		calcInfo[i].running = false;
	}
}

void delayCalcGivePermissionToRun(uint8_t id){
	if(id < NUMBER_OF_CALCULATORS)
		calcInfo[id].permissionToRun = true;
}

void delayCalcStart(uint8_t id){
	if(id < NUMBER_OF_CALCULATORS && calcInfo[id].permissionToRun && !calcInfo[id].running){
		calcInfo[id].start = millis();
		calcInfo[id].running = true;
	}
}

void delayCalcStop(uint8_t id){
	if(id < NUMBER_OF_CALCULATORS && calcInfo[id].running){
		calcInfo[id].stop = millis();
		calcInfo[id].running = false;
		calcInfo[id].permissionToRun= false;
	}
}

long delayCalcGetDelay(uint8_t id){
	if(id < NUMBER_OF_CALCULATORS && !calcInfo[id].running){
		return calcInfo[id].stop-calcInfo[id].start;
	}else{
		return 0;
	}
}

long delayCalcShowTimePassed(uint8_t id){
	return millis()-calcInfo[id].start;
}

bool delayCalcIsRunning(uint8_t id){
	return calcInfo[id].running;
}