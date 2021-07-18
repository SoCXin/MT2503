#include "ril.h"
#include "ril_util.h"
#include "ql_type.h"
#include "ql_trace.h"
#include "ql_system.h"
#include "ql_uart.h"
#include "ql_stdlib.h"
#include "ql_error.h"
#include "ril_gps.h"
#include "gps.h"
#include "uart.h"
#include "sys.h"
#include "lcd.h"
#include "pro.h"
#include "tiem.h"
#include "new_socket.h"


u32 LbsJingdu = 0;
u32 LbsWeidu = 0;
u8 Jdbuf[20];
u8 Wdbuf[20];
u8 xiaoshudian=0;
Core_data my_core_data;

nmea_msg gpsx;

void GetLocationValue(u32 *jingdu,u32 *weidu,u8 *bufs)
{
	u8 mybuf1[20]={0};
	u8 mybuf2[20]={0};
	u8 *s=NULL;
	u8 *s2=NULL;
	u8 *p=bufs;
	float temp=0.00;
	*jingdu = 0;
	*weidu = 0;
	s = mybuf1;
	s2 = mybuf2;
	while(*p!=':')p++;
	p++;
	while(*p!=',')
		{
			*s=*p;
			s++;
			p++;
		}
	temp = Ql_atof((char*)mybuf1);
	Ql_sprintf(Jdbuf,"%.6f",temp);
	*jingdu = temp*1000000;
	p++;
	while(*p!=0)
		{
			*s2=*p;
			s2++;
			p++;
		}
	temp = Ql_atof((char*)mybuf2);
	Ql_sprintf(Wdbuf,"%.6f",temp);
	*weidu = temp*1000000;
}


u8 GetLbsData(void)
{
	LbsInfoBit=1;
	return M203C_send_Abnormalcmd("AT+QCELLLOC=1",1<<0,7000);
}

u8 GetGpsData(void)
{
	return M203C_send_Abnormalcmd("AT+QGNSSRD?",1<<6,100);
}

void GetGpsDataNCentS(u8 tcent)
{
	while((gpsx.useorno!=65)&&tcent>0)
		{
			GetGpsData();
			Ql_Sleep(1000);
			if(systemset.HandInter<=300||systemset.Interval<=300)
				{
					break;
				}
			tcent--;
		}
}


u8 GetCsQ(u8 *dst,u8 *buf)
{
	buf++;
	buf++;
	while(*buf!=',')
		{
			*dst=*buf;
			dst++;
			buf++;
		}
	dst++;
	*dst=0;
	return 0;
}

void GetVol(u8 *det1,u8 *dst2,u8 *buf)
{
	buf++;
	while(*buf!=',')buf++;
	buf++;
	while(*buf!=',')
		{
			*det1=*buf;
			det1++;
			buf++;
		}
	buf++;
	while(*buf!=0x0d)
		{
			*dst2=*buf;
			dst2++;
			buf++;
		}
	dst2++;
	*dst2=0;
}
static s32 MY_ATResponse_Read_Hdlr(char* line, u32 len, void* userData)
{
	u8 *p1;
	u8 tbuf[10]={0};
	u8 tbuf1[10]={0};
	p1=(u8*)strstr((const char *)line,"+CSQ");
	if(p1!=NULL)
		{
			p1=(u8*)strstr((const char *)line,"99,99");
			if(p1!=NULL)
				{
					Signal=0;
				}
			else
				{
					p1=(u8*)strstr((const char *)line,":");
					if(p1!=NULL)
						{
							
							GetCsQ(tbuf,p1);
							Signal=strtol((const char*)tbuf,NULL,10);
							
						}
				}
		}
	p1=(u8*)strstr((const char *)line,"+CBC");
	if(p1!=NULL)
		{
			p1=(u8*)strstr((const char *)line,":");
			if(p1!=NULL)
				{
					GetVol(tbuf,tbuf1,p1);
					Pro.hvol=strtol((const char*)tbuf,NULL,10);
					SysVol=strtol((const char*)tbuf1,NULL,10);
					
				}
		}
	if(SystemDebug==4)mprintf("Pro.hvol=%d,Signal=%d,SysVol=%d\r\n",Pro.hvol,Signal,SysVol);
	
}




