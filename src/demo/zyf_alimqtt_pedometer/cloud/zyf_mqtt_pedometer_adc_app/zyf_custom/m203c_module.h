/******************************************************************************

                  版权所有 (C) 2017, 金通科技

 ******************************************************************************
  文件   : m203c_module.h
  版本   : 初稿
  作者   : zhoujie
  日期   : 2017年7月28日
  修改   :
  描述   : m203c_module.c 的头文件
  函数列表   :
  修改   :
  日期   : 2017年7月28日
    作者   : zhoujie
    内容   : 创建文件

******************************************************************************/
#ifndef __m203c_module_H__
#define __m203c_module_H__

#include <inttypes.h>
#include <stdint.h>
#include "ql_type.h"
#include <stdlib.h>



#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */
enum AT_ERROR_CODE
{
    AT_SUCCESS = 0x00,     // 命令解析完成,结果成功
    AT_ERROR_SEND_CMD,     // 发送出错
    AT_ERROR_SEND_CR,      // 发送回车符出错
    AT_ERROR_RESP_TIMEOUT, // 命令响应超时
    AT_ERROR_PARSE_FAIL,   // 命令解析完成,结果失败
    AT_URC_DISCONNECT,     // 命令解析完成,结果失败 failed times limit!
    AT_URC_TCPIP_RECV,     // TCP 收到数据
    AT_URC_SSL_RECV,       // SSL 收到数据
};
typedef struct
{
    // 16/06/28,15:42:55+32
    char year[2];
    char se1;
    char month[2];
    char se2;
    char day[2];
    char se3;
    char hour[2];
    char re1;
    char minutes[2];
    char re2;
    char secons[2];
    char zone[3]; // （用当地时间和 GMT 时间之间的差别来表示，以四分之一小时格式来表示；范围-47~+48）
} asc_time_str_t;
typedef struct
{
    int32_t act_length;   // QIRD 指令读取到的真实数据长度
    int32_t total_length; // 当前缓冲区中未读取数据字节数总数
    uint8_t* data_ptr;    // 数据指针
} tcpip_recv_t;

typedef struct
{
    uint32_t file_size; // total size
    uint32_t fhandle;   // file handle
    uint32_t blk_n;
    uint32_t blk_actual_size; // 读取的当前块大小
    uint8_t* blk_data_ptr;
} file_access_parm_t;

typedef struct 
{
    uint8_t* addr;
    uint8_t* port;
    uint8_t* user;
    uint8_t* passwd;
    uint8_t* path;
    uint8_t* file_name;
} ftp_download_t;
extern int32_t GNSS_AGPS_get0();
extern int32_t GNSS_AGPS_get1();
extern int32_t GNSS_AGPS_get2();
extern int32_t GNSS_AGPS_get3();
extern int32_t GNSS_AGPS_get4();
extern int32_t GSM_ATV_Test();
extern int32_t GSM_AT_sync(uint32_t option);
extern int32_t GSM_config_base();
extern int32_t GSM_deactivate_pdp();
extern int32_t GSM_GPRS_activate_pdp();
extern int32_t GSM_GPRS_prepare(uint8_t * po_imei, uint8_t * po_ccid, uint8_t * po_imsi);
extern int32_t GSM_SSL_close();
extern int32_t GSM_SSL_config();
extern int32_t GSM_SSL_delete_cert();
extern int32_t GSM_SSL_open(const char* host, uint16_t port);
extern int32_t GSM_SSL_retrieve(char* buf, uint32_t len, uint32_t timeout_ms);
extern int32_t GSM_SSL_transfer_mode_set();
extern int32_t GSM_SSL_transmit(const char* buf, uint32_t len, uint32_t timeout_ms);
extern int32_t GSM_SSL_upload_CACert(const char* ca_cert, u32 ca_crt_len);
extern int32_t GSM_SSL_upload_ClientCert(const char* ca_cert, u32 ca_crt_len);
extern int32_t GSM_SSL_upload_ClientKey(const char* ca_cert, u32 ca_crt_len);
extern int32_t GSM_TCP_close();
extern int32_t GSM_TCP_open(const char* host, uint16_t port);
extern int32_t GSM_TCP_retrieve(char* buf, uint32_t len, uint32_t timeout_ms);
extern int32_t GSM_TCP_transfer_mode_set();
extern int32_t GSM_TCP_transmit(const char* buf, uint32_t len, uint32_t timeout_ms);
extern void OnURCHandler_QSSLOPEN(const char* strURC, void* reserved);
extern void OnURCHandler_QISEND(const char* strURC, void* reserved);
extern void OnURCHandler_QSSLURC(const char* strURC, void* reserved);
extern void OnURCHandler_QSECWRITE(const char* strURC, void* reserved);
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
#endif /* __m203c_module_H__ */
