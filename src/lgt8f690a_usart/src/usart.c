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

	//BRG16 = 1;
	//BRGH = 1;

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

char usart_getc()
{

	RCIF = 0x0;

	CREN = 0x1;
	while ( RCIF == 0x0 );
	RCIF = 0x0;

	return RCREG;
}

void usart_puts(char *txbuf)
{
	TXIF = 0x0;

	while(*txbuf != '\0') {
		while ( TRMT == 0x0 );
		TXREG = *txbuf++;
	}

	while ( TXIF == 0x0 );
	TXIF = 0x0;
}
