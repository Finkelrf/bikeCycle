#include "hal.h"
#include "display.h"
#include "TimerOne.h"
#include "common.h"
#include "delayCalc.h"
#include "com.h"

enum stateMachine
{
  SM_INIT,
  SM_IDLE,
  SM_UPDATE_DISPLAY,
  SM_READ_SERIAL,
  SM_FEED_APP,
  SM_INTERPRETE_COMM,
  SM_EXECUTE_COMM
};

enum stateMachine sm = SM_INIT;

#ifdef DEBUG  
unsigned long previousMillis = 0;      
unsigned long interval = 1000; 
bool printou = false;
#endif

void printf(char *c){
  Serial.println(c);
}


void setup() {
#ifdef DEBUG
  delayCalcInit();
  delayCalcGivePermissionToRun(0);
#endif
}

void loop() {

  switch(sm){
    case SM_INIT:
      //initiate all modules of th system
      comInit();
      displayInit();
      displaySetInfo("123456");
      sm = SM_IDLE;
      break;
    case SM_IDLE:
      //check flags in order of priority
      if(displayUpdateFlag()){
        sm = SM_UPDATE_DISPLAY;
      }else if(comGetIfDataRcv()){
        sm = SM_READ_SERIAL;
      }else if(comGetCommandRcvFlag()){
        sm = SM_INTERPRETE_COMM;
      }else if(comGetExecCommFlag()){
        sm = SM_EXECUTE_COMM;
      }else{
        sm = SM_IDLE;
      }
      break;
    case SM_FEED_APP:
      sm = SM_IDLE;
      break;
    case SM_READ_SERIAL:
      comReadByte();
      sm = SM_IDLE;
      break;
    case SM_UPDATE_DISPLAY:
      displayHandler();
      sm = SM_IDLE;
      break;
    case SM_INTERPRETE_COMM:
      if(comInterpreteCmd()){
        comSetExecFlag();
        Serial.println("cmd found");
      }
      sm = SM_IDLE;
      break;
    case SM_EXECUTE_COMM:
      sm = SM_IDLE;
      break;
  }


  

#ifdef DEBUG
  unsigned long currentMillis = millis(); 
  if((currentMillis - previousMillis > interval)&&!printou) {
    Serial.print("delay: ");
    Serial.println(delayCalcGetDelay(0));
    printou = true;
    Serial.println(displayUpdateFlag());
  }
#endif



}

