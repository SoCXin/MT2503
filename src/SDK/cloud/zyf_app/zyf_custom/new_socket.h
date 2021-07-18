#ifndef __NEW_SOCKET_H__
#define __NEW_SOCKET_H__


#define REBUFLEN 200

extern u8 M203CIsBusy;
extern u16 CoreAbnormalBit;

extern u8 LbsInfoBit;

void M203C_init(void);
void M203C_gprs_init(void);
void AysConet2Core(u8 *buf,u8 len);
void GPRS_Send_Data(u8 *buf,u8 len);
u8 IsHaveLbsData(void);
u8 M203C_EPO_Init(u8 *lot,u8 *lat);
void M203C_Ch2Gprs(void);
void DisconnectSocket(u8 curline);
#endif


