#ifndef _MOTOR_H_
#define _MOTOR_H_

#include "ql_type.h"


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

#define RETURN_TIMEOUT_SEC 10 //等待还车上锁超时秒数
#define RENT_TIMEOUT_SEC 10 //租车保持锁开的秒数
#define SCANBLE_TIMEOUTSEC_NOSTATION 10 //还车时，检测不到电子桩，扫描间隔
#define SCANBLE_TIMEOUTSEC_INSTATION 10 //还车是，检测到电子桩，扫描间隔
#define SCANBLE_TIMEOUTSEC_WAITLOCK  10//还车等待锁车时间，超过时间开启蓝牙扫描；

// 锁状态定义
typedef enum
{
    LOCK_STATUS_UNLOCKED = 0x51,     ///!<锁已打开
    LOCK_STATUS_UNLOCKED_ERR = 0x61, ///!<锁未正确打开
    LOCK_STATUS_LOCKED = 0xA1,       ///!<锁已关闭(已上锁)
    LOCK_STATUS_LOCKABLE = 0xB1,     ///!<锁未正确关闭，处于可锁状态
    LOCK_CLOSE_PILE = 0xC1,          ///!<在电子桩范围内上锁 todo
    LOCK_STATUS_INIT = 0x01,         // 初始化锁状态
    LOCK_REVISE_LOCK = 0x71,         ///!<反锁
} LOCK_STATUS_t;

// 锁控制命令定义
typedef enum
{
    LOCK_CTRL_CMD_CLEAR = 0,   ///!<无操作 清除锁控制命令
    LOCK_CTRL_CMD_UNLOCK,      ///!<锁已打开
    LOCK_CTRL_CMD_LOCKABLE,     ///!<锁未正确关闭，处于可锁状态
} LOCK_CTRL_CMD_t;

typedef struct
{
    u8 cCmd;    // 命令
    u8 cTime;   // 定时器使用标志
    u8 cStatus; // 锁状态
    u8 cStatusCopy; // 锁状态备份
    u8 nCount; // 电机动作次数
    u16 nUnLockTime; // 保持开锁的时间,倒计时到0重新闭锁
    u16 nScanbleTime;
    u16 nWaitlockTime;//还车等待上锁时间
} motor_ctrl_t;


extern motor_ctrl_t motor_ctrl;



#endif



