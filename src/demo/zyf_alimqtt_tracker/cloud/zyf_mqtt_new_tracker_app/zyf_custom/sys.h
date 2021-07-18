#ifndef __SYS_H__
#define __SYS_H__


#include "ql_gpio.h"
#include "ql_adc.h"
#include "zyf_protocol.h"

#define MAX_POINT_LEN (20) //can not exceed (255)u8

#define LED1_H	 Ql_GPIO_SetLevel(LED1, PINLEVEL_HIGH)
#define LED1_L	 Ql_GPIO_SetLevel(LED1, PINLEVEL_LOW)
#define LED2_H	 Ql_GPIO_SetLevel(LED2, PINLEVEL_HIGH)
#define LED2_L	 Ql_GPIO_SetLevel(LED2, PINLEVEL_LOW)


extern Enum_PinName LED1;
extern Enum_PinName LED2;
extern Enum_PinName LED3;
extern volatile u8 Chargsta;

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

enum
{
    CHARG_OFF = 0,
    CHARG_ON,
};

typedef struct{
    u32 year;
    u32 mon;
    u32 day;
    u32 hour;
    u32 min;
    u32 sec;
}__collecttime;

typedef struct
{
    u32 latitude;											  
    u32 longitude;
    __collecttime collecttime;
    u8 bvol;
    u8 sysruntime;
    u8 csq;
}_save_location_point;

typedef struct 
{
	u32 HandInter;			//设置握手时间间隔
	u32 Interval;				//设置自动上传时间间隔
	u8 SN[16];				//SN
	u8 snuser;				//SN是否已经设置
	u8 secret[48];
	u8 key[16];
	u8 saveflag;
	u8 fotaflag;				//升级标志
	u8 fotaaddr[200];			//固件地址和信息
	u32 SysTime;
	u8 CallMode;				//预留
	u8 updateinfo;			//升级标志
}_system_setings;


typedef struct
{
    u8 save_data_num;       //保存数据点个数 存一个 +1
    u8 preflag;             //上次定位情况
    u8 gorlflag;            //lbs or gps 1 gps 2 lbs
    u8 faststart;           //快速开机计数 10min 重新开机
    u32 prellng;            //上一次获取到的定位数据 经度
    u32 prellat;            //纬度
	_save_location_point save_point[MAX_POINT_LEN]; //保存数据点 大地址保存新数据点
}_save_data_point;

extern volatile u8 YorNupgrading;


extern volatile u8 SocketBufRdBit;
extern volatile u8 nextfaststart;
extern volatile int savemode;




extern u8 Need_Lbs_Data;

extern _system_setings	systemset;
extern _save_data_point savedata;



void DisUpdateRTC(void);
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
void Set_FirstData_location(u8 gorlflag,s32 llng,u32 llat);

void JudgeGpsAndGSM(void);
void SaveGpsdatapoint(u32 parm);



#endif

