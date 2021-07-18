#include "zyf_auto_config.h"

ZYF_AUTO_CONFIG_T ZYFAutoconfig=
{
	"c3ba888d8e2e",
	 1883,
	 "www.hizyf.com",
	 "652541",
	 "admin",
	 "zyfadmin",
	 "",
};

/**

1792:LBS(纬度)

087B:LBS(经度)


1480:GPS(纬度)

00EA:GPS(经度）

1E9D:电量百分比

023A:信号强度

2557:运行时长

66AA:LED灯

22BC:传输间隔

**/

u8 *AutomaticRD[READONLYNUM]={"1E8D","1E9D","023A","2557","1792","087B","1480","00EA"};
u8 *AutomaticWR[READWRITE]={"66AA","22BC"};

