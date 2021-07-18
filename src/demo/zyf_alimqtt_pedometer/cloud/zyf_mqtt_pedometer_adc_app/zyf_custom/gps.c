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
#include "tiem.h"
#include "new_socket.h"
#include "stdint.h"           //重命名的所有文件
#include "math.h"
u32 LbsJingdu = 0;
u32 LbsWeidu = 0;
u8 xiaoshudian=0;
Core_data my_core_data;

nmea_msg gpsx;
u8 gpspowersta = 0;


s32 GpsOpen(void)
{
	s32 iRet = 0;
	iRet=RIL_GPS_Open(1);
	return iRet;
}

s32 GpsClose(void)
{
	s32 iRet = 0;
	iRet=RIL_GPS_Open(0);
	return iRet;
}


s32 GetGpsData(u8 *buf,u8 *name)
{
	s32 iRet = 0;
	RIL_GPS_Read(name,buf);
	return iRet;
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

u8 NMEA_Comma_PosTail(u8 *buf,u8 cx)
{	 		    
	u8 *p=buf;
	while(cx)
	{		 
		if(*buf=='*')return 0xFE;
        if(*buf<' '||*buf>'z')return 0XFF;//遇到'*'或者非法字符,则不存在第cx个逗号
		if(*buf==',')cx--;
		buf++;
	}
	return buf-p;	 
}

u8 NMEA_check_crc(char *gpsline)
{
    u8 crcvalue = 0,linecrc = 0;
    int i = 0,j = 0;
    int daoflag = 0,xingflag = 0;
    if(gpsline == NULL) return 0;
    char *p = Ql_strstr(gpsline, "$GNRMC");
    if(p == NULL)
        return 0;
    while(j < Ql_strlen(p))
    {
        if(p[j] == '\r' || p[j] == '\n')
            break;
        j++;
    }
    p[j] = '\0';
    mprintf("nmea check crc:%s\r\n",p);
    while(i < Ql_strlen(p)) 
    {
        if(p[i] == '$')
        {
            daoflag = 1;
        }
        if(p[i] == '*')
        {
            xingflag = 1;
            //crcvalue = ;
            linecrc = strtol(p + i + 1,NULL,16);
            mprintf("line crc value:%d\n",linecrc);
        }
        if(daoflag == 1 && xingflag == 1)
            break;
        if(i + 1 == Ql_strlen(p))
            return 0;
        i++;
    }
    
    for(i = 0; i < Ql_strlen(p); i++) //one line
    {
        if (p[i] == '$')
        {
            crcvalue = p[i+1];
            continue;
        }
        if(p[i + 1] == '*')
            break;
        
        crcvalue ^= p[i + 1]; 
    }
    mprintf("result:%02x\n",crcvalue % 65536);
    return crcvalue % 65536 == linecrc ? 1:0;
}

void NMEA_GSV_Analysis(nmea_msg* gpsx, u8* buf)
{
    u8* p1 = NULL;
    u8* p2 = NULL;
    u8 dx;
    s8 gpgsv_num = 0;
    s8 gbgsv_num = 0;
    u8 posx;
    u8 alivepos = 0;
    u8 temp = 0;
    gpsx->satellitenum = 0;
    //mprintf("IN NMEA_GSV_Analysis\r\n");
    //mprintf("!!!!the buf is : %s\r\n",buf);
    p1 = (u8*)strstr((const char*)buf, "$GPGSV");
    if (p1 == NULL)
    {
        p1 = (u8*)strstr((const char*)buf, "$BDGSV");
        if (p1 == NULL)
            ;
        else
        {
            posx = NMEA_Comma_Pos(p1, 1);                       
            if (posx != 0XFF)
            {
                gbgsv_num = NMEA_Str2num(p1 + posx, &dx); //如果没有gpgsv只得到gbgsv语句的个数
            }
            //mprintf("gbgsv_num %d\r\n",gbgsv_num);
        }
    }
    else
    {
        posx = NMEA_Comma_Pos(p1, 1);                       
        if (posx != 0XFF)
        {
            gpgsv_num = NMEA_Str2num(p1 + posx, &dx); //得到gpgsv语句的个数
        }          
        //mprintf("gpgsv_num %d\r\n",gpgsv_num);
        p2 = (u8 *)strstr((const char*)buf,"$BDGSV"); 
        if(p2 == NULL)
            ;
        else
        {
            posx = NMEA_Comma_Pos(p2, 1);                       
            if (posx != 0XFF)
            {
                gbgsv_num = NMEA_Str2num(p2 + posx, &dx); //得到gbgsv语句的个数
            } 
            //mprintf("gbgsv_num %d\r\n",gbgsv_num);
        }
    }
    posx = 0;
    for(p1 = buf,p2 = buf;gpgsv_num > 0 || gbgsv_num > 0; gpgsv_num--,gbgsv_num--,p1 += 6,p2 += 6) // 6 = sizeof("$GPGSV") or sizeof("$GBGSV")
    {
        //mprintf("gpgsv_num %d,gbgsv_num %d\r\n",gpgsv_num,gbgsv_num);
        if(gpgsv_num > 0)
        {
            p1 = (u8*)strstr((const char*)p1, "$GPGSV");
            if(p1 != NULL)
            {
                posx = 0;
                for(alivepos = 7;posx != 0xFE;alivepos +=4)
                {
                    posx = NMEA_Comma_PosTail(p1, alivepos);
                    if(posx != 0xFF || posx != 0xFE)
                    {
                        temp = NMEA_Str2num(p1 + posx, &dx);
                        //mprintf("snr %d db. ",temp);
                        if(temp > 10 && temp < 100)
                            gpsx->satellitenum++;
                    }
                    //mprintf("posx %d\r\n",posx);
                }
            }
        }
        if(gbgsv_num > 0)
        {
            p2 = (u8*)strstr((const char*)p2, "$BDGSV");
            if(p2 != NULL)
            {
                posx = 0;
                for(alivepos = 7;posx != 0xFE;alivepos += 4)
                {
                    posx = NMEA_Comma_PosTail(p2, alivepos);
                    if(posx != 0xFF || posx != 0xFE)
                    {
                        temp = NMEA_Str2num(p2 + posx, &dx);
                       // mprintf("snr %d db.\r\n",temp);
                        if(temp > 10 && temp < 100) //卫星的信号质量
                            gpsx->satellitenum++;
                    }
                }
            }
        }
    }
    return;
    
}


void NMEA_GPGGA_Analysis(nmea_msg *gpsx,u8 *buf)
{
	u8 *p1;
    u8 dx;			 
	u8 posx;    
	p1=(u8*)strstr((const char *)buf,"GGA");
	posx=NMEA_Comma_Pos(p1,6);								//得到GPS状态
	if(posx!=0XFF)gpsx->gpssta=NMEA_Str2num(p1+posx,&dx);	
	posx=NMEA_Comma_Pos(p1,7);								//得到用于定位的卫星数
	if(posx!=0XFF)gpsx->posslnum=NMEA_Str2num(p1+posx,&dx); 
	posx=NMEA_Comma_Pos(p1,9);								//得到海拔高度
	if(posx!=0XFF)gpsx->altitude=NMEA_Str2num(p1+posx,&dx);  
}



void NMEA_GPRMC_Analysis(nmea_msg *gpsx,u8 *buf)
{
	u8 *p1;
    u8 dx;			 
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
		mprintf("%d\n",gpsx->latitude);
		//g_latitude=gpsx->latitude;              //纬度传递给全局变量
			
	}
	posx=NMEA_Comma_Pos(p1,4);							//南纬还是北纬 
	if(posx!=0XFF)gpsx->nshemi=*(p1+posx);				
	//mprintf("%c\n",gpsx->nshemi);
	
 	posx=NMEA_Comma_Pos(p1,5);                        //得到经度
	if(posx!=0XFF)
	{												  
		temp=NMEA_Str2num(p1+posx,&dx);		 	 
		gpsx->longitude=temp/NMEA_Pow(10,dx+2);	//得到°
		rs=temp%NMEA_Pow(10,dx+2);				//得到'	
		temp=(rs*NMEA_Pow(10,6-dx))/60;
		gpsx->longitude=gpsx->longitude*NMEA_Pow(10,6)+temp;//转换为°
		mprintf("%d\n",gpsx->longitude);     //打印信息
		//g_longitude=gpsx->longitude;        //经度传递给全局变量
	}
	posx=NMEA_Comma_Pos(p1,6);								//东经还是西经
	if(posx!=0XFF)gpsx->ewhemi=*(p1+posx);	
	//mprintf("%c\n",gpsx->ewhemi);
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
    NMEA_GSV_Analysis(gpsx,buf);
	NMEA_GPRMC_Analysis(gpsx,buf);

}

