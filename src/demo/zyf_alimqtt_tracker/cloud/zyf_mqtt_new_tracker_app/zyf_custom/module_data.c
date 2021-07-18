/******************************************************************************

                  版权所有 (C) 2018, 天创金农科技

 ******************************************************************************
  文件   : module_data.c
  版本   : 初稿
  作者   : LiCM
  日期   : 2018年03月8日
  内容   : 创建文件
  描述   : 获取模块内部数据信息

  修改记录:
  日期   : 2018年03月8日
    作者   : LiCM
    内容   : 创建文件

******************************************************************************/

#include "ril.h"
#include "ql_type.h"
#include "ql_trace.h"
#include "ql_uart.h"
#include "ql_system.h"
#include "ql_stdlib.h"
#include "ql_error.h"
#include "module_data.h"
#include "uart.h"

#define MODULE_DATA_DEBUG

#ifdef MODULE_DATA_DEBUG
    #define module_data_debug(format, ...) mprintf( format "\r\n", ##__VA_ARGS__)
    #define module_data_error(format, ...) mprintf( "[error]%s() %d " format "\r\n", /*__FILE__,*/ __FUNCTION__, __LINE__, ##__VA_ARGS__)
#else
   	#define module_data_debug(format, ...)
    #define module_data_error(format, ...)
#endif



Module_Data_T module_data;		//模块内部信息合集
u8 G_Bvol = 0;

/*!
 * @brief 获取模块网络注册信息
 * \n
 *
 * @param creg 注册信息 cgreg网络信息
 * @return ret 为0表示获取成功
 * \n
 * @see
 */


s32 GetModuleRegData(s32 *creg,s32 *cgreg)
{
	 s32 ret;
	 ret=RIL_NW_GetGSMState(creg);
	 if(ret!=RIL_AT_SUCCESS)
	 	{
	 		module_data_error("RIL_NW_GetGSMState Error");
	 	}
     ret=RIL_NW_GetGPRSState(cgreg);
	  if(ret!=RIL_AT_SUCCESS)
	 	{
	 		module_data_error("RIL_NW_GetGPRSState Error");
	 	}
	  return ret;
}


/*!
 * @brief 获取模块与SIM卡相关信息
 * \n
 *
 * @param imei ccid imsi 
 * @return ret 为0表示获取成功
 * \n
 * @see
 */

s32 GetModuleSIMData(void)
{
	u8 tempdata[30];
	s32 ret;
	ret =RIL_GetIMEI(tempdata);
	if(ret!=RIL_AT_SUCCESS)
	 	{
	 		module_data_error("RIL_GetIMEI Error");
	 	}
	module_data_debug("module_data.imei=%s",tempdata);
	ret =RIL_SIM_GetCCID(tempdata);
	if(ret!=RIL_AT_SUCCESS)
	 	{
	 		module_data_error("RIL_SIM_GetCCID Error");
	 	}
	module_data_debug("module_data.ccid=%s",tempdata);
	ret =RIL_SIM_GetIMSI(tempdata);
	if(ret!=RIL_AT_SUCCESS)
	 	{
	 		module_data_error("RIL_SIM_GetIMSI Error");
	 	}
	module_data_debug("module_data.imsi=%s",tempdata);
	return ret;
}


static void sort(u16* num, int m)
{
    int j, k, temp;
    for (j = 0; j < m; j++)
    {
        for (k = 0; k < m - j; k++)
        {

            if (num[k] < num[k + 1]) //num[k]<num[k+1]
            {
                temp = num[k];

                num[k] = num[k + 1];

                num[k + 1] = temp;

            }
        }
    }
}

/*!
 * @brief 获取模块内部数据信息
 * \n
 *
 * @param void
 * @return NULL
 * \n
 * @see
 */

s32 GetModuleData(void)
{
	u8 i=0;
	s32 t1,t2;
	s32 ret,other;
	u16 batt_volt[2][5];
	ret=GetModuleRegData(&t1,&t2);
	if(ret!=RIL_AT_SUCCESS)
	 	{
	 		module_data_error("GetModuleRegData Error");
	 	}
	module_data_debug("creg=%d",t1);
	module_data_debug("cgreg=%d",t2);
	for(i=0;i<5;i++)
		{
			ret = RIL_GetPowerSupply(&t1, &t2);
			if(ret!=RIL_AT_SUCCESS)
			 	{
			 		module_data_error("RIL_GetPowerSupply Error");
					return ret;
			 	}
			module_data_debug("capacity,%d\r\n",t1);
			module_data_debug("voltage,%d\r\n",t2);
			batt_volt[0][i]=t1;
			batt_volt[1][i]=t2;
		}
	sort(batt_volt[0], 5);
	sort(batt_volt[1], 5);
	module_data.mvol=batt_volt[1][3];
	module_data.bvol=batt_volt[0][3];
	ret =RIL_NW_GetSignalQuality(&t1,&t2);
	if(ret!=RIL_AT_SUCCESS)
	 	{
	 		module_data_error("RIL_NW_GetSignalQuality Error");
	 	}
	module_data_debug("csq=%d",t1);
	module_data_debug("ber=%d",t2);
	module_data.csq=t1;
	return ret;
}







