#include "ql_trace.h"
#include "ql_system.h"
#include "ql_gpio.h"
#include "ql_stdlib.h"
#include "ql_error.h"
#include "ql_uart.h"
#include "sys.h"
#include "uart.h"
#include "tiem.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include "ril.h"
#include "ril_location.h"
#include "gps.h"
#include "ril_sms.h"
#include "ril_telephony.h"
#include "motor.h"
#include "hwhal.h"
#include "fota.h"
#include "m203c_ble.h"
#include "ql_timer.h"
#include "user_mqtt.h"


uartcomand systemfuntest;
Blesta blestas;
u8 save_point_test = 0;

#if 0
#ifdef __GNUC__ 
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch) 
#else 
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif  
  
PUTCHAR_PROTOTYPE
 { 
      Ql_UART_Write(UART_PORT1, (uint8_t) ch); 
      return ch; 
 }
#endif


static s32 ReadSerialPort(Enum_SerialPort port, u8* pBuffer, u32 bufLen)
{
    s32 rdLen = 0;
    s32 rdTotalLen = 0;
    Ql_memset(pBuffer, 0x0, bufLen);
    while (1)
    {
        rdLen = Ql_UART_Read(port, pBuffer + rdTotalLen, bufLen - rdTotalLen);
        if (rdLen <= 0)
        {
            break;
        }
        rdTotalLen += rdLen;
    }
    if (rdLen < 0)  return -99;
    return rdTotalLen;
}

static void Callback_UART_Hdlr(Enum_SerialPort port, Enum_UARTEventType msg, bool level, void* param)
{
    s32 lens;
    u8 RxBuf_Uart[800] = {0};
    s32 iRet = 0;
    switch (msg)
    {
	    case EVENT_UART_READY_TO_READ:
	    {
	        lens = ReadSerialPort(port, RxBuf_Uart, sizeof(RxBuf_Uart));
	        if (lens < 0)break;
	        else
	        {
	            if (port == UART_PORT1)User_Commad_Ays(RxBuf_Uart, lens);
	            if (port == UART_PORT2)Uart2UserCommandEncode(RxBuf_Uart, lens);
	        }
	    }
	    break;
#if 0
	case EVENT_UART_DTR_IND:
        {
           if(level)			//处理外部DTR拉高事件
           	{
           		
           	}

        }
#endif
    case EVENT_UART_READY_TO_WRITE:
        break;
    default:
        break;

    }
}



void UartInit(void)
{
    s32 ret;
    ret = Ql_UART_Register(UART_PORT1, Callback_UART_Hdlr, NULL);
    ret = Ql_UART_Open(UART_PORT1, 115200, FC_NONE);
    ret = Ql_UART_Register(UART_PORT2, Callback_UART_Hdlr, NULL);
    ret = Ql_UART_Open(UART_PORT2, 9600, FC_NONE);
}


void Get_Str_Use(u8* ipstr, u8* buf)
{
    Ql_memset(ipstr, '\0', sizeof(ipstr));
    while (*buf != 0x20)buf++;
    buf++;
    Ql_strcpy((char*)ipstr, (char*)buf);
}


void Getmac_Str_Use(u8* ipstr, u8* sbuf, u8* buf)
{
    while (*buf != 0x20)buf++;
    buf++;
    while (*buf != 0x20)
    {
        *ipstr = *buf;
        ipstr++;
        buf++;
    }
    buf++;
    while (*buf != 0x20)
    {
        *ipstr = *buf;
        ipstr++;
        buf++;
    }
    buf++;
    while (*buf != 0x20)
    {
        *ipstr = *buf;
        ipstr++;
        buf++;
    }
    buf++;
    while (*buf != 0x20)
    {
        *sbuf = *buf;
        sbuf++;
        buf++;
    }
    buf++;
    while (*buf != 0x20)
    {
        *sbuf = *buf;
        sbuf++;
        buf++;
    }
    buf++;
    while (*buf != 0)
    {
        *sbuf = *buf;
        sbuf++;
        buf++;
    }

}


