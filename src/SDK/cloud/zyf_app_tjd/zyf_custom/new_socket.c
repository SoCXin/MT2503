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
#include "flash.h"


u8 M203CIsBusy=0;

u32 CoreReDataBit=0;

u16 CoreAbnormalBit=0;

u16 CoreEpoFunBit=0;

u8 CREG_ERROR=0;

u8 GpsOrLbsBit=0;

u8 LbsInfoBit=0;


void GetImei(u8 *drt,u8 *src)
{
	while(*src!='"')
		{
			src++;
		}
	src++;
	while(*src!='"')
		{
			*drt=*src;
			drt++;
			src++;
		}
	*drt=0;
}



u8 M203C_send_cmd(u8 *cmd,u8 waitnum,u16 waittime)
{
	u8 res=0;
	u8 buf[100]={0};
	Ql_sprintf((char*)buf,"%s\r\n",cmd);
	Ql_UART_Write(VIRTUAL_PORT1, (u8*)buf, Ql_strlen(buf));
	if(waittime)
		{
			while(--waittime)
				{
					 Ql_Sleep(10);
					 if(CoreReDataBit&waitnum)break;
				}
		}
	CoreReDataBit&=~waitnum;
	if(waittime==0)res=1;
	return res;
}


u8 M203C_send_Epocmd(u8 *cmd,u8 waitnum,u16 waittime)
{
	u8 res=0;
	u8 buf[100]={0};
	Ql_sprintf((char*)buf,"%s\r\n",cmd);
	Ql_UART_Write(VIRTUAL_PORT1, (u8*)buf, Ql_strlen(buf));
	if(waittime)
		{
			while(--waittime)
				{
					 Ql_Sleep(10);
					 if(CoreEpoFunBit&waitnum)break;
				}
		}
	CoreEpoFunBit&=~waitnum;
	if(waittime==0)res=1;
	return res;
}



u8 M203C_send_Abnormalcmd(u8 *cmd,u8 waitnum,u16 waittime)
{
	u8 res=0;
	u8 buf[100]={0};
	Ql_sprintf((char*)buf,"%s\r\n",cmd);
	Ql_UART_Write(VIRTUAL_PORT1, (u8*)buf, Ql_strlen(buf));
	if(waittime)
		{
			while(--waittime)
				{
					 Ql_Sleep(10);
					 if(CoreAbnormalBit&waitnum)break;
				}
		}
	CoreAbnormalBit&=~waitnum;
	if(waittime==0)res=1;
	return res;
}

u8  M203C_GetNetStsu(u8 cent)
{
	u8 t=cent;
RE:
	M203C_send_cmd("AT+CREG?",1<<3,300);
	if(CREG_ERROR&&t>0)
		{
			t--;
			Ql_Sleep(300);
			goto RE;
			
		}
}

void M203C_init(void)
{
	u8 tcent=20;
	Ql_Sleep(1000);
	while(M203C_send_cmd("AT",1<<0,300))
		{
			tcent--;
			Ql_Sleep(300);
			if(tcent==0)
				{
					tcent=20;
					break;
				}
		}

	Ql_Sleep(50);


	M203C_send_Epocmd("AT+QGNSSC=1",1<<7,50);
	Ql_Sleep(50);
	
	M203C_send_cmd("AT+CPIN?",1<<2,300);
	Ql_Sleep(50);

	M203C_send_cmd("AT+CREG?",1<<3,300);
	Ql_Sleep(50);

	M203C_send_cmd("AT+CSQ",1<<4,300);
	Ql_Sleep(50);

	M203C_GetNetStsu(100);
	Ql_Sleep(50);

	M203C_send_Abnormalcmd("AT+CBC",1<<4,300);
	Ql_Sleep(50);

	M203C_send_cmd("AT+CMGF=1",1<<5,50);
	Ql_Sleep(50);

	M203C_send_cmd("AT+CSCS=\"GSM\"",1<<6,50);
	Ql_Sleep(50);

	M203C_send_cmd("AT+CNMI=2,2",1<<7,50);
	Ql_Sleep(50);

	M203C_send_cmd("AT+QGSN",1<<7,50);
	Ql_Sleep(50);
	
	M203C_send_cmd("AT+CCID",1<<7,50);
	Ql_Sleep(50);
	
	
}


