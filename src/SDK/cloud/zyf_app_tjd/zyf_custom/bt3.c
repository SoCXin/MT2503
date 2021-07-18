#include "custom_feature_def.h"
#include "ql_stdlib.h"
#include "ql_common.h"
#include "ql_type.h"
#include "ql_trace.h"
#include "ql_error.h"
#include "ql_uart.h"
#include "ql_uart.h"
#include "ql_gprs.h"
#include "ql_socket.h"
#include "ql_timer.h"
#include "ril_sim.h"
#include "ril_network.h"
#include "ril.h"
#include "ril_util.h"
#include "ril_telephony.h"
#include "sys.h"
#include "uart.h"
#include "new_socket.h"
#include "gps.h"
#include "stdlib.h"
#include "lcd.h"
#include "pro.h"
#include "tiem.h"
#include "bt3.h"

u8  cosbtsta=0;
u16 CoreBtBit=0;
u16 CoreBtSta=0;

_bt_manager_ Btmanager;

u8 M203C_send_Btcmd(u8 *cmd,u8 waitnum,u16 waittime)
{
	u8 res=0;
	u8 buf[100]={0};
	Ql_sprintf((char*)buf,"%s\r\n",cmd);
	CoreBtBit&=~waitnum;
	Ql_UART_Write(VIRTUAL_PORT1, (u8*)buf, Ql_strlen(buf));
	if(waittime)
		{
			while(--waittime)
				{
					 Ql_Sleep(10);
					 if(CoreBtBit&waitnum)break;
				}
		}
	CoreBtBit&=~waitnum;
	if(waittime==0)res=1;
	return res;
}

u8 CheckBtPower(void)
{
	u8 t=0;
	t=M203C_send_Btcmd("AT+QBTPWR?",1<<1,300);
	if(t==0)
		{
			if(CoreBtSta&(1<<0))
				{
					CoreBtSta&=~(1<<0);
					return 0;
				}
			else
				{
					return 1;
				}
		}
	return 0;
	
}

u8 CheckBtSerName(void)
{
	u8 t=0;
	u8 namebuf[20]={0};
	t=M203C_send_Btcmd("AT+QBTNAME?",1<<2,300);
	if(t==0)
		{
			if(CoreBtSta&(1<<1))
				{
					CoreBtSta&=~(1<<1);
					return 0;
				}
			else
				{
					Ql_sprintf(namebuf,"AT+QBTNAME=\"%s\"",systemset.btsername);
					M203C_send_Btcmd(namebuf,1<<0,300);
				}
		}
	return 0;
}


u8 CheckClientName(void)
{
	u8 t=0;
	u8 namebuf[20]={0};
	t=M203C_send_Btcmd("AT+QBTNAME?",1<<2,300);
	if(t==0)
		{
			if(CoreBtSta&(1<<1))
				{
					CoreBtSta&=~(1<<1);
					return 0;
				}
			else
				{
					Ql_sprintf(namebuf,"AT+QBTNAME=\"%s\"",systemset.btname);
					M203C_send_Btcmd(namebuf,1<<0,300);
				}
		}
	return 0;
}


u8 CheckBtClientName(void)
{
	u8 t=0;
	u8 namebuf[20]={0};
	t=M203C_send_Btcmd("AT+QBTNAME?",1<<2,300);
	if(t==0)
		{
			if(CoreBtSta&(1<<1))
				{
					CoreBtSta&=~(1<<1);
					return 0;
				}
			else
				{
					Ql_sprintf(namebuf,"AT+QBTNAME=\"%s\"",systemset.btname);
					M203C_send_Btcmd(namebuf,1<<0,300);
				}
		}
	return 0;
}


u8 CheckBtPariSta(void)
{
	u8 t=0;
	t=M203C_send_Btcmd("AT+QBTSTATE",1<<3,300);
	if(t==0)
		{
			if(CoreBtSta&(1<<2))
				{
					CoreBtSta&=~(1<<2);
					return 0;
				}
			else if(CoreBtSta&(1<<3))
				{
					CoreBtSta&=~(1<<3);
					return 1;
				}
			else
				{
					return 2;
				}
			
		}
	return 2;
}

