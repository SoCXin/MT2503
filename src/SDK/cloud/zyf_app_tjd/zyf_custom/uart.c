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
#include "pro.h"
#include "ril_sms.h"
#include "ril_telephony.h"
#include "flash.h"
#include "bt3.h"

ConSMSStruct g_asConSMSBuf[CON_SMS_BUF_MAX_CNT];

FOTAINFO fotainfo;


void tdelay(u32 data)   //20us
{
	u16 i;
	while(data--)
	{
		for(i=0;i<14;i++){}
	}
}

void delay20us(u32 data)
{
	while(data--)
		{
			tdelay(1);
		}
}


void FotaInit(void)
{
	Ql_memset((FOTAINFO*)&fotainfo,0,sizeof(FOTAINFO));
}




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
	u8 RxBuf_Uart[1024]={0};
	 s32 iRet = 0;
	 switch (msg)
	 	{
	 		 case EVENT_UART_READY_TO_READ:
			 		{
					 	 lens=ReadSerialPort(port, RxBuf_Uart, sizeof(RxBuf_Uart));
						 if(lens<0)break;
						 else 
						 	{
						 		if(port==UART_PORT1)User_Commad_Ays(RxBuf_Uart,lens);
								if(port==VIRTUAL_PORT1)AysConet2Core(RxBuf_Uart,lens);
								if(port==UART_PORT2)AysOtherData(RxBuf_Uart,lens);
						 	}
	 		 		}
				break;
			 case EVENT_UART_READY_TO_WRITE:
			 	break;
			  default:
        				break;
			 	
	 	}
}



void UartInit(void)
{
	 s32 ret;
	 ret =Ql_UART_Register(UART_PORT1, Callback_UART_Hdlr, NULL);	
	 ret = Ql_UART_Open(UART_PORT1, 115200, FC_NONE);
	 ret =Ql_UART_Register(UART_PORT2, Callback_UART_Hdlr, NULL);	
	 ret = Ql_UART_Open(UART_PORT2, 115200, FC_NONE);
}

void VIRTUAL_PORT1Init(void)
{
	s32 ret;
	if(systemset.fotaflag==0)
	 	{
	 		ret =Ql_UART_Register(VIRTUAL_PORT1, Callback_UART_Hdlr, NULL);	
	 		ret = Ql_UART_Open(VIRTUAL_PORT1, 0, FC_NONE);

			ADC_Program();
			
	 	}
}


void Get_Str_Use(u8 *ipstr,u8 *buf)
{
	Ql_memset(ipstr,'\0',sizeof(ipstr));
	while(*buf!=0x20)buf++;
	buf++;
	Ql_strcpy((char*)ipstr,(char*)buf);
}


void Getmac_Str_Use(u8 *ipstr,u8 *sbuf,u8 *buf)
{
	while(*buf!=0x20)buf++;
	buf++;
	while(*buf!=0x20)
		{
			*ipstr=*buf;
			ipstr++;
			buf++;
		}
	buf++;
	while(*buf!=0x20)
		{
			*ipstr=*buf;
			ipstr++;
			buf++;
		}
	buf++;
	while(*buf!=0x20)
		{
			*ipstr=*buf;
			ipstr++;
			buf++;
		}
	buf++;
	while(*buf!=0x20)
		{
			*sbuf=*buf;
			sbuf++;
			buf++;
		}
	buf++;
	while(*buf!=0x20)
		{
			*sbuf=*buf;
			sbuf++;
			buf++;
		}
	buf++;
	while(*buf!=0)
		{
			*sbuf=*buf;
			sbuf++;
			buf++;
		}
	
}


u8 * mid(u8 *dst,u8 *src, int n,int m) 
{
    u8 *p = src;
    u8 *q = dst;
    int len = Ql_strlen((const char*)src);
    if(n>len) n = len-m;    
    if(m<0) m=0;   
    if(m>len) return NULL;
    p += m;
    while(n--) *(q++) = *(p++);
    *(q++)='\0'; 
    return dst;
}


