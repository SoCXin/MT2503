#include "ql_type.h"
#include "ql_trace.h"
#include "ql_uart.h"
#include "ql_system.h"
#include "ql_stdlib.h"
#include "ql_error.h"
#include "ql_gpio.h"
#include "sys.h"
#include "ql_socket.h"
#include "tiem.h"
#include "uart.h"
#include "pro.h"
#include "new_socket.h"
#include "lcd.h"
#include "ql_adc.h"
#include "flash.h"
#include "PRD.h"
#include "gps.h"
#include "ql_pwm.h"
#include "ql_memory.h"
#include "bt3.h"


const u8 *modetbl[2]={"TCP","UDP"};

Protocol_Send_packets Send_packets;

_system_setings	systemset;
_system_times	systimes;
Coredata	mycoredata;
DataConfig Position;

volatile u8 SystemDebug=2;
volatile u8 SystemFlow=0;

volatile u8 BattarySta=0;

u8 wzcurrtsta=0;


u8 LbsOpen=0;

u32 jishis=0;
u32 jishit=0;


volatile u8 syssleepbit=0;


#define _USE_LED_WDZ_



#ifdef _USE_LED_WDZ_

Enum_PinName LED1=PINNAME_SIM2_CLK;
Enum_PinName LED2=PINNAME_SIM2_DATA;
Enum_PinName LED3=PINNAME_SIM2_RST;
Enum_PinName WZUD=PINNAME_GPIO0;
Enum_PinName WZINS=PINNAME_GPIO1;
Enum_PinName WZCS=PINNAME_GPIO2;

#else

Enum_PinName LED1=PINNAME_GPIO0;
Enum_PinName LED2=PINNAME_GPIO1;
Enum_PinName LED3=PINNAME_GPIO2;
Enum_PinName WZUD=PINNAME_SIM2_CLK;
Enum_PinName WZINS=PINNAME_SIM2_DATA;
Enum_PinName WZCS=PINNAME_SIM2_RST;

#endif

Enum_PinName M203C_DATA=PINNAME_CTS;
Enum_PinName M203C_EINT=PINNAME_DTR;
Enum_PinName M203C_WHG=PINNAME_RTS;

Enum_PinName ZFGPIO1=PINNAME_SD_CMD;
Enum_PinName BATTARYSPIN=PINNAME_SD_CLK;


#ifdef _SD_DATA_OUT_

Enum_PinName ZFGPIO2=PINNAME_SD_DATA;
Enum_PinName SDDATAIN=PINNAME_GPIO4;


#else


Enum_PinName ZFGPIO2=PINNAME_GPIO4;
Enum_PinName SDDATAIN=PINNAME_SD_DATA;


#endif


void power_drv_init(void)
{
	 Ql_GPIO_Init(LED1, PINDIRECTION_OUT, 0, PINPULLSEL_PULLUP);
	 Ql_GPIO_Init(LED2, PINDIRECTION_OUT, 0, PINPULLSEL_PULLUP);
	 Ql_GPIO_Init(LED3, PINDIRECTION_OUT, 0, PINPULLSEL_PULLUP);
	 Ql_GPIO_Init(M203C_DATA, PINDIRECTION_OUT, 0, PINPULLSEL_PULLUP);
	 Ql_GPIO_Init(M203C_EINT, PINDIRECTION_OUT, 0, PINPULLSEL_PULLUP);
	 Ql_GPIO_Init(M203C_WHG, PINDIRECTION_OUT, 0, PINPULLSEL_PULLUP);

	 Ql_GPIO_Init(M203C_EINT, PINDIRECTION_OUT, 0, PINPULLSEL_PULLUP);
	 Ql_GPIO_Init(M203C_WHG, PINDIRECTION_OUT, 0, PINPULLSEL_PULLUP);

	 Ql_GPIO_Init(ZFGPIO1, PINDIRECTION_OUT, 0, PINPULLSEL_PULLUP);
	 Ql_GPIO_Init(ZFGPIO2, PINDIRECTION_OUT, 0, PINPULLSEL_PULLUP);
	 Ql_GPIO_Init(SDDATAIN, PINDIRECTION_IN, 0, PINPULLSEL_PULLUP);

	  Ql_GPIO_Init(WZUD, PINDIRECTION_OUT, 0, PINPULLSEL_PULLUP);
	 Ql_GPIO_Init(WZINS, PINDIRECTION_OUT, 0, PINPULLSEL_PULLUP);
	 Ql_GPIO_Init(WZCS, PINDIRECTION_OUT, 0, PINPULLSEL_PULLUP);

	  Ql_GPIO_Init(BATTARYSPIN, PINDIRECTION_IN, 0, PINPULLSEL_PULLUP);
	 
	 Ql_PWM_Init(PINNAME_NETLIGHT,PWMSOURCE_13M,PWMSOURCE_DIV2,100,100);
	 Ql_PWM_Output(PINNAME_NETLIGHT,FALSE);

	 LED1_L;
	 LED2_L;
	 LED3_L;
	 M203C_DATA_L;
	 M203C_WHG_L;
	 M203C_EINT_L;
}




