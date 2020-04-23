//-----------------------------------
// LGT8P690A SPI Control
//	Descripter:
//------------------------------------

#include "lgt8f690a.h"
#include "spi.h"
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

	spi_init_sla();

	SPIEN = 0x1;

// exchange 1 byte data
// 收发1个字节的数据
	spi_exch(1, recv_buf, send_buf);

	//usart_send(1, recv_buf);

	asm("nop");

// exchange 2 byte data
// 收发2个字节的数据
	spi_exch(2, recv_buf, send_buf);

	//usart_send(2, recv_buf);

	asm("nop");

// exchange 3 byte data
// 收发3个字节的数据
	spi_exch(3, recv_buf, send_buf);

	//usart_send(3, recv_buf);

	asm("nop");

// exchange 4 byte data
// 收发4个字节的数据
	spi_exch(4, recv_buf, send_buf);

	//usart_send(4, recv_buf);

	asm("nop");

// exchange 32 byte data
// 收发32个字节的数据
	spi_exch(32, recv_buf, send_buf);

	usart_send(32, recv_buf);

	SPIEN = 0x0;

	// enable interrupt

	// no interrupt for SPI
	// SPI模块没有中断

	PEIE = 0x1;
	GIE = 0x0;

	while(1);
}

