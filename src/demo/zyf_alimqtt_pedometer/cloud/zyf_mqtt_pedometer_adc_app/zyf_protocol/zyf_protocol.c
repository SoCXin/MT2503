
/******************************************************************************

                  版权所有 (C) 2018, 天创金农科技

 ******************************************************************************
  文件   : zyf_protocol.c
  版本   : 初稿
  作者   : LiCM
  日期   : 2018年02月24日
  内容   : 创建文件
  描述   : 智云服MQTT版本通信协议

  修改记录:
  日期   : 2018年02月24日
    作者   : LiCM
    内容   : 创建文件

******************************************************************************/

#include "ril.h"
#include "ril_util.h"
#include "ril_telephony.h"
#include "ql_stdlib.h"
#include "ql_error.h"
#include "ql_trace.h"
#include "ql_uart.h"
#include "ql_system.h"
#include "ql_gpio.h"
#include "ql_eint.h"
#include "zyf_protocol.h"
#include "uart.h"
#include "sys.h"
#include "module_data.h"
#include "cJSON.h"
#include "m203c_ble.h"
#include "zyf_auto_config.h"
#include "user_mqtt.h"
#include "fota.h"
#include "gps.h"
#include "tiem.h"

const char* alink_headstr="{\"method\":\"thing.event.property.post\",\"id\":\"1\",\"params\":{%s},\"version\":\"1.0\"}";
const char* topic_headstr="{\"method\":\"thing.event.property.post\",\"id\":\"1\",\"params\":%s,\"version\":\"1.0\"}";
u8 alinkvaluebuf[ALINK_MAX_BUF];


stm_data stm_sensor;
frt_stm_data frt_sensor;

#ifdef ZYF_PRO_DEBUG
    #define zyf_protocol_info(format, ...)  mprintf( format "\r\n", ##__VA_ARGS__)
    #define zyf_protocol_error(format, ...) mprintf( "[error]%s() %d " format "\r\n", /*__FILE__,*/ __FUNCTION__, __LINE__, ##__VA_ARGS__)
#else
    #define zyf_protocol_info(format, ...)
    #define zyf_protocol_error(format, ...)
#endif


volatile u8 ZYF_MQTT_CONFIG=0;

u8 ZYFBleCmdBuf[31];

u8 ZYFRECMD[2][100];


u8 Jdbuf[20];
u8 Wdbuf[20];
u8 g_epo_init_sta=0;


s32 Gnss_Epo_Init(u8 *lot,u8 *lat)
{
	s32  ret = RIL_AT_SUCCESS;
	char strAT[50] = {0};
	u16  atLength = 0;

	if(g_epo_init_sta==0)
		{
	
			RIL_GPS_EPO_Config_APN("CMNET","","");
            Ql_Sleep(50);

			SetUtc2Modu();
            Ql_Sleep(50);

			Ql_memset(strAT, 0x0, sizeof(strAT));

			atLength = Ql_sprintf(strAT,"AT+QGREFLOC=%s,%s",lat,lot);

			ret = Ql_RIL_SendATCmd(strAT, atLength, NULL, NULL, 0);

			if(RIL_ATRSP_SUCCESS != ret)
			{
				return ret;
			}
            Ql_Sleep(50);
			
			ret = RIL_GPS_EPO_Enable(1);
						
		    if(RIL_AT_SUCCESS != ret) 
		       {
		          mprintf("Set EPO status to 1 fail, iRet = %d.\r\n",ret);
		          return ret;
		       }
            Ql_Sleep(50);
			ret = RIL_GPS_EPO_Aid();
						
		    if(RIL_AT_SUCCESS != ret) 
		        {
		           mprintf("EPO aiding fail, iRet = %d.\r\n", ret);
		           return ret;
		        }
		    mprintf("EPO aiding successful, iRet = %d.\r\n", ret);

			g_epo_init_sta=1;
		}
	
	return ret;	
}



