#ifndef _UART_H_
#define _UART_H_

#include "ril_location.h"
#include "ril_sms.h"
#include "ril_telephony.h"


#define APN      "cmnet"
#define USERID   ""
#define PASSWD   ""



typedef struct
{
    u8 FTP_SVR_ADDR[20];
	u8 FTP_SVR_PATH[20];
	u8 FTP_USER_NAME[20];
	u8 FTP_PASSWORD[20];
	u8 FTP_FILENAME[20];
} FOTAINFO;

extern FOTAINFO fotainfo;


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


void UartInit(void);
void VIRTUAL_PORT1Init(void);
void User_Commad_Ays(u8 *buf,u8 len);
 void MyCallback_Location(s32 result, ST_LocInfo* loc_info);

#define CON_SMS_BUF_MAX_CNT   (1)
#define CON_SMS_SEG_MAX_CHAR  (160)
#define CON_SMS_SEG_MAX_BYTE  (4 * CON_SMS_SEG_MAX_CHAR)
#define CON_SMS_MAX_SEG       (7)

/***********************************************************************
 * STRUCT TYPE DEFINITIONS
************************************************************************/
typedef struct
{
    u8 aData[CON_SMS_SEG_MAX_BYTE];
    u16 uLen;
} ConSMSSegStruct;

typedef struct
{
    u16 uMsgRef;
    u8 uMsgTot;

    ConSMSSegStruct asSeg[CON_SMS_MAX_SEG];
    bool abSegValid[CON_SMS_MAX_SEG];
} ConSMSStruct;

 extern ConSMSStruct g_asConSMSBuf[CON_SMS_BUF_MAX_CNT];


extern float Acceleration[3],AngularVelocity[3],Angle[3],Tem;

#endif
