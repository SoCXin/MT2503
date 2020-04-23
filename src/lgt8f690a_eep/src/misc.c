#include "misc.h"

// EEP read function
// can be use to read EEP and configuration words
uint16_t misc_eer(uint8_t type, uint16_t address)
{
	EEADRL = address & 0xff;
	EEADRH = (address >> 8) & 0xff;

	EEPGD = type & 1;

	EERD = 1;

	return (EEDATH << 8) | (EEDAT);
}

uint16_t axu_fmul8x8(uint8_t a, uint8_t b)
{
	MDHF = 0;
	MDLF = b;
	MULF = a;

	return M16F;
}

uint16_t axu_fmul16x8(uint16_t a, uint8_t b)
{
	M16F = a;
	MULF = b;

	return M16F;
}

uint32_t axu_wmul16x8(uint16_t a, uint8_t b)
{
	M16F = a;
	MULF = b;

	return M32F;
}

uint16_t axu_fdiv16d8(uint16_t a, uint8_t b)
{
	M16F = a;
	DIVF = b;
	while(DIVCY == 1);
	
	return M16F;
}

void axu_div16d8(uint8_t b)
{
	DIVF = b;
	while(DIVCY == 1);
}

char * misc_utoa(char * buf, unsigned val, int base)
{
	char		c;
	
	char *p = buf;
	M16F = val;	
	do {
		axu_div16d8(base);
		p++;
	} while(M16F != 0);

	*p-- = 0;

	M16F = val;
	do {
		axu_div16d8(base);
		c = MDXF;
		
		if(c >= 10)
			c += 'A'-'0'-10;
		c += '0';
		*p-- = c;
	} while(M16F != 0);

	return p;
}

uint8_t cfw_valid()
{
	uint8_t btmp;

	btmp = misc_eer(MTP, 0x100f);

	if(btmp == 0x5a)
		return 1;

	return 0;
}
