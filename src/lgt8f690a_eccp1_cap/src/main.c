//-----------------------------------
// LGT8P690A ECCP1 Control
//	Descripter:
//------------------------------------

#include "lgt8f690a.h"
#include "eccp1.h"
#include "usart.h"
#include "misc.h"

// configuration word settings
// 	16MHZ /4 = 	4MHz @ 4T

unsigned char send_buf[36]=
{
0x0f, 0x1e, 0x2d, 0x3c,
0x4b, 0x5a, 0x69, 0x78,
0x87, 0x96, 0xa5, 0xb4,
0xc3, 0xd2, 0xe1, 0xf0,
0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0x55,
0xff, 0xff, 0xff, 0xff
};

unsigned char recv_buf[36]=
{
0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff,
0x00, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xaa,
0xff, 0xff, 0xff, 0xff
};

unsigned char ecp1_f1;

// hardware interrupt service
void interrupt hisr(void)
{
	NOP();

	if(CCP1IE && CCP1IF) {
		if(ECP1_IF1) 
		{
			recv_buf[6] = ECP1R1H; 
			recv_buf[7] = ECP1R1L; 
			ecp1_f1 = 0x1;
		}
		PIR1 = 0xfb;
	}
}

int main()
{
	unsigned char ecp1_fr;
	unsigned int  u16tmp0;
	unsigned int  u16tmp1;
	unsigned int  u16tmp;

#if 1
	if(cfw_valid())
		OSCTUNE = cfw_rctrim();
	else
		OSCTUNE = 0x92;
#endif

	OSCCONbits.IRCF = 0x6;

	MCUCR = 0x55;
	MCUCR = 0xC0;

	usart_init();

	ecp1_f1 = 0x0;
	ecp1_fr = 0x0;
	u16tmp0 = 0x0;
	u16tmp1 = 0x0;

	eccp1_cap_init();

	// enable interrupt
	CCP1IE = 0x1;
	PEIE = 0x1;
	GIE = 0x1;

	while(1)
	{
		if (ecp1_f1 == 0x1)
		{
			ecp1_fr ++;
			if (ecp1_fr == 0x2) CCP1IE = 0x0;
			u16tmp1 = u16tmp0;							// 保存前次capture时的计数值
			u16tmp0 = (recv_buf[6] << 8) + recv_buf[7];	// 保存当前capture时的计数值
			ecp1_f1 = 0x0;
		}
		if (ecp1_fr == 0x2)					// 连续采样两次
		{
			CCP1IE = 0x0;
			ECP1_CE1 = 0x0;
			ecp1_fr = 0x0;
			u16tmp = u16tmp0 - u16tmp1;		// 计算两次capture之间的差值
			recv_buf[0] = u16tmp >> 8;
			recv_buf[1] = u16tmp & 0xff;
			usart_send(3, recv_buf);
			CCP1IF = 0x0;
			CCP1IE = 0x1;
			ECP1_CE1 = 0x1;
		}
	}
}

