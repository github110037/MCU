#include <REGX52.H>
#include "outputfun.h"

sbit RCK = P3^5;
sbit SCK = P3^6;
sbit SER = P3^4;

void writeByte_74HC595(unsigned char Byte)
{
    unsigned char i;
    for ( i = 0; i < 8; i++)
    {
        SER = Byte & (0x80>>i);
        SCK = 1;
        SCK = 0;
    }
    RCK = 1;
    RCK = 0;
}

void matrixLED_col(unsigned char colunm, unsigned char byte)
{
    writeByte_74HC595(byte);
    P0 = ~(0x80>>colunm);
}

void matrixLED(unsigned char * picture, unsigned char times,unsigned char page)
{
    unsigned char i;
		unsigned char t;
		do
		{
			i=0;
			while (i<8)
			{
					t = (page+i)%32;
					matrixLED_col(i,picture[t]);
					delayMs(1);
					P0 = 0xff;
					i = i+1;
			}
		}while(--times);
}

unsigned char animation[] = {0xFF,0x10,0x10,0x10,0xFF,0x00,0x0E,0x15,0x15,0x15,0x0C,0x00,0xFF,0x01,0x06,0x00,
0xFF,0x01,0x06,0x00,0x0E,0x11,0x11,0x11,0x0E,0x00,0xFD,0x00,0x00,0x00,0x00,0x00};

int main()
{
		unsigned char page=0;
    SCK = 0;
    RCK = 0;
    while (1)
    {
        matrixLED(animation,5,page);
        page=(page+1)%32;   
    }
}
