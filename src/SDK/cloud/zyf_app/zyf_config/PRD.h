#ifndef	 _PRD_H_  
#define 	_PRD_H_ 
#include "ql_type.h" 
#include "pro.h" 
/*The necessary information*/
#define USEPASSWORD  0
#define PASSWORD		 "123456789"
#define PRODUCTKEY		"c2de8066c8a4"
/*The Dynamic TLV Info */
#define READONLYNUM	  12
extern tlv AutomaticRDTlv[READONLYNUM];
#define READWRITE	  2
extern tlv AutomaticWRTlv[READWRITE];
#endif