void hwhalinit(void)
{
	 Ql_GPIO_Init(PINNAME_SD_CMD, PINDIRECTION_OUT, 0, PINPULLSEL_PULLUP);
	 Ql_GPIO_Init(PINNAME_SD_CLK, PINDIRECTION_OUT, 0, PINPULLSEL_PULLUP);
	 Ql_GPIO_Init(PINNAME_SD_DATA, PINDIRECTION_OUT, 0, PINPULLSEL_PULLUP);
	 Ql_GPIO_Init(PINNAME_NETLIGHT, PINDIRECTION_OUT, 0, PINPULLSEL_PULLUP);
	 Ql_GPIO_Init(PINNAME_CTS, PINDIRECTION_OUT, 0, PINPULLSEL_PULLUP);
	 Ql_GPIO_Init(PINNAME_RTS, PINDIRECTION_OUT, 0, PINPULLSEL_PULLUP);
	// Ql_GPIO_Init(PINNAME_DTR, PINDIRECTION_IN, 0, PINPULLSEL_PULLUP);
	 Ql_GPIO_Init(PINNAME_DCD, PINDIRECTION_OUT, 0, PINPULLSEL_PULLUP);
}
void marked_words(void)
{
	BB_ON;
	Ql_Sleep(600);
	BB_OFF;
}	

void init_motor_gpio(void)
{
	 Ql_GPIO_Init(PINNAME_SIM2_CLK, PINDIRECTION_OUT, 0, PINPULLSEL_PULLUP);
	 Ql_GPIO_Init(PINNAME_SIM2_DATA, PINDIRECTION_OUT, 0, PINPULLSEL_PULLUP);
	 Ql_GPIO_Init(PINNAME_SIM2_RST, PINDIRECTION_OUT, 0, PINPULLSEL_PULLUP);
}

void sleep_motor(void)
{
	MOTOR_NSLEEP_OFF;
	MOTOR_REVERSE_OFF;
	MOTOR_FORWARD_OFF;
}

void forward_motor(void)
{
	MOTOR_NSLEEP_ON;
	MOTOR_REVERSE_OFF;
	MOTOR_FORWARD_ON;
}

void reverse_motor(void)
{
	MOTOR_NSLEEP_ON;
	MOTOR_REVERSE_ON;
	MOTOR_FORWARD_OFF;
}


void init_infrared_sensor()
{
	Ql_GPIO_Init(PINNAME_PCM_CLK, PINDIRECTION_OUT, 0, PINPULLSEL_PULLUP);
	Ql_GPIO_Init(PINNAME_PCM_IN, PINDIRECTION_OUT, 0, PINPULLSEL_PULLUP);
	Ql_GPIO_Init(PINNAME_PCM_OUT, PINDIRECTION_IN, 0, PINPULLSEL_PULLUP);
	Ql_GPIO_Init(PINNAME_PCM_SYNC, PINDIRECTION_IN, 0, PINPULLSEL_PULLUP);

	GPIO_OUT_UNLOCKED_SENSOR_OFF;
	GPIO_OUT_LOCKED_SENSOR_OFF;
}

void motor_init_lockio()
{
    init_motor_gpio();
    sleep_motor();
    init_infrared_sensor();
}



void Delayus(u32 data)
{
	u16 i;
	while(data--)
	{
		for(i=0;i<10;i++){}
	}
}

static u32 ADC_CustomParam = 1;
u32 sysadcvol=0;

