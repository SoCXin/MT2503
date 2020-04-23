/*
  							  	****************
*******************************  C HEADER FILE  **********************************
** 	**************** 						        **
** 									        **
** project  : BSPLGT8F690A						    	**
** filename : usart.h 	   							**
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
 * @file usart.h
 * @brief Header File of USART
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
void usart_send_char(unsigned char txdat);
unsigned char usart_recv_char(void);
void usart_send(unsigned char length, unsigned char *txbuf);
void usart_recv(unsigned char length, unsigned char *rxbuf);
void usart_sendrecv(unsigned char length, unsigned char *rxbuf, unsigned char *txbuf);

#endif
/**********************************************************************************
***	       	  EOF								***													  	
**********************************************************************************/