s32 LbsAEpoStaInit(void)
{
	s32  ret = RIL_AT_FAILED;

//	GetSysTime(&time);  
//    mprintf("LbsAEpoStaInitDataTime: %04d-%02d-%02d %02d:%02d:%02d\r\n",time.year, time.month, time.day, time.hour, time.minute, time.second);
	ret=WaitLbsGetLocation(600);
	if(ret!=0)
		{
			 return ret;
		}
	ret=Gnss_Epo_Init(Jdbuf,Wdbuf);
	return ret;
	
}


static u8 isInChina(u32 longitude,u32 latitude)
{
    // 73°33′E~135°05′E 中国区域 3°51′N~53°33′N
    if((longitude >= 73666666 && longitude <= 135041666) && (latitude >= 3866666 && latitude <= 53550000))
    {
        return 1;
    }
    else
    {
        zyf_protocol_info("the point is not in China region!!!\r\n");
        return 0;
    }
}



s32 alink_msg_RegSensor_data(u8 *data_ptr,void *user_data)
{
	s32 ret,len;
	Ql_sprintf(data_ptr,"{\"method\":\"thing.event.property.post\",\"id\":\"1\",\"params\":{\"EnvironmentTemperature\":30,\"EnvironmentHumidity\":50},\"version\":\"1.0\"}");
	len = Ql_strlen(data_ptr);
	return len;
}

s32 alink_msg_Save_Sensor_data(u8 *data_ptr,void *user_data)
{
	s32 ret,len;
    int index = -1;
    u8 *buf_String;
	u8 tempbuf[20]={0};
    cJSON * root =  cJSON_CreateObject();
    if(user_data != NULL) index = *(int *)user_data;
    _save_location_point *point = NULL;
    if(index != -1) 
        point = savedata.save_point + index;
    else
        point = savedata.save_point;
    
	
	//GetModuleData();


	//GetSysTime(&time);
	Ql_sprintf(tempbuf,"%02d-%02d-%02d %02d:%02d:%02d",point->collecttime.year -2000, point->collecttime.mon, 
	point->collecttime.day, point->collecttime.hour, point->collecttime.min,point->collecttime.sec);
	cJSON_AddItemToObject(root, AutomaticRD[4], cJSON_CreateString(tempbuf));

    if(isInChina(point->longitude,point->latitude))
    {
        if(savedata.gorlflag == 1)
        {

        	Ql_sprintf(tempbuf,"%d.%06d",point->latitude/1000000,point->latitude%1000000);
        	cJSON_AddItemToObject(root, AutomaticRD[2], cJSON_CreateString(tempbuf));
        	Ql_sprintf(tempbuf,"%d.%06d",point->longitude/1000000,point->longitude%1000000);
        	cJSON_AddItemToObject(root, AutomaticRD[3], cJSON_CreateString(tempbuf));
        }
        else if(savedata.gorlflag == 2)
        {
        	Ql_sprintf(tempbuf,"%d.%06d",point->latitude/1000000,point->latitude%1000000);
            cJSON_AddItemToObject(root, AutomaticRD[0], cJSON_CreateString(tempbuf));
        	Ql_sprintf(tempbuf,"%d.%06d",point->longitude/1000000,point->longitude%1000000);
            cJSON_AddItemToObject(root, AutomaticRD[1], cJSON_CreateString(tempbuf));

        }
    }
	Ql_sprintf(tempbuf,"%d",point->bvol);
    cJSON_AddItemToObject(root, AutomaticRD[5], cJSON_CreateString(tempbuf));
	Ql_sprintf(tempbuf,"%d",point->csq);
    cJSON_AddItemToObject(root, AutomaticRD[6], cJSON_CreateString(tempbuf));
	Ql_sprintf(tempbuf,"%d",point->sysruntime);
    cJSON_AddItemToObject(root, AutomaticRD[7], cJSON_CreateString(tempbuf));
	buf_String=cJSON_Print(root);
	len = Ql_strlen(buf_String);
	Ql_strcpy(data_ptr,buf_String);
	zyf_protocol_info("%s",data_ptr);
	cJSON_Delete(root);
	Ql_MEM_Free(buf_String);
	return len;
	
}

