#include "pointerCtrl.h"

typedef enum direction
{
	NORMAL = 0,
	BACKWARDS = 1
}direction_t;


Servo servo; 

int ledState[] = {LOW,LOW,LOW,LOW,LOW};
int ledPin[] = {LED0_PIN,LED1_PIN,LED2_PIN,LED3_PIN,LED4_PIN};
int updateInterval = 100;
direction_t dir = NORMAL;
int servoPosition = 0;
bool updatePositon = true;
int distance = 0;

void pointerCtrl_init(){
	servo.attach(SERVO_PIN); 
	servo.write(0);
	int i;
	for(i=0;i<NUMBER_OF_LEDS;i++){
		pinMode(ledPin[i],OUTPUT);
	}
}

void pointerCtrl_move(int degrees){
	servoPosition = degrees;
	updatePositon = true;
}

void pointerCtrl_setDistance(int d){
	distance = d;
}

bool pointerUpdateFlag(){
	if(!delayCalcIsRunning(DELAY_CALC_POINTER)){
		delayCalcGivePermissionToRun(DELAY_CALC_POINTER);
		delayCalcStart(DELAY_CALC_POINTER);
	}else{
		//period of display has passed
		if(delayCalcShowTimePassed(DELAY_CALC_POINTER)>=updateInterval){
			delayCalcStop(DELAY_CALC_POINTER);
			delayCalcGivePermissionToRun(DELAY_CALC_POINTER);
			delayCalcStart(DELAY_CALC_POINTER);
			return true;
		}else{
			return false;
		}
	}
}


void pointerHandler(){
	if(updatePositon){
		if(servoPosition>180){
			int pos = servoPosition-180;
			servo.write(pos);
			dir = BACKWARDS;
			char c[5] = "eie";
			printf(c);
		}else{
			servo.write(servoPosition);
			dir = NORMAL;
		}
		updatePositon = false;
	}

}