void GpsAnalysis(u8 *gpsdata)
{
	GPS_Analysis(&gpsx,gpsdata);
}



void GPS_package(nmea_msg *gpsx,Core_data *core_data)
{
	u8 Warning;
//	u8 ubuf[7][3];
//	u8 hex[5][9];
//	u8 gps_byte[9];
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
			gpsx->longitude=LbsJingdu;
			gpsx->latitude=LbsWeidu;
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



u8 Need_Lbs_Data=0;
FIX_GPS fix_gpsbuf[3];


void Callback_SetGPS_CMD_Hdlr(char *str_URC)
{
	if(str_URC != NULL)
	{
		mprintf("%s", str_URC);
	}
}





s32 GetGpsLocation(u32 timeout, u8 op)
{
	u8 times=0;
    u8 rdBuff[1000];
    s32 iRet = RIL_AT_SUCCESS;
    s8 ptTimeout = 0;
    double temp=0.000;
    
    if (op)
    {
        if(gpspowersta == 0)iRet = GpsOpen();
        if (RIL_AT_SUCCESS != iRet)
        {
            mprintf("Power on GPS fail, iRet = %d.\r\n", iRet);
            return -1;
        }
        else
        {
			gpspowersta = 1;
            SendMsg2KernelForSendPMTKData();
            Ql_Sleep(500);
            
            mprintf("my_tcp_state = %d\r\n",my_tcp_state);
            if(my_tcp_state == STATE_GPRS_REGISTER)
            {
    			mprintf("_EPO_ENABLE_\r\n");
    			Ql_Sleep(1000);
    			LbsAEpoStaInit();
            }
            
            Ql_memset(rdBuff, 0, sizeof(rdBuff));
            while (timeout)
            {
                timeout--;
                times++;
                Ql_Sleep(1000);
                iRet = RIL_GPS_Read("ALL", rdBuff);
                if (RIL_AT_SUCCESS != iRet)
                {
                    mprintf("Read %d information failed.\r\n", iRet);
                }
                else
                {
                    mprintf("%s\r\n", rdBuff);
                    GpsAnalysis((u8*)rdBuff);
                    GPS_package(&gpsx, &my_core_data);
                }
                mprintf("satellitenum %d\r\n",gpsx.satellitenum);
                if(gpsx.useorno != 65 && times > 10 && gpsx.satellitenum < 4) //gps 信号不好 10秒搜不到星推出
                {
                    mprintf("gps rssi is too low\r\n");
                    break;
                }
                    

                if (gpsx.useorno == 65 && NMEA_check_crc(rdBuff))
                {
                    temp=(double)(gpsx.latitude);
                    fix_gpsbuf[ptTimeout].lat=temp/1000000;
                    temp=(double)(gpsx.longitude);
                    fix_gpsbuf[ptTimeout].lon=temp/1000000;
                    fix_gpsbuf[ptTimeout].speed=gpsx.speed;
                    mprintf("fix_gpsbuf[%d].speed=%d\r\n",ptTimeout,fix_gpsbuf[ptTimeout].speed);
                    ptTimeout++;
                    if(ptTimeout==3)
                        {
                            iRet=fix_gpslocation();
                            if(iRet==0)
                                {
                                    mprintf("\r\n+++++++++++++++ Gps = %d s ++++++++++++\r\n",times);
                                    times=0;
				                    iRet = GpsClose();
				                    if (RIL_AT_SUCCESS != iRet)
				                    {
				                        mprintf("GpsClose fail, iRet = %d.\r\n", iRet);

				                    }
				                    else
				                    {
				                        gpspowersta = 0;
				                        mprintf("GPS close ok");
				                    }
									gpsx.useorno=0;
				                    return 0;
                                }
                            else
                                {
                                    ptTimeout=0;
                                    mprintf("Useless gps position!!!!\r\n");
                                }
                        }
                }
            }
            if (gpsx.useorno != 65)
            {
                iRet = GpsClose();
                if (RIL_AT_SUCCESS != iRet)
                {
                    mprintf("GpsClose fail, iRet = %d.\r\n", iRet);

                }
                else
                {
                    gpspowersta = 0;
                    mprintf("GPS close ok");
                }
                return -1;
            }
        }
    }
    return -1;
}

s32 GetModuleLocation(u32 gpstimeout,u8 op)
{
	s32 iRet;
	iRet=GetGpsLocation(gpstimeout,op);
	if(iRet<0)
		{
			mprintf("GetGpsLocation Fail\r\n");
			iRet = RIL_GetLocation(MyCallback_Location);
			if(iRet!=RIL_AT_SUCCESS)
				{
					mprintf("Ql_Getlocation error  ret=%d\r\n",iRet);
					return -1;
				}
			else
				{
					mprintf("wait lbs data\r\n");
					return 1;
				}
			
		}
	return -1;
	
}

#if 1

s32 LbsGetLocation(void)
{
	
}


s32 WaitLbsGetLocation(u32 timeout)
{
	while(timeout)
		{
			Ql_Sleep(100);
			timeout--;
			if(Need_Lbs_Data==2)
				{
					Need_Lbs_Data=0;
					return 0;
				}
		}
	return -1;
}


extern u8 Jdbuf[20];
extern u8 Wdbuf[20];


void Callback_Location(s32 result, ST_LocInfo* loc_info)
{
    mprintf("\r\n<-- Module location: latitude=%f, longitude=%f -->\r\n", loc_info->latitude, loc_info->longitude);
	LbsJingdu = loc_info->longitude * 1000000;
    LbsWeidu = loc_info->latitude * 1000000;
    mprintf("LBSJingdu %d,LBSWeidu %d\r\n",LbsJingdu,LbsWeidu);

    Ql_sprintf(Jdbuf,"%.6f",loc_info->longitude);
	Ql_sprintf(Wdbuf,"%.6f",loc_info->latitude);
    Need_Lbs_Data = 2;
}


s32 LBSDataInit(void)
{
	u8  pdpCntxtId;
	s32 ret;
	ret = Ql_GPRS_GetPDPContextId();
	mprintf("<-- The PDP context id available is: %d (can be 0 or 1)-->\r\n", ret);
	if (ret >= 0)
	{
	    pdpCntxtId = (u8)ret;
	} else {
    	mprintf("<-- Fail to get PDP context id, try to use PDP id(0) -->\r\n");
	    pdpCntxtId = 0;
	}

	ret = RIL_NW_SetGPRSContext(pdpCntxtId);
	mprintf("<-- Set PDP context id to %d -->\r\n", pdpCntxtId);
	if (ret != RIL_AT_SUCCESS)
	{
	    mprintf("<-- Ql_RIL_SendATCmd error  ret=%d-->\r\n",ret );
	}

	// Set APN
	ret = RIL_NW_SetAPN(1, "cmnet", "", "");
	mprintf("<-- Set APN -->\r\n");

	mprintf("<--Ql_Getlocation-->\r\n");
	ret = RIL_GetLocation(Callback_Location);
		if(ret!=RIL_AT_SUCCESS)
		{
			mprintf("<-- Ql_Getlocation error  ret=%d-->\r\n",ret );
		}
	return ret;
}




#endif


#if 1


u8 CalcPMTKCRC(u8 *p)
{
	u8 i;
	u8 crcvalue;
	for(i = 0; i < Ql_strlen(p); i++) //one line
    {
        if (p[i] == '$')
        {
            crcvalue = p[i+1];
            continue;
        }
        if(p[i + 1] == '*')
            break;
        
        crcvalue ^= p[i + 1]; 
    }
    mprintf("result:%02x\n",crcvalue % 65536);
	return crcvalue;
}


void Speed_threshold(void)
{
	s32 iRet = RIL_AT_SUCCESS;
	Ql_Sleep(2000);
	iRet=RIL_GPS_CMD_Send(0,"$PMTK386,0.3*3E", Callback_SetGPS_CMD_Hdlr);
	mprintf("RIL_GPS_CMD_Send=%d\r\n",iRet);
	Ql_Sleep(2000);
	iRet=RIL_GPS_CMD_Send(0,"$PMTK225,8*23", Callback_SetGPS_CMD_Hdlr);
	mprintf("RIL_GPS_CMD_Send=%d\r\n",iRet);
}




#define PI 3.141592657  
#define EARTH_RADIUS  6378137  

static  double  rad( double degree )  
{  
    return  PI * degree / 180.0;  
}  
static  double haverSin(double x)  
{  
    double v = sin(x / 2.0);  
    return v * v;  
}  
//计算距离(单位 : m)  
static double getDistance(double lon1, double lat1, double lon2, double lat2)  
{  
    double radlon1 = rad(lon1);  
    double radlat1 = rad(lat1);  
    double radlon2 = rad(lon2);  
    double radlat2 = rad(lat2);  
      
    double a = fabs(radlat1 - radlat2);  
    double b = fabs(radlon1 - radlon2);  
      
    double h = haverSin(b) + cos(lat1)*cos(lat2)*haverSin(a);  
    double distance = 2 * EARTH_RADIUS * asin(sqrt(h));  
    return  distance;  
}  


u8 fix_gpslocation(void)
{
	double distance[3];
	distance[0]=getDistance(fix_gpsbuf[0].lon,fix_gpsbuf[0].lat,fix_gpsbuf[1].lon,fix_gpsbuf[1].lat);
	distance[1]=getDistance(fix_gpsbuf[1].lon,fix_gpsbuf[1].lat,fix_gpsbuf[2].lon,fix_gpsbuf[2].lat);
	distance[2]=getDistance(fix_gpsbuf[0].lon,fix_gpsbuf[0].lat,fix_gpsbuf[2].lon,fix_gpsbuf[2].lat);

	mprintf("distance[0]=%.3f\r\n",distance[0]);
	mprintf("distance[1]=%.3f\r\n",distance[1]);
	mprintf("distance[2]=%.3f\r\n",distance[2]);

	if(fix_gpsbuf[0].speed<3||fix_gpsbuf[1].speed<3||fix_gpsbuf[2].speed<3)
		{
			if(fix_gpsbuf[0].speed==0||fix_gpsbuf[1].speed==0||fix_gpsbuf[2].speed==0)		//静止态
				{
					if(distance[0]<5.00000&&distance[1]<5.00000&&distance[2]<5.000000)
						{
							return 0;
						}
					else
						{
							return 1;
						}
				}
			else
				{
					if(distance[0]<10.00000&&distance[1]<10.00000&&distance[2]<20.000000)
						{
							return 0;
						}
					else
						{
							return 1;
						}
				}
		}
	else
		{
			if(distance[0]<30.00000&&distance[1]<30.00000&&distance[2]<60.000000)
				{
					return 0;
				}
		}
	return 1;
	
}


#endif



