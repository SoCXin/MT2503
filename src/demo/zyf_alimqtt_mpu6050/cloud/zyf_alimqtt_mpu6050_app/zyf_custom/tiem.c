#include "ql_type.h"
#include "ql_trace.h"
#include "ql_time.h"
#include "ql_system.h"
#include "ql_stdlib.h"
#include "ql_error.h"
#include "ql_uart.h"
#include "tiem.h"
#include "uart.h"

ST_Time time;

u8 iSUartOrCenterRtc=0;


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
//     u64 totalSeconds;
//     totalSeconds= Ql_Mktime(mytime);
//     totalSeconds=totalSeconds-8*60*60;
//     Ql_MKTime2CalendarTime(totalSeconds, mytime);
     Ql_SetLocalTime(mytime);
}

u64 GetSysTime(ST_Time *mytime)
{
	u64 totalSeconds;
	Ql_GetLocalTime(mytime);	 
    totalSeconds= Ql_Mktime(mytime);
    totalSeconds=totalSeconds+8*60*60;
	Ql_MKTime2CalendarTime(totalSeconds, mytime);
    return totalSeconds;
}

u64 GetSysTimetemp(ST_Time *mytime)
{
	 u64 totalSeconds;
	 Ql_GetLocalTime(mytime);
	 totalSeconds= Ql_Mktime(mytime);
	totalSeconds=totalSeconds+8*60*60;
	Ql_MKTime2CalendarTime(totalSeconds, mytime);
    return totalSeconds;
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

void RTC_Set_UTC(u8 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec)
{
	s32 ret,atLength;
	u8 strAT[50]={0};
	atLength=Ql_sprintf(strAT,"AT+CCLK=\"%02d/%02d/%02d,%02d:%02d:%02d+00\"",syear,smon,sday,hour,min,sec);
	mprintf("%s",strAT);
	ret = Ql_RIL_SendATCmd(strAT, atLength, NULL, NULL, 0);
	if(0 != ret)
			{
				mprintf("RTC_Set_UTC Error\r\n");
			}
}



void SetUtc2Modu(void)
{
	ST_Time mytime;
	u64 totalSeconds;
	Ql_GetLocalTime(&mytime);	 
    totalSeconds= Ql_Mktime(&mytime);
    totalSeconds=totalSeconds - 8*60*60;
	Ql_MKTime2CalendarTime(totalSeconds, &mytime);
    
    mprintf("SetUtc2ModuDataTime: %04d-%02d-%02d %02d:%02d:%02d\r\n",mytime.year, mytime.month, mytime.day, mytime.hour, mytime.minute, mytime.second);
	RTC_Set_UTC(mytime.year-2000,mytime.month,mytime.day, mytime.hour,mytime.minute,mytime.second);

}

