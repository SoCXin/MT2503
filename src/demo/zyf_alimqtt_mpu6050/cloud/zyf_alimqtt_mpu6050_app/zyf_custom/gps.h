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
    u8 gpssta;
    u8 posslnum;
    int altitude;
    u8 satellitenum; //有效微信颗数
	
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
extern u8 gpspowersta;
typedef struct  
{										    
 	u32 speed;
	double lon;
	double lat;
}FIX_GPS;   	   

extern FIX_GPS fix_gpsbuf[3];


void GetGpsDataNew(void);
void AsysLbs(Core_data *core_data);
extern u32 LbsJingdu;
extern u32 LbsWeidu;
s32 GpsOpen(void);
s32 GpsClose(void);
s32 GetGpsData(u8 *buf,u8 *name);
s32 GetModuleLocation(u32 gpstimeout,u8 op);
u8 fix_gpslocation(void);
void Speed_threshold(void);
#endif