static s32 ATResponse_CFUN_Handler(char* line, u32 len, void* userdata)
{
	char* pHead;
	mprintf("%s",line);
	pHead = Ql_RIL_FindLine(line, len, "OK");
	  if (pHead)
	  	{
	  		 return RIL_ATRSP_SUCCESS;
	  	}
	  return RIL_ATRSP_CONTINUE;
}

s32  RIL_SIM_ResetCfun4(s32 *stat)
{
    s32 retRes = -1;
    s32 nStat = 0;
    char strAT[] = "AT+CFUN=0\0";
     mprintf("%s\r\n",strAT);
    retRes = Ql_RIL_SendATCmd(strAT, Ql_strlen(strAT), ATResponse_CFUN_Handler, &nStat, 0);
    if(RIL_AT_SUCCESS == retRes)
    {
       *stat = nStat; 
    }
    return retRes;
}

s32  RIL_SIM_ResetCfun1(s32 *stat)
{
    s32 retRes = -1;
    s32 nStat = 0;
    char strAT[] = "AT+CFUN=1\0";
      mprintf("%s\r\n",strAT);
    retRes = Ql_RIL_SendATCmd(strAT, Ql_strlen(strAT), ATResponse_CFUN_Handler, &nStat, 0);
    if(RIL_AT_SUCCESS == retRes)
    {
       *stat = nStat; 
    }
    return retRes;
}

u32 NMEA_Pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}


int NMEA_Str2num(u8 *buf,u8*dx)
{
	u8 *p=buf;
	u32 ires=0,fres=0;
	u8 ilen=0,flen=0,i;
	u8 mask=0;
	int res;
	while(1) //得到整数和小数的长度
	{
		if(*p=='-'){mask|=0X02;p++;}//是负数
		if(*p==','||(*p=='*'))break;//遇到结束了
		if(*p=='.'){mask|=0X01;p++;}//遇到小数点了
		else if(*p>'9'||(*p<'0'))	//有非法字符
		{	
			ilen=0;
			flen=0;
			break;
		}	
		if(mask&0X01)flen++;
		else ilen++;
		p++;
	}
	if(mask&0X02)buf++;	//去掉负号
	for(i=0;i<ilen;i++)	//得到整数部分数据
	{  
		ires+=NMEA_Pow(10,ilen-1-i)*(buf[i]-'0');
	}
	if(flen>5)flen=5;	//最多取5位小数
	*dx=flen;	 		//小数点位数
	for(i=0;i<flen;i++)	//得到小数部分数据
	{  
		fres+=NMEA_Pow(10,flen-1-i)*(buf[ilen+1+i]-'0');
	} 
	res=ires*NMEA_Pow(10,flen)+fres;
	if(mask&0X02)res=-res;		   
	return res;
}	  							 

u8 NMEA_Comma_Pos(u8 *buf,u8 cx)
{	 		    
	u8 *p=buf;
	while(cx)
	{		 
		if(*buf=='*'||*buf<' '||*buf>'z')return 0XFF;//遇到'*'或者非法字符,则不存在第cx个逗号
		if(*buf==',')cx--;
		buf++;
	}
	return buf-p;	 
}