s32 alink_msg_Sensor_data(u8 *data_ptr,void *user_data)
{
	s32 ret,len;
    u8 *buf_String;
	u8 tempbuf[100]={0};
	

/*
//	if(systemset.Interval>=3600 || module_data.csq < 12) //当上传间隔小于 1小时时 使用 gsm  信号判断是否使用gps
    if(systemset.Interval>=3600)
		{
			//LBSDataInit();
			ret=WaitLbsGetLocation(200);
			if(ret==0)
				{
                    if(isInChina( LbsJingdu,LbsWeidu))
                    {
                        Set_FirstData_location(2,LbsJingdu,LbsWeidu);
    					Ql_sprintf(tempbuf,"%d.%06d",LbsWeidu/1000000,LbsWeidu%1000000);
    					Ql_sprintf(tempbuf,"%d.%06d",LbsJingdu/1000000,LbsJingdu%1000000);
                    }
				}
		}
	else
	*/{
	
			ret=GetGpsLocation(60,1);
			if(ret==0)
				{
                    if(isInChina(gpsx.longitude, gpsx.latitude))
                    {
                        Set_FirstData_location(1,gpsx.longitude,gpsx.latitude);
						Ql_memset(alinkvaluebuf, 0, sizeof(alinkvaluebuf));
    					Ql_sprintf(tempbuf,"\"latitudeGps\":%d.%06d,",gpsx.latitude/1000000,gpsx.latitude%1000000);
						Ql_strcat(alinkvaluebuf,tempbuf);
    					Ql_sprintf(tempbuf,"\"longitudeGps\":%d.%06d,",gpsx.longitude/1000000,gpsx.longitude%1000000);
						Ql_strcat(alinkvaluebuf,tempbuf);
                    }
				}
			else
				{
					if(LbsWeidu==0||LbsJingdu==0)
						{
							//LBSDataInit();
							ret=WaitLbsGetLocation(200);
							if(ret==0)
								{
                                    if(isInChina(LbsJingdu, LbsWeidu))
                                    {
                                        Set_FirstData_location(2,LbsJingdu,LbsWeidu);
										Ql_memset(alinkvaluebuf, 0, sizeof(alinkvaluebuf));
    									Ql_sprintf(tempbuf,"\"latitudeLbs\":%d.%06d,",LbsWeidu/1000000,LbsWeidu%1000000);
										Ql_strcat(alinkvaluebuf,tempbuf);
    									Ql_sprintf(tempbuf,"\"longitudeLbs\":%d.%06d,",LbsJingdu/1000000,LbsJingdu%1000000);
										Ql_strcat(alinkvaluebuf,tempbuf);
                                    }
								}
						}
					else
						{
                            if(isInChina(LbsJingdu, LbsWeidu))
                            {
                                Set_FirstData_location(2,LbsJingdu,LbsWeidu);
								Ql_memset(alinkvaluebuf, 0, sizeof(alinkvaluebuf));
    							Ql_sprintf(tempbuf,"\"latitudeLbs\":%d.%06d,",LbsWeidu/1000000,LbsWeidu%1000000);
								Ql_strcat(alinkvaluebuf,tempbuf);
    							Ql_sprintf(tempbuf,"\"longitudeLbs\":%d.%06d,",LbsJingdu/1000000,LbsJingdu%1000000);
								Ql_strcat(alinkvaluebuf,tempbuf);
                            }
						}
                    
				}
		}
	Ql_sprintf(tempbuf,"\"step\":%d,",module_data.step);
	Ql_strcat(alinkvaluebuf,tempbuf);
	Ql_sprintf(tempbuf,"\"vol1\":%d,",module_data.vol1);
	Ql_strcat(alinkvaluebuf,tempbuf);
	Ql_sprintf(tempbuf,"\"vol2\":%d,",module_data.vol2);
	Ql_strcat(alinkvaluebuf,tempbuf);
	Ql_sprintf(tempbuf,"\"vol3\":%d,",module_data.vol3);
	Ql_strcat(alinkvaluebuf,tempbuf);
	Ql_sprintf(tempbuf,"\"vol4\":%d,",module_data.vol4);
	Ql_strcat(alinkvaluebuf,tempbuf);
	GetSysTime(&time);
	Ql_sprintf(tempbuf,"\"collectTime\":\"%02d-%02d-%02d %02d:%02d:00\",",time.year-2000, time.month, time.day, time.hour, time.minute);
	Ql_strcat(alinkvaluebuf,tempbuf);
	GetModuleData();
	Ql_sprintf(tempbuf,"\"battyPercent\":%d,",G_Bvol);
	Ql_strcat(alinkvaluebuf,tempbuf);
	Ql_sprintf(tempbuf,"\"signallev\":%d,",module_data.csq);
	Ql_strcat(alinkvaluebuf,tempbuf);
	if(systemset.SysTime<1)systemset.SysTime++;
	Ql_sprintf(tempbuf,"\"runTime\":%d",systemset.SysTime);
	Ql_strcat(alinkvaluebuf,tempbuf);
	Ql_snprintf(data_ptr,1024,alink_headstr,alinkvaluebuf);
	len = Ql_strlen(data_ptr);
	return len;
	
}

