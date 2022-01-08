#include <REGX52.H>
void delayMs(unsigned int t)
{
	unsigned char i, j;
	while(t--)
	{
		i = 2;
		j = 239;
		do
		{
			while (--j);
		} while (--i);
	}
}

unsigned char nixieTable[]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};

void nixieSingle(unsigned char Location,unsigned char number)
{
	switch(Location)		//位码输出
	{
		case 1:P2_4=1;P2_3=1;P2_2=1;break;
		case 2:P2_4=1;P2_3=1;P2_2=0;break;
		case 3:P2_4=1;P2_3=0;P2_2=1;break;
		case 4:P2_4=1;P2_3=0;P2_2=0;break;
		case 5:P2_4=0;P2_3=1;P2_2=1;break;
		case 6:P2_4=0;P2_3=1;P2_2=0;break;
		case 7:P2_4=0;P2_3=0;P2_2=1;break;
		case 8:P2_4=0;P2_3=0;P2_2=0;break;
	}
	P0=nixieTable[number];	//段码输出
}

/**
  * @brief  七段数码管全显示
  * @param  number 一组数码管的8个数字
  * @param  time 延时时长
  * @retval 无
  */
void nixieGroup(unsigned char number[8],unsigned int time)
{
	unsigned char i;
	while (1)
	{
		i=1;
		while(i<9)
		{
			nixieSingle(i,number[i-1]);
			delayMs(time);
			P0=0;
			++i;
		}
	}
}

/**
  * @brief  计时器初始化,每隔1ms产生一个中断
  * @param  timer 定时器选择，0对应定时器0，否则为定时器1
  * @param  mode 定时器模式选择，0对应16位模式，1对应8位自动重装模式
  * @param  delay 16位模式：代表定时器延，单位ms，范围[1,65]
  *               8位重装模式：代表8位重装的数据
  * @retval 无
  */
void timerInit(unsigned char timer, unsigned char mode, unsigned char delay)
{
   unsigned char high,low;
   unsigned int i;
   i=1000*delay;
   low = (-i) & 0x000000ff;
   high = ((-i)>>8) & 0x000000ff;
    if (timer)
    {
        switch (mode)//timer1 16位模式
        {
        case 0:
            TMOD &= 0x0f;
            TMOD |= 0x10;
            TL1 = low;
            TH1 = high;
            TF1 = 0;
            TR1 = 1;
				    ET1 = 1;
						PT1 = 0;
            break;
        default://timer1 8位重装模式
            TMOD &= 0x0f;
            TMOD |= 0x20;
            TL1 = delay;
            TL1 = delay;
            TF1 = 0;
            TR1 = 1;         
            break;
        }
    }
    else
    {
        switch (mode)//timer0 16位模式
        {
        case 0:
            TMOD &= 0xf0;
            TMOD |= 0x01;
            TL0 = low;
            TH0 = high;
            TF0 = 0;
            TR0 = 1;
            ET0 = 1;
						PT0 = 0;
            break;
        default://timer0 8位重装模式
            TMOD &= 0x0f;
            TMOD |= 0x20;
            TH0 = delay;
            TL0 = delay;
            TF0 = 0;
            TR0 = 1;            
            break;
        }
    }
    EA = 1;
}

/*
void Timer0_Routine() interrupt 1
{
	static unsigned int T0Count;
	TL0 = 0x18;		//设置定时初值
	TH0 = 0xFC;		//设置定时初值
	T0Count++;
	if(T0Count>=1000)
	{
		T0Count=0;
		P2_1 = ~P2_1;
	}
}

void Timer1_Routine() interrupt 3
{
	static unsigned int T0Count;
	TL1 = 0x18;		//设置定时初值
	TH1 = 0xFC;		//设置定时初值
	T0Count++;
	if(T0Count>=1000)
	{
		T0Count=0;
		P2_3 = ~P2_3;
	}
}
*/

/**
  * @brief  串口初始化，4800bps@12.000MHz
  * @param  无
  * @retval 无
  */
void UARTInit()
{
	SCON=0x50;
	PCON |= 0x80;
	TMOD &= 0x0F;		//设置定时器模式
	TMOD |= 0x20;		//设置定时器模式
	TL1 = 0xF3;		//设定定时初值
	TH1 = 0xF3;		//设定定时器重装值
	ET1 = 0;		//禁止定时器1中断
	TR1 = 1;		//启动定时器1
	EA=1;
	ES=1;
}

/**
  * @brief  串口发送一个字节数据
  * @param  Byte 要发送的一个字节数据
  * @retval 无
  */
void UARTSendByte(unsigned char Byte)
{
	SBUF=Byte;
	while(TI==0);
	TI=0;
}

/*串口中断函数模板
void UART_Routine() interrupt 4
{
	if(RI==1)
	{
		
		RI=0;
	}
}
*/