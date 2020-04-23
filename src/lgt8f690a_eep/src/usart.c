//-----------------------------------------
// USART Controller
//	USART Interface
//	UTX @ RB5, URX @ RB6
//	Master Write and Read Operation
//-----------------------------------------

#include "lgt8f690a.h"
#include "usart.h"

// XTAL_FREQ = 4000000
// br = 4000000/(16(SPBRG + 1)) = 19230(bps)

void usart_init()
{
	RB5 = 0x1;
	RB6 = 0x1;
	// Set RC6 & RC7 direction for Input
	TRISB5 = 0x0;		// tx, low for output
	TRISB6 = 0x1; 		// rx, high for input
	// USART Configuration 
	SPBRGH = 0x0;
	SPBRG  = 0xc;
	//SCKP = 0x1;
	//RCSTA  = 0x10;
	TXSTA  = 0x26; // asy
	RCSTA  = 0x90; // asy
	//TXSTA  = 0xb4; // syn master  // 0x24; syn slave
	//RCSTA  = 0x80; // syn
}

void usart_putc(unsigned char ch)
{
	TXIF = 0x0;

	while ( TRMT == 0x0 );
	TXREG = ch;

	while ( TXIF == 0x0 );
	TXIF = 0x0;
}


void usart_send(unsigned char length, unsigned char *txbuf)
{
	unsigned char i;

	TXIF = 0x0;

	for (i=length; i>0; i--) {
		while ( TRMT == 0x0 );
		TXREG = txbuf[length-i];
	}

	while ( TXIF == 0x0 );
	TXIF = 0x0;
}

void usart_recv(unsigned char length, unsigned char *rxbuf)
{
	unsigned char i;

	RCIF = 0x0;

	for (i=length; i>0; i--) {
		if (i==1) {SREN = 0x1; CREN = 0x0;}
		while ( RCIF == 0x0 );
		RCIF = 0x0;
		rxbuf[length-i] = RCREG;
	}

	RCIF = 0x0;
}