u8* mid(u8* dst, u8* src, int n, int m)
{
    u8* p = src;
    u8* q = dst;
    int len = strlen((const char*)src);
    if (n > len) n = len - m;
    if (m < 0) m = 0;
    if (m > len) return NULL;
    p += m;
    while (n--) *(q++) = *(p++);
    *(q++) = '\0';
    return dst;
}


void DeleteMid(u8* sbuf, u16* year, u8* month, u8* date)
{
    u8 buf[3] = {0};
    u8* p1 = buf;
    while (*sbuf != '-')
    {
        *p1 = *sbuf;
        p1++;
        sbuf++;
    }
    sbuf++;
    *year = (strtol((char*)buf, NULL, 10));
    Ql_memset(buf, 0, sizeof(buf));
    p1 = buf;
    while (*sbuf != '-')
    {
        *p1 = *sbuf;
        p1++;
        sbuf++;
    }
    sbuf++;
    *month = (u8)(strtol((char*)buf, NULL, 10));
    *date = (u8)(strtol((char*)sbuf, NULL, 10));
}


void GetTime2Use(u16* year, u8* month, u8* date, u8* hour, u8* min, u8* seond, u8* sbuf)
{
    u8 timebuf[10] = {0};
    u8 timebuf1[10] = {0};
    u8* p3 = timebuf1;
    u8 buf[3] = {0};
    while (*sbuf != 0x20)sbuf++;
    sbuf++;
    while (*sbuf != 0x20)
    {
        *p3 = *sbuf;
        sbuf++;
        p3++;
    }
    sbuf++;
    DeleteMid(timebuf1, year, month, date);
    strcpy((char*)timebuf, (char*)sbuf);
    mid(buf, timebuf, 2, 0);
    *hour = (u8)(strtol((char*)buf, NULL, 10));
    mid(buf, timebuf, 2, 3);
    *min = (u8)(strtol((char*)buf, NULL, 10));
    mid(buf, timebuf, 2, 6);
    *seond = (u8)(strtol((char*)buf, NULL, 10));

}

void Get_mqtt_conf(u8* buf)
{
    char   name_buf[16] = {0};
    char   key_buf[16] = {0};
    char   secret_buf[48] = {0};
    char* ptr = name_buf;
    char* ptr1 = key_buf;
    char* ptr2 = secret_buf;
    //name
    while (*buf != '/')
    {
        if (*buf != '-')
        {
            buf++;
        }
        else
        {
            buf++;
            break;
        }

    }
    while (*buf != '-')
    {
        *ptr++ = *buf++;

    }
    buf++;
    *ptr = '\0';

    //key
    while (*buf != '-')
    {
        *ptr1++ = *buf++;

    }
    buf++;
    *ptr1 = '\0';
    //sercert
    while (*buf != '/')
    {
        *ptr2++ = *buf++;

    }
    *ptr2 = '\0';
    Ql_strcpy(systemset.SN, name_buf);
    Ql_strcpy(systemset.key, key_buf);
    Ql_strcpy(systemset.secret, secret_buf);
	
}

static s32 MY_AT_Handler(char* pLine, u32 uLen, void* pUserData)
{
    mprintf("%s", pLine);
}




static s32 ATTestResponse_Handler(char* line, u32 len, void* userData)
{
    Ql_UART_Write(UART_PORT1, (u8*)line, len);

    if (Ql_RIL_FindLine(line, len, "OK"))
    {
        return  RIL_ATRSP_SUCCESS;
    }
    else if (Ql_RIL_FindLine(line, len, "ERROR"))
    {
        return  RIL_ATRSP_FAILED;
    }
    else if (Ql_RIL_FindString(line, len, "+CME ERROR"))
    {
        return  RIL_ATRSP_FAILED;
    }
    else if (Ql_RIL_FindString(line, len, "+CMS ERROR:"))
    {
        return  RIL_ATRSP_FAILED;
    }
    return RIL_ATRSP_CONTINUE; //continue wait
}