void DeleteMid(u8 *sbuf,u8 *year,u8 *month,u8 *date)
{
	u8 buf[3]={0};
	u8 *p1=buf;
	while(*sbuf!='-')
	{
		*p1=*sbuf;
		p1++;
		sbuf++;
	}
	sbuf++;
	*year=(strtol((char*)buf,NULL,10));
	Ql_memset(buf,0,sizeof(buf));
	p1=buf;
	while(*sbuf!='-')
	{
		*p1=*sbuf;
		p1++;
		sbuf++;
	}
	sbuf++;
	*month=(u8)(strtol((char*)buf,NULL,10));
	*date=(u8)(strtol((char*)sbuf,NULL,10));
}


void GetTime2Use(u8 *year,u8 *month,u8 *date,u8 *hour,u8 *min,u8 *seond,u8 *sbuf)
{
	u8 timebuf[10]={0};
	u8 timebuf1[10]={0};
	u8 *p3=timebuf1;
	u8 buf[3]={0};
	while(*sbuf!=0x20)sbuf++;
	sbuf++;
	while(*sbuf!=0x20)
		{
			*p3=*sbuf;
			sbuf++;
			p3++;
		}
	sbuf++;
	DeleteMid(timebuf1,year,month,date);
	strcpy((char*)timebuf,(char*)sbuf);
	mid(buf,timebuf,2,0);
	*hour=(u8)(strtol((char*)buf,NULL,10));
	mid(buf,timebuf,2,3);
	*min=(u8)(strtol((char*)buf,NULL,10));
	mid(buf,timebuf,2,6);
	*seond=(u8)(strtol((char*)buf,NULL,10));

}