void GetimeiInit(void)
{
	u8 tcent=20;
	Ql_Sleep(1000);
	while(M203C_send_cmd("AT",1<<0,300))
		{
			tcent--;
			Ql_Sleep(300);
			if(tcent==0)
				{
					tcent=20;
					break;
				}
		}

	Ql_Sleep(50);

	M203C_send_cmd("AT+QGSN",1<<7,50);
	Ql_Sleep(50);
}


void M203C_gprs_init(void)
{
	u8 tcent=3;
	u8 bufs[35]={0};

	Ql_sprintf((char*)bufs,"AT+QICSGP=1,\"%s\"",systemset.Centerapn);
	Ql_Sleep(50);

	M203C_send_cmd("AT+QIFGCNT=0",1<<8,50);
	Ql_Sleep(50);

	M203C_send_cmd(bufs,1<<9,50);
	Ql_Sleep(50);


	M203C_send_cmd("AT+QIMUX=1",1<<10,50);
	Ql_Sleep(50);

	M203C_send_cmd("AT+QIMODE=0",1<<11,50);
	Ql_Sleep(50);

	
	
	
}

void M203C_Ch2Gprs(void)
{
	M203C_send_cmd("AT+QIFGCNT=0",1<<8,50);
	Ql_Sleep(50);
}

void DisconnectSocket(u8 curline)
{
	u8 p[30]={0};
	Ql_sprintf((char*)p,"AT+QICLOSE=%d",curline);
	M203C_send_cmd(p,1<<16,1000);
}


u8 M203C_EPO_Init(u8 *lot,u8 *lat)
{
	u8 tebuf[50]={0};
	
	M203C_send_Epocmd("AT+QIFGCNT=2",1<<0,50);
	Ql_Sleep(50);

	M203C_send_Epocmd("AT+QICSGP=1,\"CMNET\"",1<<1,50);
	Ql_Sleep(50);

	M203C_GetNetStsu(100);
	Ql_Sleep(50);
	
	SetUtc2Modu();
	
	if(IsHaveLbsData())
		{
			Ql_sprintf(tebuf,"AT+QGREFLOC=%s,%s",lat,lot);
		}
	else
		{
			return 1;
		}
	
	M203C_send_Epocmd(tebuf,1<<2,50);
	Ql_Sleep(50);

	M203C_send_Epocmd("AT+QGNSSEPO=1",1<<3,50);
	Ql_Sleep(50);

	M203C_send_Epocmd("AT+QGEPOAID",1<<5,50);
	Ql_Sleep(50);

	

	return 0;
}




u8 M203C_tcpudp_conncet(u8 mode,u8* ipaddr,u8* port,u8 curline)
{
	u8 tcent=3;
	u8 res=0;
	u16 waittime=2000;
	u8 p[100]={0};
SERESET:
	Ql_sprintf((char*)p,"AT+QIOPEN=%d,\"%s\",\"%s\",%s",curline,modetbl[mode],ipaddr,port);
	M203C_send_cmd(p,1<<12,150);
	while(--waittime)
		{
			Ql_Sleep(10);
			if(CoreReDataBit&(1<<13))break;
			if(CoreReDataBit&(1<<14))break;
			if(CoreReDataBit&(1<<15))
				{
					res=2;
					break;
				}
		}
	if(waittime==0)res=1;
	if(res==2)
		{
			tcent--;
			if(tcent==0)
				{
					res=1;
					return 1;
				}
			Ql_sprintf((char*)p,"AT+QICLOSE=%d",curline);
			M203C_send_cmd(p,1<<16,3000);
			goto SERESET;
		}
	return res;
	
}