void User_Commad_Ays(u8* buf, u8 len)
{
    u8* p;
	u8 mybuf[20];
	u16 tem;
	p = (u8*)strstr((const char*)buf, "$$write");
    if (p != NULL)
    {
        Get_mqtt_conf(p);
		systemset.snuser=1;
        SaveFlashParamsNew(&systemset);
        mprintf("+SN %s\r\n", (u8*)systemset.SN);
		mprintf("+secret %s\r\n", (u8*)systemset.secret);
		mprintf("+key %s\r\n", (u8*)systemset.key);
		ble_server_sta=BLESERVER_DEVICE_PREINIT0;
		SendMsg2KernelForBLEStart();
        SendMsg2KernelFroCheckGSMorGPRS();
    }
	p = (u8*)strstr((const char*)buf, "$$setsn");
    if (p != NULL)
    {
        Ql_memset(systemset.SN, 0, sizeof(systemset.SN));
        Get_Str_Use(systemset.SN, p);
		systemset.snuser=1;
        SaveFlashParamsNew(&systemset);
        mprintf("+SN %s\r\n", (u8*)systemset.SN);
		ble_server_sta=BLESERVER_DEVICE_PREINIT0;
		SendMsg2KernelForBLEStart();
        SendMsg2KernelFroCheckGSMorGPRS();
    }

	p=(u8*)strstr((const char*)buf,"^^sethrt");
	if(p!=NULL)
		{
			Heart_T=0;
			Ql_memset(mybuf,0,sizeof(mybuf));
			Get_Str_Use(mybuf,p);
			systemset.HandInter=strtol((const char*)mybuf,NULL,10);
			SaveFlashParamsNew(&systemset);
			mprintf("+HandInter %d\r\n",systemset.HandInter);

		}
	p=(u8*)strstr((const char*)buf,"$$setint");
	if(p!=NULL)
		{
			IotData_T=0;
			Ql_memset(mybuf,0,sizeof(mybuf));
			Get_Str_Use(mybuf,p);
			systemset.Interval=strtol((const char*)mybuf,NULL,10);
			SaveFlashParamsNew(&systemset);
			mprintf("+Interval %d\r\n",systemset.Interval);
		}
	p = (u8*)strstr((const char*)buf, "$$getmdata");
    if (p != NULL)
    	{
    		mprintf("getmdata....\r\n");
			SendMsg2KernelForModuleData();
    	}
	p = (u8*)strstr((const char*)buf, "$$sendiotdata");
    if (p != NULL)
    	{
    		mprintf("sendiotdata....\r\n");
			SendMsg2KernelForIotData();
    	}
	p = (u8*)strstr((const char*)buf, "$$info");
    if (p != NULL)
    	{
    		ShowSysInfo();
    	}
	p=(u8*)strstr((const char *)buf,"$time");
	if(p!=NULL)
		{
			GetTime2Use(&time.year,&time.month,&time.day,&time.hour,&time.minute,&time.second,p);
			SetSysTime(&time);
			GetSysTime(&time);
			mprintf("+time ok\r\n");
		}
	p=(u8*)strstr((const char *)buf,"$stm8off");
	if(p!=NULL)
		{
            u8 start = 0;
			mprintf("\r\nOK\r\n");
			//setmcu2poweroff203c(&start);
		}
	p=(u8*)strstr((const char *)buf,"$stm8sleep");
	if(p!=NULL)
		{
			mprintf("\r\nOK\r\n");
			Ql_memset(mybuf,0,sizeof(mybuf));
			Get_Str_Use(mybuf,p);
			tem=strtol((const char*)mybuf,NULL,10);
		}
	p=(u8*)strstr((const char *)buf,"$stm8eclk");	
	if(p!=NULL)
		{
			
		}
	p=(u8*)strstr((const char *)buf,"$stm8wake");
	if(p!=NULL)
		{
			
		}
	p=(u8*)strstr((const char *)buf,"$setstm8ok");
	if(p!=NULL)
		{
			
		}
    p=(u8*)strstr((const char*)buf,"$testsavepoint");
    if(p!=NULL)
    {
        save_point_test = 1;
        mprintf("save point test %d\r\n",save_point_test);
    }
    p = (u8*)strstr((const char*)buf, "$$debug_errlog");
    if (p != NULL)
    	{
    		mprintf("+ok\r\n");
			SendMsg2KernelForErrorLog();
    	}
    p=(u8 *)strstr((const char *)buf,"AT+");
	{
        mprintf("%s\r\n",buf);
        Ql_RIL_SendATCmd(buf, len, NULL, NULL, 0);
    }
	
}



