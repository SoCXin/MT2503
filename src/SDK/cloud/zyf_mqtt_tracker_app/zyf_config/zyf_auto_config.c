#include "zyf_auto_config.h"
ZYF_AUTO_CONFIG_T ZYFAutoconfig=
{
    "b8b71aabdb39",
	 1883,
	 "www.hizyf.com",
	 "652541",
	 "admin",
	 "zyfadmin",
	 "",
};
/**
*0F82:工作模式
*1685:震动级别
*1792:LBS(纬度)
*087B:LBS(经度)
*1480:GPS(纬度)
*00EA:GPS(经度）
*1E9D:电量百分比
*023A:信号强度
*2557:运行时长
*66AA:LED灯
*22BC:传输间隔
*/
u8 *AutomaticRD[READONLYNUM]={"1792","087B","1480","00EA","1E9D","023A","2557"};
u8 *AutomaticWR[READWRITE]={"0F82","1685","66AA","22BC","150A"};