static void Callback_OnADCSampling(Enum_ADCPin adcPin, u32 adcValue, void *customParam)
{
    if(SystemDebug==4)mprintf("<-- Callback_OnADCSampling: sampling voltage(mV)=%d  times=%d -->\r\n", adcValue, *((s32*)customParam))
    *((s32*)customParam) += 1;
	sysadcvol=adcValue;
	if(SystemDebug==4)mprintf("adc=%d\r\n",sysadcvol);
}




void ADC_Program(void)
{
    Enum_PinName adcPin = PIN_ADC0;
    Ql_ADC_Register(adcPin, Callback_OnADCSampling, (void *)&ADC_CustomParam);
    Ql_ADC_Init(adcPin, 5, 400);
    Ql_ADC_Sampling(adcPin, TRUE);
}


void StrToHex(char *pbDest, char *pbSrc, int nLen)
{
	char h1,h2;
	char s1,s2;
	char tbuf[3];
	int i;

	for (i=0; i<nLen; i++)
	{
		h1 = pbSrc[2*i];
		h2 = pbSrc[2*i+1];
		Ql_sprintf(tbuf,"%c%c",h1,h2);
		pbDest[i] =strtol((const char*)tbuf,NULL,16);
	}
}


void InternetsetSN_AnalysisFor808(u8 *snuf)
{

#if 0
	u8 k;
	u8 snlength,i,j=0;
	u8 duanbuf[3]={0};
	snlength=Ql_strlen((const char*)snuf);
	i=snlength/2;
	j=snlength%2;
	if(i>6)return;
	if(j==0)
		{
			for(k=1;k<(i+1);k++)
				{
					mid(duanbuf,snuf,2,snlength-2*k);
					Send_packets.packets_Send_snone[6-k]=strtol(duanbuf,NULL,16);
				}
		}
	if(j==1)
		{
			for(k=1;k<(i+1);k++)
				{
					mid(duanbuf,snuf,2,snlength-2*k);
					Send_packets.packets_Send_snone[6-k]=strtol(duanbuf,NULL,16);
				}
			mid(duanbuf,snuf,1,snlength-2*k);
			Send_packets.packets_Send_snone[6-k]=strtol(duanbuf,NULL,16);
		}
#endif
	 StrToHex(Send_packets.packets_Send_snone, snuf, Ql_strlen(snuf) / 2);


}



void  LoadDefualtCfg(void)
{
	systemset.HandInter=500; 
	systemset.CallMode=0; 
	systemset.ACCoffDelay=20;
	systemset.Interval=60;
	systemset.Length=500;
	systemset.SpedLmt[0]=255;
	systemset.SpedLmt[1]=255;
	systemset.TCPorUDP=0;
	systemset.LbsOpen=1;
	systemset.SysTime=0;
	systemset.saveflag=0x0A;
	systemset.btsta=0;
	systemset.fotaflag=0;
	Ql_memset(systemset.Passwd, 0, sizeof(systemset.Passwd));
	Ql_memset(systemset.imei, 0, sizeof(systemset.imei));
	Ql_sprintf((char*)systemset.CenterIP,"211.159.175.43");
	Ql_sprintf((char*)systemset.CenterPort,"8100");
	Ql_sprintf((char*)systemset.Centerapn,"cmnet");
	Ql_sprintf((char*)systemset.SN,"FFFFFFFFFFFF");
	Ql_sprintf((char*)systemset.ProductID,"000000");
	Ql_sprintf((char*)systemset.btclinthead,"CZF");
	Ql_sprintf((char*)systemset.btsername,"ZFBTSER");
	Ql_sprintf((char*)systemset.btname,"CZF-SUBBT1");
}


void ShowSysInfo(void)
{
	GetSysTime(&time);
	mprintf("JT_N2S_TEST_V1.0.0\r\n");
	mprintf("ProductNumber:%s\r\n",systemset.ProductID);
	mprintf("CenterIP:%s\r\n",systemset.CenterIP);
	mprintf("CenterPort:%s\r\n",systemset.CenterPort);
	mprintf("TCP/IP:%s\r\n",modetbl[systemset.TCPorUDP]);
	mprintf("SN:%s\r\n",systemset.SN);
	mprintf("IMEI:%s\r\n",systemset.imei);
	mprintf("Passwd:%s\r\n",systemset.Passwd);
	mprintf("Interval:%d s\r\n",systemset.Interval);
	mprintf("BtSta:%d \r\n",systemset.btsta&0x01);
	mprintf("BtServer:%d \r\n",systemset.btsta&0x02);
	mprintf("BtClient:%d \r\n",systemset.btsta&0x04);
	mprintf("Btname:%s\r\n",systemset.btname);
	mprintf("BTSername:%s\r\n",systemset.btsername);
	mprintf("LbsSta:%d\r\n",systemset.LbsOpen);
	mprintf("M203CIsBusy:%d\r\n",M203CIsBusy);
	mprintf("+DataTime: %04d-%02d-%02d %02d:%02d:%02d\r\n",time.year, time.month, time.day, time.hour, time.minute, time.second);

}