u8 BtScaning(void)
{
	u8 t=0;
RScan:
	t=M203C_send_Btcmd("AT+QBTSCAN=10,5,0",1<<4,1200);
	if(t==0)
		{
			if(CoreBtSta&(1<<4))
				{
					CoreBtSta&=~(1<<4);
					if(!(CoreBtSta&(1<<5)))
						{
							M203C_send_Btcmd("AT+QBTSCANC",1<<4,200);	
						}
					return 0;
				}
			else
				{
					if(CoreBtSta&(1<<5))
						{
							CoreBtSta&=~(1<<5);
						}
					else
						{
							M203C_send_Btcmd("AT+QBTSCANC",1<<4,200);
						}
					Ql_Sleep(1000);
					goto RScan;
					
				}
			
		}
	else
		{
			M203C_send_Btcmd("AT+QBTSCANC",1<<4,200);
			goto RScan;
			
		}
	return 1;
}

void BtaysPairId(u8 *buf,u8 *parid)
{
	u8 *p;
	p=(u8*)strstr((char*)buf,"+QBTSCAN: 1");
	if(p!=NULL)
		{
			*parid=1;
		}
	else
		{
			p=(u8*)strstr((char*)buf,"+QBTSCAN:1");
			if(p!=NULL)
				{
					*parid=1;
				}
		}
	p=(u8*)strstr((char*)buf,"+QBTSCAN: 2");
	if(p!=NULL)
		{
			*parid=2;
		}
	else
		{
			p=(u8*)strstr((char*)buf,"+QBTSCAN:2");
			if(p!=NULL)
				{
					*parid=2;
				}
		}
	p=(u8*)strstr((char*)buf,"+QBTSCAN: 3");
	if(p!=NULL)
		{
			*parid=3;
		}
	else
		{
			p=(u8*)strstr((char*)buf,"+QBTSCAN:3");
			if(p!=NULL)
				{
					*parid=3;
				}
		}
	p=(u8*)strstr((char*)buf,"+QBTSCAN: 4");
	if(p!=NULL)
		{
			*parid=4;
		}
	else
		{
			p=(u8*)strstr((char*)buf,"+QBTSCAN:4");
			if(p!=NULL)
				{
					*parid=4;
				}
		}
	p=(u8*)strstr((char*)buf,"+QBTSCAN: 5");
	if(p!=NULL)
		{
			*parid=5;
		}
	else
		{
			p=(u8*)strstr((char*)buf,"+QBTSCAN:5");
			if(p!=NULL)
				{
					*parid=5;
				}
		}
	
}

void BtaysConnetId(u8 *buf,u8 *connetid)
{
	u8 *p;
	p=(u8*)strstr((char*)buf,"+QBTPAIRCNF: 1,1");
	if(p!=NULL)
		{
			*connetid=1;
		}
	else
		{
			p=(u8*)strstr((char*)buf,"+QBTPAIRCNF:1,1");
			if(p!=NULL)
				{
					*connetid=1;
				}
		}
	p=(u8*)strstr((char*)buf,"+QBTPAIRCNF: 1,2");
	if(p!=NULL)
		{
			*connetid=2;
		}
	else
		{
			p=(u8*)strstr((char*)buf,"+QBTPAIRCNF:1,2");
			if(p!=NULL)
				{
					*connetid=2;
				}
		}
	p=(u8*)strstr((char*)buf,"+QBTPAIRCNF: 1,3");
	if(p!=NULL)
		{
			*connetid=3;
		}
	else
		{
			p=(u8*)strstr((char*)buf,"+QBTPAIRCNF:1,3");
			if(p!=NULL)
				{
					*connetid=3;
				}
		}
	p=(u8*)strstr((char*)buf,"+QBTPAIRCNF: 1,4");
	if(p!=NULL)
		{
			*connetid=4;
		}
	else
		{
			p=(u8*)strstr((char*)buf,"+QBTPAIRCNF:1,4");
			if(p!=NULL)
				{
					*connetid=4;
				}
		}
	p=(u8*)strstr((char*)buf,"+QBTPAIRCNF: 1,5");
	if(p!=NULL)
		{
			*connetid=5;
		}
	else
		{
			p=(u8*)strstr((char*)buf,"+QBTPAIRCNF:1,5");
			if(p!=NULL)
				{
					*connetid=5;
				}
		}
}

