#include "hal.h"
#include "display.h"
#include "TimerOne.h"

enum stateMachine
{
  SM_IDLE,
};
void setup() {
  Serial.println("Inicializando Timer1");
  displayInit();

  Serial.begin(9600);
  Serial.println("chalieplexing");

  char msg[7] = "123456";

  Serial.println("printando 123456");
  displaySetInfo(msg);
  
}

void loop() {


  displayHandler();



}