void User_Commad_Ays(u8 *buf,u8 len)
{
	u8 i;
	u8 mybuf[5];
	u8 pataner[5];
	u8 temp[6];
	u16 year=0;
	u8 *p,*p2;
	p=(u8*)strstr((const char *)buf,"$setip");
	if(p!=NULL)
		{
			Ql_memset(systemset.CenterIP,0,sizeof(systemset.CenterIP));
			Get_Str_Use(systemset.CenterIP,p);
			SaveFlashParamsNew(&systemset);
		    mprintf("+IP %s\r\n",(u8*)systemset.CenterIP);

		}
	p=(u8*)strstr((const char *)buf,"$setpot");
	if(p!=NULL)
		{
			Ql_memset(systemset.CenterPort,0,sizeof(systemset.CenterPort));
			Get_Str_Use(systemset.CenterPort,p);
			SaveFlashParamsNew(&systemset);
			mprintf("+PORT %s\r\n",(u8*)systemset.CenterPort);

		}
	p=(u8*)strstr((const char *)buf,"$setapn");
	if(p!=NULL)
		{
			Ql_memset(systemset.Centerapn,0,sizeof(systemset.Centerapn));
			Get_Str_Use(systemset.Centerapn,p);
			SaveFlashParamsNew(&systemset);
		    mprintf("+APN %s\r\n",(u8*)systemset.Centerapn);
		}
	p=(u8*)strstr((const char *)buf,"$setsn");
	if(p!=NULL)
		{
			IsSnHavaSet();
			if(BootInfo!=0xFF)
				{
					Ql_memset(systemset.SN,0,sizeof(systemset.SN));
					Get_Str_Use(systemset.SN,p);
					p2= Ql_strstr(systemset.SN, "\r\n");
					if(p2!=NULL)
						{
							mprintf("格式错误!不可勾选回车换行!!\r\n");
							Ql_sprintf((char*)systemset.SN,"FFFFFFFFFFFF");
						}
					else
						{
							SaveFlashParamsNew(&systemset);
							InternetsetSN_AnalysisFor808(systemset.SN);
							mprintf("+SN %s\r\n",(u8*)systemset.SN);
							IsSnHavaSet();
						}
				}
			else
				{
					mprintf("\r\nModule Has set the SN, you do not have permission to change!!!\r\n");
				}
		}
#if 0
	p=(u8*)strstr((const char*)buf,"$sethnd");
	if(p!=NULL)
		{
			Ql_memset(mybuf,0,sizeof(mybuf));
			Get_Str_Use(mybuf,p);
			systemset.HandInter=strtol((const char*)mybuf,NULL,10);
			SaveFlashParamsNew(&systemset);
			mprintf("+HAND %d\r\n",systemset.HandInter);
		}
#endif
	p=(u8*)strstr((const char*)buf,"$setinter");
	if(p!=NULL)
		{
			Ql_memset(mybuf,0,sizeof(mybuf));
			Get_Str_Use(mybuf,p);
			systemset.Interval=strtol((const char*)mybuf,NULL,10);
			SaveFlashParamsNew(&systemset);
			jishit=0;
			mprintf("+Interval %d\r\n",systemset.Interval);
			jishis=0;
		}
	p=(u8*)strstr((const char*)buf,"$setlbs");
	if(p!=NULL)
		{
			Ql_memset(mybuf,0,sizeof(mybuf));
			Get_Str_Use(mybuf,p);
			systemset.LbsOpen=strtol((const char*)mybuf,NULL,10);
			SaveFlashParamsNew(&systemset);
			mprintf("+LbsSta %d\r\n",systemset.LbsOpen);
		}
	p=(u8*)strstr((const char*)buf,"$tcpip");
	if(p!=NULL)
		{
			Ql_memset(mybuf,0,sizeof(mybuf));
			Get_Str_Use(mybuf,p);
			systemset.TCPorUDP=strtol((const char*)mybuf,NULL,10);
			SaveFlashParamsNew(&systemset);
			mprintf("+IPMODE %s\r\n",modetbl[systemset.TCPorUDP]);
		}
	p=(u8*)strstr((const char*)buf,"$info");
	if(p!=NULL)
		{
			ShowSysInfo();
		}
	p=(u8*)strstr((const char*)buf,"$paswd");
	if(p!=NULL)
		{
			p=(u8*)strstr((const char*)buf,"$paswd null");
			if(p!=NULL)
				{
					Ql_memset(systemset.Passwd,0,sizeof(systemset.Passwd));
					SaveFlashParamsNew(&systemset);
					mprintf("+Passwd NULL\r\n");
					
				}
			else
				{
					Ql_memset(systemset.Passwd,0,sizeof(systemset.Passwd));
					Get_Str_Use(systemset.Passwd,buf);
					SaveFlashParamsNew(&systemset);
					mprintf("+Passwd %s\r\n",systemset.Passwd);
				}

		}
	p=(u8*)strstr((const char*)buf,"$reset");
	if(p!=NULL)
		{
			mprintf("+Reset Ok\r\n");
			SaveFlashParamsNew(&systemset);
			Ql_Sleep(300);
			Ql_Reset(0);
		}
	p=(u8*)strstr((const char*)buf,"$debug 2");
	if(p!=NULL)
		{
			SystemDebug=2;
			mprintf("DEBUG_GSM_ON\r\n");
		}
	p=(u8*)strstr((const char*)buf,"$debug 4");
	if(p!=NULL)
		{
			SystemDebug=4;
			mprintf("DEBUG_SenSor_ON\r\n");
		}
	p=(u8*)strstr((const char*)buf,"$debug 3");
	if(p!=NULL)
		{
			SystemDebug=3;
			mprintf("DEBUG_GPS_ON\r\n");
		}
	p=(u8*)strstr((const char*)buf,"$debug 5");
	if(p!=NULL)
		{
			SystemDebug=5;
			mprintf("DEBUG_ADC_ON\r\n");
		}
	p=(u8*)strstr((const char*)buf,"$debug 0");
	if(p!=NULL)
		{
			SystemDebug=0;
			mprintf("DEBUG_OFF\r\n");
		}
	p=(u8*)strstr((const char *)buf,"$time");
	if(p!=NULL)
		{
			GetTime2Use(&time.year,&time.month,&time.day,&time.hour,&time.minute,&time.second,p);
			SetSysTime(&time);
			GetSysTime(&time);
			mprintf("+time ok\r\n");
		}
	p=(u8*)strstr((const char *)buf,"$ALLON");
	if(p!=NULL)
		{
			LED1_H;
			LED2_H;
			LED3_H;
		}
	p=(u8*)strstr((const char *)buf,"$ALLOFF");
	if(p!=NULL)
		{
			LED1_L;
			LED2_L;
			LED3_L;
		}
	
	p=(u8*)strstr((const char *)buf,"^^setsn");
	if(p!=NULL)
		{
			Ql_memset(systemset.SN,0,sizeof(systemset.SN));
			Get_Str_Use(systemset.SN,p);
			p2= Ql_strstr(systemset.SN, "\r\n");
			if(p2!=NULL)
				{
					mprintf("格式错误!不可勾选回车换行!!\r\n");
					Ql_sprintf((char*)systemset.SN,"FFFFFFFFFFFF");
				}
			else
				{
					SaveFlashParamsNew(&systemset);
					InternetsetSN_AnalysisFor808(systemset.SN);
					mprintf("+SU^SN %s\r\n",(u8*)systemset.SN);
				}
		}
	p=(u8*)strstr((const char *)buf,"^^fotatest");
	if(p!=NULL)
		{
			 systemset.fotaflag=1;
			 SaveFlashParamsNew(&systemset);
			 mprintf("+fotatest ......\r\n");
			 Ql_Sleep(3000);
			 Ql_Reset(0);
		}
	p=(u8*)strstr((const char *)buf,"$$downdata");
	if(p!=NULL)
		{
			u8 i;
			for(i=0;i<Position.saveposion;i++)
				{
					LoadDataFromFlash(&mycoredata,i);
					mprintf("\r\n");
					mprintf("%02X/%02X/%02X %02X:%02X:%02X ",mycoredata.time[0],mycoredata.time[1],mycoredata.time[2],mycoredata.time[3],mycoredata.time[4],mycoredata.time[5]);
					mprintf("%d.%06d ",mycoredata.gpsj/1000000,mycoredata.gpsj%1000000);
					mprintf("%d.%06d ",mycoredata.gpsw/1000000,mycoredata.gpsw%1000000);
					mprintf("%d.%06d ",mycoredata.lbsj/1000000,mycoredata.lbsj%1000000);
					mprintf("%d.%06d ",mycoredata.lbsw/1000000,mycoredata.lbsw%1000000);
					mprintf("%d ",mycoredata.dbm);
					mprintf("%d ",mycoredata.vbai);
					mprintf("%d ",mycoredata.vol);
					mprintf("\r\n");
				}
		}
	p=(u8*)strstr((const char *)buf,"$$bmaid");
	if(p!=NULL)
		{
			bma222_init_client(0x30, 1);
		}
	p=(u8*)strstr((const char *)buf,"$$bmard");
	if(p!=NULL)
		{
			BMA250_ReadSensorData(0x30,NULL);
		}
	p=(u8*)strstr((const char *)buf,"$$PIT:");
	if(p!=NULL)
		{
			u8 pwmSrcClk,pwmDiv;
			u32 lt,ht;
			AynsCmd(buf,&pwmSrcClk,&pwmDiv,&lt,&ht);
			mprintf("pwmSrcClk=%d\r\n",pwmSrcClk);
			mprintf("pwmDiv=%d\r\n",pwmDiv);
			mprintf("lt=%d\r\n",lt);
			mprintf("ht=%d\r\n",ht);
		}
	p=(u8*)strstr((const char *)buf,"$$wzinit");
	if(p!=NULL)
		{
			WZinit();
			mprintf("wzinit ok\r\n");
		}
	p=(u8*)strstr((const char *)buf,"$$wzadd");
	if(p!=NULL)
		{
			SaveData2Flash(&mycoredata);
			mprintf("wzadd ok\r\n");
		}
	p=(u8*)strstr((const char *)buf,"$$wzsub");
	if(p!=NULL)
		{
			WZSubOne();
			mprintf("wzsub ok\r\n");
		}
	p=(u8*)strstr((const char *)buf,"$$wzcurrt");
	if(p!=NULL)
		{
			Ql_memset(mybuf,0,sizeof(mybuf));
			Get_Str_Use(mybuf,p);
			systemset.CallMode=strtol((const char*)mybuf,NULL,10);
			WZAysCuurtSta();
			mprintf("+wzcurrt %d\r\n",systemset.CallMode);
		}
	p=(u8*)strstr((const char *)buf,"$setbtsta 0");
	if(p!=NULL)
		{
			if(systemset.btsta&(1<<0))
				{
					systemset.btsta&=~(1<<2);
					systemset.btsta|=1<<1;
					SaveFlashParamsNew(&systemset);
					mprintf("\r\nsetbtsta ok\r\n");
					cosbtsta=1;
				}
		}
	p=(u8*)strstr((const char *)buf,"$setbtsta 1");
	if(p!=NULL)
		{
			if(systemset.btsta&(1<<0))
				{
					systemset.btsta&=~(1<<1);
					systemset.btsta|=1<<2;
					SaveFlashParamsNew(&systemset);
					mprintf("\r\nsetbtsta ok\r\n");
					cosbtsta=1;
				}
		}
	p=(u8*)strstr((const char *)buf,"$checkdatasta");
	if(p!=NULL)
		{
			CheckSDDATA();
		}
	p=(u8*)strstr((const char *)buf,"^^pcgetimeiandsn^^");
	if(p!=NULL)
		{
			if(BootInfo==0xFF)
				{
					Ql_Sleep(300);
					mprintf("+PC:%s,%s;\r\n",systemset.imei,systemset.SN);
				}
			else
				{
					mprintf("+WAIT:\r\n");
				}
		}
	p=(u8*)strstr((const char *)buf,"$stm8off");
	if(p!=NULL)
		{
			Ql_memset(mybuf,0,sizeof(mybuf));
			Get_Str_Use(mybuf,p);
			year=strtol((const char*)mybuf,NULL,10);
			SendNumFromPin(year);
		}
	p=(u8*)strstr((const char *)buf,"$stm8sleep");
	if(p!=NULL)
		{
			Ql_memset(mybuf,0,sizeof(mybuf));
			Get_Str_Use(mybuf,p);
			year=strtol((const char*)mybuf,NULL,10)|1<<14;
			SendNumFromPin(year);
		}
	p=(u8*)strstr((const char *)buf,"$stm8eclk");	
	if(p!=NULL)
		{
			Ql_memset(mybuf,0,sizeof(mybuf));
			Get_Str_Use(mybuf,p);
			year=strtol((const char*)mybuf,NULL,10)|1<<15;
			SendNumFromPin(year);
		}
	p=(u8*)strstr((const char *)buf,"$stm8wake");
	if(p!=NULL)
		{
			M203C_EINT_H;
			mprintf("\r\M203C_EINT_H\r\n");
			Ql_Sleep(500);
			M203C_EINT_L;
			mprintf("\r\nM203C_WHG_L\r\n");
		}
	p=(u8*)strstr((const char *)buf,"$setstm8ok");
	if(p!=NULL)
		{
			M203C_WHG_H;
			mprintf("\r\nM203C_WHG_H\r\n");
			Ql_Sleep(7000);
			M203C_WHG_L;
			mprintf("\r\nM203C_WHG_L\r\n");
		}
	p=(u8*)strstr((const char *)buf,"$203csleep");
	if(p!=NULL)
		{
			syssleepbit=1;
			mprintf("\r\nM203C_SLEEP_ON\r\n");
		}
	
	
}


