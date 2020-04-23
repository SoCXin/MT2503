//-----------------------------------
// LGT8PE663A TWI Control
//	Descripter:
//------------------------------------

#include "lgt8f690a.h"
#include "usart.h"
#include "misc.h"

// configuration word settings
// default: 16M/4 = 4M @ 4T

// hardware interrupt service
void interrupt hisr(void)
{
	asm("nop");
}

int main()
{
#if 1	// in case of not trimmed

	if(cfw_valid())
		OSCTUNE = cfw_rctrim();
	else
		OSCTUNE = 0x92;
	
#endif

	usart_init();

	usart_puts((char *)"hello from lgt8f690!\r\n");

	// enable interrupt
	//PEIE = 0x1;
	//GIE = 0x0;

	while(1)
	{
		usart_putc(usart_getc());
	}
}

