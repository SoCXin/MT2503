//-----------------------------------
// LGT8P690A TWI Control
//	Descripter:
//------------------------------------

#include "lgt8f690a.h"
#include "eccp2.h"
#include "usart.h"
#include "misc.h"

// configuration word settings
// 1. disalbe WDT
// 2. disable system clock output
// 3. select 1T core cycle
//__L_CONFIG(CF1_ON & WDTE_OFF & OSCO_OFF);
//__L_CONFIG(CF2_ON & TCYC_1T);

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

unsigned char ecp2_f4;

// hardware interrupt service
void interrupt hisr(void)
{
	//asm("nop");
	if(ECP2IE && ECP2IF) {
		if(ECP2_IF4) {recv_buf[6] = ECP2R4H; recv_buf[7] = ECP2R4L; ecp2_f4 = 0x1;}
		PIR1 = 0x7f;
	}
}

int main()
{
	unsigned char ecp2_fr;
	unsigned int  u16tmp0;
	unsigned int  u16tmp1;
	unsigned int  u16tmp;

#if 1	// load trimming for HFINTOSC
	if(cfw_valid())
		OSCTUNE = cfw_rctrim();
	else
		OSCTUNE = 0x92;
#endif

	OSCCONbits.IRCF = 0x6;

	MCUCR = 0x55;
	MCUCR = 0xC0;

	usart_init();

	ecp2_f4 = 0x0;
	ecp2_fr = 0x0;
	u16tmp0 = 0x0;
	u16tmp1 = 0x0;

	eccp2_cap_init();

	// enable interrupt
	ECP2IE = 0x1;
	PEIE = 0x1;
	GIE = 0x1;

	while(1)
	{
		if (ecp2_f4 == 0x1)
		{
			ecp2_fr ++;
			if (ecp2_fr == 0x2) ECP2IE = 0x0;
			u16tmp1 = u16tmp0;							// 保存前次capture时的计数值
			u16tmp0 = (recv_buf[6] << 8) + recv_buf[7];	// 保存当前capture时的计数值
			ecp2_f4 = 0x0;
		}
		if (ecp2_fr == 0x2)					// 连续采样两次
		{
			ECP2IE = 0x0;
			ECP2_CE4 = 0x0;
			ecp2_fr = 0x0;
			u16tmp = u16tmp0 - u16tmp1;		// 计算两次capture之间的差值
			recv_buf[0] = u16tmp >> 8;
			recv_buf[1] = u16tmp & 0xff;
			usart_send(3, recv_buf);
			ECP2IF = 0x0;
			ECP2IE = 0x1;
			ECP2_CE4 = 0x1;
		}
	}
}