void BtaysSendId(u8 *buf,u8 *connetid)
{
	u8 *p;
	p=(u8*)strstr((char*)buf,"+QBTCONN: 1,1");
	if(p!=NULL)
		{
			*connetid=1;
		}
	else
		{
			p=(u8*)strstr((char*)buf,"+QBTCONN:1,1");
			if(p!=NULL)
				{
					*connetid=1;
				}
		}
	p=(u8*)strstr((char*)buf,"+QBTCONN: 1,2");
	if(p!=NULL)
		{
			*connetid=2;
		}
	else
		{
			p=(u8*)strstr((char*)buf,"+QBTCONN:1,2");
			if(p!=NULL)
				{
					*connetid=2;
				}
		}
	p=(u8*)strstr((char*)buf,"+QBTCONN: 1,3");
	if(p!=NULL)
		{
			*connetid=3;
		}
	else
		{
			p=(u8*)strstr((char*)buf,"+QBTCONN:1,3");
			if(p!=NULL)
				{
					*connetid=3;
				}
		}
	p=(u8*)strstr((char*)buf,"+QBTCONN: 1,4");
	if(p!=NULL)
		{
			*connetid=4;
		}
	else
		{
			p=(u8*)strstr((char*)buf,"+QBTCONN:1,4");
			if(p!=NULL)
				{
					*connetid=4;
				}
		}
	p=(u8*)strstr((char*)buf,"+QBTCONN: 1,5");
	if(p!=NULL)
		{
			*connetid=5;
		}
	else
		{
			p=(u8*)strstr((char*)buf,"+QBTCONN:1,5");
			if(p!=NULL)
				{
					*connetid=5;
				}
		}
}

void BtaysReadId(u8 *buf,u8 *readid)
{
	u8 *p;
	p=(u8*)strstr((char*)buf,",1");
	if(p!=NULL)
		{
			*readid=1;
		}
	p=(u8*)strstr((char*)buf,",2");
	if(p!=NULL)
		{
			*readid=2;
		}
	p=(u8*)strstr((char*)buf,",3");
	if(p!=NULL)
		{
			*readid=3;
		}
	p=(u8*)strstr((char*)buf,",4");
	if(p!=NULL)
		{
			*readid=4;
		}
	p=(u8*)strstr((char*)buf,",5");
	if(p!=NULL)
		{
			*readid=5;
		}
}

void Bt_SerInit(void)
{
	M203C_send_Btcmd("AT+QBTPWR=1",1<<0,300);
	Ql_Sleep(50);
	CheckBtPower();
	Ql_Sleep(50);
	CheckBtSerName();
	Ql_Sleep(50);
	
}

void Bt_ClientInit(void)
{
	M203C_send_Btcmd("AT+QBTPWR=1",1<<0,300);
	Ql_Sleep(50);
	CheckBtPower();
	Ql_Sleep(50);
	CheckBtClientName();
	Ql_Sleep(50);
	
}


void Bt_Clint_Init(void)
{
	u8 t;
	u8 sta=0;
	u8 tbuf[20]={0};
	sta=CheckBtPariSta();
	if(sta==2)
		{
			BtScaning();
Rpari:
			Ql_sprintf(tbuf,"AT+QBTPAIR=%d",Btmanager.parid);
			M203C_send_Btcmd(tbuf,1<<5,500);
			t=M203C_send_Btcmd("AT+QBTPAIRCNF=1",1<<6,3000);
			if(t==0)
				{
					if(CoreBtSta&(1<<6))
						{
							CoreBtSta&=~(1<<6);
						}
					if(CoreBtSta&(1<<7))
						{
							CoreBtSta&=~(1<<7);
							Ql_Sleep(2000);
							goto Rpari;
						}
				}
			else
				{
					Ql_Sleep(2000);
					goto Rpari;
				}
		}
	if(sta==1||sta==2)
		{
			Ql_sprintf(tbuf,"AT+QBTCONN=%d,0,1",Btmanager.connectid);
RECONNECT:
			t=M203C_send_Btcmd(tbuf,1<<7,6000);
			if(t==0)
				{
					if(CoreBtSta&(1<<8))
						{
							CoreBtSta&=~(1<<8);
						}
					if(CoreBtSta&(1<<9))
						{
							CoreBtSta&=~(1<<9);
							Ql_Sleep(2000);
							goto RECONNECT;
						}
				}
			else
				{
					goto RECONNECT;	
				}
		}
}



void Bt_Clint_SendData(u8 *buf,u16 len)
{
	u8 res=0,i;
	u8 GcCmdIntfeer[20];
	Ql_sprintf((char*)GcCmdIntfeer,"AT+QSPPSEND=%d,%d",Btmanager.sendid,len);
	res=M203C_send_Btcmd(GcCmdIntfeer,1<<8,1000);
	if(res)return res;
	else
		{
			if(SystemDebug==2)
				{
					mprintf("\r\n");
					for(i=0;i<len;i++)mprintf("%02X",buf[i]);
					mprintf("\r\n");
				}
			Ql_UART_Write(VIRTUAL_PORT1, (u8*)buf, len);
		}
}

