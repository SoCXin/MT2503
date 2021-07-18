#include "zyf_auto_config.h"
ZYF_AUTO_CONFIG_T ZYFAutoconfig=
{
    "02353245a47e",
	 1883,
	 "www.hizyf.com",
	 "652541",
	 "admin",
	 "zyfadmin",
	 "",
};
/**
*1863:开关4
*07AD:开关3
*117B:开关2
*1425:开关1
*1E70:计步
*24B2:模数转化器4
*1ED9:模数转化器3
*2592:模数转化器2
*18F0:模数转化器1	
*12DD:GPS(经度）
*04A6:GPS(纬度）
*1EF9:LBS(经度）
*0DD3:LBS(纬度）
*1E9D:电量百分比
*023A:信号强度
*2557:运行时长
*66AA:LED灯
*22BC:传输间隔
*/
u8 *AutomaticRD[READONLYNUM]={"1863","07AD","117B","1425","1E70","24B2","1ED9","2592","18F0","12DD","04A6","1EF9","0DD3","1E9D","023A","2557"};
u8 *AutomaticWR[READWRITE]={"66AA","22BC"};
