
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
 *     Set "C_PREDEF=-D __ZFY_MQTT_PEDOMETER__; CLOUD_SOLUTION =ZYF_MQTT_PEDOMETER_SOLUTION " in gcc_makefile file. And compile the 
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
#include "ads1151.h"
#include "module_data.h"
#include "mpu6050.h"
#include "gps.h"
#include "tiem.h"
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
                    if(gpspowersta == 0)
                    {
                        gpspowersta = 1;
                        GpsOpen();
                    }
                    Ql_SleepEnable();  
		            SendMsg2KernelForBLEStart();		//发送消息给BLE任务,处理BLE数据
		        }
			 break;
			case MSG_IOT_TASK_GET_SENSOR_DATA:				
		        {
		           //GetUserSensorData();					//获取外设传感器数据
		        }
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
			case MSG_IOT_TASK_START_JUDGE_GSMGPRS:
                JudgeGpsAndGSM();
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
        	 
        	 case MSG_IOT_TASK_START_BLE:           //开启蓝牙
				  Ble_InitConnect_Start();
				  SendMsg2KernelForMqttStart();     //开始连接网络
				break;
			 case MSG_IOT_TASK_Get_ModuData:
				  GetModuleData();                  //获取模块数据
				break;
			 case MSG_IOT_TASK_HeartDATA_MQTT:				
			 	   Iotdata_Timer_Stop(TIMER_ID_USER_START + IOTDATA_TIMER_ID, IOTDATA_TIMER_MS); //关闭定时器
		           MqttPubUserHeartData();					//发送心跳数据
		           Iotdata_Timer_Start(TIMER_ID_USER_START + IOTDATA_TIMER_ID, IOTDATA_TIMER_MS);   //开启定时器
		        break;
			 case MSG_IOT_TASK_SENDDATA_MQTT:
			 	   Iotdata_Timer_Stop(TIMER_ID_USER_START + IOTDATA_TIMER_ID, IOTDATA_TIMER_MS);
		           MqttPubUserSensorData();					//推送上行数据到服务器
		           SendMsg2KernelForMcuOff();
		           Iotdata_Timer_Start(TIMER_ID_USER_START + IOTDATA_TIMER_ID, IOTDATA_TIMER_MS);
		        break;
			 case MSG_IOT_TASK_REDATA_MQTT:
				   MqttPubUserReSensorData();       //回复下行数据
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
            case MSG_IOT_TASK_PMTK_SET_STATIC_NAV_THD:
                Speed_threshold();
	            break;
			case MSG_IOT_TASK_GET_LOCATION:
//                iRet = GetGpsLocation(45,1); //信号极差 不使用gps定位 直接保存上一次的数据点
                SaveGpsdatapoint(msg.param1); //不使用定位功能 
				break;
			case MSG_IOT_TASK_SENDSAVEDATA_MQTT:
				   MqttPubUserSaveSensorData();
                   Ql_Sleep(1000);              //不能省去否则无法升级
                   SendMsg2KernelForIotData();
                break;
			case MSG_IOT_TASK_MCU_OFF:
#if defined(USE_MPU6050) || defined(USE_ADS1115) || defined(USE_BOTH_MPU6050_ADS1115)
					break;
#endif
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
				 setmcu2poweroff203c(&msg.param1);
				 Ql_PowerDown(1);
				break;
        }

    }
}
/*!
 * @brief adc读取任务\n
 *
 * @param taskId  
 * @return NULL
 * \n
 * @see
 */

void proc_subtask1(s32 TaskId)
{
#if defined(USE_ADS1115) || defined(USE_BOTH_MPU6050_ADS1115)	//参考sys.h
	int vol1,vol2,vol3,vol4;
	int i;
     int vol[20] = {0};
	while(1)
		{
		
		    for(i=0;i<20;i++)
		    	{
		    	vol[i]=Get_ATOD(0);
		    	}
			   vol1=vol[19];			
			for(i=0;i<20;i++)
				{
				vol[i]=Get_ATOD(1);
				}
			vol2=vol[19];
			for(i=0;i<20;i++)
				{
				vol[i]=Get_ATOD(2);
			}
			vol3=vol[19]; 
            for(i=0;i<20;i++)
				{
				vol[i]=Get_ATOD(3);
				}
			vol4=vol[19];
			module_data.vol1=vol1;
			module_data.vol2=vol2;
			module_data.vol3=vol3;
			module_data.vol4=vol4;
			mprintf("vol1:%d,vol2:%d,vol3:%d,vol4:%d\r\n",vol1,vol2,vol3,vol4);
			Ql_Sleep(1500);
		    
    }
#else
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

#endif
    
}


/*!
 * @brief mpu6050处理任务 \n
 *
 * @param taskId 
 * @return NULL
 * \n
 * @see
 */

void proc_subtask2(s32 TaskId)
{
	 Ql_Sleep(3000);
#if defined(USE_MPU6050) || defined(USE_BOTH_MPU6050_ADS1115)	//参考sys.h
	 CountStepInit();
	 MPU_Wakeup();
	 while (1)
	 	{
	      
	 		Ql_Sleep(10);
			     module_data.step=CountStep();
			
	    }	
#else
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
#endif
}
	 	

void proc_subtask3(s32 TaskId)
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


