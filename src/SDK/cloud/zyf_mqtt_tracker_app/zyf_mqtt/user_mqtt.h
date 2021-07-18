
#ifndef __USER_MQTT__
#define __USER_MQTT__


#include "ril.h"
#include "ril_util.h"
#include "ril_telephony.h"
#include "ql_stdlib.h"
#include "ql_error.h"
#include "ql_trace.h"
#include "ql_uart.h"
#include "ql_system.h"
#include "aliot_mqtt_client.h"



#define USER_MQTT_DEBUG				//user_mqtt调试开关

#define MQTT_YUN_COMPANY_NAME	"xxtopic"

#define AMC_PUB_LENGTH_MAX 1024
#define MQTT_MSG_WRITE_BUF_SIZE 1024
#define MQTT_MSG_READ_BUF_SIZE 1024


#define AIOT_TIMER_ID		0x08
#define HEART_TIMER_ID		0x06
#define IOTDATA_TIMER_ID	0x04
#define LED_TIMER_ID		0x10



typedef enum
{
   MQTT_SLEEP_TIME_MS=5000U,
   MQTT_YIELD_TIME_MS=140000U,
   MQTT_YIELD_INTERVAL_MS=1000U,
   MQTT_REQUEST_TIMEOUT_MS=25000U,
   MQTT_KEEPALIVE_INTERVAL_MS=300000U,
   IOTDATA_TIMER_MS=10000U,
   LED_TIMER_MS=1000U,
   
} MQTT_TIEMS_T;



typedef enum
{
   USER_MQTT_GPRS_INIT,			    //检查网络状态
   USER_MQTT_PARMA_INIT,			//用户MQTT参数初始化
   USER_MQTT_AUTH_INIT,				//用户AUTH信息初始化
   USER_MQTT_CONNECTING,			//MQTT连接服务器
   USER_MQTT_PUB_REGISTER_MSG,		//MQTT发布注册信息
   USER_MQTT_CLINT_OK,				//用户MQTT连接过程初始化完成
   	
} USER_MQTT_CLINT_STA_T;


typedef struct 
{
	aliot_mqtt_param_t mqtt_params;	//mqtt参数
    aliot_mqtt_topic_info_t topic_msg;
}user_mqtt_param_t;


typedef struct 
{
	u32 heartping;
}user_mqtt_info_t;


extern user_mqtt_info_t user_mqtt_info;



extern u32 Heart_T;
extern u32 IotData_T;


#endif