void FlashTimeInit(void)
{
	GetSysTime(&time);
	mycoredata.time[0]=Dec2Hex(time.year-2000);
	mycoredata.time[1]=Dec2Hex(time.month);
	mycoredata.time[2]=Dec2Hex(time.day);
	mycoredata.time[3]=Dec2Hex(time.hour);
	mycoredata.time[4]=Dec2Hex(time.minute);
	mycoredata.time[5]=Dec2Hex(time.second);
}

void IsSnHavaSet(void)
{
	u8 *p=NULL;
	p=(u8*)strstr((const char *)systemset.SN,"FFFFFFFF");
	if(p!=NULL)
		{
			BootInfo=0x02;
		}
	else
		{
			BootInfo=0xFF;
		}
}

void SYS_Parameter_Init(void)
{
	ReadFlashParamsNew(&systemset);
	CreatNewFlashFile();
	ShowSysInfo();
	IsSnHavaSet();
	TlvInitList();
	FotaInit();
	InternetsetSN_AnalysisFor808(systemset.SN);
	systimes.gpst=0;
	systimes.sockett=0;
	systimes.sta=0;
	ReCenterTlV.type=B_FOTA;
	ReCenterTlV.length=0x01;
	if(systemset.fotaflag==2)
		{
			ReCenterTlV.value[0]=0x01;
			systemset.fotaflag=0;
			SaveFlashParamsNew(&systemset);
			mprintf("\r\n***************Software upgrade is successful*************\r\n");
		}
	else
		{
			ReCenterTlV.value[0]=0x00;
		}
	systemset.CallMode=0;
	WZAysCuurtSta();
	
}


void CheckBattaryS(void)
{
	s32 res=0;
	res=Ql_GPIO_GetLevel(BATTARYSPIN);
	if(res==0)BattarySta=1;
	if(res==1)BattarySta=0;
}


void CheckSDDATA(void)
{
	s32 res=0;
	res=Ql_GPIO_GetLevel(SDDATAIN);
	mprintf("SDDATA=%d\r\n",res);
}

#if 0
void Ch2Protocol(Coredata data)
{
	Ql_sprintf(AutomaticRDTlv[7].value,"%s",data.xyz);
	Ql_sprintf(AutomaticRDTlv[6].value,"%d.%06d",data.gpsj/1000000,data.gpsj%1000000);
	Ql_sprintf(AutomaticRDTlv[5].value,"%d.%06d",data.gpsw/1000000,data.gpsw%1000000);
	Ql_sprintf(AutomaticRDTlv[4].value,"%d.%06d",data.lbsj/1000000,data.lbsj%1000000);
	Ql_sprintf(AutomaticRDTlv[3].value,"%d.%06d",data.lbsw/1000000,data.lbsw%1000000);
	Ql_sprintf(AutomaticRDTlv[2].value,"%d",data.dbm);
	Ql_sprintf(AutomaticRDTlv[1].value,"%04d%03d%d",data.vol,data.vbai,BattarySta);
	Ql_sprintf(AutomaticRDTlv[0].value,"%d",systemset.SysTime);
}

#endif



