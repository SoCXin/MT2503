/******************************************************************************

                  版权所有 (C) 2017, 金通科技

 ******************************************************************************
  文件   : m203c_module_at.h
  版本   : 初稿
  作者   : zhoujie
  日期   : 2017年11月29日
  修改   :
  描述   : m203c_module_at.c 头文件

  修改记录:
  日期   : 2017年11月29日
    作者   : zhoujie
    内容   : 创建文件

******************************************************************************/

#ifndef __m203c_module_AT_H__
#define __m203c_module_AT_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */
extern const char* const AT_commands[];
enum AT_CMD_INDEX
{
    AT_URC = 0,
    AT_QINISTAT,
    AT,       // at sync
    ATV,      // 响应返回格式
    ATE0,     // 回显
    AT_CMEE2, //上报错误设置
    AT_IPR,   // baudrate
    ATI,      // module info
    AT_GSN,   // gsn/imei
    AT_CPIN,  // 查询sim卡状态
    AT_CIMI,  // sim imsi
    AT_QCCID, // sim ccid
    AT_CSQ,
    AT_CREG,
    AT_CGREG,
    AT_COPS,
    AT_CGATT,
    AT_QIHEAD1,
    AT_QIDNSIP,
    AT_QIDNSIP_ip,
    AT_QIDNSIP_domain,
    AT_QISHOWRA0,
    AT_QISHOWPT0,
    AT_QISHOWPT1,
    AT_QIMODE,  // 透传模式或非透传模式查询
    AT_QIMODE0, // 非透传模式设置
    AT_QIMODE1, // 透传模式
    AT_QIMUX_0,
    AT_QIMUX_1,
    AT_QIMUX_status,
    AT_QINDI2,
    AT_QINDI1,
    AT_QINDI0,
    AT_QINDIstatus,
    AT_QISDE0,
    AT_QIFGCNT0, // 设置PDP Context0
    AT_QIFGCNT2, // 设置PDP Context2
    AT_QICSGP,
    AT_QISEND,
    AT_QSSLSEND,
    AT_QIREGAPP,
    AT_QIACT,
    AT_QILOCIP,
    AT_QIOPEN,
    AT_QISACK,
    AT_QICLOSE,
    AT_QIDEACT,
    AT_QISTAT,
    AT_QSSLSTATE,
    AT_QIRD,
    AT_QSSLRECV,   // 收取 ssl 数据
    ATF,           // 恢复缺省设置
    ATZ,           // 恢复出厂设置
    AT_CFUNstatus, // 设置模块功能 15s，受网络状态影响
    AT_CFUN0,      // 设置模块功能 15s，受网络状态影响
    AT_CFUN1,      // 设置模块功能 15s，受网络状态影响
    AT_QPOWD,      // 关机
    AT_QITCFG,     // 配置透明传输模式
    AT_DTR,        //设置 DTR  功能模式 DTR 由 ON 至 OFF：TA 在保持当前数据通话的同时，切换至命令模式
    AT_QCELLLOC,   //Get Current Location
    AT_QNSTATUS,   //查询 GSM  网络状态
    AT_CTZU,       //网络时间同步以及更新 RTC 无需保存，可以自动写入 Flash
    AT_CTZR,       //网络时间同步上报
    AT_QNITZ,      //同步网络时间
    AT_CCLK,       //实时时钟
    AT_QLEDMODE0,  //网络指示灯配置
    AT_QSCLKstate, //慢时钟配置
    AT_QSCLK2,     //慢时钟配置
    ATW,           //保存用户配置参数
    AT_QHTTPURL,   //Set HTTP Server URL
    AT_QHTTPPOST,  //Send HTTP POST Request
    AT_QHTTPREAD,  //Read HTTP Server Response
    AT_QHTTPGET,   //Send HTTP GET Request
    AT_QHTTPDL,    //Send HTTP file download
    AT_QFLDS,      //Get data storage size
    AT_QFLST,      // List files
    AT_QFDWL,      // Download file from storage
    AT_QFDEL,      // Delete file in storage
    AT_QFOPEN,     // Open file
    AT_QFREAD,     // Read file
    AT_QFCLOSE,    // Close file
    ATS0,          // ATS0 and save
    AT_QNTP,       // 通过网络时间服务器同步本地时间
    AT_QGPCLASS,   // 配置 GPRS    多时隙级别
    AT_CDETXPW,    // reduce TX power,The configuration can be stored in NVRAM automatically.
    AT_QENG,
    AT_QAUDCH,
    AT_CLVL,
    AT_QWTTS,
    AT_QFTPCFG,  // AT+QFTPCFG=4,"/RAM/fm0.3.bin"
    AT_QFTPUSER, //AT+QFTPUSER="test"
    AT_QFTPPASS, //AT+QFTPPASS="test"
    AT_QFTPOPEN, //AT+QFTPOPEN="114.215.196.51",21
    AT_QFTPSTAT, //AT+QFTPSTAT
    AT_QFTPPATH, //AT+QFTPPATH? AT+QFTPPATH="/firmware/"
    AT_QFTPGET,  // AT+QFTPGET="0.3"
    AT_QFTPCLOSE,
    AT_QSIMSTAT,       // SIM卡查吧状态上报
    AT_QSIMDET,        // 开启 SIM 卡检测功能
    AT_QGNSSC,         // 查询GNSS模块电源状态
    AT_QGNSSC1,        // 开启GNSS模块电源   gprs_atcmd_ex(AT_QGNSSC, NULL);
    AT_QGNSSC0,        // 关闭GNSS模块电源   gprs_atcmd_ex(AT_QGNSSC, NULL);
    AT_QGNSSRD,        // 读取定位数据
    AT_QGNSSCMD,       // 给GNSS模块发送命令 gprs_atcmd_ex(AT_QGNSSCMD, "\"指令字符串\"");
    AT_QGNSSEPO1,      // 使能EPO功能        gprs_atcmd_ex(AT_QGNSSEPO, NULL);
    AT_QGNSSEPOstatus, // 查询epo是否使能
    AT_QGEPOAID,       // 触发EPO功能
    AT_QGNSSTS,        // 查询时间同步状态
    AT_QGREFLOC,       // 为秒定功能设置Reference location信息
    AT_CBC,            // 查询电池电量和充电状态
    AT_PROFILE,        // Display Current Configuration
    AT_QSSLOPEN,
    AT_QSSLCFG_sslversion,
    AT_QSSLCFG_ciphersuite,
    AT_QSSLCFG_seclevel,
    AT_QSECREAD_cacert,
    AT_QSECREAD_clientcert,
    AT_QSECREAD_clientkey,
    AT_QSECWRITE_cacert,
    AT_QSECWRITE_clientcert,
    AT_QSECWRITE_clientkey,
    AT_QSSLCFG_cacert,
    AT_QSSLCFG_clientcert,
    AT_QSSLCFG_clientkey,
    AT_QSECDEL_cacert,
    AT_QSECDEL_clientcert,
    AT_QSECDEL_clientkey,
    AT_QSSLCLOSE_idx0,
    AT_CMD_DUMMY, // DUMMY
};

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __m203c_module_AT_H__ */
