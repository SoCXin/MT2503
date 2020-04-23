
#include "lgt8f690a.h"
#include "eccp2.h"

//-----------------------------------------
// Capture on Channel 4, T2C4 @ RA1
// 	clock: RC16M*2=32MHz 
//  prescaler: 1/1
//  top: PR2 = 0xffff
//	source: T2C4 @ RA1
//	mode: 16th rising edge
//-----------------------------------------
void eccp2_cap_init()
{
	TCCR = 0x80;

	TRISA1 = 0x1;		// RA1 input 

	// 8MSB of 16bit Top Value
	PR2H = 0xff; 
	// 8LSB of 16bit Top Value
	PR2L = 0xff; 

	ECP2CR0 = 0x00;
	ECP2CR1 = 0x00;
	ECP2CR2 = 0x00;
	ECP2CR3 = 0x00;

	ECP2PR0 = 0x10;		// T2C4@RA1

	ECP2IR0 = 0x80;		// 通道4中断使能

	ECP2CR1 = 0xf0;		// T2C4通道16次上升沿
	ECP2CR0 = 0x40;		// CAP & EN4 

	ECP2IF = 0x0;
	ECP2IE = 0x0;
	
	TMR2IF = 0x0; 		
	TMR2IE = 0x0; 		

	TCCR = 0x90;		// 选用32MHz时钟源

	T2CON = 0x84;		// TMR2ON
}

//-----------------------------------------
// Complementary PWM Generator
// 	clock: RC16M*2=32MHz 
//	resolution: 16bits ()
//	prescaler: select *1/1
//	top value: (PR2H:PR2L) = 0x1ff
//	frequency: 32M/(PR2H:PR2L) = 62.5KHz
//  output: T2C1 @ RA5, T2C1N @ RA2 	--> complementary output
//          T2C2 @ RB0, T2C2N @ RA3		--> complementary output
//          T2C3 @ RA4&RA6, T2C3N @ RA0	--> complementary output
//          T2C4 @ RA1					--> single output
//-----------------------------------------
void eccp2_pwm_init()
{
	TCCR = 0x80;

	TRISB &= 0xfe;
	TRISA = 0x80;

	// 8MSB of 16bit Top Value
	PR2H = 0x01; 
	// 8LSB of 16bit Top Value
	PR2L = 0xff; // 0x7f;

	ECP2CR0 = 0x00;
	ECP2CR1 = 0x00;
	ECP2CR2 = 0x00;
	ECP2CR3 = 0x00;
	ECP2CR4 = 0x80;
	ECP2CR5 = 0x00;

	ECP2DTP = 0x3;
	ECP2DTN = 0x5;

	ECP2PR0 = 0x1c;		//T2C4@RA1, T2C3@RA6, T2C3@RA4

	ECP2R1H = 0x00;
	ECP2R1L = 0xf0;

	ECP2R2H = 0x00;
	ECP2R2L = 0xc0;

	ECP2R3H = 0x00;
	ECP2R3L = 0xa0;

	ECP2R4H = 0x00;
	ECP2R4L = 0x70;

	ECP2CR2 = 0x55;		// Out & OE2N & OE2 & OE1N & OE1
	ECP2CR3 = 0x15;		// Out & OE4  & OC3 & OE3N

	ECP2IR0 = 0xf0;

	ECP2CR1 = 0x01;		// LDM
	ECP2CR0 = 0xaa;		// PWM & EN4 & EN3 & EN2 & EN1

	ECP2IF = 0x0;
	ECP2IE = 0x0;
	
	TMR2IF = 0x0; 		
	TMR2IE = 0x0; 		
	TCCR = 0x90;		// 选用32MHz时钟源

	T2CON = 0x84;		// TMR2ON
}