u8 M203CSendDataNoAck(u8 *data,u16 length,u8 curline)
{
	u8 res=0,i;
	u8 GcCmdIntfeer[20];
	Ql_sprintf((char*)GcCmdIntfeer,"AT+QISEND=%d,%d",curline,length);
	res=M203C_send_Abnormalcmd(GcCmdIntfeer,1<<1,1000);
	if(res)return res;
	else
		{
			if(SystemDebug==2)
				{
					mprintf("\r\n");
					for(i=0;i<length;i++)mprintf("%02X",data[i]);
					mprintf("\r\n");
				}
			Ql_UART_Write(VIRTUAL_PORT1, (u8*)data, length);
		}
	return res;
	
}

u8 M203CSendDataCheckOK(u16 waittime)
{
	u8 res=0;
	while(--waittime)	
		{
			Ql_Sleep(10);
			if(CoreAbnormalBit&(1<<2))break;
		}
	CoreAbnormalBit&=~(1<<2);
	if(waittime==0)res=1;
	return res;
}



//////////////////////////////////////////////////////////////////////////////


void GetGsmCsq(void)
{
	M203C_send_cmd("AT+CSQ",1<<4,300);
}

void GetModuVol(void)
{
	M203C_send_Abnormalcmd("AT+CBC",1<<4,300);
}


void mystrcpy(u8 *src,u8 *drt,u16 *lens)
{
	u8 sbuf[5]={0};
	u8 *p=sbuf;
	u16 i,j;
	while(*src!=',')src++;
	src++;
	while(*src!=0x0D)
		{
			*p=*src;
			src++;
			p++;
		}
	*lens=strtol((char*)sbuf,NULL,10);
	j=*lens;
	while(*src!=0x7E)
		{
			src++;
		}
	for(i=0;i<j;i++)
		{
			*drt=*src;
			drt++;
			src++;
		}
}