float Acceleration[3],AngularVelocity[3],Angle[3],Tem;
u16 tcent=0;


void AysOtherData(u8 *buf,u8 len)
{
	if(len>=33)
		{
			if(buf[0]==0x55)
				{
					if(buf[1]==0x51)
						{
							Acceleration[0] =(float)(buf [3]<<8| buf [2])/32768.0*16;
							Acceleration[1] = (float)(buf [5]<<8| buf [4])/32768.0*16;
							Acceleration[2] =(float)(buf [7]<<8| buf [6])/32768.0*16;
							Tem =(float)(buf [9]<<8| buf [8])/340.0+36.25;
						}
					if(buf[1+11]==0x52)
						{
							AngularVelocity[0] = (float)(buf [3+11]<<8| buf [2+11])/32768.0*2000;
							AngularVelocity[1] = (float)(buf [5+11]<<8| buf [4+11])/32768.0*2000;
							AngularVelocity[2] = (float)(buf [7+11]<<8| buf [6+11])/32768.0*2000;
							Tem = (float)(buf [9+11]<<8| buf [8+11])/340.0+36.25;
						}
					if(buf[1+11+11]==0x53)
						{
							Angle[0] = (float)(buf [3+11+11]<<8| buf [2+11+11])/32768.0*180;
							Angle[1] = (float)(buf [5+11+11]<<8| buf [4+11+11])/32768.0*180;
							Angle[2] = (float)(buf [7+11+11]<<8| buf [6+11+11])/32768.0*180;
							Tem = (float)(buf [9+11+11]<<8| buf [8+11+11])/340.0+36.25;
						}
					
				}
		}
	if(len>=11&&len<22)
		{
			if(buf[0]==0x55)
				{
					if(buf[1]==0x51)
						{
							Acceleration[0] =(float)(buf [3]<<8| buf [2])/32768.0*16;
							Acceleration[1] = (float)(buf [5]<<8| buf [4])/32768.0*16;
							Acceleration[2] =(float)(buf [7]<<8| buf [6])/32768.0*16;
							Tem =(float)(buf [9]<<8| buf [8])/340.0+36.25;
						}
					if(buf[1]==0x52)
						{
							AngularVelocity[0] = (float)(buf [3]<<8| buf [2])/32768.0*2000;
							AngularVelocity[1] = (float)(buf [5]<<8| buf [4])/32768.0*2000;
							AngularVelocity[2] = (float)(buf [7]<<8| buf [6])/32768.0*2000;
							Tem = (float)(buf [9+11]<<8| buf [8+11])/340.0+36.25;
						}
					if(buf[1]==0x53)
						{
							Angle[0] = (float)(buf [3]<<8| buf [2])/32768.0*180;
							Angle[1] = (float)(buf [5]<<8| buf [4])/32768.0*180;
							Angle[2] = (float)(buf [7]<<8| buf [6])/32768.0*180;
							Tem = (float)(buf [9+11+11]<<8| buf [8])/340.0+36.25;
						}
				}
		}
	if(SystemDebug==4)
		{
			tcent++;
			if(tcent>=50)
				{
					tcent=0;
					mprintf("\r\n");
					mprintf("Acceleration:\r\n");
					mprintf("X=%4.3f,Y=%4.3f,Z=%4.3f\r\n",Acceleration[0],Acceleration[1],Acceleration[2]);
					mprintf("AngularVelocity:\r\n");
					mprintf("X=%4.3f,Y=%4.3f,Z=%4.3f\r\n",AngularVelocity[0],AngularVelocity[1],AngularVelocity[2]);
					mprintf("Angle:\r\n");
					mprintf("X=%4.3f,Y=%4.3f,Z=%4.3f\r\n",Angle[0],Angle[1],Angle[2]);
					mprintf("\r\n");
				}
		}
	
}

