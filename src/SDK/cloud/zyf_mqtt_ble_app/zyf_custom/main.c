
/*****************************************************************************
 *
 * Filename:
 * ---------
 *   main.c
 *
 * Project:
 * --------
 *   OpenCPU
 *
 *
 * Usage:
 * ------
 *   Compile & Run:
 *
 *     Set "C_PREDEF=-D __ZFY_MQTT_APP__; CLOUD_SOLUTION =ZYF_MQTT_SOLUTION " in gcc_makefile file. And compile the 
 *     app using "make clean/new".
 *     Download image bin to module to run.
 *
 * Author:
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 *----------------------------------------------------------------------------
 * 
 ****************************************************************************/

/******************************************************************************

                  版权所有 (C) 2018, 天创金农科技

 ******************************************************************************
  文件   : main.c
  版本   : 初稿
  作者   : LiCM
  日期   : 2018年02月24日
  内容   : 创建文件
  描述   : M203C3.0开发板单线程版本

  修改记录:
  日期   : 2018年02月24日
    作者   : LiCM
    内容   : 创建文件

******************************************************************************/


#include "ql_type.h"
#include "ql_trace.h"
#include "ql_uart.h"
#include "ql_system.h"
#include "ql_stdlib.h"
#include "ql_error.h"
#include "iot203c_msg.h"
#include "uart.h"
#include "ql_timer.h"
#include "user_mqtt.h"
#include "sys.h"
#include "fota_main.h"
#include "fota.h"


/*!
 * @brief 主任务:初始化BSP,完成定时采集数据并且发送到智云服
 * \n
 *
 * @param taskId 消息通讯ID
 * @return NULL
 * \n
 * @see
 */

void proc_main_task(s32 taskId)
{
	ST_MSG msg;	
    M203C_BSP_init();
    SYS_Parameter_Init();
	Led_Timer_init(TIMER_ID_USER_START + LED_TIMER_ID, LED_TIMER_MS);
	aliot_Timer_init(TIMER_ID_USER_START + AIOT_TIMER_ID, 1000);
    while (1)
    {
        
        Ql_OS_GetMessage(&msg);
        switch (msg.message)
        {
	        case MSG_ID_RIL_READY:				
		        {
		            Ql_RIL_Initialize();				//等待RIL层初始化完成,才可以调用AT CMD指令,用户无需关心
		           	Ql_SleepEnable();  
		            SendMsg2KernelForBLEStart();		//发送消息给BLE任务,处理BLE数据
		        }
			 break;
			case MSG_IOT_TASK_GET_SENSOR_DATA:				
		        {
		           //GetUserSensorData();					//获取外设传感器数据
		        }
			 break;
			case MSG_IOT_TASK_START_MQTT:
				  Mqtt_InitConnect_Start();
				  Iotdata_Timer_init(TIMER_ID_USER_START + IOTDATA_TIMER_ID, IOTDATA_TIMER_MS);
			 break;
			case MSG_ID_FTP_RESULT_IND:
	            mprintf("\r\n<##### Restart FTP 3s later #####>\r\n");
	            Ql_Sleep(3000);
	            ftp_downfile_timer(FotaUpdate_Start_TmrId, NULL);
             break;
        	case MSG_ID_RESET_MODULE_REQ:
	            mprintf("\r\n<##### Restart the module... #####>\r\n");
	            Ql_Sleep(50);
	            Ql_Reset(0);
			 break;
	        default:
	            break;
        }
    }
}

/*!
 * @brief MQTT任务:初始化网络TCP/IP，然后建立连接到智云服,处理MQTT上下行数据,并且可以自动完成断线重连
 * \n
 *
 * @param taskId 消息通讯ID
 * @return NULL
 * \n
 * @see
 */


void user_mqtt_task(s32 TaskId)
{
    ST_MSG msg;
    while (1)
    {
        Ql_OS_GetMessage(&msg);
        switch (msg.message)
        {
        	 
        	 case MSG_IOT_TASK_START_BLE:
				  Ble_InitConnect_Start();
				  SendMsg2KernelForMqttStart();
				break;
			 case MSG_IOT_TASK_Get_ModuData:
				  GetModuleData();
				break;
			 case MSG_IOT_TASK_HeartDATA_MQTT:				
			 	   Iotdata_Timer_Stop(TIMER_ID_USER_START + IOTDATA_TIMER_ID, IOTDATA_TIMER_MS);
		           MqttPubUserHeartData();					//发送心跳数据
		           Iotdata_Timer_Start(TIMER_ID_USER_START + IOTDATA_TIMER_ID, IOTDATA_TIMER_MS);
		        break;
			 case MSG_IOT_TASK_SENDDATA_MQTT:
			 	   Iotdata_Timer_Stop(TIMER_ID_USER_START + IOTDATA_TIMER_ID, IOTDATA_TIMER_MS);
		           MqttPubUserSensorData();					//推送上行数据到服务器
		           Iotdata_Timer_Start(TIMER_ID_USER_START + IOTDATA_TIMER_ID, IOTDATA_TIMER_MS);
		        break;
			 case MSG_IOT_TASK_REDATA_MQTT:
				   MqttPubUserReSensorData();
				break;
			 case MSG_IOT_TASK_OTA_MQTT:
				   fota_app();
				break;
	        default:
	            break;

        }

    }
}


/*!
 * @brief 监视任务:防止系统死机
 * \n
 *
 * @param taskId 消息通讯ID
 * @return NULL
 * \n
 * @see
 */

void user_overlook_task(s32 TaskId)
{
    ST_MSG msg;
    while (1)
    {
        Ql_OS_GetMessage(&msg);
        switch (msg.message)
        {
	        default:
	            break;
        }

    }
}