void AysConet2Core(u8 *buf,u8 len)
{
	u8 *p;
	u8 mybuf[150]={0}; 
	u16 mylen=0;    
	u8 i;
	u8 tbuf[10]={0};
	u8 tbuf1[10]={0};
	if(SystemDebug==2)
		{
			mprintf("%s\r\n",buf);
		}
	p = (u8*)strstr((const char *)buf, "AT");
	if(p!=NULL)
		{
			
			p = (u8*)strstr((const char *)buf, "AT+");
			if(p!=NULL)
				{
					p=(u8*)strstr((const char *)buf,"AT+CPIN");
					if(p!=NULL)
						{
							p=(u8*)strstr((const char *)buf,"+CPIN:");
							if(p!=NULL)
								{
									CoreReDataBit|=1<<2;
								}
							
						}
					p=(u8*)strstr((const char *)buf,"AT+CREG");
					if(p!=NULL)
						{
							p=(u8*)strstr((const char *)buf,"+CREG:");
							if(p!=NULL)
								{
									CoreReDataBit|=1<<3;
									p=(u8*)strstr((const char *)buf,"0,1");
									if(p!=NULL)CREG_ERROR=0;
									else
										{
											p=(u8*)strstr((const char *)buf,"1,1");
											if(p!=NULL)CREG_ERROR=0;
											else
												{
													p=(u8*)strstr((const char *)buf,"0,5");
													if(p!=NULL)CREG_ERROR=0;
													else
														{
															p=(u8*)strstr((const char *)buf,"1,5");
															if(p!=NULL)CREG_ERROR=0;
															else
																{
																	CREG_ERROR=1;
																}
														}
												}
										}
								}
							
						}
					p=(u8*)strstr((const char *)buf,"AT+CSQ");
					if(p!=NULL)
						{
							p=(u8*)strstr((const char *)buf,"+CSQ:");
							if(p!=NULL)
								{
									CoreReDataBit|=1<<4;
									p=(u8*)strstr((const char *)buf,"99,99");
									if(p!=NULL)
										{
											Signal=0;
											mycoredata.dbm=Signal;
										}
									else
										{
											p=(u8*)strstr((const char *)buf,":");
											if(p!=NULL)
												{
													
													GetCsQ(tbuf,p);
													Signal=strtol((const char*)tbuf,NULL,10);
													mycoredata.dbm=Signal;
													
												}
										}
								}
							
						}
					p=(u8*)strstr((const char *)buf,"AT+CBC");
					if(p!=NULL)
						{
							p=(u8*)strstr((const char *)buf,"+CBC:");
							if(p!=NULL)
								{
									CoreAbnormalBit|=1<<4;
									p=(u8*)strstr((const char *)buf,":");
									if(p!=NULL)
										{
											GetVol(tbuf,tbuf1,p);
											mycoredata.vol=(strtol((const char*)tbuf1,NULL,10))&0xffff;
											mycoredata.vbai=strtol((const char*)tbuf,NULL,10);
										}
								}
						}
					p=(u8*)strstr((const char *)buf,"AT+CMGF");
					if(p!=NULL)
						{
							p=(u8*)strstr((const char *)buf,"OK");
							if(p!=NULL)
								{
									CoreReDataBit|=1<<5;
								}
							
						}
					p=(u8*)strstr((const char *)buf,"AT+CSCS");
					if(p!=NULL)
						{
							p=(u8*)strstr((const char *)buf,"OK");
							if(p!=NULL)
								{
									CoreReDataBit|=1<<6;
								}
							
						}
					p=(u8*)strstr((const char *)buf,"AT+CNMI");
					if(p!=NULL)
						{
							p=(u8*)strstr((const char *)buf,"OK");
							if(p!=NULL)
								{
									CoreReDataBit|=1<<7;
								}
							
						}
					p=(u8*)strstr((const char *)buf,"AT+QIFGCNT");
					if(p!=NULL)
						{
							p=(u8*)strstr((const char *)buf,"OK");
							if(p!=NULL)
								{
									CoreReDataBit|=1<<8;
									CoreEpoFunBit|=1<<0;
								}
							
						}
					p=(u8*)strstr((const char *)buf,"AT+QICSGP");
					if(p!=NULL)
						{
							CoreReDataBit|=1<<9;
							CoreEpoFunBit|=1<<1;
						}
					p=(u8*)strstr((const char *)buf,"AT+QIMUX");
					if(p!=NULL)
						{
							p=(u8*)strstr((const char *)buf,"OK");
							if(p!=NULL)
								{
									CoreReDataBit|=1<<10;
								}
							
						}
					p=(u8*)strstr((const char *)buf,"AT+QIMODE");
					if(p!=NULL)
						{
							p=(u8*)strstr((const char *)buf,"OK");
							if(p!=NULL)
								{
									CoreReDataBit|=1<<11;
								}
							
						}
					p=(u8*)strstr((const char *)buf,"AT+QIOPEN");
					if(p!=NULL)
						{
							p=(u8*)strstr((const char *)buf,"OK");
							if(p!=NULL)
								{
									CoreReDataBit|=1<<12;
									p = (u8*)strstr((const char *)buf, "ALREADY CONNECT");
									if(p!=NULL)
										{
											CoreReDataBit|=1<<13;
										}
									p = (u8*)strstr((const char *)buf, "CONNECT OK");
									if(p!=NULL)
										{
											CoreReDataBit|=1<<14;
										}
									p = (u8*)strstr((const char *)buf, "CONNECT FAIL");
									if(p!=NULL)
										{
											CoreReDataBit|=1<<15;
										}
								}
							
						}
					p=(u8*)strstr((const char *)buf,"AT+QICLOSE");
					if(p!=NULL)
						{
							p=(u8*)strstr((const char *)buf,"OK");
							if(p!=NULL)
								{
									CoreReDataBit|=1<<16;
									p=(u8*)strstr((const char *)buf,"CLOSE OK");
									if(p!=NULL)
										{
											CoreReDataBit|=1<<16;
										}
								}
							
						}
					p=(u8*)strstr((const char *)buf,"AT+QISEND");
					if(p!=NULL)
						{
							p=(u8*)strstr((const char *)buf,">");
							if(p!=NULL)
								{
									CoreAbnormalBit|=1<<1;
								}
							
						}
					p=(u8*)strstr((const char *)buf,"AT+QCELLLOC");
					if(p!=NULL)
						{
							p=(u8*)strstr((const char *)buf,"+QCELLLOC:");
							if(p!=NULL)
								{
									CoreAbnormalBit|=1<<0;
									GetLocationValue(&LbsJingdu,&LbsWeidu,p);
									AsysLbs(&my_core_data);
									GpsOrLbsBit=1;
								}
						}
					p=(u8*)strstr((const char *)buf,"AT+QGREFLOC");
					if(p!=NULL)
						{
							CoreEpoFunBit|=1<<2;
						}
					p=(u8*)strstr((const char *)buf,"AT+QGNSSEPO");
					if(p!=NULL)
						{
							CoreEpoFunBit|=1<<3;
						}
					p=(u8*)strstr((const char *)buf,"AT+QGNSSC");
					if(p!=NULL)
						{
							CoreEpoFunBit|=1<<7;
						}
				
				}
			else
				{
					p=(u8*)strstr((const char *)buf,"OK");
					if(p!=NULL)
						{
							CoreReDataBit|=1<<0;
						}
				}
		}
	else
		{
			
			p=(u8*)strstr((const char *)buf,"+CPIN:");
			if(p!=NULL)
				{
					CoreReDataBit|=1<<2;
				}
			p=(u8*)strstr((const char *)buf,"+CREG:");
			if(p!=NULL)
				{
					CoreReDataBit|=1<<3;
					p=(u8*)strstr((const char *)buf,"0,1");
					if(p!=NULL)CREG_ERROR=0;
					else
						{
							p=(u8*)strstr((const char *)buf,"1,1");
							if(p!=NULL)CREG_ERROR=0;
							else
								{
									p=(u8*)strstr((const char *)buf,"0,5");
									if(p!=NULL)CREG_ERROR=0;
									else
										{
											p=(u8*)strstr((const char *)buf,"1,5");
											if(p!=NULL)CREG_ERROR=0;
											else
												{
													CREG_ERROR=1;
												}
										}
								}
						}
					
				}
			p=(u8*)strstr((const char *)buf,"+CSQ:");
			if(p!=NULL)
				{
					CoreReDataBit|=1<<4;
					p=(u8*)strstr((const char *)buf,"99,99");
					if(p!=NULL)
						{
							Signal=0;
							mycoredata.dbm=0;
						}
					else
						{
							p=(u8*)strstr((const char *)buf,":");
							if(p!=NULL)
								{
									
									GetCsQ(tbuf,p);
									Signal=strtol((const char*)tbuf,NULL,10);
									mycoredata.dbm=Signal;
									
								}
						}
				}
			p=(u8*)strstr((const char *)buf,"+CBC:");
			if(p!=NULL)
				{
					CoreAbnormalBit|=1<<4;
					p=(u8*)strstr((const char *)buf,":");
					if(p!=NULL)
						{
							GetVol(tbuf,tbuf1,p);
							mycoredata.vbai=strtol((const char*)tbuf,NULL,10);
							mycoredata.vol=(strtol((const char*)tbuf1,NULL,10))&0xffff;
							
						}
				}
			p = (u8*)strstr((const char *)buf, "ALREADY CONNECT");
			if(p!=NULL)
				{
					CoreReDataBit|=1<<13;
				}
			p = (u8*)strstr((const char *)buf, "CONNECT OK");
			if(p!=NULL)
				{
					CoreReDataBit|=1<<14;
				}
			p = (u8*)strstr((const char *)buf, "CONNECT FAIL");
			if(p!=NULL)
				{
					CoreReDataBit|=1<<15;
				}
			p=(u8*)strstr((const char *)buf,"CLOSE OK");
			if(p!=NULL)
				{
					CoreReDataBit|=1<<16;
				}
			p=(u8*)strstr((const char *)buf,"+QCELLLOC:");
			if(p!=NULL)
				{
					LbsInfoBit=0;
					if(gpsx.useorno!=65)
						{
							CoreAbnormalBit|=1<<0;
							GetLocationValue(&LbsJingdu,&LbsWeidu,p);
							if(LbsJingdu<(360*1000000)&&LbsWeidu<(360*1000000))
								{
									mycoredata.lbsj=LbsJingdu;
									mycoredata.lbsw=LbsWeidu;
									AsysLbs(&my_core_data);
								}
							GpsOrLbsBit=1;
						}
				}
			p=(u8*)strstr((const char *)buf,">");
			if(p!=NULL)
				{
					CoreAbnormalBit|=1<<1;
				}
			p=(u8*)strstr((const char *)buf,"SEND OK");
			if(p!=NULL)
				{
					CoreAbnormalBit|=1<<2;
				}
			
		}
	p=(u8*)strstr((char*)buf,"+RECEIVE");
	if(p!=NULL)
		{
			mystrcpy(buf,mybuf,&mylen);
			if(mylen<=150)
				{
					if(SystemDebug==2)
						{
							mprintf("RE:\r\n");
							for(i=0;i<mylen;i++)mprintf("%02X",mybuf[i]);
							mprintf("\r\n");
						}
					DataCore(mybuf,mylen);
				}
		}
	p=(u8*)strstr((char*)buf,"+QGNSSRD:");
	if(p!=NULL)
		{
			GpsAnalysis((u8*)buf);
			if(gpsx.latitude<(360*1000000)&&gpsx.longitude<(360*1000000))
				{
					mycoredata.gpsw=gpsx.latitude;
					mycoredata.gpsj=gpsx.longitude;
					GPS_package(&gpsx,&my_core_data);
				}
			CoreAbnormalBit|=1<<6;
		}
	p=(u8*)strstr((char*)buf,"+CCLK:");
	if(p!=NULL)
		{
			Ql_sprintf(ostimebuf,"%s",buf);
			CoreEpoFunBit|=1<<4;
		}
	p=(u8*)strstr((char*)buf,"+CME ERROR:");
	if(p!=NULL)
		{
			if(LbsInfoBit)
				{
					LbsInfoBit=0;
					CoreAbnormalBit|=1<<0;
					GpsOrLbsBit=0;
				}
		}
	p=(u8*)strstr((char*)buf,"+QBTPWR:");
	if(p!=NULL)
		{
			p=(u8*)strstr((char*)buf,"1");
			if(p!=NULL)
				{
					CoreBtSta|=1<<0;
				}
			CoreBtBit|=1<<1;
		}
	p=(u8*)strstr((char*)buf,"+QBTNAME:");
	if(p!=NULL)
		{
			if(systemset.btsta&(1<<1))
				{
					p=(u8*)strstr((char*)buf,systemset.btsername);
					if(p!=NULL)
						{
							CoreBtSta|=1<<1;
						}
					CoreBtBit|=1<<2;
				}
			if(systemset.btsta&(1<<2))
				{
					p=(u8*)strstr((char*)buf,systemset.btname);
					if(p!=NULL)
						{
							CoreBtSta|=1<<1;
						}
					CoreBtBit|=1<<2;
				}
		}
	p=(u8*)strstr((char*)buf,"+QBTSCAN:");
	if(p!=NULL)
		{
			if(systemset.btsta&(1<<1))
				{
					p=(u8*)strstr((char*)buf,systemset.btclinthead);
					if(p!=NULL)
						{
							ZFBtAsysData(p,mybuf);
							Ql_memset(Tetsbuf, 0, sizeof(Tetsbuf));
							Ql_sprintf(Tetsbuf,"%s:%s",systemset.btsername,mybuf);
							CoreBtSta|=1<<4;
							CoreBtBit|=1<<4;
							
						}
					p=(u8*)strstr((char*)buf,"+QBTSCAN:0");
					if(p!=NULL)
						{
							CoreBtSta|=1<<5;
							CoreBtBit|=1<<4;
						}
				}
			
		}
	p=(u8*)strstr((char*)buf,"+QGSN:");
	if(p!=NULL)
		{
			Ql_memset(systemset.imei,0,sizeof(systemset.imei));
			Ql_memset(ProSeNum.imei,0,sizeof(ProSeNum.imei));
			GetImei(ProSeNum.imei,buf);
			Ql_strcpy(systemset.imei, ProSeNum.imei);
		}
	p=(u8*)strstr((char*)buf,"+CCID:");
	if(p!=NULL)
		{
			Ql_memset(ProSeNum.iccid,0,sizeof(ProSeNum.iccid));
			GetImei(ProSeNum.iccid,buf);
		}
	
}