void NMEA_GPRMC_Analysis(nmea_msg *gpsx,u8 *buf)
{
	u8 *p1,dx;			 
	u8 posx;     
	u32 temp;	   
	float rs; 
	p1=(u8*)strstr((const char *)buf,"$GPRMC");
	if(p1==NULL)
		{
			p1=(u8*)strstr((const char *)buf,"$GNRMC");
			if(p1==NULL)return;
		}
	posx=NMEA_Comma_Pos(p1,1);								//得到UTC时间
	if(posx!=0XFF)
	{
		temp=NMEA_Str2num(p1+posx,&dx)/NMEA_Pow(10,dx);	 	//得到UTC时间,去掉ms
		gpsx->utc.hour=temp/10000;
		gpsx->utc.min=(temp/100)%100;
		gpsx->utc.sec=temp%100;	 	 
	}	
	posx=NMEA_Comma_Pos(p1,2);
	if(posx!=0XFF)gpsx->useorno=*(p1+posx);
	posx=NMEA_Comma_Pos(p1,3);								//得到纬度
	if(posx!=0XFF)
	{
		temp=NMEA_Str2num(p1+posx,&dx);
		gpsx->latitude=temp/NMEA_Pow(10,dx+2);	//得到°
		rs=temp%NMEA_Pow(10,dx+2);				//得到'
		temp=(rs*NMEA_Pow(10,6-dx))/60;
		gpsx->latitude=gpsx->latitude*NMEA_Pow(10,6)+temp;//转换为° 
		
	}
	posx=NMEA_Comma_Pos(p1,4);							//南纬还是北纬 
	if(posx!=0XFF)gpsx->nshemi=*(p1+posx);					 
 	posx=NMEA_Comma_Pos(p1,5);								//得到经度
	if(posx!=0XFF)
	{												  
		temp=NMEA_Str2num(p1+posx,&dx);		 	 
		gpsx->longitude=temp/NMEA_Pow(10,dx+2);	//得到°
		rs=temp%NMEA_Pow(10,dx+2);				//得到'	
		temp=(rs*NMEA_Pow(10,6-dx))/60;
		gpsx->longitude=gpsx->longitude*NMEA_Pow(10,6)+temp;//转换为° 
	}
	posx=NMEA_Comma_Pos(p1,6);								//东经还是西经
	if(posx!=0XFF)gpsx->ewhemi=*(p1+posx);	
	posx=NMEA_Comma_Pos(p1,7);						
	if(posx!=0XFF)
		{
			temp=NMEA_Str2num(p1+posx,&dx);
			gpsx->speed=temp;
			xiaoshudian=dx;
			gpsx->speed=(gpsx->speed/NMEA_Pow(10,dx));
		}
	posx=NMEA_Comma_Pos(p1,8);
	if(posx!=0XFF)
		{
			temp=NMEA_Str2num(p1+posx,&dx);
			gpsx->longitudewei=temp;
		}
	posx=NMEA_Comma_Pos(p1,9);								//得到UTC日期
	if(posx!=0XFF)
	{
		temp=NMEA_Str2num(p1+posx,&dx);		 				//得到UTC日期
		gpsx->utc.date=temp/10000;
		gpsx->utc.month=(temp/100)%100;
		gpsx->utc.year=temp%100;	 	 
	} 
	

}


void GPS_Analysis(nmea_msg *gpsx,u8 *buf)
{
	NMEA_GPRMC_Analysis(gpsx,buf);

}

void GpsAnalysis(u8 *gpsdata)
{
	GPS_Analysis(&gpsx,gpsdata);
}



