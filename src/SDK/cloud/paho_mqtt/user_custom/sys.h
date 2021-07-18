#ifndef __SYS_H__
#define __SYS_H__


#include "ql_gpio.h"
#include "ql_adc.h"



typedef enum
{
    MSG_LOCK_CTRL_SERVICE_NONE = 0x100,  		//预留    
    MSG_LOCK_CTRL_SERVICE_RENT ,     			//租车
    MSG_RD_ALIYUN_MQTT_RE,						//读阿里云下行数据
    MSG_LOCK_CTRL_SERVICE_RETURN,       		//还车
    MSG_SEND_ALIYUN_MQTT_HRET, 				//发送心跳包
    MSG_SEND_ALIYUN_IOTSTA, 		//发送业务数据 
    MSG_SEND_ALIYUN_INIT, 		//初始化阿里云连接
} MSG_LOCK_CTRL_SERVICE_TYPE_T;



typedef struct 
{
	u32 HandInter;			//设置握手时间间隔
	u32 Interval;				//设置自动上传时间间隔
	u8 SN[16];				//SN
	u8 snuser;				//SN是否已经设置
	u8 saveflag;
	u8 fotaflag;				//升级标志
	u8 fotaaddr[200];			//固件地址和信息
	u32 SysTime;
	u8 CallMode;				//预留
}_system_setings;


extern volatile u8 SocketBufRdBit;




extern u8 Need_Lbs_Data;

extern _system_setings	systemset;






void GpsDelay(u16 delaytime);
void LbsDelay(u16 delaytime);
void power_drv_init(void);
void LcdPin_init(void);
void Delayus(u32 data);
void SysSleep(void);
void  LoadDefualtCfg(void);
void SYS_Parameter_Init(void);
void ShowSysInfo(void);
void InternetsetSN_Analysis(u8 *snuf,u8 len);
void Callback_ChageInit(Enum_ADCPin adcPin, u32 adcValue, void *customParam);
void SysWakeUp(void);
void ADC_Program(void);
void DrvBell_Ring_R(u8 num);
void M203C_BSP_init(void);
#endif

