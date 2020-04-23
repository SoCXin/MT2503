//-----------------------------------------
// USART Controller
//	USART Interface
//	UTX @ RB5, URX @ RB6
//	Master Write and Read Operation
//-----------------------------------------

#include "lgt8f690a.h"
#include "usart.h"

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
	TXSTA  = 0x24; // 0xb4;
	RCSTA  = 0x80;
}

void usart_send_char(unsigned char txdat)
{
	TXIF = 0x0;
	TXEN = 0x1;

	while ( TRMT == 0x0 );
	TXREG = txdat;

	while ( TXIF == 0x0 );
	TXIF = 0x0;

	TXEN = 0x0;
}

unsigned char usart_recv_char(void)
{
	unsigned char rxdat;

	RCIF = 0x0;

	CREN = 0x1;
	while ( RCIF == 0x0 );
	RCIF = 0x0;
	rxdat = RCREG;

	RCIF = 0x0;
	CREN = 0x0;
	return rxdat;
}

void usart_send(unsigned char length, unsigned char *txbuf)
{
	unsigned char i;

	TXIF = 0x0;
	TXEN = 0x1;

	for (i=length; i>0; i--) {
		while ( TRMT == 0x0 );
		TXREG = txbuf[length-i];
	}

	while ( TXIF == 0x0 );
	TXIF = 0x0;

	TXEN = 0x0;
}

void usart_recv(unsigned char length, unsigned char *rxbuf)
{
	unsigned char i;

	RCIF = 0x0;
	CREN = 0x1;

	for (i=length; i>0; i--) {
		while ( RCIF == 0x0 );
		RCIF = 0x0;
		rxbuf[length-i] = RCREG;
	}

	RCIF = 0x0;
	CREN = 0x0;
}

void usart_sendrecv(unsigned char length, unsigned char *rxbuf, unsigned char *txbuf)
{
	unsigned char i;

	RCIF = 0x0;
	TXIF = 0x0;

	TXEN = 0x1;
	CREN = 0x1;

	TXREG = txbuf[0];

	for (i=length; i>0; i--) {

		if ((i>1) && (TRMT == 1)) { TXREG = txbuf[length-i+1];}
		while ( RCIF == 0x0 );
		RCIF = 0x0;
		rxbuf[length-i] = RCREG;
	}

	CREN = 0x0;

	while ( TXIF == 0x0 );
	TXIF = 0x0;
	TXEN = 0x0;
}

