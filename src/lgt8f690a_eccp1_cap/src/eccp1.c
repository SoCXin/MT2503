
#include "lgt8f690a.h"
#include "eccp1.h"

//-----------------------------------------
// Capture on Channel 1, T1C1 @ RB4
// 	clock: CLKsys=8MHz 
//  prescaler: 1/1
//  top: PR1 = 0xffff
//	source: T1C1 @ RB4
//	mode: 4th rising edge
//-----------------------------------------
void eccp1_cap_init()
{
	TRISB4 = 0x1;		// RA1 input 

	// 8MSB of 16bit Top Value
	PR1H = 0xff; 
	// 8LSB of 16bit Top Value
	PR1L = 0xff; 

	ECP1CR0 = 0x00;
	ECP1CR1 = 0x00;
	ECP1CR2 = 0x00;

	ECP1PR0 = 0x02;		// T1C1@RB4

	ECP1IR0 = 0x10;		// 通道1中断使能

	ECP1CR1 = 0x30;		// T1C1通道4次上升沿
	ECP1CR0 = 0x01;		// CAP & EN1 

	CCP1IF = 0x0;
	CCP1IE = 0x0;
	
	TMR1IF = 0x0; 		
	TMR1IE = 0x0; 		

	T1CON = 0x01;		// TMR1ON
}

//-----------------------------------------
// Complementary PWM Generator
// 	clock: CLKsys=RC16M/2=8MHz 
//	resolution: 16bits ()
//	prescaler: select *1/1
//	top value: (PR1H:PR1L) = 0x0ff
//	frequency: 8M/(PR1H:PR1L) = 31.25KHz
//  output: T1C1 @ RB4, --> single output
//          T1C2 @ RB3, --> single output
//          T1C3 @ RA6,	--> single output
//-----------------------------------------
void eccp1_pwm_init()
{
	TRISB4 = 0x01;
	TRISB3 = 0x01;
	TRISA6 = 0x01;

	// 8MSB of 16bit Top Value
	PR1H = 0x00; 
	// 8LSB of 16bit Top Value
	PR1L = 0xff; // 0x7f;

	ECP1CR0 = 0x00;
	ECP1CR1 = 0x00;
	ECP1CR2 = 0x00;

	ECP1PR0 = 0x06;		//T1C1@RB4, T1C2@RB3, T1C3@RA6

	ECP1R1H = 0x00;
	ECP1R1L = 0xf0;

	ECP1R2H = 0x00;
	ECP1R2L = 0xc0;

	ECP1R3H = 0x00;
	ECP1R3L = 0xa0;

	ECP1CR2 = 0x15;		// Out & OE3 & OE2 & OE1

	ECP1IR0 = 0x00;

	ECP1CR0 = 0xaa;		// LDM & PWM & EN4 & EN3 & EN2 & EN1

	CCP1IF = 0x0;
	CCP1IE = 0x0;
	
	TMR1IF = 0x0; 		
	TMR1IE = 0x0; 		

	T1CON = 0x01;		// TMR1ON
}
