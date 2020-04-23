//-----------------------------------------
// TWI Controller
//	I2C Interface
//	SCL @ RA1, SDA @ RA0
//	Master Write and Read Operation
//-----------------------------------------

#include "lgt8f690a.h"
#include "twi.h"

void twi_init()
{
	// Initial RA1 & RA0 Status
	RA0 = 0x1;		// SCL High
	RA1 = 0x1; 		// SDA High
	// Set RA1 & RA0 direction for Input
	TRISA0 = 0x1;		// high for input
	TRISA1 = 0x1; 		// high for input
	// Set RA1 & RA0 pullup enable
	WPUA0 = 0x1;		// high for pullup
	WPUA1 = 0x1; 		// high for pullup
	nRABPU = 0x0; 		// low  for global pullup enable
	// TWI Configuration 
	TWCR = 0xc1; 		// (1 << TWEN) | (1 << TWMST) | (1 << CKPS0)
}

unsigned char twi_slave_recv (unsigned char sla_addr, unsigned char *recv_buf, unsigned char length)
{
	unsigned char i;
	unsigned char recv_adr;

	recv_adr = 0x0;

	TWSR = 0x0; 
	TWIF = 0x0; 	// clear TWIF
	TWIEN = 0x1;
//----------------------------------------------------------------
// wait start
//----------------------------------------------------------------
	TWIRXP = 0x0;
	while(TWIF==0x0); 	// wait for TWIF set

	if (TWIRXS==0x1) {		// tx start will receive start		
		TWIRXP = 0x0;	// clear rxp
		TWIRXS = 0x0;	// clear rxs
	}

//----------------------------------------------------------------
// receive sla+w 
//----------------------------------------------------------------
	TWITACK = 0x1;		// set NAK enable	
	TWIF = 0x0; 	// clear TWIF to continue i2c operation

	while(TWIF==0x0); 	// wait for TWIF set, sla+w sent and acknowledge received

	if (TWIRXD==0x1) {
		if (TWDR != ((sla_addr << 1) + 0x0)) {
			goto _not_addressed_;
		}
		TWITACK = 0x0;	// set ACK enable
		TWIRXD = 0x0;	// clear RXD
		TWITXK = 0x1;	// send ACK
	}

//----------------------------------------------------------------
// wait until ACK sent for sla+w 
//----------------------------------------------------------------	
	TWIF = 0x0; 	// clear TWIF to continue i2c operation

	while(TWIF==0x0); 	// wait for TWIF set, ACK sent for acknowledge sla+w

//----------------------------------------------------------------
// receive addr
//----------------------------------------------------------------
	TWIF = 0x0; 	// clear TWIF to continue i2c operation

	while(TWIF==0x0); 	// wait for TWIF set, sla+w sent and acknowledge received

	if (TWIRXD==0x1) {
		recv_adr = TWDR;// save addr
		TWITACK = 0x0;	// set ACK enable
		TWIRXD = 0x0;	// clear RXD
		TWITXK = 0x1;	// send ACK
	}

//----------------------------------------------------------------
// receive data
//----------------------------------------------------------------
	for(i=length; i>0; )
	{
		TWIF = 0x0; 	// clear TWIF to continue i2c operation

		while(TWIF==0x0); 	// wait for TWIF set, sla+w sent and acknowledge received

		if (TWIRXD==0x1) {
			recv_buf[length-i] = TWDR;	// save data
			if (i==1) { TWITACK = 0x1; }	// set NAK enable
			else { TWITACK = 0x0; }		// set ACK enable
			TWIRXD = 0x0;	// clear RXD
			i--;
			TWITXK = 0x1;	// send ACK
		}
	}

	while(TWIF==0x0); 	// wait for TWIF set, sla+w sent and acknowledge received

//----------------------------------------------------------------
// not address state
//----------------------------------------------------------------
_not_addressed_:

	TWIF = 0x0; 	// clear TWIF to continue i2c opeartion
	TWIEN = 0x0;
	
	return recv_adr;
}

