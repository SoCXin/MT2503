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

#ifndef _USART_H_
#define _USART_H_

/**********************************************************************************
***	          TYPEDEFS AND STRUCTURES					***													  	
**********************************************************************************/

/**********************************************************************************
***	  	  EXPORTED VARIABLES						***													  	
**********************************************************************************/

/**********************************************************************************
*** 	  	  EXPORTED FUNCTIONS						*** 													
**********************************************************************************/
void usart_init(void);
void usart_putc(unsigned char ch);
void usart_send(unsigned char length, unsigned char *txbuf);
void usart_recv(unsigned char length, unsigned char *rxbuf);
void usart_txrx(unsigned char length, unsigned char *rxbuf, unsigned char *txbuf);
void usart_sendrecv(unsigned char length, unsigned char *rxbuf, unsigned char *txbuf);

#endif
/**********************************************************************************
***	       	  EOF								***													  	
**********************************************************************************/

