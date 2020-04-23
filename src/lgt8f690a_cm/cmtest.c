#include <stdint.h>
#include "lgt8f690a.h"

#define _XTAL_FREQ 1000000

#define	ENABLE_ACFS		1
#define ENABLE_HYSE		0

void interrupt _L_HISR()
{
	if(CMIE & CMIF)
	{
		if(COUT == 0)
			LATA0 = 0;
		else
			LATA0 = 1;

		CMIF = 0;
	}
}


int main()
{
	// DAC configuration
	VCFG0 = 0x01;			// set IVREF to 1.5V

	ADCON3 = (0x03 << 6);	// DAC source from IVREF

	DACON = (0x01 << 7) | // enable DAC
			(0x01 << 6) | // enable dac output to RA5
			(0x1f << 0);	// set DAC level to 1/2*IVREF

	// CM configuration
	CMCON0 = (0x01 << 7) |	// enable CM
			 (0x01 << 5) | // enable CMO output to CM0/RC1
			 (0x01 << 2) | // posege source from DAC
			 (0x00 << 0);	// negede source from CIN0/RB4

// CM output settings
#if ENABLE_ACFS == 1
	CMCON1 = (0x03 << 4);	// enable 96us digital filter
#endif

#if ENABLE_HYSE == 1
	CMCON1 |= (0x1 << 2); // enable hysteresis
#endif


	// set RA0 to output
	// RA0 will be used as a shadow of CM's output
	TRISA0 = 0;

	// interrupt settings
	CMIE = 1;
	PEIE = 1;
	GIE = 1;

	while(1);

}