u8 CheckFlashData2Send(void)
{
	u8 num;
	u8 res;
RECALC:
	if(Position.saveposion>Position.sendpostion)
		{
			while(Position.sendpostion<Position.saveposion)
				{
					LoadDataFromFlash(&mycoredata,Position.sendpostion);
					Ch2Protocol(mycoredata);
					CoreDataInit_v2(&num,0x4E22);
					res=M203CSendDataNoAck(PROBUF,num,0);
					if(res)return res;
					res=M203CSendDataCheckOK(500);
					if(res)return res;
					Position.sendpostion++;
					Ql_Sleep(1000);
				}
		}
	else if(Position.saveposion==Position.sendpostion)
		{
			
		}
	else
		{
			while(Position.sendpostion<MAXDATANUM)
				{
					LoadDataFromFlash(&mycoredata,Position.sendpostion);
					Ch2Protocol(mycoredata);
					CoreDataInit_v2(&num,0x4E22);
					res=M203CSendDataNoAck(PROBUF,num,0);
					if(res)return res;
					res=M203CSendDataCheckOK(500);
					if(res)return res;
					Position.sendpostion++;
					Ql_Sleep(1000);
				}
			if(Position.sendpostion==MAXDATANUM)
				{
					Position.sendpostion=0;
					goto RECALC;
				}
		}
	return 0;
	
}


u8 ReciveTheMsgFromIO(u8 pinNum)
{
	
}

void WzAynsCmd(u8 *src,u8 *wznum)
{
	u8 tbuf[5]={0};
	u8 *p=NULL;
	u8 *s=tbuf;
	*wznum=0;
	p=(u8*)strstr((const char*)src,"DTPT:");
	if(p!=NULL)
		{
			while(*p!=':')p++;
			p++;
			while(*p!=0)
				{
					*s=*p;
					s++;
					p++;
				}
			*s=0;
			*wznum=strtol((const char*)tbuf,NULL,10);
		}
}
void AynsCmd(u8 *src,u8 *clk,u8 *div, u32 *lt,u32 *ht)
{
	u8 tbuf[5]={0};
	u8 *p=NULL;
	u8 *s=tbuf;
	p=(u8*)strstr((const char*)src,"PIT:");
	if(p!=NULL)
		{
			while(*p!=':')p++;
			p++;
			while(*p!=',')
				{
					*s=*p;
					s++;
					p++;
				}
			*s=0;
			*clk=strtol((const char*)tbuf,NULL,10);
			p++;
			s=&tbuf[0];
			while(*p!=',')
				{
					*s=*p;
					s++;
					p++;
				}
			*s=0;
			*div=strtol((const char*)tbuf,NULL,10);
			p++;
			s=&tbuf[0];
			while(*p!=',')
				{
					*s=*p;
					s++;
					p++;
				}
			*s=0;
			*lt=strtol((const char*)tbuf,NULL,10);
			p++;
			s=&tbuf[0];
			while(*p!=';')
				{
					*s=*p;
					s++;
					p++;
				}
			*s=0;
			*ht=strtol((const char*)tbuf,NULL,10);
			
		}
	else
		{
			*clk=0;
			*div=1;
			*lt=100;
			*ht=100;
		}
}

////////////////////////////////////////////////////////USEDATA///////////////////////////////////

void SendData2Protocol(void)
{	
	//Ql_sprintf(AutomaticRDTlv[7].value,"%s",gpsx.latitude/1000000,gpsx.latitude%1000000);
	Ql_sprintf(AutomaticRDTlv[6].value,"%d.%06d",gpsx.latitude/1000000,gpsx.latitude%1000000);
	Ql_sprintf(AutomaticRDTlv[5].value,"%d.%06d",gpsx.longitude/1000000,gpsx.longitude%1000000);
	Ql_sprintf(AutomaticRDTlv[4].value,"%d.%06d",LbsWeidu/1000000,LbsWeidu%1000000);
	Ql_sprintf(AutomaticRDTlv[3].value,"%d.%06d",LbsJingdu/1000000,LbsJingdu%1000000);
	Ql_sprintf(AutomaticRDTlv[2].value,"%d",Signal);
	Ql_sprintf(AutomaticRDTlv[1].value,"100");
	Ql_sprintf(AutomaticRDTlv[0].value,"%d",systemset.SysTime);
	
	
}


void SendNumFromPin(u16 num)
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

void WZinit(void)
{
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
	u8 t=0,i;
	if(systemset.CallMode>wzcurrtsta)
		{
			t=systemset.CallMode-wzcurrtsta;
			for(i=0;i<t;i++)
				{
					WZAddOne();
					Delayus(1);
				}
		}
	else 
		{
			t=wzcurrtsta-systemset.CallMode;
			for(i=0;i<t;i++)
				{
					WZSubOne();
					Delayus(1);
				}
		}
	wzcurrtsta=systemset.CallMode;
}