void Bt_Server_js(void)
{
	u8 tbuf[20]={0};
	u8 num,res;
	if(CoreBtBit&(1<<10))
		{
			CoreBtBit&=~(1<<10);
		}
	if(CoreBtBit&(1<<11))
		{
			CoreBtBit&=~(1<<11);
			CoreDataInit_v3(&num,0x4E22);
			M203CSendDataNoAck(PROBUF,num,0);
			M203CSendDataCheckOK(500);
		}
	if(CoreBtSta&(1<<11))
		{
			M203C_send_Btcmd("AT+QBTPAIRCNF=1",1<<0,300);
			CoreBtSta&=~(1<<11);
		}
	if(CoreBtSta&(1<<12))
		{
			M203C_send_Btcmd("AT+QBTACPT=1",1<<0,300);
			CoreBtSta&=~(1<<12);
		}
	if(CoreBtSta&(1<<13))
		{
			
		}
	if(CoreBtBit&(1<<12))
		{
			Ql_sprintf(tbuf,"AT+QSPPREAD=%d,20\r\n",Btmanager.readid);
			Ql_UART_Write(VIRTUAL_PORT1, (u8*)tbuf, Ql_strlen(tbuf));
			CoreBtBit&=~(1<<12);
		}
}

void BtAsysData(u8 *buf,u8 *dst)
{
	u8 num=0;
	u8 *p=buf;
	u8 tem[20]={0};
	u8 *s=tem;
	while(*p!=':')
		{
			p++;
		}
	p++;
	while(*p!=0x0d)
		{
			*s=*p;
			s++;
			p++;
		}
	p++;
	p++;
	num=strtol((const char*)tem,NULL,10);
	if(num>0)Ql_strncpy(dst,p,num);
}


void ZFBtAsysData(u8 *buf,u8 *dst)
{
	u8 *p=buf;
	p+=3;
	while(*p!='"')
		{
			*dst=*p;
			dst++;
			p++;
		}
	*dst=0;
}


u8 BtServerScaning(void)
{
	u8 t=0;
RScan:
	t=M203C_send_Btcmd("AT+QBTSCAN=20,10,0",1<<4,3000);
	if(t==0)
		{
			if(CoreBtSta&(1<<4))
				{
					CoreBtSta&=~(1<<4);
					if(!(CoreBtSta&(1<<5)))
						{
							M203C_send_Btcmd("AT+QBTSCANC",1<<4,200);	
						}
					return 0;
				}
			else
				{
					if(CoreBtSta&(1<<5))
						{
							CoreBtSta&=~(1<<5);
						}
					else
						{
							M203C_send_Btcmd("AT+QBTSCANC",1<<4,200);
						}
					return 1;
					
				}
			
		}
	else
		{
			return 1;
			
		}
	return 1;
}

void BtNewInit(void)
{
	if(systemset.btsta&(1<<0))		//蓝牙打开
		{
			if(systemset.btsta&(1<<1))	//蓝牙设置为主机模式
				{
					LED2_L;
					Bt_SerInit();
				}
			if(systemset.btsta&(1<<2))	//蓝牙设置为从机模式
				{
					LED2_H;
					Bt_ClientInit();
				}
		}
}


void BtNewInit2(void)
{
	if(systemset.btsta&(1<<0))		//蓝牙打开
		{
			if(systemset.btsta&(1<<1))	//蓝牙设置为主机模式
				{
					LED2_L;
					CheckBtPower();
					Ql_Sleep(50);
					CheckBtSerName();
					Ql_Sleep(50);
				}
			if(systemset.btsta&(1<<2))	//蓝牙设置为从机模式
				{
					LED2_H;
					CheckBtPower();
					Ql_Sleep(50);
					CheckClientName();
					Ql_Sleep(50);
				}
		}
}



u8 ScanSerIng(void)
{
	u8 res;
	u8 num;
	u8 error=0;
	if(systemset.btsta&(1<<0))
		{
RE:
			if(systemset.btsta&(1<<1))			//主机模式
				{
					res=BtServerScaning();
					if(res==0)
						{
							error=0;
							CoreDataInit_v3(&num,0x4E22);
							M203CSendDataNoAck(PROBUF,num,0);
							M203CSendDataCheckOK(500);
						}
					else
						{
							error++;
							if(error<2)
								{
									M203C_send_Btcmd("AT+QBTPWR=0",1<<0,300);
									M203C_send_Btcmd("AT+QBTPWR=1",1<<0,300);
									goto RE;
								}
						}
				}
		}
	return 0;
}






