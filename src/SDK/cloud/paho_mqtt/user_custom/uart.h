#ifndef _UART_H_
#define _UART_H_

#include "ril_location.h"
#include "ril_sms.h"
#include "ril_telephony.h"


#define DEBUG_ENABLE 1
#if DEBUG_ENABLE > 0
#define DEBUG_PORT  UART_PORT1
#define DBG_BUF_LEN   512
static char DBG_BUFFER[DBG_BUF_LEN];
#define mprintf(FORMAT,...) {\
    Ql_memset(DBG_BUFFER, 0, DBG_BUF_LEN);\
    Ql_sprintf(DBG_BUFFER,FORMAT,##__VA_ARGS__); \
    if (UART_PORT2 == (DEBUG_PORT)) \
    {\
        Ql_Debug_Trace(DBG_BUFFER);\
    } else {\
        Ql_UART_Write((Enum_SerialPort)(DEBUG_PORT), (u8*)(DBG_BUFFER), Ql_strlen((const char *)(DBG_BUFFER)));\
    }\
}
#else
#define mprintf(FORMAT,...)
#endif

extern u8 Need_lbs_Init;

void UartInit(void);
void User_Commad_Ays(u8 *buf,u8 len);
void MyCallback_Location(s32 result, ST_LocInfo* loc_info);



 typedef struct
{
	u8 aliyun;
	u8 bleser;
	u8 bleclient;
	u8 led;
	u8 moto;
	u8 spk;
	u8 ftp_audio;
	u8 fota;
} uartcomand;


extern uartcomand systemfuntest;



 typedef struct
{
    u8 asta;
    u16 atimer;
} Blesta;

extern Blesta blestas;

#endif
