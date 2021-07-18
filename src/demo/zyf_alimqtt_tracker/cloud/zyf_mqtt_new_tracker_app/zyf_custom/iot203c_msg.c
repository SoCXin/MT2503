/******************************************************************************

                  版权所有 (C) 2018, 天创金农科技

 ******************************************************************************
  文件   : iot203c_msg.c
  版本   : 初稿
  作者   : LiCM
  日期   : 2018年02月24日
  内容   : 创建文件
  描述   : 各个任务之间发送msg消息方法

  修改记录:
  日期   : 2018年02月24日
    作者   : LiCM
    内容   : 创建文件

******************************************************************************/

#include "ql_trace.h"
#include "ql_system.h"
#include "ql_gpio.h"
#include "ql_stdlib.h"
#include "ql_error.h"
#include "ql_uart.h"
#include "iot203c_msg.h"
#include "sys.h"


/*!
 * @brief 发送消息给MQTT任务，启动MQTT建立连接
 * \n
 *
 * @param void
 * \n
 * @see
 */

void SendMsg2KernelFroCheckGSMorGPRS(void)
{
    Ql_OS_SendMessage(main_task_id, MSG_IOT_TASK_START_JUDGE_GSMGPRS, 0, 0);
}
void SendMsg2KernelForMqttStart(void)
{
	 if(systemset.snuser)								//只要设置完成 SN号码之后,才可以进行连接
	 	{
	 		Ql_OS_SendMessage(subtask1_id, MSG_IOT_TASK_START_MQTT, 0, 0);
	 	}
}


void SendMsg2KernelForBLEStart(void)
{
	 Ql_OS_SendMessage(user_mqtt_task_id, MSG_IOT_TASK_START_BLE, 0, 0);
}

void SendMsg2KernelForIotHeart(void)
{
	 Ql_OS_SendMessage(user_mqtt_task_id, MSG_IOT_TASK_HeartDATA_MQTT, 0, 0);
}


void SendMsg2KernelForIotData(void)
{
	 Ql_OS_SendMessage(user_mqtt_task_id, MSG_IOT_TASK_SENDDATA_MQTT, 0, 0);
}

void SendMsg2KernelForModuleData(void)
{
	 Ql_OS_SendMessage(user_mqtt_task_id, MSG_IOT_TASK_Get_ModuData, 0, 0);
}

void SendMsg2KernelForReIotData(void)
{
	 Ql_OS_SendMessage(user_overlook_task_id, MSG_IOT_TASK_REDATA_MQTT, 0, 0);
}


void SendMsg2KernelForOTAData(void)
{
	 Ql_OS_SendMessage(subtask1_id, MSG_IOT_TASK_OTA_MQTT, 0, 0);
}


void SendMsg2KernelForMcuOffP(void *nexttime)
{
	 Ql_OS_SendMessage(user_overlook_task_id, MSG_IOT_TASK_MCU_OFF, nexttime, 0);
}

void SendMsg2KernelForGetLocation(void* mode)
{
	 Ql_OS_SendMessage(user_overlook_task_id, MSG_IOT_TASK_GET_LOCATION, mode, 0);
}

void SendMsg2KernelForSendSaveData(void)
{
	 Ql_OS_SendMessage(user_overlook_task_id, MSG_IOT_TASK_SENDSAVEDATA_MQTT, 0, 0);
}
void SendMsg2KernelForSendPMTKData(void)
{
	 Ql_OS_SendMessage(user_overlook_task_id, MSG_IOT_TASK_PMTK_SET_STATIC_NAV_THD, 0, 0);
}


void SendMsg2KernelForErrorLog(void)
{
	 Ql_OS_SendMessage(user_mqtt_task_id, MSG_IOT_TASK_ERR_LOG, 0, 0);
}



