/*
  							  	****************
*******************************  C HEADER FILE  **********************************
** 	**************** 						        **
** 									        **
** project  : BSPLGT8PE663A						    	**
** filename : twi.h 	   							**
** version  : 1.0 					   			**
** date     : April 01, 2016 				   			**
** 								   		**
**********************************************************************************
** 								   		**
** Copyright (c) 2016, 	LogicGreen Technologies Co., LTD			**
** All rights reserved.                                                		**
**                                                                     		**
**********************************************************************************
VERSION HISTORY:
----------------
Version 	: 1.0
Date 		: April 01, 2016
Revised by 	: LogicGreen Software Group
Description 	: Original version.
*/
	
/**
 * @file twi.h
 * @brief Header File of EEPC
 *		
 */

#ifndef _TWI_H_
#define _TWI_H_

/**********************************************************************************
***	          TYPEDEFS AND STRUCTURES					***													  	
**********************************************************************************/

/**********************************************************************************
***	  	  EXPORTED VARIABLES						***													  	
**********************************************************************************/

/**********************************************************************************
*** 	  	  EXPORTED FUNCTIONS						*** 													
**********************************************************************************/
void twi_init(void);
unsigned char twi_slave_recv (unsigned char sla_addr, unsigned char *recv_buf, unsigned char length);
void twi_slave_send (unsigned char sla_addr, unsigned char send_adr, unsigned char *send_buf, unsigned char length);
void twi_master_recv (unsigned char sla_addr, unsigned char send_adr, unsigned char *recv_buf, unsigned char length);
void twi_master_send (unsigned char sla_addr, unsigned char send_adr, unsigned char *send_buf, unsigned char length);

#endif
/**********************************************************************************
***	       	  EOF								***													  	
**********************************************************************************/

