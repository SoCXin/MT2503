
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


#ifdef ZYF_PRO_DEBUG
    #define zyf_protocol_info(format, ...)  mprintf( format "\r\n", ##__VA_ARGS__)
    #define zyf_protocol_error(format, ...) mprintf( "[error]%s() %d " format "\r\n", /*__FILE__,*/ __FUNCTION__, __LINE__, ##__VA_ARGS__)
#else
    #define zyf_protocol_info(format, ...)
    #define zyf_protocol_error(format, ...)
#endif


volatile u8 ZYF_MQTT_CONFIG=0;

u8 ZYFBleCmdBuf[31];

u8 ZYFRECMD[2][20];

#ifdef _USE_USER_DWQ_
_DJKZ_mode djkzmode;
#endif

/*!
 * @brief 智云服注册初始化
 * \n
 *
 * @param NULL
 * @return 返回数据长度
 * \n
 * @see
 */


s32 zyf_msg_register_data(u8 *data_ptr,void *user_data)
{
#ifdef _NOT_TEST_
		Ql_sprintf(data_ptr,"hello mqtt");
		return Ql_strlen(data_ptr);
#else
		s32 ret,len;
        u8 *buf_String;
		u8 tempbuf[20]={0};
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

		cJSON_AddItemToObject(root, "pk", cJSON_CreateString(ZYFAutoconfig.pk));

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

s32 zyf_msg_RegSensor_data(u8 *data_ptr,void *user_data)
{
	s32 ret,len;
    u8 *buf_String;
	u8 tempbuf[20]={0};
	cJSON * root =  cJSON_CreateObject();
	GetModuleData();
	LBSDataInit();
	WaitLbsGetLocation(100);
	Ql_sprintf(tempbuf,"%d",module_data.mvol);
    cJSON_AddItemToObject(root, AutomaticRD[0], cJSON_CreateString(tempbuf));
	Ql_sprintf(tempbuf,"%d",module_data.bvol);
    cJSON_AddItemToObject(root, AutomaticRD[1], cJSON_CreateString(tempbuf));
	Ql_sprintf(tempbuf,"%d",module_data.csq);
    cJSON_AddItemToObject(root, AutomaticRD[2], cJSON_CreateString(tempbuf));
	Ql_sprintf(tempbuf,"%d",systemset.SysTime);
    cJSON_AddItemToObject(root, AutomaticRD[3], cJSON_CreateString(tempbuf));
	Ql_sprintf(tempbuf,"%d.%06d",LbsWeidu/1000000,LbsWeidu%1000000);
	cJSON_AddItemToObject(root, AutomaticRD[4], cJSON_CreateString(tempbuf));
	Ql_sprintf(tempbuf,"%d.%06d",LbsJingdu/1000000,LbsJingdu%1000000);
	cJSON_AddItemToObject(root, AutomaticRD[5], cJSON_CreateString(tempbuf));
	buf_String=cJSON_Print(root);
	len = Ql_strlen(buf_String);
	Ql_strcpy(data_ptr,buf_String);
	zyf_protocol_info("%s",data_ptr);
	cJSON_Delete(root);
	Ql_MEM_Free(buf_String);
	return len;
	
}



s32 zyf_msg_Sensor_data(u8 *data_ptr,void *user_data)
{
	s32 ret,len;
    u8 *buf_String;
	u8 tempbuf[20]={0};
	cJSON * root =  cJSON_CreateObject();
	GetModuleData();
	ret=GetGpsLocation(60,1);
	if(ret==0)
		{
			Ql_sprintf(tempbuf,"%d.%06d",gpsx.latitude/1000000,gpsx.latitude%1000000);
			cJSON_AddItemToObject(root, AutomaticRD[6], cJSON_CreateString(tempbuf));
			Ql_sprintf(tempbuf,"%d.%06d",gpsx.longitude/1000000,gpsx.longitude%1000000);
			cJSON_AddItemToObject(root, AutomaticRD[7], cJSON_CreateString(tempbuf));
		}
	else
		{
			LBSDataInit();
			WaitLbsGetLocation(100);
			Ql_sprintf(tempbuf,"%d.%06d",LbsWeidu/1000000,LbsWeidu%1000000);
			cJSON_AddItemToObject(root, AutomaticRD[4], cJSON_CreateString(tempbuf));
			Ql_sprintf(tempbuf,"%d.%06d",LbsJingdu/1000000,LbsJingdu%1000000);
			cJSON_AddItemToObject(root, AutomaticRD[5], cJSON_CreateString(tempbuf));
		}
	Ql_sprintf(tempbuf,"%d",module_data.mvol);
    cJSON_AddItemToObject(root, AutomaticRD[0], cJSON_CreateString(tempbuf));
	Ql_sprintf(tempbuf,"%d",module_data.bvol);
    cJSON_AddItemToObject(root, AutomaticRD[1], cJSON_CreateString(tempbuf));
	Ql_sprintf(tempbuf,"%d",module_data.csq);
    cJSON_AddItemToObject(root, AutomaticRD[2], cJSON_CreateString(tempbuf));
	Ql_sprintf(tempbuf,"%d",systemset.SysTime);
    cJSON_AddItemToObject(root, AutomaticRD[3], cJSON_CreateString(tempbuf));
	buf_String=cJSON_Print(root);
	len = Ql_strlen(buf_String);
	Ql_strcpy(data_ptr,buf_String);
	zyf_protocol_info("%s",data_ptr);
	cJSON_Delete(root);
	Ql_MEM_Free(buf_String);
	return len;
	
}


s32 zyf_ays_cmd(char *src,char *key,char *value)
{
	u8 *p=NULL;
	p=Ql_strstr(src,key);
	if(p!=NULL)
		{
			while(*p!=':')p++;
			p++;
			while(*p!='"')p++;
			p++;
			while(*p!='"')
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


s32 zyf_msg_Iot_Redata(u8 *data_ptr,u8 *type,u8 *runid)
{
	s32 ret,len;
    u8 *buf_String;
	u8 tempbuf[20]={0};
	cJSON * root =  cJSON_CreateObject();
	cJSON_AddItemToObject(root, "insType", cJSON_CreateString(type));
    cJSON_AddItemToObject(root, "insRunId", cJSON_CreateString(runid));
	Ql_strcpy(tempbuf,systemset.SN);
	cJSON_AddItemToObject(root, "sn", cJSON_CreateString(tempbuf));
    cJSON_AddItemToObject(root, "isOk", cJSON_CreateString("0"));
	buf_String=cJSON_Print(root);
	len = Ql_strlen(buf_String);
	Ql_strcpy(data_ptr,buf_String);
	zyf_protocol_info("%s",data_ptr);
	cJSON_Delete(root);
	Ql_MEM_Free(buf_String);
	return len;
}


#define _TEST_ZFY_CMD_

s32 zyf_decode_cmd(char *Payload,u16 len)
{
	s32 ret;
	u8 value[3][20]={0};
	Ql_memset(value[0], 0,sizeof(value[0]));
	zyf_ays_cmd(Payload,"insType",value[0]);
	zyf_protocol_info("value[0]=%s",value[0]);
	Ql_memset(value[1], 0,sizeof(value[1]));
	zyf_ays_cmd(Payload,"insRunId",value[1]);
	zyf_protocol_info("value[1]=%s",value[1]);
	Ql_memset(value[2], 0,sizeof(value[2]));
	zyf_ays_cmd(Payload,"insValue",value[2]);
	zyf_protocol_info("value[2]=%s",value[2]);
#ifdef _TEST_ZFY_CMD_
	ret=Ql_strcmp(value[0], AutomaticWR[0]);
	if(ret==0)
		{
			u8 *p=NULL;
			p=Ql_strstr(value[2],"1");
			if(p!=NULL)
				{
					LED2_H;
					zyf_protocol_info("LED2_H");
				}
			else 
				{
					LED2_L;
					zyf_protocol_info("LED2_L");
				}
		}
	ret=Ql_strcmp(value[0], AutomaticWR[1]);
	if(ret==0)
		{
			systemset.Interval=strtol((const char*)value[2],NULL,10);
			SaveFlashParamsNew(&systemset);
			IotData_T=0;
		}
	

	
	Ql_memset(ZYFRECMD[0], 0,sizeof(ZYFRECMD[0]));
	Ql_memset(ZYFRECMD[1], 0,sizeof(ZYFRECMD[1]));
	Ql_strcpy(ZYFRECMD[0],value[0]);
	Ql_strcpy(ZYFRECMD[1],value[1]);
	SendMsg2KernelForReIotData();
#else

#endif
	
	return 0;
}







s32 zyf_decode_upbcmd(char *Payload,u16 len)
{
	s32 ret;
	u8 value[2][20]={0};
	u8 keyvalue[100]={0};
	Ql_memset(value[0], 0,sizeof(value[0]));
	zyf_ays_cmd(Payload,"insType",value[0]);
	zyf_protocol_info("value[0]=%s",value[0]);
	Ql_memset(value[1], 0,sizeof(value[1]));
	zyf_ays_cmd(Payload,"insRunId",value[1]);
	zyf_protocol_info("value[1]=%s",value[1]);
	
	Ql_memset(keyvalue, 0,sizeof(keyvalue));
	zyf_ays_cmd(Payload,"insValue",keyvalue);
	zyf_protocol_info("keyvalue=%s",keyvalue);
	ret=Ql_strcmp(value[0], "update");
	if(ret==0)
		{
			systemset.updateinfo=1;
			Ql_sscanf(keyvalue, "%[^,],",fotainfo.FTP_SVR_ADDR);
            Ql_sscanf(keyvalue, "%*[^,],%[^,],",fotainfo.FTP_USER_NAME);
			Ql_sscanf(keyvalue, "%*[^,],%*[^,],%[^,],",fotainfo.FTP_PASSWORD);
			Ql_sscanf(keyvalue, "%*[^,],%*[^,],%*[^,],%[^,],",fotainfo.FTP_SVR_PATH);
			Ql_sscanf(keyvalue, "%*[^,],%*[^,],%*[^,],%*[^,],%[^,],",fotainfo.FTP_FILENAME);
			Ql_sprintf(systemset.fotaaddr,"ftp://%s%s%s:%s@%s:%s",fotainfo.FTP_SVR_ADDR, fotainfo.FTP_SVR_PATH, fotainfo.FTP_FILENAME, FTP_SVR_PORT, fotainfo.FTP_USER_NAME, fotainfo.FTP_PASSWORD);
			SaveFlashParamsNew(&systemset);
			zyf_protocol_info("fotainfo.FTP_FILENAME=%s",fotainfo.FTP_FILENAME);
			SendMsg2KernelForOTAData();
		}
	ret=Ql_strcmp(value[0], "time");
	if(ret==0)
		{
			GetTime2Use(&time.year,&time.month,&time.day,&time.hour,&time.minute,&time.second,keyvalue);
			SetSysTime(&time);
			GetSysTime(&time);
			ZYF_MQTT_CONFIG|=1<<0;
		}
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
					 //if(p_data[3]==LED_STA_ON)LED_B_ON;
					 //if(p_data[3]==LED_STA_OFF)LED_B_OFF;
					 Ble_zyf_ReSetdataInit(ZYFBleCmdBuf,&len,BLE_CMD_AUTH_REQU,APP_SUCCESS);
					 ble_nus_string_send(ZYFBleCmdBuf,len);
					 break;	
				}
#ifdef _USE_USER_DWQ_
			case BLE_DWQ_CMD:	
				{
					 u8 *p=NULL;
					 u8 temstr[20]={0};
					 Ql_memset(temstr, 0, sizeof(temstr));
					 for(i=0;i<p_data[2];i++)
					 	{
					 		temstr[i]=p_data[3+i];
					 	}
					 p=(u8*)strstr((const char*)temstr,"+:");
					 if(p!=NULL)
						{
							WzAynsMode(p);
							zyf_protocol_info("+mode %d\r\n",djkzmode.mode);
							zyf_protocol_info("+per %d\r\n",djkzmode.per);
							Ble_zyf_ReSetdataInit(ZYFBleCmdBuf,&len,BLE_DWQ_CMD,APP_SUCCESS);
					 		ble_nus_string_send(ZYFBleCmdBuf,len);
						}
					 else
					 	{
					 		Ble_zyf_ReSetdataInit(ZYFBleCmdBuf,&len,BLE_DWQ_CMD,APP_FAIL);
					 		ble_nus_string_send(ZYFBleCmdBuf,len);
					 	}
				}
#endif
			 default:
			 	{
				     Ble_zyf_ReSetdataInit(ZYFBleCmdBuf,&len,BLE_ERROR_CMD,APP_ERROR_CMD);
					 ble_nus_string_send(ZYFBleCmdBuf,len);
					 break;
			 	}
		}
}


u8 ZYFJsonCmdAnalysis(char *Payload,u16 len)
{
	cJSON *root,*type;
	root = cJSON_Parse(Payload);
	if (!root)  
	    {  
	        zyf_protocol_error("Error before: [%s]\n",cJSON_GetErrorPtr());
			goto END;
	    }
	else
		{
			 
			 type = cJSON_GetObjectItem( root , AutomaticWR[0]);
			 zyf_protocol_info("type->valuestring=%s\r\n",type->valuestring);
		}

END:
	cJSON_Delete(root);
	return 0;

}







#ifdef _USE_USER_DWQ_


u8 wzcurrtsta=0;


void Dwq_drv_init(void)
{
	Ql_GPIO_Uninit(PINNAME_SIM2_CLK);
	Ql_GPIO_Uninit(PINNAME_SIM2_DATA);
	Ql_GPIO_Uninit(PINNAME_SIM2_RST);
	Ql_GPIO_Init(PINNAME_SIM2_CLK, PINDIRECTION_OUT, 0, PINPULLSEL_PULLUP);
	Ql_GPIO_Init(PINNAME_SIM2_DATA, PINDIRECTION_OUT, 0, PINPULLSEL_PULLUP);
	Ql_GPIO_Init(PINNAME_SIM2_RST, PINDIRECTION_OUT, 0, PINPULLSEL_PULLUP);
	Ql_GPIO_Init(PINNAME_SD_CMD, PINDIRECTION_OUT, 0, PINPULLSEL_PULLUP);

}


u8 CheckSDDATA(void)
{
	s32 res=0;
	res=Ql_GPIO_GetLevel(PINNAME_SD_CMD);
	return res;
}





u8 GetModeNum(u8 *buf)
{
	u8 numbuf[4]={0};
	u8 *p=numbuf;
	while(*buf!=';')buf++;
	buf++;
	while(*buf!=';')buf++;
	buf++;
	while(*buf!=';')
		{
			*p=*buf;
			buf++;
			p++;
		}
	*p=0;
	return strtol((const char*)numbuf,NULL,10);
	
}

u8 GetMode4(u8 *buf)
{
	u8 numbuf[4]={0};
	u8 *p=numbuf;
	while(*buf!=';')buf++;
	buf++;
	while(*buf!=';')buf++;
	buf++;
	while(*buf!=',')
		{
			*p=*buf;
			buf++;
			p++;
		}
	*p=0;
	djkzmode.muxmode[0].per=strtol((const char*)numbuf,NULL,10);
	buf++;
	*p=&numbuf[0];
	while(*buf!=',')
		{
			*p=*buf;
			buf++;
			p++;
		}
	*p=0;
	djkzmode.muxmode[1].per=strtol((const char*)numbuf,NULL,10);
	buf++;
	*p=&numbuf[0];
	while(*buf!=';')
		{
			*p=*buf;
			buf++;
			p++;
		}
	*p=0;
	djkzmode.muxmode[0].sec=strtol((const char*)numbuf,NULL,10);
	
	
	
}


u8 GetMode5(u8 *buf)
{
	u8 numbuf[4]={0};
	u8 *p=numbuf;
	while(*buf!=';')buf++;
	buf++;
	while(*buf!=';')buf++;
	buf++;
	while(*buf!=',')
		{
			*p=*buf;
			buf++;
			p++;
		}
	*p=0;
	djkzmode.muxmode[0].per=strtol((const char*)numbuf,NULL,10);
	buf++;
	*p=&numbuf[0];
	while(*buf!=',')
		{
			*p=*buf;
			buf++;
			p++;
		}
	*p=0;
	djkzmode.muxmode[0].sec=strtol((const char*)numbuf,NULL,10);
	buf++;
	*p=&numbuf[0];
	while(*buf!=',')
		{
			*p=*buf;
			buf++;
			p++;
		}
	*p=0;
	djkzmode.muxmode[1].per=strtol((const char*)numbuf,NULL,10);
	buf++;
	*p=&numbuf[0];
	while(*buf!=';')
		{
			*p=*buf;
			buf++;
			p++;
		}
	*p=0;
	djkzmode.muxmode[1].sec=strtol((const char*)numbuf,NULL,10);
	
	
	
}

u8 GetMode6(u8 *buf)
{
	djkzmode.muxmode[0].per=30;
	djkzmode.muxmode[0].sec=5;
	djkzmode.muxmode[1].per=50;
	djkzmode.muxmode[1].sec=5;
	djkzmode.muxmode[2].per=100;
	djkzmode.muxmode[2].sec=5;
	djkzmode.muxmode[3].per=60;
	djkzmode.muxmode[3].sec=15;
	djkzmode.muxmode[4].per=80;
	djkzmode.muxmode[4].sec=5;
	djkzmode.muxmode[5].per=30;
	djkzmode.muxmode[5].sec=10;
	djkzmode.muxmode[6].per=70;
	djkzmode.muxmode[6].sec=15;
	djkzmode.muxmode[7].per=100;
	djkzmode.muxmode[7].sec=5;
}


u8 WzAynsMode(u8 *pbuf)
{
	u8 *p=NULL;
	p=(u8*)strstr((const char*)pbuf,"+:0;");
	if(p!=NULL)
		{
			djkzmode.mode=0;
			djkzmode.per=GetModeNum(pbuf);
			
		}
	p=(u8*)strstr((const char*)pbuf,"+:1;");
	if(p!=NULL)
		{
			djkzmode.mode=1;
			djkzmode.per=GetModeNum(pbuf);
		}
	p=(u8*)strstr((const char*)pbuf,"+:2;");
	if(p!=NULL)
		{
			djkzmode.mode=2;
			djkzmode.per=GetModeNum(pbuf);
		}
	p=(u8*)strstr((const char*)pbuf,"+:3;");
	if(p!=NULL)
		{
			djkzmode.mode=3;
			djkzmode.per=GetModeNum(pbuf);
		}
	p=(u8*)strstr((const char*)pbuf,"+:4;");
	if(p!=NULL)
		{
			djkzmode.mode=4;
			GetMode4(pbuf);
		}
	p=(u8*)strstr((const char*)pbuf,"+:5;");
	if(p!=NULL)
		{
			djkzmode.mode=5;
			GetMode5(pbuf);
		}
	p=(u8*)strstr((const char*)pbuf,"+:6;");
	if(p!=NULL)
		{
			djkzmode.mode=6;
			GetMode6(pbuf);
		}
	p=(u8*)strstr((const char*)pbuf,"+:7;");
	if(p!=NULL)
		{
			djkzmode.mode=7;
		}
	p=(u8*)strstr((const char*)pbuf,"+:8;");
	if(p!=NULL)
		{
			djkzmode.mode=8;
		}
}

void os_delays(u16 s)
{
	u16 i;
	for(i=0;i<s;i++)
		{
			Ql_Sleep(1000);
			if(djkzmode.mode!=4)break;
		}
}


void WZinit(void)
{
	u8 res;
	u8 m;
	WZINS_H;
	WZCS_L;
	WZUD_L;
	Delayus(1);
	for(m=100;m>0;m--)
	{
		WZINS_L;
		Delayus(2);
		WZINS_H;
	}
	WZCS_H;
	wzcurrtsta=0;
	res=CheckSDDATA();
	if(res==1)
		{
			M203C_SDDATAIN_L;
		}
	
}
void WZAddOne(void)
{
	u8 n;
	WZCS_L;
	WZUD_H;
	Delayus(1);
	for(n=1;n>0;n--)
		{
			WZINS_L;
			Delayus(2);
			WZINS_H;
		}
	WZCS_H;
}

void WZSubOne(void)
{
	u8 n;
	WZCS_L;
	WZUD_L;
	Delayus(1);
	for(n=1;n>0;n--)
		{
			WZINS_L;
			Delayus(2);
			WZINS_H;
		}
	WZCS_H;
}



void WZAysCuurtSta(void)
{
	u8 res;
	u8 t=0,i;
	if(systemset.CallMode>wzcurrtsta)
		{
			res=CheckSDDATA();
			if(res==0)
				{
					M203C_SDDATAIN_H;
				}
			t=systemset.CallMode-wzcurrtsta;
			for(i=0;i<t;i++)
				{
					WZAddOne();
					Delayus(1);
				}
		}
	else 
		{
			res=CheckSDDATA();
			if(res==0)
				{
					M203C_SDDATAIN_H;
				}
			t=wzcurrtsta-systemset.CallMode;
			for(i=0;i<t;i++)
				{
					WZSubOne();
					Delayus(1);
				}
		}
	wzcurrtsta=systemset.CallMode;
	if(wzcurrtsta==0)
		{
			res=CheckSDDATA();
			if(res==1)
				{
					M203C_SDDATAIN_L;
				}
		}
}



void Dwq_User_App(void)
{
	u8 i=0,j=0;
	while(1)
		{
			Ql_Sleep(10);
			if(djkzmode.mode==0||djkzmode.mode==1||djkzmode.mode==2||djkzmode.mode==3)
					{
						systemset.CallMode=djkzmode.per;
						WZAysCuurtSta();
					}
				else
					{
						if(djkzmode.mode==4)
							{
								systemset.CallMode=djkzmode.muxmode[0].per;
								WZAysCuurtSta();
								if(djkzmode.muxmode[1].per>=djkzmode.muxmode[0].per)
									{
										j=djkzmode.muxmode[1].per-djkzmode.muxmode[0].per;
										for(i=0;i<j;i++)
											{
												systemset.CallMode+=1;
												WZAysCuurtSta();
												os_delays(djkzmode.muxmode[1].sec/j);
												if(djkzmode.mode!=4)break;
											}
									}
							}
						if(djkzmode.mode==5)
							{
								systemset.CallMode=djkzmode.muxmode[0].per;
								WZAysCuurtSta();
								for(i=0;i<djkzmode.muxmode[0].sec;i++)
								{
									Ql_Sleep(1000);
									if(djkzmode.mode!=5)break;
								}
								systemset.CallMode=djkzmode.muxmode[1].per;
								WZAysCuurtSta();
								for(i=0;i<djkzmode.muxmode[1].sec;i++)
								{
									Ql_Sleep(1000);
									if(djkzmode.mode!=5)break;
								}
								
							}
						if(djkzmode.mode==6)
							{
								for(j=0;j<8;j++)
									{
										systemset.CallMode=djkzmode.muxmode[j].per;
										WZAysCuurtSta();
										for(i=0;i<djkzmode.muxmode[j].sec;i++)
										{
											Ql_Sleep(1000);
											if(djkzmode.mode!=6)break;
										}
										if(djkzmode.mode!=6)break;
									}
							}
						if(djkzmode.mode==7)
							{
								WZinit();
								systemset.CallMode=0;
								djkzmode.mode=1;
							}
						if(djkzmode.mode==8)
							{
								WZinit();
								systemset.CallMode=0;
								djkzmode.mode=0xff;
							}
					}
		}
}



#endif