s32 zyf_msg_register_data(u8 *data_ptr,void *user_data)
{
#ifdef _NOT_TEST_
		Ql_sprintf(data_ptr,"hello mqtt");
		return Ql_strlen(data_ptr);
#else
		s32 ret,len;
        u8 *buf_String;
		u8 tempbuf[40]={0};
		GetSysTime(&time);
			
		cJSON * root =  cJSON_CreateObject();
		Ql_strcpy(tempbuf,systemset.SN);
		cJSON_AddItemToObject(root, "sn", cJSON_CreateString(tempbuf));
		ret =RIL_GetIMEI(tempbuf);
		if(ret!=RIL_AT_SUCCESS)
		 	{
		 		zyf_protocol_error("RIL_GetIMEI Error");
		 	}
		cJSON_AddItemToObject(root, "imei", cJSON_CreateString(tempbuf));
		ret =RIL_SIM_GetCCID(tempbuf);
	    if(ret!=RIL_AT_SUCCESS)
	 	{
	 		zyf_protocol_error("RIL_SIM_GetCCID Error");
	 	}
		cJSON_AddItemToObject(root, "iccid", cJSON_CreateString(tempbuf));
		ret =RIL_SIM_GetIMSI(tempbuf);
		if(ret!=RIL_AT_SUCCESS)
		 	{
		 		zyf_protocol_error("RIL_SIM_GetIMSI Error");
		 	}
		cJSON_AddItemToObject(root, "imsi", cJSON_CreateString(tempbuf));

		cJSON_AddItemToObject(root, "pk", cJSON_CreateString(systemset.key));

		Ql_memset(tempbuf,0,sizeof(tempbuf));

		Ql_sprintf(tempbuf,"%d",systemset.updateinfo);

		cJSON_AddItemToObject(root, "mode", cJSON_CreateString(tempbuf));

		cJSON_AddItemToObject(root, "version", cJSON_CreateString(SW_VERSION));

		if(systemset.updateinfo==1)
			{

				systemset.updateinfo=0;
				SaveFlashParamsNew(&systemset);
			}
		
		buf_String=cJSON_Print(root);
		len = Ql_strlen(buf_String);
		Ql_strcpy(data_ptr,buf_String);

		zyf_protocol_info("%s",data_ptr);
		cJSON_Delete(root);
		Ql_MEM_Free(buf_String);
		return len;
		
#endif
}

