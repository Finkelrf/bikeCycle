#include "com.h"


#define BUFFER_MAX_SIZE 60
#define FINAL_BYTE 'p'

#define NUMBER_OF_CMDS 2
#define END_CMD_CHAR ':'

#define CMD_DATA_BUFF 15

//circular buffer to store rcvd bytes
char buffer[BUFFER_MAX_SIZE] = {'\0'};
uint16_t writeBuffPtr = 0;
uint16_t readBuffPtr = 0;

//flags
uint8_t comCommandRcvFlag = 0;
bool execCommFlag = false;



struct cmdInfo{
	int id;	
	char data[CMD_DATA_BUFF];
};

struct cmdInfo cmdInfo = {.id = -1};


#ifdef BT_SOFTSERIAL
int bluetoothTx = 12;
int bluetoothRx = 13;
SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);
#endif

char cmds[][15] = {
	"SET_DISPLAY:",
	"RIGHT:",//turn right
	"LEFT:",//turn left
	"RDB1:",//take the 1st exit in the round about
	"RDB2:",//take the 2nd exit in the round about
	"RDB3:",//take the 3th exit in the round about
	"RDB4:",//take the 4th exit in the round about
	"STRAIGHT:",//keep going straight
	"BACK:", //turn around
	"DEMO:",
	"PWM:",
	"ARROW_POS:",
	"DISTANCE",
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
static void clrCmdData();

static void clrCmdData(){
	int i;
	for(i=0;i<CMD_DATA_BUFF;i++){
		cmdInfo.data[i] = '\0';
	}
}

bool comExecuteCmd(){
	//Serial.println("exec cmd");
	switch(cmdInfo.id){
		case 0:														 //SET_DISPLAY
			displaySetInfo(cmdInfo.data);
			execCommFlag = false;
			//Serial.println("set display");
			cmdInfo.id = -1;
			clrCmdData();
			return true;
			break;

		case 1:														//RIGHT
			ledCtrlShowNavRight(atoi(cmdInfo.data));				
			execCommFlag = false;
			//Serial.println("RIGHT");
			cmdInfo.id = -1;
			clrCmdData();
			return true;
			break;

		case 2:														//LEFT
			ledCtrlShowNavLeft(atoi(cmdInfo.data));	
			execCommFlag = false;
			//Serial.println("LEFT");
			cmdInfo.id = -1;
			clrCmdData();
			return true;
			break;

		case 3:														//RDB1
			ledCtrlShowNavRDB1(atoi(cmdInfo.data));	
			execCommFlag = false;
			//Serial.println("RDB1");
			cmdInfo.id = -1;
			clrCmdData();
			return true;
			break;

		case 4:														//RDB2
			ledCtrlShowNavRDB2(atoi(cmdInfo.data));
			execCommFlag = false;
			//Serial.println("RDB2");
			cmdInfo.id = -1;
			clrCmdData();
			return true;
			break;

		case 5:														//RDB3
			ledCtrlShowNavRDB3(atoi(cmdInfo.data));
			execCommFlag = false;
			//Serial.println("RDB3");
			cmdInfo.id = -1;
			clrCmdData();
			return true;
			break;

		case 6:														//RDB4
			ledCtrlShowNavRDB4(atoi(cmdInfo.data));
			execCommFlag = false;
			//Serial.println("RDB4");
			cmdInfo.id = -1;
			clrCmdData();
			return true;
			break;

		case 7:														//STRAIGHT
			ledCtrlShowNavStraight();
			execCommFlag = false;
			//Serial.println("STRAIGHT");
			cmdInfo.id = -1;
			clrCmdData();
			return true;
			break;

		case 8:														//BACK
			ledCtrlShowNavBack();
			execCommFlag = false;
			//Serial.println("BACK");
			cmdInfo.id = -1;
			clrCmdData();
			return true;
			break;

		case 9:														//BACK
			startDemo();
			execCommFlag = false;
			//Serial.println("BACK");
			cmdInfo.id = -1;
			clrCmdData();
			return true;
			break;

		case 10:														//BACK
			ledCtrlSetBright(atoi(cmdInfo.data));
			execCommFlag = false;
			//Serial.println("BACK");
			cmdInfo.id = -1;
			clrCmdData();
			return true;
			break;

		case 11:														//SET ARROW ANGLE
			pointerCtrl_move(atoi(cmdInfo.data));
			execCommFlag = false;
			//Serial.println("BACK");
			cmdInfo.id = -1;
			clrCmdData();
			return true;
			break;

		case 12:														//SET DISTANCE
			pointerCtrl_setDistance(atoi(cmdInfo.data));
			execCommFlag = false;
			//Serial.println("BACK");
			cmdInfo.id = -1;
			clrCmdData();
			return true;
			break;

		default:
			TurnOffAllLeds();
			execCommFlag = false;
			//Serial.println("FAIL");
			cmdInfo.id = -1;
			clrCmdData();
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
	//Serial.print("-> ");
	//Serial.println(stateInt.cmd[stateInt.auxReadBuff]);
	//Serial.print("cmp ");
	//Serial.println(cmds[stateInt.cmdsIterator][stateInt.auxReadCmd]);
	if((stateInt.cmd[stateInt.auxReadBuff] != cmds[stateInt.cmdsIterator][stateInt.auxReadCmd])&&!stateInt.cmdFound){ //it is not the same letter
		stateInt.cmdsIterator++;
		stateInt.auxReadCmd=0;
		stateInt.auxReadBuff = 0;
	}else{															//it is the same letter, it means the it could be the command
		if(!stateInt.cmdFound){
			if(stateInt.cmd[stateInt.auxReadBuff] == END_CMD_CHAR){	//check if is end of command char
			//Serial.print("cmd ");
			//Serial.print(stateInt.cmdsIterator);
			//Serial.println(" encontrado");
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
  	Serial.begin(9600); //if the BT module is in default 
	Serial.print("AT+BAUD5");
	delay(200);
	Serial.begin(19200);
	Serial.println();
	Serial.println("Smart Bike");

#ifdef BT_SOFTSERIAL
	bluetooth.begin(9600);
	bluetooth.print("AT+BAUD5");
	delay(200);
	bluetooth.begin(19200);
	Serial.println("Software Serial");
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


