#ifndef __BT3_H__
#define __BT3_H__

#include "ql_type.h"
#include "ql_stdlib.h"


typedef struct 
{
	u8 parid;
	u8 connectid;
	u8 sendid;
	u8 readid;
}_bt_manager_;

extern _bt_manager_ Btmanager;

extern u16 CoreBtBit;
extern u16 CoreBtSta;

extern u8  cosbtsta;

u8 BtServerScaning(void);
void BtNewInit(void);

#endif
