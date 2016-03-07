#include "com.h"


#define BUFFER_MAX_SIZE 30
#define FINAL_BYTE 'p'

#define NUMBER_OF_CMDS 2
#define END_CMD_CHAR ':'

//circular buffer to store rcvd bytes
char buffer[BUFFER_MAX_SIZE] = {'\0'};
uint16_t writeBuffPtr = 0;
uint16_t readBuffPtr = 0;

//flags
uint8_t comCommandRcvFlag = 0;
bool execCommFlag = false;



struct cmdInfo{
	int id;	
	char data[15];
};

struct cmdInfo cmdInfo = {.id = -1};


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
	int auxReadBuff;
	int auxReadCmd;
};
struct intComPreempState stateInt = {.empty = true};

static char btGetChar();

bool comExecuteCmd(){
	//Serial.println("exec cmd");
	switch(cmdInfo.id){
		case 0:
			displaySetInfo(cmdInfo.data);
			execCommFlag = false;
			//Serial.println("set display");
			cmdInfo.id = -1;
			return true;
			break;

		case 1:

			break;

		default:
			cmdInfo.id = -1;
			return false;
			break;
	}
}

bool comInterpreteCmdPreemptive(){
	bool ret = false;
	if(stateInt.empty){							//first iteration, it must be initialized
		stateInt.cmd = &buffer[readBuffPtr]; //point to the position of the first byte not read from the buffer 
		stateInt.cmdsIterator = 0; //ptr to iterate in the cmd list
		stateInt.empty = false; //flag to sinalize if is the first iteration
		stateInt.cmdFound = false; //flag to sinalize if the END_CMD_CHAR was found
		stateInt.dataIterator = 0;//ptr to write in data
		stateInt.auxReadBuff = 0; //ptr to read the circular buffer
		stateInt.auxReadCmd = 0; //ptr to read the cmd vector 
	}
	//Serial.print("anal --> ");
	//Serial.println(stateInt.cmd[stateInt.auxReadBuff]);
	if((stateInt.cmd[stateInt.auxReadBuff] != cmds[stateInt.cmdsIterator][stateInt.auxReadCmd])&&!stateInt.cmdFound){ //it is not the same letter
		stateInt.cmdsIterator++;
		stateInt.auxReadCmd++;
		stateInt.auxReadBuff = 0;
	}else{															//it is the same letter, it means the it could be the command
		if(!stateInt.cmdFound){
			if(stateInt.cmd[stateInt.auxReadBuff] == END_CMD_CHAR){	//check if is end of command char
			//Serial.println("Char de fim de cmd");
			cmdInfo.id = stateInt.cmdsIterator;
			stateInt.cmdFound = true;
			}else{
				//keep comparing 
			}
			stateInt.auxReadCmd++;
		}else if(stateInt.cmdFound){ //start to copy cmd data
			if(stateInt.cmd[stateInt.auxReadBuff] != FINAL_BYTE){
				//Serial.println("Not final");
				cmdInfo.data[stateInt.dataIterator++] = stateInt.cmd[stateInt.auxReadBuff];
			}else{ //all data has been copied
				comCommandRcvFlag--; //clear flag
				stateInt.empty = true;//
				//Serial.print("cmd data: ");
				//Serial.println(cmdInfo.data);

				//update readBuff
				readBuffPtr +=(stateInt.auxReadBuff+1); //+1 because the FINAL_BYTE was not yet counted

				//TODO teste de variaveis, remover
				//Serial.print(writeBuffPtr);
				//Serial.print(" ");
				//Serial.print(readBuffPtr);
				//Serial.print(" ");
				//Serial.print(comCommandRcvFlag);			
				ret = true;
			}
		}
		//increment circular read Buffer
		stateInt.auxReadBuff = ((stateInt.auxReadBuff+readBuffPtr + 1) % BUFFER_MAX_SIZE)-readBuffPtr;

		//Serial.print("aux: ");
		//Serial.println(stateInt.auxReadBuff);

		//TODO implement a way to handle incorrect msgs
		
	}
	return ret;

}

//deprecated - takes too long to execute and maker the processor restart
bool comInterpreteCmd(){
	//Serial.println("entrou no interpretador");
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
					//Serial.println("Buscando data");
					foundIt = true;
					check = false;
					bool done = false;
					int j = 0;
					while(!done){
						if(cmd[readBuffPtr] != FINAL_BYTE){
							cmdInfo.data[j] = cmd[readBuffPtr];
							j++;
						}else{
							done = true;
							//Serial.println(cmdInfo.data);
						}
					}
				}else{
					readBuffPtr++;
				}
			}
		}
		if(foundIt){
			cmdInfo.id = i;
			return true;
		}
	}
	//Serial.println("nao achou cmd");
	cmdInfo.id = -1;
	return false;
}
void comInit(){
	Serial.begin(38400);
  	//Serial.println("Bike Cycle 1.0");

#ifdef BT_SOFTSERIAL
	bluetooth.begin(115200);
	bluetooth.print("$$$");
	delay(100);
	bluetooth.println("U,9600,N");
	bluetooth.begin(9600);
#endif

	//TODO teste, remover 

	//Serial.print(writeBuffPtr);
	//Serial.print(" ");
	//Serial.print(readBuffPtr);
	//Serial.print(" ");
	//Serial.print(comCommandRcvFlag);

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
	//Serial.print(buffer[writeBuffPtr]);
	//Serial.print(" pos ");
	//Serial.println(writeBuffPtr);
	//check if the command is fully received
	if(buffer[writeBuffPtr]==FINAL_BYTE){
		comCommandRcvFlag++;		
	}
	writeBuffPtr = (writeBuffPtr + 1) % BUFFER_MAX_SIZE;
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

