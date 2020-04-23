//-----------------------------------
// LGT8P690A TWI Control
//	Descripter:
//------------------------------------

#include "lgt8f690a.h"
#include "twi.h"
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

// hardware interrupt service
void interrupt hisr(void)
{
	asm("nop");
}

int main()
{
	unsigned char i;
	unsigned char reg_addr;

#if 1
	if(cfw_valid())
		OSCTUNE = cfw_rctrim();
	else
		OSCTUNE = 0x92;
#endif

	OSCCONbits.IRCF = 0x6;

	MCUCR = 0x55;
	MCUCR = 0xC6;

	usart_init();

	twi_init();

// receive 8 byte data from master, local slave addrress is 0x50
// 接收8个字节的数据，初始寄存器地址为reg_addr
	reg_addr = twi_slave_recv(0x50, recv_buf, 0x8);

	for(i=0; i < 8; i++) { send_buf[i+reg_addr] = recv_buf[i]; }

// send 8 byte data to master
// 发送8个字节的数据至主机
	reg_addr = twi_slave_recv(0x50, recv_buf, 0x0);
	twi_slave_send(0x50, reg_addr, send_buf, 0x8);

	usart_send(8, send_buf);

	TWIEN = 0x0;

	// enable interrupt
	// no interrupt for TWI

	PEIE = 0x1;
	GIE = 0x0;

	while(1);
}

