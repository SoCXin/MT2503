#include <stdint.h>
#include "lgt8f690a.h"
#include "misc.h"

#define _XTAL_FREQ 4000000  

#define	 	DAVS_VCC	0
#define		DAVS_AVREF	1
#define		DAVS_IVREF	2

int main()
{
	uint8_t dacv = 0;

	if(cfw_valid())
	{
		OSCTUNE = cfw_rctrim();
	}
	else
	{
		OSCTUNE = 0x92;
	}

	// DAC
	ADCON3 = (DAVS_VCC << 6) |	// DAC source from IVREF 
			 (0 << 5);	// enable internal 1.5V IVREF

	DACON = (1 << 7) |	// enable DAC
			(1 << 6);	// enable DAC output to RA5

	if(cfw_valid())
	{
		// load trimming for 1.5V IVREF
		VRTUNE = cfw_v1trim(); 
	}
	else
	{
		VRTUNE = 0x2a;
	}

	while(1) 
	{
		DACON = (dacv++ | 0xc0);	
		//__delay_us(1);
		//NOP(); NOP();
	}
}
