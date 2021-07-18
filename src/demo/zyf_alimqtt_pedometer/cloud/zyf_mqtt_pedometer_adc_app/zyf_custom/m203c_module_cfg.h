/******************************************************************************

                  版权所有 (C) 2017, 金通科技

 ******************************************************************************
  文件   : m203c_module_cfg.h
  版本   : 初稿
  作者   : zhoujie
  日期   : 2017年12月7日
  修改   :
  描述   : 移远 GPRS 模块 TCPIP/SSL/GNSS 配置

  修改记录:
  日期   : 2017年12月7日
    作者   : zhoujie
    内容   : 创建文件

******************************************************************************/


#ifndef __m203c_module_CFG_H
#define __m203c_module_CFG_H



#define WAIT_400MS (400)
#define WAIT_500MS (500)
#define WAIT_600MS (600)
#define WAIT_1S (1000)
#define WAIT_5S (5000)
#define WAIT_6S (6000)
#define WAIT_10S (10000)
#define WAIT_15S (15000)
#define WAIT_20S (20000)
#define WAIT_30S (30000)
#define WAIT_40S (40000)
#define WAIT_75S (75000)
#define WAIT_90S (90000)
#define WAIT_120S (120000)
#define WAIT_150S (150000)
#define AT_RESP_TIMEOUT_GENERAL WAIT_500MS
#define AT_RESP_TIMEOUT_5S WAIT_5S
#define AT_RESP_TIMEOUT_6S WAIT_6S
#define AT_RESP_TIMEOUT_10S WAIT_10S
#define AT_RESP_TIMEOUT_75S WAIT_75S
#define AT_RESP_TIMEOUT_150S WAIT_150S
#define AT_RESP_TIMEOUT_40S WAIT_40S
#define AT_CMD_LEN_MAX 128
#define AT_RESP_LEN_MAX 1024
#define QSCLK_M 2          // 慢时钟模式2,5秒串口无数据自动进入休眠
#define NORMAL_MODE 0      // 非透传模式
#define TRANSPARENT_MODE 1 // 透传模式
#define MODE_NOT_SET 2     // 不对tcp应用模式设置
#define MULTIPLE_DISABLE 0 // 禁用多路连接
#define MULTIPLE_ENABLE 1  // 启用多路连接
#define MULTIPLE_NOT_SET 2 // 不对多路连接设置


#define AT_DEBUG_VERBOSE 1

/*
多路连接---只能非透传模式
单路连接-透传/非透传都可
*/
// TCP
// QIMODE 指令设置
#define TCPIP_TRANSFER_MODE NORMAL_MODE
// QIMUX 指令设置
#define TCPIP_SESSIONS_MODE MULTIPLE_DISABLE
// QIOPEN 指令设置
#define TCPIP_TRANSFER_OPEN_MODE NORMAL_MODE
// SSL 暂不支持透传模式
#define SSL_TRANSFER_MODE NORMAL_MODE
#define SSL_SESSIONS_MODE MULTIPLE_DISABLE
#define SSL_TRANSFER_OPEN_MODE NORMAL_MODE
// cert and key settings
#define SSL_USE_SERVER_CERT 1
#define SSL_USE_CLIENT_CERT 0
#define SSL_USE_CLIENT_KEY 0


#define GNSS_GET0_TIMES_MAX 3
#define GNSS_GET1_TIMES_MAX 3
#define GNSS_GET2_TIMES_MAX 3
#define GNSS_GET3_TIMES_MAX 3


#endif /* __m203c_module_CFG_H */