void GPS_package(nmea_msg *gpsx,Core_data *core_data)
{
	u8 Warning;
	u8 ubuf[7][3];
	u8 hex[5][9];
	u8 gps_byte[9];
	double temp;
	u32 res,speed;
	
	core_data->gps_head_byte[1]=0;
	core_data->gps_head_byte[2]=0;

	core_data->time[0]=gpsx->utc.year;
	core_data->time[1]=gpsx->utc.month;
	core_data->time[2]=gpsx->utc.date;
	core_data->time[3]=gpsx->utc.hour;
	core_data->time[4]=gpsx->utc.min;
	core_data->time[5]=gpsx->utc.sec;
	
	if(gpsx->useorno!=65)
		{
			Pro.gpssta=0x02;
		}
	else
		{
			Pro.gpssta=0x01;
		}
		
	temp=(gpsx->latitude)*3.6;
	res=temp;
	core_data->jingdu[0]=(res&0xff000000)>>24;
	core_data->jingdu[1]=(res&0x00ff0000)>>16;
	core_data->jingdu[2]=(res&0x0000ff00)>>8;
	core_data->jingdu[3]=(res&0x000000ff);

	res=gpsx->longitude;
	core_data->jingdu[4]=(res&0xff000000)>>24;
	core_data->jingdu[5]=(res&0x00ff0000)>>16;
	core_data->jingdu[6]=(res&0x0000ff00)>>8;
	core_data->jingdu[7]=(res&0x000000ff);
	
	temp=(gpsx->longitude)*3.6;
	res=temp;
	core_data->weidu[0]=(res&0xff000000)>>24;
	core_data->weidu[1]=(res&0x00ff0000)>>16;
	core_data->weidu[2]=(res&0x0000ff00)>>8;
	core_data->weidu[3]=(res&0x000000ff);

	res=gpsx->latitude;
	core_data->weidu[4]=(res&0xff000000)>>24;
	core_data->weidu[5]=(res&0x00ff0000)>>16;
	core_data->weidu[6]=(res&0x0000ff00)>>8;
	core_data->weidu[7]=(res&0x000000ff);
	
	temp=gpsx->speed*0.1852*10;
	speed=temp;
	core_data->sudu[0]=(speed&0x0000ff00)>>8;
	core_data->sudu[1]=(speed&0x000000ff);

	temp=gpsx->speed*1.852*10;
	speed=temp;
	core_data->sudu[2]=(speed&0x0000ff00)>>8;
	core_data->sudu[3]=(speed&0x000000ff);

	temp=gpsx->speed*0.1852*10;
	speed=temp;

	if(gpsx->longitudewei<=3600)
		{
			temp=gpsx->longitudewei/10;
			res=temp;
		}
	else
		{
			temp=gpsx->longitudewei/100;
			res=temp;
		}
	core_data->fangxiang[0]=(res&0x0000ff00)>>8;
	core_data->fangxiang[1]=(res&0x000000ff);
	
	if(speed>systemset.SpedLmt[0])
		{
			if(systemset.Spedtime>0)systemset.Spedtime--;
			if(systemset.Spedtime==0)
				{
					systemset.Spedtime=systemset.SpedLmt[1];
					Warning=1;
				}
		}
	else
		{
			Warning=0;
		}
	
	temp=gpsx->speed*0.051444444444*10;
	res=temp;
	gpsx->Mileage=gpsx->Mileage+res;
	core_data->licheng[0]=(gpsx->Mileage&0xff000000)>>24;
	core_data->licheng[1]=(gpsx->Mileage&0x00ff0000)>>16;
	core_data->licheng[2]=(gpsx->Mileage&0x0000ff00)>>8;
	core_data->licheng[3]=(gpsx->Mileage&0x000000ff);

	res=gpsx->Mileage/100;
	core_data->licheng[4]=(res&0xff000000)>>24;
	core_data->licheng[5]=(res&0x00ff0000)>>16;
	core_data->licheng[6]=(res&0x0000ff00)>>8;
	core_data->licheng[7]=(res&0x000000ff);
	
	
	if(Warning)
		{
			
			if(gpsx->useorno==65)core_data->gps_head_byte[3]=0x43;
			if(gpsx->useorno==86)core_data->gps_head_byte[3]=0x42;
		}
	else
		{
			if(gpsx->useorno==65)core_data->gps_head_byte[3]=0x01;
			if(gpsx->useorno==86)core_data->gps_head_byte[3]=0x00;
		}
	
}

void AsysLbs(Core_data *core_data)
{
	u32 res;
	double temp;
	temp=LbsWeidu*3.6;
	res=temp;
	core_data->jingdu[0]=(res&0xff000000)>>24;
	core_data->jingdu[1]=(res&0x00ff0000)>>16;
	core_data->jingdu[2]=(res&0x0000ff00)>>8;
	core_data->jingdu[3]=(res&0x000000ff);

	res=LbsJingdu;
	core_data->jingdu[4]=(res&0xff000000)>>24;
	core_data->jingdu[5]=(res&0x00ff0000)>>16;
	core_data->jingdu[6]=(res&0x0000ff00)>>8;
	core_data->jingdu[7]=(res&0x000000ff);
	
	temp=LbsJingdu*3.6;
	res=temp;
	core_data->weidu[0]=(res&0xff000000)>>24;
	core_data->weidu[1]=(res&0x00ff0000)>>16;
	core_data->weidu[2]=(res&0x0000ff00)>>8;
	core_data->weidu[3]=(res&0x000000ff);

	res=LbsWeidu;
	core_data->weidu[4]=(res&0xff000000)>>24;
	core_data->weidu[5]=(res&0x00ff0000)>>16;
	core_data->weidu[6]=(res&0x0000ff00)>>8;
	core_data->weidu[7]=(res&0x000000ff);
}

