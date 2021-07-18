#ifndef __GPS_H__
#define __GPS_H__



typedef struct  
{										    
 	u16 year;	//年份
	u8 month;	//月份
	u8 date;	//日期
	u8 hour; 	//小时
	u8 min; 	//分钟
	u8 sec; 	//秒钟
}nmea_utc_time;   	   

typedef struct  
{										    
 	
	nmea_utc_time utc;			
	u8 useorno;				
	u32 latitude;				
	u8 nshemi;							  
	u32 longitude;			 
	u8 ewhemi;					
	u32 speed;				
	u32 longitudewei;		
	u32  Mileage;
	
}nmea_msg; 

typedef struct 
{
	u8 waring[4];
	u8 statu[4];
	u8 weidu[8];
	u8 jingdu[8];
	u8 gaodu[2];
	u8 time[6];
	u8 licheng[8];
	u8 sudu[4];
	u8 fangxiang[2];
	u8 gps_head_byte[4];
}Core_data;

extern Core_data my_core_data;

extern nmea_msg gpsx;


extern u8 Jdbuf[20];
extern u8 Wdbuf[20];

void GetGpsDataNew(void);
void AsysLbs(Core_data *core_data);
extern u32 LbsJingdu;
extern u32 LbsWeidu;
s32 GpsOpen(void);
s32 GpsClose(void);

#endif
