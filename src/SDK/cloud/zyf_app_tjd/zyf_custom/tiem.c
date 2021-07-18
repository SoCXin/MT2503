#include "ql_type.h"
#include "ql_trace.h"
#include "ql_time.h"
#include "ql_system.h"
#include "ql_stdlib.h"
#include "ql_error.h"
#include "ql_uart.h"
#include "tiem.h"
#include "uart.h"


 _calendar_obj calendar;

u8 iSUartOrCenterRtc=0;

u8 ostimebuf[30];


int normalMonthDays[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};  

Mydatastrcut DataStrat;
Mydatastrcut DataEnd;

int dateDiff(Mydatastrcut mindate,Mydatastrcut maxdate)
{
	int days=0,j,flag;
	const int primeMonth[][12]={{31,28,31,30,31,30,31,31,30,31,30,31},{31,29,31,30,31,30,31,31,30,31,30,31}};	
	Mydatastrcut tmp;
	if ((mindate.MyYear>maxdate.MyYear)|| (mindate.MyYear==maxdate.MyYear&&mindate.MyMonth>maxdate.MyMonth)||(mindate.MyYear==maxdate.MyYear&&mindate.MyMonth==maxdate.MyMonth&&mindate.MyData>maxdate.MyData))
	{
		tmp=mindate;
		mindate=maxdate;
		maxdate=tmp;
	} 
	for(j=mindate.MyYear;j<maxdate.MyYear;++j)
		days+=isPrime(j)?366:365;
	
	flag=isPrime(maxdate.MyYear);
	for (j=1;j<maxdate.MyMonth;j++)
		days+=primeMonth[flag][j-1];
    flag=isPrime(maxdate.MyYear);
    for (j=1;j<mindate.MyMonth;j++)
          days-=primeMonth[flag][j-1];
    days=days+maxdate.MyData-mindate.MyData;
    return days;
} 

int leapYear(int year)  
{
    if(year %4 ==0 && year%100 !=0 || year %400 ==0) return 1;  
    else return 0;  
}  
Mydatastrcut getNewDate(Mydatastrcut initDate, int diffDays)  
{  
    Mydatastrcut reDate = initDate;  
  
    int daysAyear = 365;  
    if(leapYear(reDate.MyYear))  
    {
        daysAyear = 366;  
    }  
    while(diffDays/daysAyear)  
    {  
        diffDays = diffDays - daysAyear;  
        reDate.MyYear ++;  
        if(leapYear(reDate.MyYear))  
        {  
            daysAyear = 366;  
        }  
    }  
  
    if(leapYear(reDate.MyYear))  
    {  
        normalMonthDays[2]=29;  
    }  
    while(diffDays/normalMonthDays[reDate.MyMonth])  
    {  
        diffDays = diffDays - normalMonthDays[reDate.MyMonth];  
        reDate.MyMonth++;  
        if(reDate.MyMonth >= 13)  
        {  
            reDate.MyYear++;  
            if(leapYear(reDate.MyYear))  
            {  
                normalMonthDays[2]=29;  
            }  
            reDate.MyMonth = reDate.MyMonth%12;  
        }  
    }  
    if(leapYear(reDate.MyYear))  
    {  
        normalMonthDays[2]=29;  
    }  
    if(diffDays + reDate.MyData <= normalMonthDays[reDate.MyMonth])  
        reDate.MyData = diffDays + reDate.MyData;  
    else  
    {  
        reDate.MyData = diffDays + reDate.MyData - normalMonthDays[reDate.MyMonth];  
        reDate.MyMonth++;  
        if(reDate.MyMonth > 12)  
        {  
            reDate.MyYear++;  
            reDate.MyMonth = reDate.MyMonth%12;   
        }     
    }  
  
    return reDate;  
}  



void TimeInit(void)
{
   RTC_Set_UTC(17,01,01,00,00,00);
}



void RTC_Get(void)
{
	u8 hour;
	RTC_Get_UTC(&calendar.w_year,&calendar.w_month,&calendar.w_date,&calendar.hour,&calendar.min,&calendar.sec);
	hour=calendar.hour+8;
	if(hour>=24)
		{
			hour=hour-24;
			DataStrat.MyData=calendar.w_date;
			DataStrat.MyMonth=calendar.w_month;
			DataStrat.MyYear=calendar.w_year;
			DataEnd=getNewDate(DataStrat,1);
			calendar.w_date=DataEnd.MyData;
			calendar.w_month=DataEnd.MyMonth;
			calendar.w_year=DataEnd.MyYear;
		}
	calendar.hour=hour;
}
	

