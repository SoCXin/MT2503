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
	p=(u8*)strstr((const char *)buf,"ceshi");
	if(p!=NULL)
		{
			mprintf("+ceshi ok\r\n");
		}
}

void Uart2UserCommandEncode(u8* buf, u8 len)
{
    mprintf("++++++++++++++++\r\n");
    mprintf("stm8lsay:%s\r\n", buf);
    mprintf("++++++++++++++++\r\n");
}