#define printf2cmd(format, ...)  mprintf( "[uart2C]"format "\r\n", ##__VA_ARGS__)
#define printf2log(format, ...)  mprintf( "[uart2L]"format "\r\n", ##__VA_ARGS__)


u32 getcmdbuf(u8 *cmdbuf,u8 *data)
{
	u8 errorT=0;
	u8 *p=cmdbuf;
	u8 v[10]={0};
	u8 *pv=v;
	
	while(*p!='_'&&errorT<10)p++;
		{
			p++;
			errorT++;
		}

	errorT=0;
	while(*p!=':'&&errorT<10)
		{
			*pv=*p;
			p++;
			pv++;
			errorT++;
		}
	*pv=0;
	 p++;

	 errorT=0;
	while(*p!=';'&&errorT<20)
		{
			*data=*p;
			p++;
			data++;
			errorT++;
		}
	errorT=0;
	*data=0;
	return (u32)(strtol((char*)v,NULL,10));
}



void Uart2UserCommandEncode(u8* buf, u8 len)
{
#if !defined(_USE_MCU_PRO_)
   u8 *p;
   u8 value[20]={0};
   p=(u8*)strstr((const char *)buf,"$_");
   if(p!=NULL)
   	{
   		printf2cmd("%s",buf);
		u8 cmd=getcmdbuf(buf,value);
		if(cmd==M203C_SET_DATA0||cmd==M203C_SET_DATA1||cmd==M203C_SET_DATA3||cmd==M203C_RDOFF_STM8_RDOFF)
			{
				systemfuntest.ack_cmd=cmd;
			}
		else if(cmd==SOC_POWER_OFF)					//不建议在回调处理,暂时测试用
			{
				SendCmd2Mcu(cmd,0,NULL,0);
				Ql_Sleep(100);
				Ql_PowerDown(1);
			}
   	}
   else
   	{
   		printf2log("%s",buf);
   	}
   #endif
}

#if !defined(_USE_MCU_PRO_)
u8 SendCmd2Mcu(u8 cmd,u8 ack,u8 *value,u32 timeout)
{
	u8 buf[200]={0};
	if(value==NULL)
		{
			Ql_sprintf((char*)buf,"$_%d:;\r\n",cmd);
		}
	else
		{
			Ql_sprintf((char*)buf,"$_%d:%s;\r\n",cmd,value);
		}
	Ql_UART_Write(UART_PORT2,buf,Ql_strlen(buf));
	if(ack)
		{
			while(timeout)
				{
					Ql_Sleep(10);
					timeout--;
					if(systemfuntest.ack_cmd==cmd)
						{
							systemfuntest.ack_cmd=0;
							return 0;
						}
				}
			return 1;
		}
	return 0;
}


u8 setmcu2poweroff203c(u32 param)
{
	u8 ret;
	u8 vbuf[10]={0};
    u8 start = *(u8 *)param;
    if(start == 1 && systemset.Interval >= 1200)//180)
    {
        Ql_sprintf(vbuf,"%d", 1200);//systemset.Interval / 3); //default 20min
    }
    else
    {
	    Ql_sprintf(vbuf,"%d",systemset.Interval);
    }
    ret=SendCmd2Mcu(M203C_SET_DATA0,1,vbuf,300);
	if(ret==0)
		{
			ret=SendCmd2Mcu(M203C_RDOFF_STM8_RDOFF,1,NULL,300);
		}
	return ret;
}

#endif





