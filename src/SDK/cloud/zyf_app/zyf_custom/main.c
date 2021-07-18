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
 *     Set "C_PREDEF=-D __ZFY_APP__; CLOUD_SOLUTION =ZYF_SOLUTION " in gcc_makefile file. And compile the 
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


#include "ql_type.h"
#include "ql_trace.h"
#include "ql_uart.h"
#include "ql_system.h"
#include "ql_stdlib.h"
#include "ql_error.h"
#include "sys.h"
#include "lcd.h"
#include "ql_uart.h"
#include "uart.h"
#include "ril_network.h"
#include "ql_gprs.h"
#include "ql_socket.h"
#include "custom_feature_def.h"
#include "ril.h"
#include "new_socket.h"
#include "pro.h"
#include "gps.h"
#include "ql_timer.h"
#include "ql_time.h"
#include "tiem.h"
#include "flash.h"
#include "ql_gprs.h"
#include "fota_main.h"
#include "bma22x.h"
#include "bt3.h"


static u8 my_URL_Buffer[255]={0};

static u32 FotaUpdate_Start_TmrId =     (TIMER_ID_USER_START + 1);
static s32 FotaUpdate_TimeOut_TmrId =   (TIMER_ID_USER_START + 2);


static void SIM_Card_State_Ind(u32 sim_stat)
{
    switch (sim_stat)
    {
    case SIM_STAT_NOT_INSERTED:
        mprintf("<-- SIM Card Status: NOT INSERTED -->\r\n");
    	break;
    case SIM_STAT_READY:
        mprintf("<-- SIM Card Status: READY -->\r\n");
        break;
    case SIM_STAT_PIN_REQ:
        mprintf("<-- SIM Card Status: SIM PIN -->\r\n");
        break;
    case SIM_STAT_PUK_REQ:
        mprintf("<-- SIM Card Status: SIM PUK -->\r\n");
        break;
    case SIM_STAT_PH_PIN_REQ:
        mprintf("<-- SIM Card Status: PH-SIM PIN -->\r\n");
        break;
    case SIM_STAT_PH_PUK_REQ:
        mprintf("<-- SIM Card Status: PH-SIM PUK -->\r\n");
        break;
    case SIM_STAT_PIN2_REQ:
        mprintf("<-- SIM Card Status: SIM PIN2 -->\r\n");
        break;
    case SIM_STAT_PUK2_REQ:
        mprintf("<-- SIM Card Status: SIM PUK2 -->\r\n");
        break;
    case SIM_STAT_BUSY:
        mprintf("<-- SIM Card Status: BUSY -->\r\n");
        break;
    case SIM_STAT_NOT_READY:
        mprintf("<-- SIM Card Status: NOT READY -->\r\n");
        break;
    default:
        mprintf("<-- SIM Card Status: ERROR -->\r\n");
        break;
    }
}


static void ftp_downfile_timer(u32 timerId, void* param)// timer 启动开始下载文件
{
    s32 strLen;
    ST_GprsConfig apnCfg;
    Ql_memcpy(apnCfg.apnName,   APN, Ql_strlen(APN));
    Ql_memcpy(apnCfg.apnUserId, USERID, Ql_strlen(USERID));
    Ql_memcpy(apnCfg.apnPasswd, PASSWD, Ql_strlen(PASSWD));

    Ql_UART_Write(UART_PORT1, systemset.fotaaddr, strLen);

	systemset.fotaflag=2;
	SaveFlashParamsNew(&systemset);
    Ql_FOTA_StartUpgrade(systemset.fotaaddr, &apnCfg, NULL);
}



