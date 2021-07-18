
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
 *     Set "C_PREDEF=-D __ZFY_TRACKER_MQTT_APP__; CLOUD_SOLUTION =ZYF_TRACKER_MQTT_SOLUTION " in gcc_makefile file. And compile the 
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
#include "gps.h"
#include "tiem.h"
#include "module_data.h"
#include "ril.h"
#include "ril_sim.h"
#include "ril_network.h"
#include "logfile.h"
#include "flash.h"
 

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
    TEST_Timer_init(TIMER_ID_USER_START + HEART_TIMER_ID, 5000);
    s_iflashMutex = Ql_OS_CreateMutex("FlashMutex");
    while (1)
    {
        
        Ql_OS_GetMessage(&msg);
        switch (msg.message)
        {
	        case MSG_ID_RIL_READY:				
		        {
		            Ql_RIL_Initialize();				//等待RIL层初始化完成,才可以调用AT CMD指令,用户无需关心
		            DisUpdateRTC();
                    if(systemset.snuser)
                    {
                        Ql_SleepEnable();
                        JudgeGpsAndGSM();               //判断是否打开gps以及是否保存数据点
                    }
                    Ql_Timer_Start(TIMER_ID_USER_START + HEART_TIMER_ID,5000,TRUE);     //开启电量检测timer
                }
			 break;
            case MSG_IOT_TASK_START_JUDGE_GSMGPRS:
                JudgeGpsAndGSM();
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
				break;
			 case MSG_IOT_TASK_Get_ModuData:
				  GetModuleData();
				break;
			 case MSG_IOT_TASK_HeartDATA_MQTT:				
		           MqttPubUserHeartData();					//发送心跳数据
		        break;
			 case MSG_IOT_TASK_SENDDATA_MQTT:
		           MqttPubUserSensorData();					//推送上行数据到服务器
		           if(YorNupgrading == 0)                   //是否升级标志
		           {
                        nextfaststart = 0;//
				        SendMsg2KernelForMcuOffP(&nextfaststart);				//采集数据完成,可以通知MCU了
		           }
                break;
             case MSG_IOT_TASK_ERR_LOG:
				  	LoadLogbuf();                   //从文件读出日志数据
					ClearLogbuf();                  //清除日志数据
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
    s32 iRet = 0;
    while (1)
    {
        Ql_OS_GetMessage(&msg);
        switch (msg.message)
        {
        	case MSG_IOT_TASK_MCU_OFF:
                 if(user_mqtt_clint_sta != USER_MQTT_GPRS_INIT && systemset.Interval < 3600)
                 {
				    MqttPubUserDisData();
                    GetSysTimetemp(&time);
                    SetSysTime(&time);      //重新设置时间防止使用epo时设置时间少8个时区
				    Ql_Sleep(3000);
                 }
                 else if(user_mqtt_clint_sta != USER_MQTT_GPRS_INIT)
                 {
                    MqttPubUserDisData();
                    Ql_Sleep(3000);
                 }
                 GetSysTime(&time);
				 //setmcu2poweroff203c(msg.param1);       //发送到stm8休眠
				 setmcu2poweroff203c(&msg.param1);
				 Ql_PowerDown(1);
				break;
			 case MSG_IOT_TASK_REDATA_MQTT:
				   MqttPubUserReSensorData();
                break;
             case MSG_IOT_TASK_SENDSAVEDATA_MQTT:
				   MqttPubUserSaveSensorData();
                   Ql_Sleep(1000);              //不能省去否则无法升级
                   SendMsg2KernelForIotData();
                break;
             case MSG_IOT_TASK_GET_LOCATION:
//                iRet = GetGpsLocation(45,1); //信号极差 不使用gps定位 直接保存上一次的数据点
                SaveGpsdatapoint(msg.param1); //不使用定位功能 
				break;
            case MSG_IOT_TASK_PMTK_SET_STATIC_NAV_THD:
                Speed_threshold();
	        default:
	            break;
        }

    }
}

void proc_subtask1(s32 TaskId)
{
	ST_MSG msg;
    while (1)
    {
        Ql_OS_GetMessage(&msg);
        switch (msg.message)
       	{
        	case MSG_IOT_TASK_START_MQTT:
				  Mqtt_InitConnect_Start();
				  Iotdata_Timer_init(TIMER_ID_USER_START + IOTDATA_TIMER_ID, IOTDATA_TIMER_MS);
				  break;

			case MSG_IOT_TASK_OTA_MQTT:
				  fota_app();
				  break;
       	}
	}
}