s32 alink_msg_data(u8 *data_ptr,void *user_data)
{
	u8 res;
	s32 ret,len;
	u8 tempbuf[200]={0};
	

	Ql_memset(alinkvaluebuf, 0, sizeof(alinkvaluebuf));


	GetSysTime(&time);
	Ql_sprintf(tempbuf,"\"collectTime\":\"%02d-%02d-%02d %02d:%02d:00\",",time.year-2000, time.month, time.day, time.hour, time.minute);
	Ql_strcat(alinkvaluebuf,tempbuf);
	GetModuleData();
	//mprintf("\r\n33333333333\r\n");
	Ql_sprintf(tempbuf,"\"battyPercent\":%d,",G_Bvol);
	Ql_strcat(alinkvaluebuf,tempbuf);
	Ql_sprintf(tempbuf,"\"signallev\":%d,",module_data.csq);
	Ql_strcat(alinkvaluebuf,tempbuf);
	if(systemset.SysTime<1)systemset.SysTime++;
	Ql_sprintf(tempbuf,"\"runTime\":%d",systemset.SysTime);
	Ql_strcat(alinkvaluebuf,tempbuf);

	
	Ql_snprintf(data_ptr,1024,alink_headstr,alinkvaluebuf);
	len = Ql_strlen(data_ptr);
	return len;

}

s32 zyf_msg_Dis_data(u8 *data_ptr)
{
	u8 res;
	s32 ret,len;
	u8 tempbuf[100]={0};
	

	Ql_memset(alinkvaluebuf, 0, sizeof(alinkvaluebuf));


	Ql_sprintf(tempbuf,"\"sn\":\"%s\"",systemset.SN);

    Ql_strcat(alinkvaluebuf,tempbuf);
    len = Ql_snprintf(data_ptr,1024,alink_headstr,alinkvaluebuf);



	return len;
	
}


s32 alink_ays_cmd(char *src,char *key,char *value)
{
	u8 *p=NULL;
	p=Ql_strstr(src,"{");
	if(p!=NULL)
		{
			while(*p!='"')p++;
			p++;
			while(*p!='"')
				{
					*key=*p;
					p++;
					key++;
				}
			*key=0;
			while(*p!=':')p++;
			p++;
            if(*p == ' ') while(*p!='"') p++;
            if(*p == '"') p++;
			while(*p!='}' && *p != '"')
				{
					*value=*p;
					p++;
					value++;
				}
			*value=0;
			return 0;
		}
	return -1;
}


s32 alink_decode_cmd(char *Payload,u16 len)
{
	u8 *p=NULL;
	u8 key[50]={0};
	u8 value[100]={0};
	s32 ret;
	p=Ql_strstr(Payload,"params");
	if(p!=NULL)
		{
			alink_ays_cmd(p,key,value);
			ret=Ql_strcmp(key,"ledSwitch");
			if(ret==0)
				{
					if(strtol((const char*)value,NULL,10))
						LED1_H;	
					else
						LED1_L;
				}
			ret=Ql_strcmp(key,"transTime");
			if(ret==0)
				{
					systemset.Interval=(strtol((const char*)value,NULL,10));
					SaveFlashParamsNew(&systemset);
					IotData_T=0;
				}
			ret=Ql_strcmp(key, "update");
			if(ret==0)
				{
					YorNupgrading = 1;
					systemset.updateinfo=1;
					Ql_sscanf(value, "%[^,],",fotainfo.FTP_SVR_ADDR);
		            Ql_sscanf(value, "%*[^,],%[^,],",fotainfo.FTP_USER_NAME);
					Ql_sscanf(value, "%*[^,],%*[^,],%[^,],",fotainfo.FTP_PASSWORD);
					Ql_sscanf(value, "%*[^,],%*[^,],%*[^,],%[^,],",fotainfo.FTP_SVR_PATH);
					Ql_sscanf(value, "%*[^,],%*[^,],%*[^,],%*[^,],%[^,],",fotainfo.FTP_FILENAME);
					Ql_sprintf(systemset.fotaaddr,"ftp://%s%s%s:%s@%s:%s",fotainfo.FTP_SVR_ADDR, fotainfo.FTP_SVR_PATH, fotainfo.FTP_FILENAME, FTP_SVR_PORT, fotainfo.FTP_USER_NAME, fotainfo.FTP_PASSWORD);
					SaveFlashParamsNew(&systemset);
					zyf_protocol_info("fotainfo.FTP_FILENAME=%s",fotainfo.FTP_FILENAME);
					SendMsg2KernelForOTAData();
					return 2;
				}
			Ql_memset(ZYFRECMD[0], 0,sizeof(ZYFRECMD[0]));
			Ql_memset(ZYFRECMD[1], 0,sizeof(ZYFRECMD[1]));
			Ql_strcpy(ZYFRECMD[0],key);
			Ql_strcpy(ZYFRECMD[1],value);
			SendMsg2KernelForReIotData(NULL);
			return 0;
		}
	return -1;
}