void twi_slave_send (unsigned char sla_addr, unsigned char send_adr, unsigned char *send_buf, unsigned char length)
{
	unsigned char i;
 
	TWSR = 0x0;
	TWIF = 0x0; 	// clear TWIF
	TWIEN = 0x1;
//----------------------------------------------------------------
// wait start
//----------------------------------------------------------------

	while(TWIF==0x0); 	// wait for TWIF set

	if (TWIRXS==0x1) {		// tx start will receive start		
		TWIRXP = 0x0;	// clear rxp
		TWIRXS = 0x0;	// clear rxs
	}

	TWITACK = 0x1;		// set NAK enable	
	TWIF = 0x0; 	// clear TWIF to continue i2c operation

//----------------------------------------------------------------
// receive sla+r 
//----------------------------------------------------------------

	while(TWIF==0x0); 	// wait for TWIF set, sla+w sent and acknowledge received

	if (TWIRXD==0x1) {
		if (TWDR != ((sla_addr << 1) + 0x1)) {
			goto _not_addressed_;
		}
		
		TWIRXD = 0x0;	// clear RXD		

		TWITACK = 0x0;	// set ACK enable
		TWITXK = 0x1;	// send ACK

		TWIF = 0x0; 	// clear TWIF to continue i2c operation
	}

//----------------------------------------------------------------
// send data
//----------------------------------------------------------------
	while(TWIF==0x0); 	// wait for TWIF set, data send and acknowledge received

	for(i=length; i>0; )
	{
		
		TWDR = send_buf[send_adr + length - i];

		TWITXD = 0x1;

		TWIF = 0x0; 	// clear TWIF to continue i2c operation

		while(TWIF==0x0); 	// wait for TWIF set, data send and acknowledge received

		if (TWIRXK==0x1) {
			if (TWIRACK == 0x1) { goto _not_addressed_; }	// terminate transfer 
			else {i--;}
			TWIRXK = 0x0;	// clear RXK
		}
	}

	while(TWIF==0x0); 	// wait for TWIF set, data send and acknowledge received

//----------------------------------------------------------------
// not addressed state
//----------------------------------------------------------------
_not_addressed_:

	TWIF = 0x0; 	// clear TWIF to continue i2c opeartion
	TWIEN = 0x0;

}



void twi_master_send (unsigned char sla_addr, unsigned char send_adr, unsigned char *send_buf, unsigned char length)
{
	unsigned char i;
 
	TWIF = 0x0; 	// clear TWIF
//----------------------------------------------------------------
// send start
//----------------------------------------------------------------
	TWIRXS = 0x0;
	TWITXS = 0x1;

	while(TWIF==0x0); 	// wait for TWIF set

	if (TWIRXS==0x1) {		// tx start will receive start		
		//TWIRXP = 0x0;	// clear rxp
		TWIRXS = 0x0;	// clear rxs
	}

//----------------------------------------------------------------
// send sla+w 
//----------------------------------------------------------------
	TWDR = (sla_addr << 1) + 0x0;
	TWITXD = 0x1;

	TWIF = 0x0;

	while(TWIF==0x0); 	// wait for TWIF set, sla+w sent and acknowledge received

	if (TWIRXK==0x1) {
		if (TWIRACK == 0x1) {goto _not_addressed_;}
		TWIRXK = 0x0;	// clear RXK
	}

//----------------------------------------------------------------
// send addr 
//----------------------------------------------------------------
	TWDR = send_adr;
	TWITXD = 0x1;

	TWIF = 0x0;

	while(TWIF==0x0); 	// wait for TWIF set, sla+w sent and acknowledge received

	if (TWIRXK==0x1) {
		if (TWIRACK == 0x1) {goto _not_addressed_;}
		TWIRXK = 0x0;	// clear RXK
	}

//----------------------------------------------------------------
// send data
//----------------------------------------------------------------

	for(i=length; i>0; i--)
	{
		
		TWDR = send_buf[length - i];

		TWITXD = 0x1;

		TWIF = 0x0; 	// clear TWIF to continue i2c operation

		while(TWIF==0x0); 	// wait for TWIF set, data send and acknowledge received

		if (TWIRXK==0x1) {
			if (TWIRACK == 0x1) { goto _not_addressed_; }	// terminate transfer 
			TWIRXK = 0x0;	// clear RXK
		}
	}

//----------------------------------------------------------------
// not addressed state
//----------------------------------------------------------------
_not_addressed_:

	TWIRXP = 0x0;
	TWITXP = 0x1;
	TWIF = 0x0; 	// clear TWIF to continue i2c opeartion
	while(TWITXP == 0x1); // wait for stop sent
}



