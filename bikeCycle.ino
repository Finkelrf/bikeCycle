#include "hal.h"
#include "display.h"
#include "common.h"
#include "delayCalc.h"
#include "com.h"
#include "ledControl.h"

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

 
unsigned long previousMillis = 0;      
unsigned long interval = 250; 
bool demoIsOn = false;
int dist;
int demoTurnCount;

void printf(char *c){
  Serial.println(c);
}


void setup() {
  previousMillis = millis();
}

void loop() {

  switch(sm){
    case SM_INIT:
      //initiate all modules of th system
      comInit();
      displayInit();
      displaySetInfo("      ");
      ledCtrlInit();
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
      checkDemo();
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
      if(comInterpreteCmdPreemptive()){
        //Serial.println("CMD FOUND!!!");
        comSetExecFlag();
      }
      sm = SM_IDLE;
      break;
    case SM_EXECUTE_COMM:
      comExecuteCmd();
      sm = SM_IDLE;
      break;
  }


}



void startDemo(){
  demoIsOn = true;
  Serial.println("Starting Demo");
  dist = random(25,30);
  demoTurnCount = 4;
}


void checkDemo(){

  unsigned long currentMillis = millis(); 
  if((currentMillis - previousMillis > interval)&&demoIsOn) {
    previousMillis = currentMillis;
    dist--;
    //update distance
    char buff[6];
    sprintf(buff, "%d", dist);
    displaySetInfo(buff);

    if(dist==0){
      demoTurnCount--;
      if(demoTurnCount==0){
        demoIsOn = false;
        TurnOffAllLeds();
        displaySetInfo("      ");
      }
      dist = random(25,30); 
    }
    switch(demoTurnCount){
      case 4:
        ledCtrlShowNavRight(dist);
        break;
      case 3:
        ledCtrlShowNavLeft(dist);
        break;
      case 2:
        ledCtrlShowNavRDB2(dist);
        break;
      case 1:
        ledCtrlShowNavRDB3(dist);
        break;
    }
  }

}