s32 alink_msg_Iot_Redata(u8 *data_ptr,u8 *key,u8 *value)
{
	s32 ret,len;
	Ql_memset(alinkvaluebuf, 0, sizeof(alinkvaluebuf));
	Ql_sprintf(alinkvaluebuf,"\"%s\":\"%s\"",key,value);
	Ql_snprintf(data_ptr,1024,alink_headstr,alinkvaluebuf);
	len = Ql_strlen(data_ptr);
	return len;
}



void Ble_zyf_ReSetdataInit(u8 *p_data,u8 *len,u8 cmd,u8 op)
{
	u8 i=0;
	p_data[i++]=BLE_RESP_HEAD;
	p_data[i++]=cmd;
	p_data[i++]=0x01;
	p_data[i++]=op;
	*len=i;
}


void Ble_zyf_checkble_data(u8 *p_data,u8 length)
{
	u8 i,len;
	if (BLE_CMD_HEAD != p_data[0])return;
	switch (p_data[1])
		{
			case BLE_CMD_AUTH_KEY:				//设置 SN号码
				{
					 Ql_memset(systemset.SN, 0, sizeof(systemset.SN));
					 for(i=0;i<p_data[2];i++)
					 	{
					 		systemset.SN[i]=p_data[3+i];
					 	}
					 Ble_zyf_ReSetdataInit(ZYFBleCmdBuf,&len,BLE_CMD_AUTH_KEY,APP_SUCCESS);
					 ble_nus_string_send(ZYFBleCmdBuf,len);
					 systemset.snuser=1;
        			 SaveFlashParamsNew(&systemset);
        			 ble_server_sta=BLESERVER_DEVICE_PREINIT0;
					 break;	
				}
			case BLE_CMD_AUTH_REQU:				//设置LED灯的开关
				{
					 Ble_zyf_ReSetdataInit(ZYFBleCmdBuf,&len,BLE_CMD_AUTH_REQU,APP_SUCCESS);
					 ble_nus_string_send(ZYFBleCmdBuf,len);
					 break;	
				}
			 default:
			 	{
				     Ble_zyf_ReSetdataInit(ZYFBleCmdBuf,&len,BLE_ERROR_CMD,APP_ERROR_CMD);
					 ble_nus_string_send(ZYFBleCmdBuf,len);
					 break;
			 	}
		}
}

#ifdef _USE_MCU_PRO_



Enum_PinName M203C_DATA=PINNAME_CTS;
Enum_PinName M203C_EINT=PINNAME_DTR;
Enum_PinName M203C_WHG=PINNAME_RTS;

