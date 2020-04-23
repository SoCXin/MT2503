#include "lgt8f690a.h"
#include <stdint.h>
#include "usart.h"
#include "misc.h"

#define _XTAL_FREQ 4000000

void EEW(uint8_t type, uint16_t address, uint8_t data)
{
    volatile uint8_t btmp;

    EEPGD = type;  // E2PROM or Program
    EEADRL = address & 0xff;
    EEADRH = (address >> 8) & 0xff;
    EEDAT = data;
    
	// enable write
    WREN = 1;

    btmp = INTCON;
    GIE = 0;
    //while(GIE == 1);

    EECON2 = 0x55;
    EECON2 = 0xaa;
    EEWR = 1; // set WR bit to begin write

    WREN = 0;
    INTCON = btmp;
}

// read can be start from EEPROM/FLASH
uint16_t EER(uint8_t type, uint16_t address)
{
    EEADRL = address & 0xff;
    EEADRH = (address >> 8) & 0xff;

    EEPGD = type;  // select EEPROM or program memory
    EERD = 1;

    return (EEDATH << 8) | EEDAT;
}

int main()
{
	uint8_t i;
	uint8_t btmp, bval;

#if 1	// in case of chip not trimmed
	if(cfw_valid())
	{
		OSCTUNE = cfw_rctrim();
	}
	else
	{
		OSCTUNE = 0x92;
	}
#endif

	// 19200bps @8bit @1stop @noparity
	usart_init();

	// (optional)
	// enable clock for EE/programming
	//EECKE = 1;
	
	// data and direction of RA0
	TRISA0 = 0;
	LATA0 = 0;

	btmp = 0;

	while(1) {
		for(i = 0; i < 128; i++) {
			EEW(E2P, i, btmp);
			__delay_us(10);
			LATA0 = ~LATA0;
		}
	
		btmp++;
		LATA0 = 1;
	
		for(i = 0; i < 128; i++)
		{
			bval = EER(E2P, i);
			usart_putc(bval);
	
			__delay_ms(100);
		}

	}


	while(1) {
#if 0
		// read and echo data in information block
		for(i = 0; i < 16; i++)
		{
			wtmp = EER(MTP, 0x1000+i);
			usart_putc(wtmp >> 8);
			usart_putc(wtmp & 0xff);
			usart_putc(0x55);
			__delay_ms(100);
		}
#endif
	}
}