void twi_master_recv (unsigned char sla_addr, unsigned char send_adr, unsigned char *recv_buf, unsigned char length)
{
	unsigned char i;
 
	TWIF = 0x0; 	// clear TWIF
//----------------------------------------------------------------
// send start
//----------------------------------------------------------------
	TWIRXS = 0x0;
	TWITXS = 0x1;

	while(TWIF==0x0); 	// wait for TWIF set

	if (TWIRXS==0x1) {		// tx start will receive start		
		//TWIRXP = 0x0;	// clear rxp
		TWIRXS = 0x0;	// clear rxs
	}

//----------------------------------------------------------------
// send sla+w 
//----------------------------------------------------------------
	TWDR = (sla_addr << 1) + 0x0;
	TWITXD = 0x1;

	TWIF = 0x0;

	while(TWIF==0x0); 	// wait for TWIF set, sla+w sent and acknowledge received

	if (TWIRXK==0x1) {
		if (TWIRACK == 0x1) {goto _not_addressed_;}
		TWIRXK = 0x0;	// clear RXK
	}

//----------------------------------------------------------------
// send addr 
//----------------------------------------------------------------
	TWDR = send_adr;
	TWITXD = 0x1;

	TWIF = 0x0;

	while(TWIF==0x0); 	// wait for TWIF set, sla+w sent and acknowledge received

	if (TWIRXK==0x1) {
		if (TWIRACK == 0x1) {goto _not_addressed_;}
		TWIRXK = 0x0;	// clear RXK
	}
//----------------------------------------------------------------
// send start
//----------------------------------------------------------------
	TWIRXS = 0x0;
	TWITXS = 0x1;

	TWIF = 0x0;
	while(TWIF==0x0); 	// wait for TWIF set

	if (TWIRXS==0x1) {		// tx start will receive start		
		//TWIRXP = 0x0;	// clear rxp
		TWIRXS = 0x0;	// clear rxs
	}

//----------------------------------------------------------------
// send sla+r 
//----------------------------------------------------------------
	TWDR = (sla_addr << 1) + 0x1;
	TWITXD = 0x1;

	TWIF = 0x0;

	while(TWIF==0x0); 	// wait for TWIF set, sla+w sent and acknowledge received

	if (TWIRXK==0x1) {
		if (TWIRACK == 0x1) {goto _not_addressed_;}
		TWIRXK = 0x0;	// clear RXK
	}
//----------------------------------------------------------------
// receive data
//----------------------------------------------------------------

	for(i=length; i>0; i--)
	{
		TWITACK = 0x0; 
		if (i==0x1) { TWITACK = 0x1; }
		TWITXK = 0x1;

		TWIF = 0x0; 	// clear TWIF to continue i2c operation

		while(TWIF==0x0); 	// wait for TWIF set, data send and acknowledge received

		if (TWIRXD==0x1) {
			recv_buf[length-i] = TWDR;
		}
	}

//----------------------------------------------------------------
// not addressed state
//----------------------------------------------------------------
_not_addressed_:

	TWIRXP = 0x0;
	TWITXP = 0x1;
	TWIF = 0x0; 	// clear TWIF to continue i2c opeartion
	while(TWITXP == 0x1); // wait for stop sent
}