void mcu_drv_init(void)
{
	
	 Ql_GPIO_Init(M203C_DATA, PINDIRECTION_OUT, 0, PINPULLSEL_PULLUP);
	 Ql_GPIO_Init(M203C_WHG, PINDIRECTION_OUT, 0, PINPULLSEL_PULLUP);
	 Ql_GPIO_Init(M203C_EINT, PINDIRECTION_OUT, 0, PINPULLSEL_PULLUP);
	 
	 M203C_DATA_L;
	 M203C_WHG_L;
	 M203C_EINT_L;

	
}




static void mycallback_eint_handle(Enum_PinName eintPinName, Enum_PinLevel pinLevel, void* customParam)
{
    s32 ret;
	
    Ql_EINT_Mask(M203C_EINT);
    
    zyf_protocol_info("<--Eint callback: pin(%d), levle(%d)-->\r\n",eintPinName,pinLevel);
    ret = Ql_EINT_GetLevel(eintPinName);
    zyf_protocol_info("<--Get Level, pin(%d), levle(%d)-->\r\n",eintPinName,ret);
    
    Ql_EINT_Unmask(M203C_EINT);
}

void Eint_Init(void)
{
	s32 ret;
	ret = Ql_EINT_Register(M203C_EINT,mycallback_eint_handle, NULL);    
    if(ret != 0)
    {
        zyf_protocol_info("<--OpenCPU: Ql_EINT_RegisterFast fail.-->\r\n"); 
        return;    
    }
    zyf_protocol_info("<--OpenCPU: Ql_EINT_RegisterFast OK.-->\r\n"); 

	ret = Ql_EINT_Init(M203C_EINT, EINT_LEVEL_TRIGGERED, 0, 5,0);
    if(ret != 0)
    {
        zyf_protocol_info("<--OpenCPU: Ql_EINT_Init fail.-->\r\n"); 
        return;    
    }
    zyf_protocol_info("<--OpenCPU: Ql_EINT_Init OK.-->\r\n"); 
}




void SendNumFromProPin(u16 num)
{
	u16 i;
	M203C_DATA_H;
	Ql_Sleep(50);
	M203C_DATA_L;
	Ql_Sleep(20);
	for(i=0;i<16;i++)
		{
			if(num&(1<<(15-i)))
				{
					M203C_DATA_H;						
					Ql_Sleep(30);
				}
			else
				{
					M203C_DATA_H;							
					Ql_Sleep(10);
				}
			M203C_DATA_L;					
			Ql_Sleep(20);
		}
	M203C_DATA_L;
}


void setmcu2poweroff(u16 min)
{
	u16 t;
	t=min;
	SendNumFromProPin(t);
}


void setmcu2sleep(u16 min)
{
	u16 t;
	t=min|1<<14;
	SendNumFromProPin(t);
}

void setmcu2clk(u16 clk)
{
	u16 t;
	t=clk|1<<15;
	SendNumFromProPin(t);
}


void setmcu2eint(void)
{
	M203C_EINT_H;
	Ql_Sleep(500);
	M203C_EINT_L;
}

void setmcu2pinoff(void)
{
	M203C_WHG_H;
	Ql_Sleep(7000);
	M203C_WHG_L;
	Ql_Sleep(5000);
}


void SendEintMsg2Mcu(void)
{
	Ql_EINT_Uninit(M203C_EINT);
	Ql_GPIO_Init(M203C_EINT, PINDIRECTION_OUT, 0, PINPULLSEL_PULLUP);
	setmcu2eint();
	Ql_GPIO_Uninit(M203C_EINT);
	Eint_Init();
}

void setmcu2poweroff203c(u32 param)
{
	u32 sleeptime;
    u8 start = *(u8 *)param;
    if(start == 1 && systemset.Interval >= 1200)//180)
    {
        sleeptime = 1200; //default 20min
    }
    else
    {
	    sleeptime = systemset.Interval;
    }
	u32 t2=sleeptime/60;
	if(t2<1)t2=1;
	zyf_protocol_info("setmcu2poweroff203c=%d min\r\n",t2); 
	setmcu2eint();
	setmcu2poweroff(t2);
	setmcu2pinoff();
}





#endif