void proc_main_task(s32 taskId)
{

   u8 i;
   ST_MSG msg;
   power_drv_init();
   UartInit();
   WZinit();
   SYS_Parameter_Init();
   VIRTUAL_PORT1Init();
   //bma_i2c_init();
    while (1)
    {

	   if(systemset.fotaflag==0)
	   	{
		       Ql_OS_GetMessage(&msg);
		        switch(msg.message)
		        {
		            case MSG_ID_RIL_READY:
		                Ql_RIL_Initialize();
		                mprintf("RIL is ready\r\n");
		            break;
		            case MSG_ID_USER_START:
		                break;

		            default:
		                break;
		        
		    	}
	   	}
	   else
	   	{
	   		  	Ql_OS_GetMessage(&msg);
		        switch(msg.message)
		        {
		            case MSG_ID_RIL_READY:
		                mprintf("<-- RIL is ready -->\r\n");
		                Ql_RIL_Initialize();
		            case MSG_ID_URC_INDICATION:
		                switch (msg.param1)
		                {
		                case URC_SYS_INIT_STATE_IND:
		                    mprintf("<-- Sys Init Status %d -->\r\n", msg.param2);
		                    break;
		                case URC_CFUN_STATE_IND:
		                    mprintf("<-- CFUN Status:%d -->\r\n", msg.param2);
		                    break;
		                case URC_SIM_CARD_STATE_IND:
		                    SIM_Card_State_Ind(msg.param2);
		                    break;
		                case URC_GSM_NW_STATE_IND:
		                    mprintf("<-- GSM Network Status:%d -->\r\n", msg.param2);
		                    break;
		                case URC_GPRS_NW_STATE_IND:
		                    mprintf("<-- GPRS Network Status:%d -->\r\n", msg.param2);
		                    if (NW_STAT_REGISTERED == msg.param2)
		                    {
		                        //Ql_Timer_Stop(FotaUpdate_Start_TmrId);
		                        ftp_downfile_timer(FotaUpdate_Start_TmrId, NULL);
		                    }
		                    break;
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
		            default:
		                break;
		        }
	   	}
    }
}


void proc_subtask1(s32 TaskId)
{
	u8 setime=0;
	u8 res=0;
	u8 initflow=0,pregps=0;
	u8 initres=0;
	u16 errort=0;
	u32 waitt=0,sleept=0;
	if(BootInfo==0xFF)
		{
			Ql_Sleep(3000);
		}
	while(1)
	    {    
	         Ql_Sleep(20);
			 if(systemset.fotaflag==0)
				{
					if(BootInfo==0xFF)
					{
						if(systemset.Interval>300)
							{
								if(SystemFlow==0)
									{
										M203C_init();
										M203C_gprs_init();
										res=M203C_tcpudp_conncet((u8)systemset.TCPorUDP,systemset.CenterIP,systemset.CenterPort,0);
										if(res==1)
											{
												mprintf("\r\n++++Reset System++++\r\n");
												SaveFlashParamsNew(&systemset);
												Ql_Sleep(300);
												Ql_Reset(0);
											}
										SystemFlow=1;
									}
								if(SystemFlow==1)
									{
										 
										 M203CIsBusy=1;
										 FirstonthelineInit(&res,0x4E20);
										 setime=M203CSendDataNoAck(PROBUF,res,0);
										 if(setime)goto  OVER;
										  setime=M203CSendDataCheckOK(500);
										  if(setime)goto  OVER;
										  M203CIsBusy=0;
										  Ql_Sleep(3000);
										 if(ProRtcSetBit)
											{
												 if(gpsx.useorno!=65)
												 	{
												 		pregps=0;
												 		Ql_Sleep(3000);
														 GetLbsData();
														 M203C_EPO_Init(Jdbuf,Wdbuf);
														 M203C_Ch2Gprs();
												 	}
												  SystemFlow=2;
											}
										else
											{
												 Ql_Sleep(3000);
											}
									}
								if(SystemFlow==2&&initflow==0)
									{
										FlashTimeInit();
										bma222_init_client(0x30, 1);
										BMA250_ReadSensorData(0x30,NULL);
										BMA250_Sleep();
										GetGsmCsq();
										GetModuVol();
										GetGpsDataNCentS(60);
										if(systemset.LbsOpen)
											{
												if(gpsx.useorno!=65)
														{
															GetLbsData();
														}
											}
										SaveData2Flash(&mycoredata);
										M203CIsBusy=1;
										Ql_Sleep(1000);
										setime=CheckFlashData2Send();
										SaveDataConfig2Flash(&Position);
										if(setime)goto  OVER;
										Ql_Sleep(1000);
										M203CIsBusy=0;
										initflow=2;
										
									}
								if(SystemFlow==2&&(initflow==1||initflow==2))
									{
										waitt++;
										if(waitt>30*50)
											{
												waitt=0;
												sleept=systemset.Interval;
												systemset.SysTime+=sleept;
												SaveFlashParamsNew(&systemset);
												DisconnectSocket(0);
												Ql_Sleep(1000);
												mprintf("SendNumFromPin=%d min\r\n",sleept/60);
												systemset.CallMode=0;
												WZAysCuurtSta();
												SendNumFromPin(sleept/60);
												Ql_Sleep(1000);
												SendNumFromPin(sleept/60);
												Ql_Sleep(1000);
												SendNumFromPin(sleept/60);
												Ql_Sleep(1000);
												M203C_WHG_H;
												Ql_Sleep(7000);
												M203C_WHG_L;
												mprintf("\r\n++++System Sleep++++\r\n");
											}
									}
							}
						else
							{
								if(SystemFlow==0)
									{
										M203C_init();
										BtNewInit();
										M203C_gprs_init();
										res=M203C_tcpudp_conncet((u8)systemset.TCPorUDP,systemset.CenterIP,systemset.CenterPort,0);
										if(res==1)
											{
												mprintf("\r\n++++Reset System++++\r\n");
												SaveFlashParamsNew(&systemset);
												Ql_Sleep(300);
												Ql_Reset(0);
											}
										SystemFlow=1;
									}
							if(SystemFlow==1&&initflow==0)
								{
									 M203CIsBusy=1;
#ifdef _USE_LBS_
									  if(gpsx.useorno!=65)
									  	{
									  		Ql_Sleep(8000);
									  		GetLbsData();
									  	}
#endif
									  bma222_init_client(0x30, 1);
REREG:
									  FirstonthelineInit(&res,0x4E20);
									 	setime=M203CSendDataNoAck(PROBUF,res,0);
										if(setime)goto  OVER;
										setime=M203CSendDataCheckOK(500);
										if(setime)goto  OVER;
									  M203CIsBusy=0;
									  initflow=1;
									  Ql_Sleep(4000);
								}
							if(SystemFlow==1&&initflow==1)
								{
									
									Ql_Sleep(1000);
									if(ProRtcSetBit)
										{
											 if(gpsx.useorno!=65)
											 	{
											 		M203CIsBusy=1;
											 		pregps=0;
											 		Ql_Sleep(4000);
													 GetLbsData();
													 M203C_EPO_Init(Jdbuf,Wdbuf);
													 M203C_Ch2Gprs();
													  M203CIsBusy=0;
											 	}
											  Ql_Sleep(1000);
											  initflow=2;
											
										}
									else
										{
											 Ql_Sleep(1000);
											 goto REREG;
										}
									 
								}
							
							if(SystemFlow==1&&initflow==2)
								{
									FlashTimeInit();
									bma222_init_client(0x30, 1);
									BMA250_ReadSensorData(0x30,NULL);
									BMA250_Sleep();
									GetGsmCsq();
									GetModuVol();
									GetGpsDataNCentS(1);
										if(systemset.LbsOpen)
											{
												if(gpsx.useorno!=65)
														{
															GetLbsData();
														}
											}
									SaveData2Flash(&mycoredata);
									M203CIsBusy=1;
									Ql_Sleep(1000);
									setime=CheckFlashData2Send();
									M203CIsBusy=0;
									SaveDataConfig2Flash(&Position);
									if(setime)goto  OVER;
									Ql_Sleep(1000);
									 SystemFlow=2;
								}
OVER:
							if(setime)
								{
									mprintf("\r\n++++Reset System++++\r\n");
									SaveFlashParamsNew(&systemset);
									Ql_Sleep(300);
									Ql_Reset(0);
								}
							}

					}
				else
					{
						if(initres==0)
							{
								initres=1;
								mprintf("\r\n模块未设置SN号码,请先设置SN号码!!\r\n");
								mprintf("设置命令: $setsn xxxxxxxxxx\r\n");
								mprintf("提示:SN号码贴在模块的标签上面\r\n");
							}
						errort++;
						if(errort>1000)
							{
								errort=0;
								mprintf("\r\n!!!未收到本地设置指令,开启网络同步SN号码!!!\r\n");
								GetSnFromSocket();
								
							}
					}
				}
	    }    
}


void proc_subtask2(s32 TaskId)
{
   u8 t=0,num;
   u8 setime;
    while(1)
    {
    	Ql_Sleep(100);
		if(systemset.fotaflag==0)
			{
				CheckBattaryS();
				if(SystemFlow==2)
					{
						if(systemset.Interval>300)
							{
								Ql_Sleep(10);
							}
						else
							{
								jishit++;
								if(jishit>=(systemset.Interval*10))
									{
										mprintf("\r\njishit time out\r\n");
										FlashTimeInit();
										BMA250_ReadSensorData(0x30,NULL);
										GetGsmCsq();
										GetModuVol();
										GetGpsData();
										if(systemset.LbsOpen)
											{
												if(gpsx.useorno!=65)
														{
															GetLbsData();
														}
											}
										jishit=0;
										jishis=0;
										M203CIsBusy=1;
										Ql_Sleep(1000);
										mprintf("\r\nM203CSendData\r\n");
										Ch2Protocol(mycoredata);
										CoreDataInit_v2(&num,0x4E22);
										setime=M203CSendDataNoAck(PROBUF,num,0);
										if(!setime)
											{
												setime=M203CSendDataCheckOK(500);
											}
										if(setime)
											 {
											  	mprintf("\r\n++++Reset System++++\r\n");
												SaveFlashParamsNew(&systemset);
												Ql_Sleep(300);
												Ql_Reset(0);
											  }
										Ql_Sleep(1000);
										M203CIsBusy=0;
									}
								else
									{
										if(gpsx.useorno!=65)
											{
												jishis++;
												if(jishis>=10)
													{
														jishis=0;
														GetGpsData();
													}
											}
									}
							}
						
					}
			}
    }
	
}



void proc_subtask3(s32 TaskId)
{

     while(1)
    {
    	Ql_Sleep(1000);
		if(systemset.fotaflag==0)
			{
				systemset.SysTime++;
				if(SystemFlow==0&&systimes.sta==0)
					{
						systimes.sockett++;
					}
				if(SystemFlow==1&&systimes.sta==0)
					{
						mprintf("\r\n***********Network Time  = %d s*************\r\n",systimes.sockett);
						networkTime=systimes.sockett;
						systimes.sta=1;
						systimes.sockett=0;
					}
				if(SystemFlow==2&&systimes.sta==1)
					{
						systimes.gpst++;
						if(gpsx.useorno==65)
							{
								mprintf("\r\n***********GpsPositioning Time  = %d s*************\r\n",systimes.gpst);
								gpsTime=systimes.gpst;
								systimes.sta=2;
								systimes.gpst=0;
							}
					}
				if(gpsx.useorno!=65)
				  	{
				  		LED3_L;
				  	}
				  else
				  	{
				  		LED3_H;
				  	}
			}
    }
}


void proc_subtask4(s32 TaskId)
{
    
     while(1)
    {
    	Ql_Sleep(10);
		if(systemset.fotaflag==0)
			{
				if(SystemFlow==1||SystemFlow==2)
						{
							Recive_OutData();
						}
			}
    }
}



void proc_subtask5(s32 TaskId)
{
    u16 i;
    while(1)
    {
    	Ql_Sleep(10);
		if(SystemFlow==0)
			{
				Ql_Sleep(200);
				LED1_H;
				Ql_Sleep(200);
				LED1_L;
				Ql_Sleep(200);
				LED1_H;
				Ql_Sleep(200);
				LED1_L;
				Ql_Sleep(1500);
			}
		if(SystemFlow==1||SystemFlow==2)
		{
			while(M203CIsBusy==1)
				{
					LED1_H;
					Ql_Sleep(50);
					LED1_L;
					Ql_Sleep(50);
				}
			LED1_H;
			Ql_Sleep(500);
			LED1_L;
			Ql_Sleep(500);
		}
    }
}



