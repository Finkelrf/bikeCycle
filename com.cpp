#include "com.h"


#define BUFFER_MAX_SIZE 30
#define FINAL_BYTE '='

#define NUMBER_OF_CMDS 2
#define END_CMD_CHAR '='

char buffer[BUFFER_MAX_SIZE] = {'\0'};
uint16_t writeBuffPtr = 0;
uint8_t comCommandRcvFlag = 0;
bool execCommFlag = false;

uint16_t readBuffPtr = 0;
int cmdId = -1;

#ifdef BT_SOFTSERIAL
int bluetoothTx = 12;
int bluetoothRx = 13;
SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);
#endif

char cmds[][15] = {
	"SET_DISPLAY=",
	"SAY_HI="
};

static char btGetChar();

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
		comCommandRcvFlag++;
		Serial.println("Achou final byte");
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

bool comInterpreteCmd(){
	Serial.println("entrou no interpretador");
	char *cmd = &buffer[readBuffPtr];
	comCommandRcvFlag--;
	int i;
	bool foundIt = false;
	for(i=0;i<NUMBER_OF_CMDS;i++){
		printf("%d\n",i );
		bool check = true;
		while(check){
			//compare if it is the same letter
			if(cmd[readBuffPtr] != cmds[i][readBuffPtr]){
				check = false;
			}else{
				//check if it end of comd
				if(cmd[readBuffPtr] == END_CMD_CHAR){
					foundIt = true;
					check = false;
				}else{
					readBuffPtr++;
				}
			}
		}
		if(foundIt){
			cmdId = i;
			return true;
		}
	}
	Serial.println("nao achou cmd");
	cmdId = -1;
	return false;
}
void comSetExecFlag(){
	execCommFlag = true;
}
