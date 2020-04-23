#include <stdint.h>
#include "lgt8f690a.h"
#include "misc.h"

// default prescalar = MRC/4
// default system clock = 16MHz/4 = 4MHz
#define _XTAL_FREQ 4000000

int main()
{ 
#if 1	// in case of not trimmed
	if(cfw_valid())
		OSCTUNE = cfw_rctrim();
	else
		OSCTUNE = 0x92;
#endif
	

	// output 1.5V IVREF to RA7
	// so IVREF can be test from RA7
	VCFG0 = 1;
	ANTM = 1; 
	L_ADCON0bits.CHS = 0x0a;

#if 1	// in case of not trimmed
	if(cfw_valid())
		VRTUNE = misc_eer(1, 0x100d); //cfw_v1trim();
	else
		VRTUNE = 0x2a;
#endif

	__delay_ms(1);

	NOP();
	NOP();

	// set PORTA[6:0] to output
	TRISA = 0x80;

	while(1) {
		LATA = ~LATA;
		
		__delay_us(2);
	}

}
