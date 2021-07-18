#ifndef __TIME_H__
#define __TIME_H__

#include "ql_time.h"


#define isPrime(year) ((year%4==0&&year%100!=0)||(year%400==0))

typedef struct 
{
	u8 MyYear;
	u8 MyMonth;
	u8 MyData;
}Mydatastrcut;


typedef struct 
{
	u8 hour;
	u8 min;
	u8 sec;			
	u8 w_year;
	u8  w_month;
	u8  w_date;	 
}_calendar_obj;					 
extern _calendar_obj calendar;	//日历结构体


extern Mydatastrcut DataStrat;
extern Mydatastrcut DataEnd;


extern u8 iSUartOrCenterRtc;

extern u8 ostimebuf[30];

extern ST_Time time;

void TimeInit(void);

void RTC_Get_UTC(u8 *syear,u8 *smon,u8 *sday,u8 *hour,u8 *min,u8 *sec);
void RTC_Get(void);

void SetSysTime(ST_Time *mytime);
void GetSysTime(ST_Time *mytime);
void SetUtc2Modu(void);

#endif

