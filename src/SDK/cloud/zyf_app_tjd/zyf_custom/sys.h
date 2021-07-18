#ifndef __SYS_H__
#define __SYS_H__


#include "ql_gpio.h"
#include "ql_adc.h"

//#define _USE_LED_MIS_




#ifdef _USE_LED_MIS_

#define LED1_L	 Ql_GPIO_SetLevel(LED1, PINLEVEL_LOW)
#define LED1_H	 Ql_GPIO_SetLevel(LED1, PINLEVEL_HIGH)
#define LED2_L	 Ql_GPIO_SetLevel(LED2, PINLEVEL_LOW)
#define LED2_H	 Ql_GPIO_SetLevel(LED2, PINLEVEL_HIGH)
#define LED3_L	 Ql_GPIO_SetLevel(LED3, PINLEVEL_LOW)
#define LED3_H	 Ql_GPIO_SetLevel(LED3, PINLEVEL_HIGH)
#define M203C_DATA_L	 Ql_GPIO_SetLevel(M203C_DATA, PINLEVEL_LOW)
#define M203C_DATA_H	 Ql_GPIO_SetLevel(M203C_DATA, PINLEVEL_HIGH)
#define M203C_EINT_L	 Ql_GPIO_SetLevel(M203C_EINT, PINLEVEL_LOW)
#define M203C_EINT_H	 Ql_GPIO_SetLevel(M203C_EINT, PINLEVEL_HIGH)
#define M203C_WHG_L	 Ql_GPIO_SetLevel(M203C_WHG, PINLEVEL_LOW)
#define M203C_WHG_H	 Ql_GPIO_SetLevel(M203C_WHG, PINLEVEL_HIGH)


#else

#define LED1_H	 Ql_GPIO_SetLevel(LED1, PINLEVEL_LOW)
#define LED1_L	 Ql_GPIO_SetLevel(LED1, PINLEVEL_HIGH)
#define LED2_H	 Ql_GPIO_SetLevel(LED2, PINLEVEL_LOW)
#define LED2_L	 Ql_GPIO_SetLevel(LED2, PINLEVEL_HIGH)
#define LED3_H	 Ql_GPIO_SetLevel(LED3, PINLEVEL_LOW)
#define LED3_L	 Ql_GPIO_SetLevel(LED3, PINLEVEL_HIGH)
#define M203C_DATA_L	 Ql_GPIO_SetLevel(M203C_DATA, PINLEVEL_LOW)
#define M203C_DATA_H	 Ql_GPIO_SetLevel(M203C_DATA, PINLEVEL_HIGH)
#define M203C_EINT_L	 Ql_GPIO_SetLevel(M203C_EINT, PINLEVEL_LOW)
#define M203C_EINT_H	 Ql_GPIO_SetLevel(M203C_EINT, PINLEVEL_HIGH)
#define M203C_WHG_L	 Ql_GPIO_SetLevel(M203C_WHG, PINLEVEL_LOW)
#define M203C_WHG_H	 Ql_GPIO_SetLevel(M203C_WHG, PINLEVEL_HIGH)



#endif


#define WZUD_L	 Ql_GPIO_SetLevel(WZUD, PINLEVEL_LOW)
#define WZUD_H	 Ql_GPIO_SetLevel(WZUD, PINLEVEL_HIGH)
#define WZINS_L	 Ql_GPIO_SetLevel(WZINS, PINLEVEL_LOW)
#define WZINS_H	 Ql_GPIO_SetLevel(WZINS, PINLEVEL_HIGH)
#define WZCS_L	 Ql_GPIO_SetLevel(WZCS, PINLEVEL_LOW)
#define WZCS_H	 Ql_GPIO_SetLevel(WZCS, PINLEVEL_HIGH)


#define M203C_ZFGPIO1_L	 Ql_GPIO_SetLevel(ZFGPIO1, PINLEVEL_LOW)
#define M203C_ZFGPIO1_H	 Ql_GPIO_SetLevel(ZFGPIO1, PINLEVEL_HIGH)
#define M203C_ZFGPIO2_L	 Ql_GPIO_SetLevel(ZFGPIO2, PINLEVEL_LOW)
#define M203C_ZFGPIO2_H	 Ql_GPIO_SetLevel(ZFGPIO2, PINLEVEL_HIGH)





#define LED_R_ON	Ql_GPIO_SetLevel(PINNAME_SD_CMD, PINLEVEL_HIGH)
#define LED_R_OFF	Ql_GPIO_SetLevel(PINNAME_SD_CMD, PINLEVEL_LOW)
#define LED_G_ON	Ql_GPIO_SetLevel(PINNAME_SD_DATA, PINLEVEL_HIGH)
#define LED_G_OFF	Ql_GPIO_SetLevel(PINNAME_SD_DATA, PINLEVEL_LOW)
#define LED_B_ON	Ql_GPIO_SetLevel(PINNAME_NETLIGHT, PINLEVEL_HIGH)
#define LED_B_OFF	Ql_GPIO_SetLevel(PINNAME_NETLIGHT, PINLEVEL_LOW)
#define BB_ON		Ql_GPIO_SetLevel(PINNAME_SD_CLK, PINLEVEL_HIGH)
#define BB_OFF		Ql_GPIO_SetLevel(PINNAME_SD_CLK, PINLEVEL_LOW)
#define M203C_DATA_ON		Ql_GPIO_SetLevel(PINNAME_CTS, PINLEVEL_HIGH)
#define M203C_DATA_OFF		Ql_GPIO_SetLevel(PINNAME_CTS, PINLEVEL_LOW)
#define SPK_AMP_EN_ON		Ql_GPIO_SetLevel(PINNAME_DCD, PINLEVEL_HIGH)
#define SPK_AMP_EN_OFF		Ql_GPIO_SetLevel(PINNAME_DCD, PINLEVEL_LOW)

