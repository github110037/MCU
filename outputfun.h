#ifndef __OUTPUTFUN_H__
#define __OUTPUTFUN_H__

void delayMs(unsigned int t);
void nixieSingle(unsigned char Location,unsigned char number);
void nixieGroup(unsigned char number[8],unsigned int time);
void timerInit(unsigned char timer, unsigned char mode, unsigned char delay);
void UARTInit();
void UARTSendByte(unsigned char Byte);
	
#endif