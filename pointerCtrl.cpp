#include "pointerCtrl.h"

typedef enum direction
{
	NORMAL = 0,
	BACKWARDS = 1
}direction_t;

typedef enum mode
{
	ONE_AT_TIME,
	UNTIL_FULL,
	ARRIVED,

}blink_mode_t;


Servo servo; 

int ledState[] = {LOW,LOW,LOW,LOW,LOW};
int ledPin[] = {LED0_PIN,LED1_PIN,LED2_PIN,LED3_PIN,LED4_PIN};
int updateInterval = 200;
direction_t dir = NORMAL;
int servoPosition = 0;
bool updatePositon = true;
int distance = 0;

blink_mode_t mode = UNTIL_FULL;
int blinkPos =0;

void pointerCtrl_init(){
	servo.attach(SERVO_PIN); 
	int i;
	for(i=0;i<NUMBER_OF_LEDS;i++){
		pinMode(ledPin[i],OUTPUT);
	}
	for(i=0;i<NUMBER_OF_LEDS;i++){
		digitalWrite(ledPin[i],LOW);
	}
}

void pointerCtrl_move(int degrees){
	servoPosition = degrees;
	updatePositon = true;
}

void pointerCtrl_setDistance(int d){
	distance = d;
	if(distance>2000){
		updateInterval = 200;
	}else if(distance<1000){
		updateInterval = map(distance,0,2000,50,200);
	}
	if(distance<15){
		mode = ARRIVED;
	}
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
		}else{
			servo.write(servoPosition);
			dir = NORMAL;
		}
		updatePositon = false;
	}

	switch(mode){
		case ONE_AT_TIME:
			if(dir == NORMAL){
				//turn of all leds
				int i;
				for(i=0;i<NUMBER_OF_LEDS;i++){
					digitalWrite(ledPin[i],LOW);
				}
				digitalWrite(ledPin[blinkPos],HIGH);
				blinkPos = (blinkPos+1)%NUMBER_OF_LEDS;		
			}else{
				//turn of all leds
				int i;
				for(i=0;i<NUMBER_OF_LEDS;i++){
					digitalWrite(ledPin[i],LOW);
				}
				digitalWrite(ledPin[(NUMBER_OF_LEDS-1)-blinkPos],HIGH);
				blinkPos = (blinkPos+1)%NUMBER_OF_LEDS;
			}
			break;

		case UNTIL_FULL:
			if(dir == NORMAL){
				//turn of all leds
				int i;
				if(blinkPos == 0){
					for(i=0;i<NUMBER_OF_LEDS;i++){
						digitalWrite(ledPin[i],LOW);
					}
				}
				digitalWrite(ledPin[blinkPos],HIGH);
				blinkPos = (blinkPos+1)%NUMBER_OF_LEDS;		
			}else{
				//turn of all leds
				int i;
				if(blinkPos == 0){
					for(i=0;i<NUMBER_OF_LEDS;i++){
						digitalWrite(ledPin[i],LOW);
					}
				}
				digitalWrite(ledPin[(NUMBER_OF_LEDS-1)-blinkPos],HIGH);
				blinkPos = (blinkPos+1)%NUMBER_OF_LEDS;		
			}
			break;

		case ARRIVED:
		int i;
			for(i=0;i<NUMBER_OF_LEDS;i++){
				digitalWrite(ledPin[i],HIGH);
			}
			break;

		default:
			break;
	}

	
}