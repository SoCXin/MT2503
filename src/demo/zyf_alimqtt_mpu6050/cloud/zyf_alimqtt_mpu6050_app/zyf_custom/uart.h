#ifndef _UART_H_
#define _UART_H_

#include "ril_location.h"
#include "ril_sms.h"
#include "ril_telephony.h"
#include "zyf_protocol.h"


#define DEBUG_ENABLE 1
#if DEBUG_ENABLE > 0
#define DEBUG_PORT  UART_PORT1
#define DBG_BUF_LEN   1024
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
extern u8 save_point_test;

void UartInit(void);
void User_Commad_Ays(u8 *buf,u8 len);
void MyCallback_Location(s32 result, ST_LocInfo* loc_info);



 typedef struct
{
	u8 ack_cmd;
	u32 chargout;
} uartcomand;


extern uartcomand systemfuntest;



 typedef struct
{
    u8 asta;
    u16 atimer;
} Blesta;

extern Blesta blestas;


enum
{
    M203C_START = 1,
    M203C_SET_DATA0,		//休眠时间
    M203C_SET_DATA1,		//震动频率
    M203C_SET_DATA3,		//振动持续时间
    M203C_GET_ACCDATA,
    M203C_GET_GAYDATA,
    M203C_ON_STM8_ON,
    M203C_RDOFF_STM8_RDOFF,
    M203C_OFF_STM8_OFF,
    M203C_CRC,
    M203C_ERROR,
    M203C_OVER,
    SOC_POWER_OFF=100,
    CMD_CHARG_OFF,
    CMD_CHARG_ON,
};

#if !defined(_USE_MCU_PRO_)
u8 SendCmd2Mcu(u8 cmd,u8 ack,u8 *value,u32 timeout);
//u8 setmcu2poweroff203c(u32 param);
#endif

#endif
