#ifndef POINTERCTRL_H	
#define POINTERCTRL_H


#include "common.h"
#include "Servo.h"
#include "delayCalc.h"

#define SERVO_PIN 9
#define LED0_PIN 2
#define LED1_PIN 3
#define LED2_PIN 4
#define LED3_PIN 5
#define LED4_PIN 6

#define NUMBER_OF_LEDS 5
/**
	Init the arrow pointer and the leds used to show distance
*/
void pointerCtrl_init();
void pointerCtrl_move(int degrees);
bool pointerUpdateFlag();
void pointerHandler();
void pointerSetPosition();
void pointerCtrl_setDistance(int d);

#endif