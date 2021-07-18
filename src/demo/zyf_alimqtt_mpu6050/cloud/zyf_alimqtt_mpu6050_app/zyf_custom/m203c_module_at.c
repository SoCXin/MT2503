/******************************************************************************

                  版权所有 (C) 2017, 金通科技

 ******************************************************************************
  文件   : M203C_MODULE_at.c
  版本   : 初稿
  作者   : zhoujie
  日期   : 2017年11月29日
  修改   :
  描述   : at 指令

  修改记录:
  日期   : 2017年11月29日
    作者   : zhoujie
    内容   : 创建文件

******************************************************************************/

#include <stddef.h>

const char* const AT_commands[] =
{
    NULL, // for URCs
    "AT+QINISTAT",
    "AT",
    "ATV",
    "ATE0",
    "AT+CMEE=1",
    "AT+IPR=9600",
    "ATI",
    "AT+GSN",
    "AT+CPIN?",
    "AT+CIMI",
    "AT+QCCID",
    "AT+CSQ",
    "AT+CREG?",
    "AT+CGREG?",
    "AT+COPS?",
    "AT+CGATT?",
    "AT+QIHEAD=1",
    "AT+QIDNSIP?",
    "AT+QIDNSIP=0",  // ip
    "AT+QIDNSIP=1",  // dns
    "AT+QISHOWRA=0", // 配置接收数据时是否显示发送方的 IP  地址和端口号
    "AT+QISHOWPT=0", /* 在接收到的数据之前增加传输层的协议类型，TCP或者UDP。 */
    "AT+QISHOWPT=1", /* 在接收到的数据之前增加传输层的协议类型，TCP或者UDP。 */
    "AT+QIMODE?",    // 配置 TCPIP  应用模式为透传模式, 0: 非透传模式,1 透传模式.
    "AT+QIMODE=0",   // 配置 TCPIP  应用模式为透传模式, 0: 非透传模式,1 透传模式.
    "AT+QIMODE=1",   // 配置 TCPIP  应用模式为透传模式, 0: 非透传模式,1 透传模式.
    "AT+QIMUX=0",
    "AT+QIMUX=1",
    "AT+QIMUX?",
    "AT+QINDI=2",   // 缓存模式 2
    "AT+QINDI=1",   // 缓存模式 1
    "AT+QINDI=0",   // 缓存模式 0
    "AT+QINDI?",    // 缓存模式 查询
    "AT+QISDE=0",   // 不允许数据回显
    "AT+QIFGCNT=0", //前置场景0
    "AT+QIFGCNT=2", //前置场景2
    "AT+QICSGP=1,\"CMNET\"",
    "AT+QISEND=%d",
    "AT+QSSLSEND=0,%d", // SSL send data<ssid[,]length]>
    "AT+QIREGAPP",
    "AT+QIACT",
    "AT+QILOCIP",
    "AT+QIOPEN=\"TCP\",\"%s\",\"%d\"",
    "AT+QISACK",
    "AT+QICLOSE",
    "AT+QIDEACT",
    "AT+QISTAT",
    "AT+QSSLSTATE",
    "AT+QIRD=0,1,0,%d",    // AT+QIRD=<id>,<sc>,<sid>,<len>
    "AT+QSSLRECV=0,0,%d",  // at+qsslrecv=cid,sid,len
    "AT&F",                // 恢复出厂设置
    "ATZ",                 // 恢复默认设置
    "AT+CFUN?",            // 15s
    "AT+CFUN=0",           // 15s
    "AT+CFUN=1",           // 15s
    "AT+QPOWD=2",          // 立即重启模块
    "AT+QITCFG=3,2,512,1", // 配置透明传输模式
    "AT&D1",               //   设置 DTR  功能模式 DTR 由 ON 至 OFF：TA 在保持当前数据通话的同时，切换至命令模式
    "AT+QCELLLOC=1",       //Get Current Location
    "AT+QNSTATUS",         //查询 GSM  网络状态
    "AT+CTZU=3",           //网络时间同步以及更新 RTC NITZ 时自动更新本地时间至 RTC，时区保存在 NVROM
    "AT+CTZR=0",           // 网络时间同步时上报时区信息的改变 以+CTZE: <tz>,<dst>,[<time>]格式上报
    "AT+QNITZ=1",          //同步网络时间 启用同步网络时间, 开启同步网络时间功能，会直接上报“+QNITZ:<time>,<ds>”
    "AT+CCLK?",            //实时时钟
    "AT+QLEDMODE=0",       //网络指示灯配置:来电振铃时网络指示灯快速闪烁
    "AT+QSCLK?",           //慢时钟配置
    "AT+QSCLK=2",          //慢时钟配置
    "AT&W",                //保存用户配置参数
    "AT+QHTTPURL",         //Set HTTP Server URL
    "AT+QHTTPPOST",        //Send HTTP POST Request
    "AT+QHTTPREAD",        //Read HTTP Server Response
    "AT+QHTTPGET",         //Send HTTP GET Request
    "AT+QHTTPDL",          //Send HTTP download request
    "AT+QFLDS=\"RAM\"",    //Get data storage size
    "AT+QFLST=\"RAM:*\"",  // List files
    "AT+QFDWL",            // Download file from storage
    "AT+QFDEL=\"RAM:*\"",  // Delete file in storage
    "AT+QFOPEN=",          // Open file
    "AT+QFREAD=",          // Read file
    "AT+QFCLOSE=",         // Close file
    "ATS0=0&W",            // ATS0 and save
    "AT+QNTP=",            // 通过网络时间服务器同步本地时间
    "AT+QGPCLASS",
    "AT+CDETXPW", // reduce TX power.
    "AT+QENG=2,3",
    "AT+QAUDCH=",
    "AT+CLVL=",
    "AT+QWTTS=0,7,2,",
    "AT+QFTPCFG=4,\"/RAM/%s\"", // AT+QFTPCFG=4,"/RAM/fm0.3.bin"
    "AT+QFTPUSER=\"%s\"",       //AT+QFTPUSER="test"
    "AT+QFTPPASS=\"%s\"",       //AT+QFTPPASS="test"
    "AT+QFTPOPEN=\"%s\",%s",    //AT+QFTPOPEN="114.215.196.51",21
    "AT+QFTPSTAT",              //AT+QFTPSTAT
    "AT+QFTPPATH=\"%s\"",       //AT+QFTPPATH? AT+QFTPPATH="/firmware/" 注意最后的'/'符号
    "AT+QFTPGET=\"%s\"",        //AT+QFTPGET="0.3"
    "AT+QFTPCLOSE",
    "AT+QSIMSTAT=1",
    "AT+QSIMDET=1,0,1",
    "AT+QGNSSC?",        // 查询GNSS模块电源状态
    "AT+QGNSSC=1",       // =1 开启GNSS模块电源
    "AT+QGNSSC=0",       // =1 开启GNSS模块电源
    "AT+QGNSSRD=%s",     // 读取定位数据
    "AT+QGNSSCMD=0,",    // 给GNSS模块发送命令
    "AT+QGNSSEPO=1",     // 开启epo
    "AT+QGNSSEPO?",      // 查询epo
    "AT+QGEPOAID",       // 触发epo
    "AT+QGNSSTS?",       // 查询时间同步状态
    "AT+QGREFLOC=%s,%s", // 纬度,经度 为秒定功能设置 Reference-location 信息
    "AT+CBC",            // 获取电池电压
    "AT&V",
    "AT+QSSLOPEN=0,0,\"%s\",%d,%d,75",
    "AT+QSSLCFG=\"sslversion\",0,3",           // 3: TLS1.2
    "AT+QSSLCFG=\"ciphersuite\",0,\"0XFFFF\"", // All support
    "AT+QSSLCFG=\"seclevel\",0,0",             // 0,0: no authentication
    "AT+QSECREAD=\"RAM:cacert.pem\"",
    "AT+QSECREAD=\"RAM:clientcert.pem\"",
    "AT+QSECREAD=\"RAM:clientkey.pem\"",
    "AT+QSECWRITE=\"RAM:cacert.pem\",%d,100",
    "AT+QSECWRITE=\"RAM:clientcert.pem\",%d,100",
    "AT+QSECWRITE=\"RAM:clientkey.pem\",%d,100",
    "AT+QSSLCFG=\"cacert\",0,\"RAM:cacert.pem\"",
    "AT+QSSLCFG=\"clientcert\",0,\"RAM:clientcert.pem\"",
    "AT+QSSLCFG=\"clientkey\",0,\"RAM:clientkey.pem\"",
    "AT+QSECDEL=\"RAM:cacert.pem\"",
    "AT+QSECDEL=\"RAM:clientcert.pem\"",
    "AT+QSECDEL=\"RAM:clientkey.pem\"",
    "AT+QSSLCLOSE=0", //Close socket index 0
    NULL,             //AT_CMD_DUMMY
};
