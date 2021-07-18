

/******************************************************************************

                  版权所有 (C) 2018, 天创金农科技

 ******************************************************************************
  文件   : zyf_protocol.h
  版本   : 初稿
  作者   : LiCM
  日期   : 2018年02月24日
  内容   : 创建文件
  描述   : 智云服MQTT版本通信协议

  修改记录:
  日期   : 2018年02月24日
    作者   : LiCM
    内容   : 创建文件

******************************************************************************/



#ifndef __ZYF_PROTOCOL_
#define __ZYF_PROTOCOL_

#include "ql_stdlib.h"
#include "ql_error.h"
#include "ql_trace.h"


#define ZYF_PRO_DEBUG

//#define _USE_USER_DWQ_

#define SW_VERSION	"18041801"



#define BLE_CMD_HEAD 0xAA  // 从 App 发送给蓝牙
#define BLE_RESP_HEAD 0x55 // 从蓝牙发送给 App 的数据为：帧头（1 字节）0x55

#define APP_SUCCESS 0x00
#define APP_FAIL 0x01
#define APP_ERROR_CMD 0x02



typedef enum
{
    BLE_CMD_AUTH_KEY  = 0X01,      
    BLE_CMD_AUTH_REQU = 0x02,           
    BLE_CMD_DEVRESET = 0XF0,
    BLE_ERROR_CMD = 0xFF,
#ifdef _USE_USER_DWQ_
	BLE_DWQ_CMD = 0xC0,
#endif
} BLECMD_CODE_t;


extern u8 ZYFRECMD[2][20];

extern volatile u8 ZYF_MQTT_CONFIG;






#ifdef _USE_USER_DWQ_


#define WZUD_L	 Ql_GPIO_SetLevel(PINNAME_SIM2_CLK, PINLEVEL_LOW)
#define WZUD_H	 Ql_GPIO_SetLevel(PINNAME_SIM2_CLK, PINLEVEL_HIGH)
#define WZINS_L	 Ql_GPIO_SetLevel(PINNAME_SIM2_DATA, PINLEVEL_LOW)
#define WZINS_H	 Ql_GPIO_SetLevel(PINNAME_SIM2_DATA, PINLEVEL_HIGH)
#define WZCS_L	 Ql_GPIO_SetLevel(PINNAME_SIM2_RST, PINLEVEL_LOW)
#define WZCS_H	 Ql_GPIO_SetLevel(PINNAME_SIM2_RST, PINLEVEL_HIGH)


#define M203C_SDDATAIN_L	 Ql_GPIO_SetLevel(PINNAME_SD_CMD, PINLEVEL_LOW)
#define M203C_SDDATAIN_H	 Ql_GPIO_SetLevel(PINNAME_SD_CMD, PINLEVEL_HIGH)


typedef struct 
{
	u32 sec;
	u8 per;
}mux_mode;


typedef struct 
{
	u8 mode;
	u8 per;
	mux_mode muxmode[8];
}_DJKZ_mode;


extern _DJKZ_mode djkzmode;

u8 WzAynsMode(u8 *pbuf);



#endif


s32 zyf_msg_register_data(u8 *data_ptr,void *user_data);
s32 zyf_msg_Sensor_data(u8 *data_ptr,void *user_data);
s32 zyf_decode_cmd(char *Payload,u16 len);

#endif