#define MOTOR_NSLEEP_ON		Ql_GPIO_SetLevel(PINNAME_SIM2_CLK, PINLEVEL_HIGH)
#define MOTOR_NSLEEP_OFF	Ql_GPIO_SetLevel(PINNAME_SIM2_CLK, PINLEVEL_LOW)
#define MOTOR_REVERSE_ON		Ql_GPIO_SetLevel(PINNAME_SIM2_RST, PINLEVEL_HIGH)
#define MOTOR_REVERSE_OFF	Ql_GPIO_SetLevel(PINNAME_SIM2_RST, PINLEVEL_LOW)
#define MOTOR_FORWARD_ON		Ql_GPIO_SetLevel(PINNAME_SIM2_DATA, PINLEVEL_HIGH)
#define MOTOR_FORWARD_OFF	Ql_GPIO_SetLevel(PINNAME_SIM2_DATA, PINLEVEL_LOW)

#define GPIO_OUT_UNLOCKED_SENSOR_ON		Ql_GPIO_SetLevel(PINNAME_PCM_CLK, PINLEVEL_HIGH)
#define GPIO_OUT_UNLOCKED_SENSOR_OFF	Ql_GPIO_SetLevel(PINNAME_PCM_CLK, PINLEVEL_LOW)
#define GPIO_OUT_LOCKED_SENSOR_ON		Ql_GPIO_SetLevel(PINNAME_PCM_IN, PINLEVEL_HIGH)
#define GPIO_OUT_LOCKED_SENSOR_OFF		Ql_GPIO_SetLevel(PINNAME_PCM_IN, PINLEVEL_LOW)
#define GPIO_IN_UNLOCKED_SENSOR			Ql_GPIO_GetLevel(PINNAME_PCM_OUT)		//in1
#define GPIO_IN_LOCKED_SENSOR			Ql_GPIO_GetLevel(PINNAME_PCM_SYNC)		//in2


typedef enum {
    IOMSG_BEGIN = 0,
	IOMSG_POWERON,
    IOMSG_DATA_COL,
    IOMSG_DATA_SEND,
    IOMSG_DO_OTHER
}Enum_IOMsgType;


typedef struct 
{
	u32 HandInter;			//设置握手时间间隔
	u8 CallMode;				//设置自动上传时间间隔
	u32 Interval;
	u32 Length;
	u32 SpedLmt[2]; 			//设置超速
	u32 Spedtime;			//真正延迟时间
	u32 SysTime;			//系统总时间
	u16 ACCoffDelay;               //ACC断开后的延迟时间
	u8 CenterIP[16];			//IP地址			
	u8 CenterPort[6];			//端口号
	u8 SN[16];				//SN
	u8 TCPorUDP;			//设置：1是TCP，0是UDP
	u8 Passwd[20];			//设置密码
	u8 ProductID[20];			//产品ID
	u8 Centerapn[20];
	u8 LbsOpen;
	u8 saveflag;				//保存标志
	u8 fotaflag;				//升级标志
	u8 fotaaddr[200];			//固件地址和信息
	u8 btsta;					//蓝牙标识	
	u8 btpower;					//蓝牙开关
	u8 btclinthead[5];			//蓝牙从设备头部 
	u8 btname[20];				//蓝牙从设备名字
	u8 btsername[20];			//蓝牙主设备名字
	u8 proinfos[8];				//协议信息
	u8 imei[15];				//imei
}_system_setings;

typedef struct 
{
	u8 sta;
	u32 sockett;
	u32 gpst;
}_system_times;

typedef struct 
{
	u32 lbsj;
	u32 lbsw;
	u32 gpsj;
	u32 gpsw;
	u32 vol;
	u8	xyz[20]; 
	u8 time[6];
	u8 dbm;
	u8 vbai;
}Coredata;


typedef struct 
{
	u32 sendpostion;
	u32 saveposion;
}DataConfig;

extern DataConfig Position;

extern Coredata	mycoredata;

extern _system_setings	systemset;
extern _system_times	systimes;

extern u8 wzcurrtsta;

extern volatile u8 syssleepbit;


extern const u8 *modetbl[2];

extern Enum_PinName LED1;
extern Enum_PinName LED2;
extern Enum_PinName LED3;
extern Enum_PinName M203C_DATA;
extern Enum_PinName M203C_EINT;
extern Enum_PinName M203C_WHG;

extern Enum_PinName ZFGPIO1;
extern Enum_PinName ZFGPIO2;

extern Enum_PinName WZUD;
extern Enum_PinName WZINS;
extern Enum_PinName WZCS;


typedef struct
{
	u8 packets_Send_sn[16];
	u8 packets_Send_snone[6];
	u8 num[2];
	u8 id[2];
}Protocol_Send_packets;

extern Protocol_Send_packets Send_packets;


extern volatile u8 SystemDebug;
extern volatile u8 SystemFlow;
extern volatile u8 BattarySta;

extern u32 jishis;
extern u32 jishit;

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
void WZAysCuurtSta(void);
#endif

