#ifndef	 	_PRD_H_  
#define 	_PRD_H_ 
#include "ql_type.h"
#define READONLYNUM	7
#define READWRITE	  5
typedef struct {
	u8 *pk;
	u16 port;
    u8 * host_name;
    u8 * client_id;
    u8 * user_name;
    u8 * password;
    u8  *pubKey;
}ZYF_AUTO_CONFIG_T;
extern ZYF_AUTO_CONFIG_T ZYFAutoconfig;
extern u8 *AutomaticRD[READONLYNUM];
extern u8 *AutomaticWR[READWRITE];
#endif
