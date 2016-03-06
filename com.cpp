#include "com.h"


#define BUFFER_MAX_SIZE 30
#define FINAL_BYTE 'p'

#define NUMBER_OF_CMDS 2
#define END_CMD_CHAR ':'

char buffer[BUFFER_MAX_SIZE] = {'\0'};
uint16_t writeBuffPtr = 0;
uint8_t comCommandRcvFlag = 0;
bool execCommFlag = false;

uint16_t readBuffPtr = 0;

struct cmdInfo{
	int cmdId = -1;	
	char cmdData[15];
};

struct cmdInfo cmdInfo;


#ifdef BT_SOFTSERIAL
int bluetoothTx = 12;
int bluetoothRx = 13;
SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);
#endif

char cmds[][15] = {
	"SET_DISPLAY:",
	"SAY_HI:"
};

struct intComPreempState{
	bool empty;
	char *cmd;
	int cmdsIterator;
	bool cmdFound;
	int dataIterator;
};
struct intComPreempState stateInt = {.empty = true};

static char btGetChar();

bool comInterpreteCmdPreemptive(){
	if(stateInt.empty){							//first iteration, it must be initialized
		stateInt.cmd = &buffer[readBuffPtr];
		stateInt.cmdsIterator = 0;
		stateInt.empty = false;
		stateInt.cmdFound = false;
		stateInt.dataIterator = 0;
	}
	if(stateInt.cmd[readBuffPtr] != cmds[stateInt.cmdsIterator][readBuffPtr]){ //it is not the same letter
		stateInt.cmdsIterator++;
	}else{															//it is the same letter, it means the it could be the command
		if(!stateInt.cmdFound){
			if(stateInt.cmd[readBuffPtr++] == END_CMD_CHAR){	//check if is end of command char
			Serial.println("Achou cmd");
			cmdInfo.cmdId = stateInt.cmdsIterator;
			stateInt.cmdFound = true;
			}
		}else if(stateInt.cmdFound){
			if(stateInt.cmd[readBuffPtr] != FINAL_BYTE){
				cmdInfo.cmdData[stateInt.dataIterator++] = stateInt.cmd[readBuffPtr++];
				Serial.println(cmdInfo.cmdData[stateInt.dataIterator-1]);
			}else{ //all data has been copied
				comCommandRcvFlag--; //clear flag
				stateInt.empty = true;//
				Serial.println(cmdInfo.cmdData);
				return true;
			}
		}
	}
	return false;

}

//deprecated - takes too long to execute and maker the processor restart
bool comInterpreteCmd(){
	Serial.println("entrou no interpretador");
	char *cmd = &buffer[readBuffPtr];
	comCommandRcvFlag--;
	int i;
	bool foundIt = false;
	for(i=0;i<NUMBER_OF_CMDS;i++){
		bool check = true;
		while(check){
			//compare if it is the same letter
			if(cmd[readBuffPtr] != cmds[i][readBuffPtr]){
				check = false;
			}else{
				//check if it end of comd
				if(cmd[readBuffPtr] == END_CMD_CHAR){
					Serial.println("Buscando data");
					foundIt = true;
					check = false;
					bool done = false;
					int j = 0;
					while(!done){
						if(cmd[readBuffPtr] != FINAL_BYTE){
							cmdInfo.cmdData[j] = cmd[readBuffPtr];
							j++;
						}else{
							done = true;
							Serial.println(cmdInfo.cmdData);
						}
					}
				}else{
					readBuffPtr++;
				}
			}
		}
		if(foundIt){
			cmdInfo.cmdId = i;
			return true;
		}
	}
	Serial.println("nao achou cmd");
	cmdInfo.cmdId = -1;
	return false;
}
void comInit(){
	Serial.begin(38400);
  Serial.println("Bike Cycle 1.0");

#ifdef BT_SOFTSERIAL
	bluetooth.begin(115200);
	bluetooth.print("$$$");
	delay(100);
	bluetooth.println("U,9600,N");
	bluetooth.begin(9600);
#endif
}

bool comGetIfDataRcv(){
	bool isAvailable;
#ifdef BT_SOFTSERIAL
	if(bluetooth.available())
		isAvailable = true;
	else 
		isAvailable = false;
#else
	if(Serial.available())
		isAvailable = true;
	else 
		isAvailable = false;
#endif
	return isAvailable;
}

char comReadByte(){
	char *c = &buffer[writeBuffPtr];
	buffer[writeBuffPtr] = btGetChar();
	Serial.println(buffer[writeBuffPtr]);
	//check if the command is fully received
	if(buffer[writeBuffPtr]==FINAL_BYTE){
		//Serial.println("Achou final byte");
		comCommandRcvFlag++;		
	}else{
		Serial.println("NFB");
	}
	writeBuffPtr++;
	if(writeBuffPtr >= BUFFER_MAX_SIZE){
		writeBuffPtr = 0;
	}
	return *c;
} 

uint8_t comGetCommandRcvFlag(){
	return comCommandRcvFlag;
}
bool comGetExecCommFlag(){
	return execCommFlag;
}

 static char btGetChar(){
 	char rcvChar;
 #ifdef BT_SOFTSERIAL
 	rcvChar = bluetooth.read();
 #else
 	rcvChar = (char)Serial.read();
 #endif
 	return rcvChar;
 }

void comSetExecFlag(){
	execCommFlag = true;
}

void comExecuteCommand(){
  
}