/////////////////////////////////////////////////////////////////////////////////


/*AT+CCLK="17/08/22,02:50:10+00"*/

u8 GetTimeSrc(u8 *buf,u8 index,u8 num)
{
	u8 i=0;
	u8 reitime[3]={0};
	u8 timebuf[30]={0};
	u8 *p=timebuf;
	while(index)
		{
			index--;
			while(*buf!=num)
				{
					p[i]=*buf;
					i++;
					buf++;
				}
			buf++;
		}
	Ql_sprintf(reitime,"%c%c",p[i-2],p[i-1]);
	return strtol((const char*)reitime,NULL,10);
	
}




void RTC_Set_UTC(u8 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec)
{
	u8 timebuf[30]={0};
	Ql_sprintf(timebuf,"AT+CCLK=\"%02d/%02d/%02d,%02d:%02d:%02d+00\"",syear,smon,sday,hour,min,sec);
	mprintf("%s",timebuf);
	M203C_send_Epocmd(timebuf,1<<5,200);
	Ql_Sleep(50);
	M203C_send_Epocmd("AT+CCLK?",1<<4,500);
	Ql_Sleep(50);
}

void RTC_Get_UTC(u8 *syear,u8 *smon,u8 *sday,u8 *hour,u8 *min,u8 *sec)
{
	u8 *p=NULL;
	M203C_send_Epocmd("AT+CCLK?",1<<4,500);
	Ql_Sleep(50);
	p=(u8*)strstr((const char *)ostimebuf,"+CCLK:");
	if(p!=NULL)
		{
			*syear=GetTimeSrc(ostimebuf,1,'/');
			*smon=GetTimeSrc(ostimebuf,2,'/');
			*sday=GetTimeSrc(ostimebuf,1,',');
			*hour=GetTimeSrc(ostimebuf,2,':');
			*min=GetTimeSrc(ostimebuf,3,':');
			*sec=GetTimeSrc(ostimebuf,2,'+');
		}
	else
		{
			*syear=0;
			*smon=0;
			*sday=0;
			*hour=0;
			*min=0;
			*sec=0;
		}
	
}

///////////////////////////////ST_APIS//////////////////////////////////////


ST_Time time;


void StTimeInit(void)
{
    time.year = 2016;
    time.month = 12;
    time.day = 31;
    time.hour = 18;
    time.minute = 30;
    time.second = 18;
    SetSysTime(&time);
}



void SetSysTime(ST_Time *mytime)
{
     u64 totalSeconds;
     totalSeconds= Ql_Mktime(mytime);
     totalSeconds=totalSeconds-8*60*60;
     Ql_MKTime2CalendarTime(totalSeconds, mytime);
     Ql_SetLocalTime(mytime);
}

void GetSysTime(ST_Time *mytime)
{
	 u64 totalSeconds;
	 Ql_GetLocalTime(mytime);
	 totalSeconds= Ql_Mktime(mytime);
	totalSeconds=totalSeconds+8*60*60;
	Ql_MKTime2CalendarTime(totalSeconds, mytime);
}

u8 StRTC_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec)
{
    
    time.year = syear;
    time.month = smon;
    time.day = sday;
    time.hour = hour;
    time.minute = min;
    time.second = sec;
    SetSysTime(&time);
	return 0;
}


void StRTC_Get(void)
{
	GetSysTime(&time);
}


void ChLoc2Utc(u8 *syear,u8 *smon,u8 *sday,u8 *hour,u8 *min,u8 *sec)
{
	ST_Time mytime;
	u64 totalSeconds;
	Ql_GetLocalTime(&mytime);
	*syear=(u8)(mytime.year-2000);
	*smon=(u8)mytime.month;
	*sday=(u8)mytime.day;
	*hour=(u8)mytime.hour;
	*min=(u8)mytime.minute;
	*sec=(u8)mytime.second;
	
}


void SetUtc2Modu(void)
{
	u8 syear,smon,sday, hour,min,sec;
	ChLoc2Utc(&syear,&smon,&sday,&hour,&min,&sec);
	RTC_Set_UTC(syear,smon,sday, hour,min,sec);
}

















