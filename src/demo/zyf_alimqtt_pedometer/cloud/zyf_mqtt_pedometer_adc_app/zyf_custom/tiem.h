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

extern Mydatastrcut DataStrat;
extern Mydatastrcut DataEnd;



extern ST_Time time;

extern u8 iSUartOrCenterRtc;


void TimeInit(void);
void SetSysTime(ST_Time *mytime);
void GetSysTime(ST_Time *mytime);
u64 GetSysTimetemp(ST_Time *mytime);




#endif

