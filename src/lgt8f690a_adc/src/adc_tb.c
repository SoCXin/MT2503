#include <stdint.h>
#include "lgt8f690a.h"
#include "usart.h"
#include "misc.h"

// system clock settings:
// 16M/4 = 4MHz
#define	_XTAL_FREQ 4000000

// selected ADC channel
#define	M_CHN	0x0		// adc input from AN0

// local variables definition
char caTmp[10];
int8_t g_ofs;

// internal function definition
void sys_init();
void adc_init();

uint16_t adc_read();
uint16_t adc_readchannel(uint8_t);

int main()
{
	uint8_t btmp;
	uint16_t adcv;
	float ftmp = 0.0;

	// core initialization
	sys_init();

	// ADC
	adc_init();

	// USART 
	// 19200bps/8bit/1stop/noparity
	usart_init();

	adc_readchannel(M_CHN);	// dummy

	while(1)
	{ 
		adcv = 0;

		for(uint8_t i = 0; i < 16; i++)
			adcv += adc_read();
	
		adcv >>= 4;		

		// average
		usart_puts((char *)"ADC channel(");
		btmp = M_CHN + 0x30;
		usart_putc(btmp);
		usart_puts((char *)") = ");
		
		// 0.366 = 1500/4096
		ftmp = 0.366 * adcv;
		misc_utoa(caTmp, (uint16_t)ftmp, 10);
		usart_puts(caTmp);

		usart_puts((char*)"mV (0x");
		misc_utoa(caTmp, adcv, 16);
		usart_puts(caTmp);
		usart_puts((char *)")\r\n");

		__delay_ms(200);
	}

	return 0;
}

// Core setting and intialization
void sys_init()
{
	// load trimming data for HFINTOSC
	if(cfw_valid())
	{
		OSCTUNE = cfw_rctrim();
	}
	else
	{	// if no valid data, using default value
		OSCTUNE = 0x92;
	}

	// main clock prescalar
	// using default 1/4

	// set core cycle to 1T mode
	MCUCR = 0x55;
	MCUCR = MCUCR & 0xf9; //  TCYC = 0
}

// ADC initialization and offset test
void adc_init()
{
	uint16_t vp, vn;

	//volatile uint8_t dacr_b = DACON;
	g_ofs = 0;

	// ADCON1
	//================================
	ADCON1 = (1 << 7) |	// select internal IVREF (1.5V)
			 (1 << 4);	// adc sample clock = 4MHz/8 = 500KHz
			
	// calibration ADC using DAC
	ADCON3 = (3 << 6); 	// DAC reference from IVREF
	DACON = (1 << 7) | 	// enable DAC
			(0 << 0);	// dac level: IVREF/64

	// Select DAC channel for ADC
	ADCON0 = (1 << 7) | // right alignment
			 (9 << 2) | // channel 9(DAC)
			 (1 << 0);	// enable ADC module

	// load trimming data for IVREF1.5V
	if(cfw_valid())
		VRTUNE = cfw_v1trim();
	else
		VRTUNE = 0x2a;

	// start error-correction test
#if 1	// enable P/N offset calibration
	SPN = 1; 
	vn = adc_read();

	SPN = 0;
	vp = adc_read();

	if(vn > 0x40) vn -= 0x40;
	else vn = 0x40 - vn;

	if(vp > 0x40) vp -= 0x40;
	else vp = 0x40 - vp;
	
	if(vp > vn) SPN = 1;
#endif

	// test and store offset value
	vn = adc_read();
	g_ofs = vn - 0x40;

	// restore DAC (if necessary)
	DACON = 0;
}

// Read and return ADC convert result
uint16_t adc_read()
{
	int16_t adres;

	GO = 1;
	NOP();
	while(nDONE);
	
	adres = (ADRESH << 8) | ADRESL;

	if(adres >= g_ofs)
		adres -= g_ofs;

	return (uint16_t)adres; 
}

// Read and return ADC convert result
uint16_t adc_readchannel(uint8_t chn)
{
	// channel
	L_ADCON0bits.CHS = chn;
	
	return adc_read();
}
