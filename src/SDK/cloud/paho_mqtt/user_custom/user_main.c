

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
 *     Set "C_PREDEF=-D __PAHO_MQTT_APP__; CLOUD_SOLUTION =PAHO_MQTT_SOLUTION " in gcc_makefile file. And compile the 
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




#include <stdint.h>
#include <string.h>

#include "ril.h"
#include "ril_util.h"
#include "ril_telephony.h"
#include "ql_stdlib.h"
#include "ql_error.h"
#include "ql_trace.h"
#include "ql_uart.h"
#include "ql_system.h"
#include "user_lmq.h"
#include "uart.h"
#include "user_mqtt.h"
#include "ql_timer.h"
#include "sys.h"
#include "m203c_ble.h"


#define BLE_TIMER_ID		0x04
#define BLE_TIMER_MS		1000

u32 MyMutexID=0;



static void Callback_BLE_Timer(u32 timerId, void* param)
{
	if(ble_mqtt_info.mqttsta)
		{
			if(ble_mqtt_info.heartping>=120)
					{
							MyMutexID=1;
							if(ble_mqtt_info.blesta==1)
								{
									ScanBlecom(0,"ABCD",10);
									ble_mqtt_info.blesta=0;
								}
							Send_BleList2Server();
							ble_infos.index=0;
							ble_mqtt_info.heartping=0;
							MyMutexID=0;

					}
				mprintf("heartping=%d\r\n",ble_mqtt_info.heartping);
				ble_mqtt_info.heartping++;
		}
}


void BLE_Timer_init(u32 TIMER_ID, u32 ms)
{
    Ql_Timer_Register(TIMER_ID, Callback_BLE_Timer, NULL);
    Ql_Timer_Start(TIMER_ID,ms,TRUE);
}



void proc_main_task(s32 taskId)
{
	ST_MSG msg;	
    M203C_BSP_init();
	systemset.Interval=120;
	ble_mqtt_info.blesta=0;
	ble_mqtt_info.heartping=0;
	ble_mqtt_info.mqttsta=0;
	ble_info_listInit();
	BLE_Timer_init(TIMER_ID_USER_START + BLE_TIMER_ID, BLE_TIMER_MS);
    while (1)
    {
        
        Ql_OS_GetMessage(&msg);
        switch (msg.message)
        {
	        case MSG_ID_RIL_READY:				
		        {
		            Ql_RIL_Initialize();
					Ble_PowerOn();
			        RIL_BT_GetLeLocalAddr(bleaddr, 12);
					Ble_PowerOff();
					RIL_BT_Setcfg_M20(2,30);
			        mprintf("\r\n<--bleaddr:%s:-->\r\n", bleaddr);
					User_PahoMqtt_Test(0);
		        }
			 break;
	        default:
	            break;
        }
    }
}

void proc_subtask1(s32 TaskId)
{
	while (1)
	 	{
	 		Ql_Sleep(100);
			RdsubData();
	 	}
}


void proc_subtask2(s32 TaskId)
{
	
	while (1)
	 	{
	 		Ql_Sleep(100);
			if(ble_mqtt_info.mqttsta)
				{
					if(ble_sta_t)
						{
							Ql_Sleep(1000);
							if(ble_mqtt_info.blesta==0)
								{
									if(MyMutexID==0)
										{
											ble_sta_t=0;
											ble_mqtt_info.blesta=1;
											ScanBlecom(1,"ABCD",1);
										}
								}
						}
				}
	 	}
}














