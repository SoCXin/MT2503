
#ifndef __IOT203C_MSG__
#define __IOT203C_MSG__

#include "ql_trace.h"
#include "ql_system.h"
#include "ql_gpio.h"
#include "ql_stdlib.h"


typedef enum
{
   MSG_IOT_TASK_START_MQTT=0x100,
   MSG_IOT_TASK_SENDDATA_MQTT,
   MSG_IOT_TASK_GET_SENSOR_DATA,
   MSG_IOT_TASK_START_BLE,
   MSG_IOT_TASK_SENDDATA_BLE,
   MSG_IOT_TASK_HeartDATA_MQTT,
   MSG_IOT_TASK_Get_ModuData,
   MSG_IOT_TASK_REDATA_MQTT,
   MSG_IOT_TASK_OTA_MQTT,
   
   	
} MSG_IOT203C_TASK_T;




#endif






