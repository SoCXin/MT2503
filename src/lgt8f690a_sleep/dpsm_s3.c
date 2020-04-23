//----------------------------------------------------
// LGT8F690A test cases
// Case name : sleep mode test
// 	decription:
//		dpsm = 3, power/off mode
//		bring to sleep by low input on RC0
//		wakeup by level change on RA2
//		
//----------------------------------------------------
#include "lgt8f690a.h"


int main(void)
{
	// MCUCR
	// disable external reset
	//MCUCR = 0x55;
	//MCUCR |= 0x01;

	// CLOCK
	OSCTUNE = 0x91;

	// enable all pullup of PORTC
	WPUC = 0xff;
	WPUA = 0xfe;
	WPUB = 0xff;

	// global pullup enable
	nRABPU = 0;

	// enable deep sleep mode
	DPSM1 = 1;
	// disable LVRE (for test chip only)
	LVRE = 0;

	// set RA0 to output
	TRISA0 = 0;

	// enable RA2 level change function
	// for wakeup purpose
	IOCA2 = 1;

	while(1)
	{
		LATA0 = ~LATA0;
		NOP(); NOP();

		if(RB0 == 0) 
		{
			SLEEP();
		}

		NOP(); NOP();
	}
}