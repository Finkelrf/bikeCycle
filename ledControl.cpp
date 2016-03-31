#include "ledControl.h"

#define DATA_PIN 11
#define LATCH_PIN 8
#define CLOCK_PIN 13
#define PWM_PIN 6 

#define LONG_DIST	20
#define SHORT_DIST	10

const uint8_t numShiftReg = (uint8_t)(((NUMBER_OF_LEDS*3)/8)+1);
uint8_t data[numShiftReg] = {0}; //data of the shift registers

static void SetRGB(int led, uint16_t r, uint16_t b, uint16_t g);
static void SendDataToShftRegs();
static void ClearAllShiftRegData();


void ledCtrlSetBright(int percentage){
	if(percentage<0 || percentage>100){
		return;
	}else{
		int aux = map(percentage,0,100,0,255);
		analogWrite(PWM_PIN,aux);
	}
}

void ledCtrlInit(){
	//Serial.print("num sr ");
	//Serial.println(numShiftReg);
	pinMode(DATA_PIN,OUTPUT);
	pinMode(CLOCK_PIN,OUTPUT);
	pinMode(LATCH_PIN,OUTPUT);
	pinMode(PWM_PIN,OUTPUT);
	ledCtrlSetBright(100);

	int i;
	for (i = 0; i < NUMBER_OF_LEDS; ++i)
	{
		data[i] = 0;
	}
	TurnOffAllLeds();
}

void ledCtrlShowNavStraight(){
	//Serial.println("EIta");
	//set the leds in the forward position as green 
	ClearAllShiftRegData();
#ifdef BASIC_LED_MODE
	SetRGB(1,0,1023,0);
	//Serial.println("basic led straight");
#else
	//Serial.println("full led straight");
	int i;
	for (int i = 2; i < 10; ++i)
	{
		SetRGB(i,0,1023,0);
	}
#endif

}

void ledCtrlShowNavRight(int dist){
	//Serial.print("Dist ");
	//Serial.println(dist);
	ClearAllShiftRegData();
#ifdef BASIC_LED_MODE 
	if(dist<SHORT_DIST){
		SetRGB(0,0,1023,0);
	}else if(dist<LONG_DIST){
		SetRGB(0,0,1023,1023);
	}else{
		SetRGB(0,0,0,1023);	
	}
	
#else
	//TODO FULL LED MODE
#endif
}
void ledCtrlShowNavLeft(int dist){
	ClearAllShiftRegData();
#ifdef BASIC_LED_MODE
	if(dist<SHORT_DIST){
		SetRGB(2,0,1023,0);
	}else if(dist<LONG_DIST){
		SetRGB(2,0,1023,1023);
	}else{
		SetRGB(2,0,0,1023);	
	}
	
#else
	//TODO FULL LED MODE
#endif
}
void ledCtrlShowNavRDB1(int dist){
	ClearAllShiftRegData();
#ifdef BASIC_LED_MODE
	if(dist<SHORT_DIST){
		SetRGB(0,0,1023,0);
	}else if(dist<LONG_DIST){
		SetRGB(0,0,1023,1023);
	}else{
		SetRGB(0,0,0,1023);	
	}
	SetRGB(1,1023,0,0);
	SetRGB(2,1023,0,0);
	SetRGB(3,1023,0,0);
#else
	//TODO FULL LED MODE
#endif
}
void ledCtrlShowNavRDB2(int dist){
	ClearAllShiftRegData();
#ifdef BASIC_LED_MODE
	if(dist<SHORT_DIST){
		SetRGB(1,0,1023,0);
	}else if(dist<LONG_DIST){
		SetRGB(1,0,1023,1023);
	}else{
		SetRGB(1,0,0,1023);	
	}
	SetRGB(0,1023,0,0);
	SetRGB(2,1023,0,0);
	SetRGB(3,1023,0,0);
#else
	//TODO FULL LED MODE
#endif
}
void ledCtrlShowNavRDB3(int dist){
	ClearAllShiftRegData();
#ifdef BASIC_LED_MODE
	if(dist<SHORT_DIST){
		SetRGB(2,0,1023,0);
	}else if(dist<LONG_DIST){
		SetRGB(2,0,1023,1023);
	}else{
		SetRGB(2,0,0,1023);	
	}
	SetRGB(0,1023,0,0);
	SetRGB(1,1023,0,0);
	SetRGB(3,1023,0,0);
#else
	//TODO FULL LED MODE
#endif
}
void ledCtrlShowNavRDB4(int dist){
	ClearAllShiftRegData();
#ifdef BASIC_LED_MODE
	if(dist<SHORT_DIST){
		SetRGB(3,0,1023,0);
	}else if(dist<LONG_DIST){
		SetRGB(3,0,1023,1023);
	}else{
		SetRGB(3,0,0,1023);	
	}
	SetRGB(0,1023,0,0);
	SetRGB(1,1023,0,0);
	SetRGB(2,1023,0,0);
#else
	//TODO FULL LED MODE
#endif
}
void ledCtrlShowNavBack(){
	ClearAllShiftRegData();
#ifdef BASIC_LED_MODE
	SetRGB(1,1023,0,0);
	SetRGB(2,1023,0,0);
	SetRGB(3,1023,0,0);
	SetRGB(4,1023,0,0);
#else
	//TODO FULL LED MODE
#endif
}

static void SetRGB(int led, uint16_t r, uint16_t b, uint16_t g){
	uint16_t ledValue[3];
	ledValue[0]=map(r,0,1023,0,MAX_LED_BRIGHTNESS);
	ledValue[1]=map(g,0,1023,0,MAX_LED_BRIGHTNESS);
	ledValue[2]=map(b,0,1023,0,MAX_LED_BRIGHTNESS);
	//Serial.print("r ");
	//Serial.print(ledValue[0]);
	//Serial.print(" g ");
	//Serial.print(ledValue[1]);
	//Serial.print(" b ");
	//Serial.println(ledValue[2]);


	int i;
	for (i = 0; i < 3; ++i)
	{
		int iReg = (led*3+i)/8;
		int desl = (led*3+i)%8;
		//Serial.print("iReg ");
		//Serial.print(iReg);
		//Serial.print(" desl ");
		//Serial.println(desl);
		if(ledValue[i] <= 1){
			//Serial.print("antes ");
			//Serial.println(data[iReg]);
			data[iReg] = data[iReg]|1<<desl;
			//Serial.print("depois ");
			//Serial.println(data[iReg]);
		}else{
			//Serial.print("antes ");
			//Serial.println(data[iReg]);
			data[iReg] = data[iReg] & ~(1<<desl);
			//Serial.print("depois ");
			//Serial.println(data[iReg]);
		}
	}
	SendDataToShftRegs();
}

static void SendDataToShftRegs(){
	digitalWrite(LATCH_PIN, LOW);
	
	int i;
	for (i = 0; i < numShiftReg; ++i)
	{
		shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, data[numShiftReg-i-1]);
	}
	digitalWrite(LATCH_PIN, HIGH);
}


void TurnOffAllLeds(){
	digitalWrite(LATCH_PIN, LOW);
	
	int i;
	for (i = 0; i < numShiftReg; ++i)
	{
		shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, B11111111);
	}
	digitalWrite(LATCH_PIN, HIGH);
}

static void ClearAllShiftRegData(){
	int i;
	for (i = 0; i < numShiftReg; ++i)
	{
		data[i] = B11111111;
	}
}