u8 IsHaveLbsData(void)
{
	return GpsOrLbsBit;
}


u8 GetSnFromSocket(void)
{
	u8 error=0;
	u8 res,num;
	M203C_init();
	M203C_gprs_init();
	res=M203C_tcpudp_conncet((u8)systemset.TCPorUDP,systemset.CenterIP,systemset.CenterPort,0);
	if(res==1)
		{
OVER:
			mprintf("\r\n++++Reset System++++\r\n");
			SaveFlashParamsNew(&systemset);
			Ql_Sleep(300);
			Ql_Reset(0);
		}
	else
		{
			SystemFlow=1;
			M203CIsBusy=1;
			FirstonthelineInit(&num,0x5E20);
			res=M203CSendDataNoAck(PROBUF,num,0);
			if(res)goto  OVER;
			res=M203CSendDataCheckOK(500);
			if(res)goto  OVER;
			M203CIsBusy=0;
			Ql_Sleep(3000);
			while(BootInfo!=0xFF)
				{
					Ql_Sleep(1000);
					IsSnHavaSet();
					error++;
					if(error>10)goto  OVER;
				}
		}
	return 0;
	
}

void Get_imeiStr_Use(u8 *snnum,u8 *imei,u8 num)
{
	u8 i;
	for(i=0;i<12;i++)
		{
			snnum[i]=imei[14-i]+num;
			if(snnum[i]>0x39)snnum[i]=0x39;
		}
}


void GetSnFromImei(u8 num)
{
	u8 error=0;
OVER:
	GetimeiInit();
	Ql_Sleep(1000);
	if(ProSeNum.imei[0]==0x00||ProSeNum.imei[1]==0x00||ProSeNum.imei[2]==0x00||ProSeNum.imei[3]==0x00)
		{
			mprintf("!!!GetSnFromImei Fail!!!\r\n");
			Ql_Sleep(3000);
			Ql_Reset(0);
		}
	else
		{
			Ql_memset(systemset.SN,0,sizeof(systemset.SN));
			Ql_strcpy(systemset.imei,ProSeNum.imei);
			Get_imeiStr_Use(systemset.SN,ProSeNum.imei,num);
			SaveFlashParamsNew(&systemset);
			InternetsetSN_AnalysisFor808(systemset.SN);
			mprintf("+SN %s\r\n",(u8*)systemset.SN);
			IsSnHavaSet();
			while(BootInfo!=0xFF)
				{
					Ql_Sleep(1000);
					IsSnHavaSet();
					error++;
					if(error<3)goto  OVER;
				}
		}
}



