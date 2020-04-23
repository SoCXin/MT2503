//-----------------------------------------
// SPI Controller
//	SPI Interface
//	SPCK @ RA4, MOSI @ RA5, MISO @ RB0, SPSS @ RA6
//	Master Write and Read Operation
//-----------------------------------------

#include "lgt8f690a.h"
#include "spi.h"

void spi_init_mst()
{
	RA6 = 0x1;
	RA4 = 0x0;
	RA5 = 0x0;
	RB0 = 0x0;
	// Set direction for Input/Output
	TRISA4 = 0x0;		// spck
	TRISA5 = 0x0;		// mosi 
	TRISA6 = 0x0; 		// spcs 
	TRISB0 = 0x1;		// miso
	// SPI Configuration 
	SPSR = 0x00;
	SPCR = 0x11;
	SPCR = 0x51;
}

void spi_exch(unsigned char length, unsigned char *rxbuf, unsigned char *txbuf)
{
	unsigned char i;
	unsigned char tmp;

// clear tx & rx buffer status 清除TX和RX缓冲器状态
	SPFR = 0x44;	

// dummy read SPDR for 3times first @2T/4T, no dummy read @1T
// 在2T或4T指令模式下，预先读取SPDR三次，以便内核得到正确的接收数据，1T模式下不需要
	if(L_MCUCRbits.TCYC > 0x0)	
	{
		tmp = SPDR;tmp = SPDR;tmp = SPDR; 
	}

// fill tx buffer as many as possible，no more than 4
// 尽可能多地填充TX缓冲器，最多4个
	for(i=0; ((i<length) && (i < 4)); i++) { SPDR = *txbuf++; }	

	while(length > 4)
	{
// get rx and fill tx when tx buffer is not full
// 当TX缓冲器处于非满状态时，读取SPDR得到接收数据，并再次填充TX缓冲器
		if (!(SPFR & (1 << 3))) {*rxbuf++ = SPDR; length--; SPDR = *txbuf++;}	
	}

	while(length > 0)
	{
// get rx when tx buffer is empty
// 当TX缓冲器处于空状态时，读取SPDR得到接收数据
		if ((SPFR & (1 << 2))) {*rxbuf++ = SPDR; length--;}	
	}
}



