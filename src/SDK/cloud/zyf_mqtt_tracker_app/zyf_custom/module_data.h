/******************************************************************************

                  版权所有 (C) 2018, 天创金农科技

 ******************************************************************************
  文件   : module_data.h
  版本   : 初稿
  作者   : LiCM
  日期   : 2018年03月8日
  内容   : 创建文件
  描述   : 获取模块内部数据API

  修改记录:
  日期   : 2018年03月8日
    作者   : LiCM
    内容   : 创建文件

******************************************************************************/

#include "ql_type.h"
#include "ql_trace.h"
#include "ql_uart.h"
#include "ql_system.h"
#include "ql_stdlib.h"


#ifndef __MODULE_DATA__
#define __MODULE_DATA__


//#define MODULE_DATA_DEBUG				//模块内部数据调试开关


typedef struct
{
	u8  creg;			//网络状态
	u8  cgreg;			//GPRS状态
	u8  csq;			//信号强度
	u8  bvol;			//电压百分比
	u16 mvol;		//模块电压
} Module_Data_T;

extern Module_Data_T module_data;

#endif

