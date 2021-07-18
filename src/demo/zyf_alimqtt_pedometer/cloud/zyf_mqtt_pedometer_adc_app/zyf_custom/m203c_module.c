#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "ql_trace.h"
#include "ql_uart.h"
#include "ql_system.h"
#include "ql_stdlib.h"
#include "ql_error.h"
#include "ql_gpio.h"
#include "sys.h"
#include "ril.h"
#include "ql_socket.h"
#include "m203c_module.h"
#include "m203c_module_cfg.h"
#include "m203c_module_at.h"
#include "uart.h"

#if 1
#define at_print(format, ...) mprintf( format, ##__VA_ARGS__)
// 调试信息:
#define at_printl(format, ...) mprintf( format "\r\n", ##__VA_ARGS__)
#define at_printe(format, ...) mprintf( format "\r\n", ##__VA_ARGS__)
// 解析函数中的调试信息和错误信息
#define at_printd(format, ...) mprintf(format "\r\n", ##__VA_ARGS__)
// gnss 调试信息
#define gnss_printl(format, ...) mprintf( format "\r\n", ##__VA_ARGS__)
// tcp/ssl 打印信息
#define tcp_printl(format, ...) mprintf( format "\r\n", ##__VA_ARGS__)
#define tcp_printd(format, ...) mprintf( format "\r\n", ##__VA_ARGS__)
#endif
#define SYSTEM_DELAY_MS(nms) Ql_Sleep(nms)


u8 SSLWRITESTA=0;



static char at_cmd_buf[AT_CMD_LEN_MAX];
static char at_resp_buf[AT_RESP_LEN_MAX];
static char at_ret_data[32];
static tcpip_recv_t tcpip_recv;


// 判断字符串是ip地址还是域名,返回0为ip，否则为域名
int32_t is_ip_domain(const char* psz)
{
    uint8_t i;
    const char* b = psz;
#if 0
    // 判断前4个是否是字符
    for (i = 0; i < 4; i++)
    {
        if (0 == isprint(*(psz + i)))
        {
            return 0xff;
        }
    }
#endif
    // 超过15个字符 一定不是ip无疑了
    if (Ql_strlen(psz) > 15 || Ql_strlen(psz) < 7)
    {
        return 1;
    }
    i = 0;
    while (*psz)
    {
        if (*psz == '.')
        {
            if (psz - b < 1)
            {
                return 2;
            }
            if (psz - b > 1 && *b == '0')
            {
                return 3;
            }
            if (Ql_atoi(b) > 255)
            {
                return 4;
            }
            i++;
            b = psz + 1;
        }
        else if (!isdigit(*psz))
        {
            return 5;
        }
        psz++;
    }
    if (i != 3)
    {
        return 6;
    }
    if (psz - b < 1)
    {
        return 7;
    }
    if (psz - b > 1 && *b == '0')
    {
        return 8;
    }
    if (Ql_atoi(b) > 255)
    {
        return 9;
    }
    return 0;
}


static s32 AT_resp_parser_generic(char* line, u32 len, void* userData)
{
    at_print("%s",line);
    
    if (Ql_RIL_FindLine(line, len, "OK"))
    {  
        return  RIL_ATRSP_SUCCESS;
    }
    else if (Ql_RIL_FindLine(line, len, "ERROR"))
    {  
        return  RIL_ATRSP_FAILED;
    }
    else if (Ql_RIL_FindString(line, len, "+CME ERROR"))
    {
        return  RIL_ATRSP_FAILED;
    }
    else if (Ql_RIL_FindString(line, len, "+CMS ERROR:"))
    {
        return  RIL_ATRSP_FAILED;
    }
    return RIL_ATRSP_CONTINUE;
}

static s32 AT_resp_parser_QSECWRITE(char* line, u32 len, void* userData)
{
    char* p = NULL;
	 at_print("%s",line);
    // CONNECT
    if (userData == NULL)
    {
        p = Ql_strstr(line, "\r\nCONNECT\r\n");
        if ((p != NULL) && (p < (line + len)))
        {
            return RIL_ATRSP_SUCCESS;
        }
	  else if (Ql_RIL_FindLine(line, len, "ERROR"))
	    {  
	        return  RIL_ATRSP_FAILED;
	    }
	   else if (Ql_RIL_FindString(line, len, "+CME ERROR"))
	    {
	        return  RIL_ATRSP_FAILED;
	    }
	   at_printd("error %d", __LINE__);

    }
    return RIL_ATRSP_CONTINUE;
}


static s32 AT_resp_parser_QIMUX(char* resp, uint32_t index, void* userdata)
{
    uint32_t* mode = userdata;
	 at_print("%s",resp);
    if (mode == NULL)
    {
        return AT_resp_parser_generic(resp, index, NULL);
    }
    if (Ql_sscanf(resp, "\r\n+QIMUX: %d\r\n", mode) >= 1)
    {
        at_printd("mux=%d", *mode);
        if ((*mode == 0) || (*mode == 1))
        {
            //*userdata = mode;
            return RIL_ATRSP_SUCCESS;
        }
    }
	else if (Ql_RIL_FindLine(resp, index, "ERROR"))
    {  
        return  RIL_ATRSP_FAILED;
    }
    else if (Ql_RIL_FindString(resp, index, "+CME ERROR"))
    {
        return  RIL_ATRSP_FAILED;
    }
    return RIL_ATRSP_CONTINUE;
}

static s32 AT_resp_parser_QIMODE(char* resp, uint32_t index, void* userdata)
{
    uint32_t* mode = userdata;
	 at_print("%s",resp);
    if (mode == NULL)
    {
        return AT_resp_parser_generic(resp, index, userdata);
    }
    if (Ql_sscanf(resp, "\r\n+QIMODE: %d\r\n", mode) >= 1)
    {
        at_printd("imode=%d", *mode);
        if ((*mode == 0) || (*mode == 1))
        {
            //*userdata = mode;
            return RIL_ATRSP_SUCCESS;
        }
    }
	else if (Ql_RIL_FindLine(resp, index, "ERROR"))
    {  
        return  RIL_ATRSP_FAILED;
    }
    else if (Ql_RIL_FindString(resp, index, "+CME ERROR"))
    {
        return  RIL_ATRSP_FAILED;
    }
    return RIL_ATRSP_CONTINUE;
}


static s32 AT_resp_parser_QIDEACT(char* resp, uint32_t index, void* userdata)
{
    char* p;
    /*
    响应
    如果关闭成功返回：
    DEACT OK
    如果关闭失败，返回：
    ERROR
    最大响应时间:40s，受网络状态影响
    */
    at_print("%s", resp);
    p = Ql_strstr(resp, "DEACT OK");
    if ((p != NULL) && (p < (resp + index)))
    {
        return RIL_ATRSP_SUCCESS;
    }
	else if (Ql_RIL_FindLine(resp, index, "ERROR"))
    {  
        return  RIL_ATRSP_FAILED;
    }
    else if (Ql_RIL_FindString(resp, index, "+CME ERROR"))
    {
        return  RIL_ATRSP_FAILED;
    }
    return RIL_ATRSP_CONTINUE;
}

static s32 AT_resp_parser_CSQ(char* resp, uint32_t index, void* userdata)
{
    int32_t rssi_val, b, c, d;
    int32_t in = 0;
    int32_t in2 = 0;
    int32_t j = 0;
    char* p[16];
    char* q[16];
    char* buf = resp;
    char* inner_ptr = NULL;
	
	at_printd("%s", resp);
	
    while ((p[in] = strtok_r(buf, ":", &inner_ptr)) != NULL)
    {
        in++;
        buf = NULL;
    }
#if AT_DEBUG_VERBOSE
    at_printd("1st, now we have %d strings:\n", in);
    for (j = 0; j < in; j++)
    {
        at_printd("p[%d] =  %s", j, p[j]);
    }
#endif
    // 第二个字符串 p[1] 就是包含有信号质量数值的
    if (in >= 2)
    {
        while ((q[in2] = strtok_r(p[1], ",", &inner_ptr)) != NULL)
        {
            in2++;
            p[1] = NULL;
        }
#if AT_DEBUG_VERBOSE
        at_printd("2nd, now we have %d strings:\n", in2);
        for (j = 0; j < in2; j++)
        {
            at_printd("q[%d] =  %s", j, q[j]);
        }
#endif
        if (in2 >= 2)
        {
            rssi_val = Ql_atoi(q[0]);
            b = Ql_atoi(q[1]);
            //gprs_csq = (uint8_t)rssi_val;
            return RIL_ATRSP_SUCCESS;
        }
    }
    return RIL_ATRSP_CONTINUE;
}

static s32 AT_resp_parser_CPIN(char* resp, uint32_t index, void* userdata)
{
    char* p = NULL;
	 at_print("%s",resp);
    p = Ql_strstr(resp, "+CPIN: READY");
    if ((p != NULL) && (p < (resp + index)))
    {
        return RIL_ATRSP_SUCCESS;
    }
   else if (Ql_RIL_FindLine(resp, index, "ERROR"))
    {  
        return  RIL_ATRSP_FAILED;
    }
    else if (Ql_RIL_FindString(resp, index, "+CME ERROR"))
    {
        return  RIL_ATRSP_FAILED;
    }
	return RIL_ATRSP_CONTINUE;
}

s32 AT_resp_parse_QILOCIP(char* resp, uint32_t index, void* userdata)
{
	u8 j;
    uint32_t in = 0;
    char* p[4] = { NULL, NULL, NULL, NULL };
    char* buf = resp;
    char* inner_ptr = NULL;
	 at_print("%s",resp);
    while ((p[in] = strtok_r(buf, ".", &inner_ptr)) != NULL)
    {
        in++;
        buf = NULL;
    }
#if AT_DEBUG_VERBOSE
    for (j = 0; j < in; j++)
    {
        at_printd("p[%d] =  %s\r\n", j, p[j]);
    }
#endif
    if (in >= 3)
    {
        // 取值范围判断
        // a = atoi(p[0]);
        // b = atoi(p[1]);
        // c = atoi(p[2]);
        // d = atoi(p[3]);
        return RIL_ATRSP_SUCCESS;
    }
    else
    {
        return RIL_ATRSP_FAILED;
    }
}


static s32 AT_resp_parser_QIDNSIP(char* resp, uint32_t index, void* userdata)
{
    uint32_t* mode = userdata;

	 at_printd("%s", *resp);
	
    if (mode == NULL)
    {
        return AT_resp_parser_generic(resp, index, NULL);
    }
    if (Ql_sscanf(resp, "\r\n+QIDNSIP: %d\r\n", mode) >= 1)
    {
        at_printd("dnsip=%d", *mode);
        if ((*mode == 0) || (*mode == 1))
        {
            //*userdata = mode;
            return RIL_ATRSP_SUCCESS;
        }
    }
    return RIL_ATRSP_FAILED;
}

static s32 AT_resp_parser_QISEND_Prepare(char* resp, uint32_t index, void* userdata)
{
    char* p = NULL;
	 at_print("%s",resp);
    if (index > 0)
    {
        // ATV1
        p = Ql_strstr(resp, ">");
    }
    if ((p != NULL) && (p < (resp + index)))
    {
        return RIL_ATRSP_SUCCESS;
    }
    return RIL_ATRSP_FAILED;
}

u8 QSSLRECV[500]={0};

static int32_t AT_resp_parser_QSSLRECV(char* resp, uint32_t index, void* userdata)
{
    tcpip_recv_t* p_ssl_recv = userdata;
    uint8_t* ptrmsg = NULL;
    uint8_t* head = NULL;
    uint8_t* data_ptr = NULL;
	char* myhead = NULL;

	 Ql_strncat((char*)QSSLRECV,resp,Ql_strlen(resp));
	 myhead = (u8*)strstr((const char*)QSSLRECV,"message");
	 if(myhead)
		{
				at_print("%s",QSSLRECV);
#if 1
			    // 解析出读取到的实际数据长度和数据
			    if (head = Ql_strstr(QSSLRECV, ",TCP,"))
			    {
			        ptrmsg = head + 5;
			        p_ssl_recv->act_length = Ql_atoi(ptrmsg);
			        at_printd("len=%d", p_ssl_recv->act_length);
			        data_ptr = Ql_strstr(ptrmsg, "\r\n");
			        if (data_ptr)
			        {
			            data_ptr += 2;
			            Ql_memcpy(p_ssl_recv->data_ptr, data_ptr, p_ssl_recv->act_length);
			            return RIL_ATRSP_SUCCESS;
			        }
			    }
#endif
			 return  RIL_ATRSP_FAILED;
		}
	else
		{
			if (Ql_RIL_FindLine(resp, index, "ERROR"))
		    {  
		        return  RIL_ATRSP_FAILED;
		    }		
			return  RIL_ATRSP_CONTINUE;
		}	
    return RIL_ATRSP_CONTINUE;
}

static int32_t AT_resp_parser_QISTAT(char* resp, uint32_t index, void* userdata)
{
    uint32_t in = 0;
    uint32_t j = 0;
    char* p[4] = { NULL, NULL, NULL, NULL };
    char* buf = resp;
    char* inner_ptr = NULL;
    char* outer_ptr;
#if 1
    at_printd("TODO %s", __FUNCTION__);
    return RIL_ATRSP_SUCCESS;
#endif
    while ((p[in] = strtok_r(buf, "\r", &inner_ptr)) != NULL)
    {
        in++;
        buf = NULL;
    }
    // 如果正常 将会拆分出3个字符串 分别为指令执行状态码，tcp状态码，结束符 <lf>
    if (in >= 3)
    {
#if AT_DEBUG_VERBOSE
        for (j = 0; j < in; j++)
        {
            at_printd("p[%d] %#x\r\n", j, *p[j]);
        }
#endif
        at_printd("IPState = %#x", *p[1]);
        //        g_gprs_state.ipstate = *p[1];
        return RIL_ATRSP_SUCCESS;
    }
    else
    {
        return RIL_ATRSP_FAILED;
    }
}

static int32_t AT_resp_parser_QICLOSE(char* resp, uint32_t index, void* userdata)
{
    char* p = NULL;
	 at_print("%s",resp);
    p = Ql_strstr(resp, "CLOSE OK");
    if ((p != NULL) && (p < (resp + index)))
    {
        //        g_gprs_state.ipstate = GPRS_STATE_IP_CLOSE;
        return RIL_ATRSP_SUCCESS;
    }
    return RIL_ATRSP_FAILED;
}

static int32_t AT_resp_parser_GSN(char* resp, uint32_t index, void* userdata)
{
    char* gprs_imei = userdata;
   if (Ql_RIL_FindLine(resp, index, "ERROR"))
		    {  
		        return  RIL_ATRSP_FAILED;
		    }	
    if (Ql_sscanf(resp, "\r\n%s\r\nOK\r\n", gprs_imei) >= 1)
    {
        at_printd("imei=%s", gprs_imei);
        return RIL_ATRSP_SUCCESS;
    }
    at_printd("imei error");
    return RIL_ATRSP_CONTINUE;
}
static int32_t AT_resp_parser_CIMI(char* resp, uint32_t index, void* userdata)
{
    char* gprs_imsi = userdata;
  if (Ql_RIL_FindLine(resp, index, "ERROR"))
		    {  
		        return  RIL_ATRSP_FAILED;
		    }	
    if (Ql_sscanf(resp, "\r\n%s\r\nOK\r\n", gprs_imsi) >= 1)
    {
        at_printd("imsi=%s", gprs_imsi);
        return RIL_ATRSP_SUCCESS;
    }
    at_printd("imsi error");
    return RIL_ATRSP_CONTINUE;
}

static int32_t AT_resp_parser_QCCID(char* resp, uint32_t index, void* userdata)
{
    char* gprs_ccid = userdata;
   if (Ql_RIL_FindLine(resp, index, "ERROR"))
		    {  
		        return  RIL_ATRSP_FAILED;
		    }	
    if (Ql_sscanf(resp, "\r\n%s\r\nOK\r\n", gprs_ccid) >= 1)
    {
        at_printd("ccid=%s", gprs_ccid);
        return RIL_ATRSP_SUCCESS;
    }
    at_printd("ccid error");
    return RIL_ATRSP_CONTINUE;
}


static s32 AT_cmd_send(const char* atCmd, u32 atCmdLen,Callback_ATResponse atRsp_callBack, void* userData, u32 timeout_ms)
{
	at_print("%s\r\n",atCmd);
	return Ql_RIL_SendATCmd(atCmd, atCmdLen, atRsp_callBack, userData,timeout_ms);
}


int32_t GSM_AT_sync(uint32_t option)
{
	return 0;	
}

void OnURCHandler_QSECWRITE(const char* strURC, void* reserved)
{
	u8 *p;
	at_printd("%s",strURC);
	p=(u8*)Ql_strstr((const char *)strURC,"+QSECWRITE:");
	if(p!=NULL)
		{
			SSLWRITESTA|=1<<0;
		}
}

void OnURCHandler_QSSLOPEN(const char* strURC, void* reserved)
{
	u8 *p;
	at_printd("%s",strURC);
	p=(u8*)Ql_strstr((const char *)strURC,"+QSSLOPEN:");
	if(p!=NULL)
		{
			SSLWRITESTA|=1<<1;
		}
}


void OnURCHandler_QISEND(const char* strURC, void* reserved)
{
	u8 *p;
	at_print("%s",strURC);
	p=(u8*)Ql_strstr((const char *)strURC,"SEND");
	if(p!=NULL)
		{
			 p = Ql_strstr(strURC, "SEND OK");
			 if(p!=NULL)
			 	{
			 		SSLWRITESTA|=1<<2;
			 	}
			  p = Ql_strstr(strURC, "SEND FAIL");
			 if(p!=NULL)
			 	{
			 		SSLWRITESTA|=1<<3;
			 	}
		}
}

void OnURCHandler_QSSLURC(const char* strURC, void* reserved)
{
	u8 *p;
	char* inner_ptr = NULL;
	at_printd("%s",strURC);
	p=(u8*)Ql_strstr((const char *)strURC,"+QSSLURC:");
	if(p!=NULL)
		{
			  	inner_ptr = NULL;
		        inner_ptr = strstr(strURC, "\"recv\"");
		        if (inner_ptr != NULL)
		        {
		            SSLWRITESTA|=1<<4;
		        }
		        inner_ptr = strstr(strURC, "\"closed\"");
		        if (inner_ptr != NULL)
		        {
		            SSLWRITESTA|=1<<5;
		        }
		}
}


u8 WaitURCSSLRe(u8 waitnum,u32 timeout)
{
	u8 res=0;
	u16 t=timeout/10;
	while(--t)	
		{
			Ql_Sleep(10);
			if(SSLWRITESTA&waitnum)break;
		}
	SSLWRITESTA&=~waitnum;
	if(t==0)res=1;
	return res;
}


s32 WaitQSSLURCRe(u8 waitnum1,u8 waitnum2,u32 timeout)
{
	s32 res=0;
	u16 t=timeout/10;
	while(--t)	
		{
			Ql_Sleep(10);
			if(SSLWRITESTA&waitnum1)
				{
					res=AT_URC_SSL_RECV;
					break;
				}
			if(SSLWRITESTA&waitnum2)
				{
					res=AT_URC_DISCONNECT;
					break;
				}
		}
	SSLWRITESTA&=~waitnum1;
	SSLWRITESTA&=~waitnum2;
	if(t==0)res=AT_ERROR_PARSE_FAIL;
	return res;
}



int32_t GSM_SSL_upload_CACert(const char* ca_cert, u32 ca_crt_len)
{
	 s32 at_ret;
    int32_t ret;
    uint32_t checksum = 0;
    uint32_t* sec_len = (uint32_t*)at_ret_data;
    at_printd("%s", __FUNCTION__);
#if defined(SSL_USE_SERVER_CERT) && (SSL_USE_SERVER_CERT == 1)
    //Check CA certificate is correct or not.
    at_ret = AT_cmd_send(AT_commands[AT_QSECREAD_cacert],Ql_strlen(AT_commands[AT_QSECREAD_cacert]),AT_resp_parser_generic, NULL, AT_RESP_TIMEOUT_GENERAL);
    if (at_ret == RIL_AT_SUCCESS)
    {
        at_printd("证书已存在\r\n");
        return 0;
    }
    ret = Ql_snprintf(at_cmd_buf, AT_CMD_LEN_MAX, AT_commands[AT_QSECWRITE_cacert], ca_crt_len);
    if ((ret < 0) || (ret > AT_CMD_LEN_MAX))
    {
        return __LINE__;
    }
    // Upload the CA certificate to RAM.
    at_printd("secwrite cmd\r\n");
    at_ret = AT_cmd_send(at_cmd_buf, Ql_strlen(at_cmd_buf),AT_resp_parser_QSECWRITE, NULL, AT_RESP_TIMEOUT_GENERAL);
    if (at_ret != RIL_AT_SUCCESS)
    {
        return __LINE__;
    }
    at_printd("send cert %d bytes...", ca_crt_len);
    at_ret = Ql_RIL_WriteDataToCore(ca_cert, ca_crt_len);
    if (at_ret <0)
    {
        return __LINE__;
    }
    at_printd("check resp\r\n");
	SYSTEM_DELAY_MS(3000);
    at_ret = WaitURCSSLRe(1<<0,WAIT_5S);
   if (at_ret != 0)
    {
    	at_printd("证书写入出错");
        return __LINE__;
    }
#endif
    return 0;
}


int32_t GSM_SSL_transfer_mode_set()
{
    int32_t i;
    int32_t ret;
    s32 at_ret;
    uint32_t checksum = 0;
    uint32_t ca_cert_len = 0;
    uint32_t* mode = (uint32_t*)at_ret_data;
    at_printl("%s", __LINE__);
#if (SSL_SESSIONS_MODE == MULTIPLE_ENABLE)
    // 查询多路连接配置
    *mode = 0xff;
    at_ret = AT_cmd_send(AT_commands[AT_QIMUX_status], Ql_strlen(AT_commands[AT_QIMUX_status]),AT_resp_parser_QIMUX, mode, AT_RESP_TIMEOUT_GENERAL);
    if (at_ret != RIL_AT_SUCCESS)
    {
        return __LINE__;
    }
    at_printl("muxmode=%d", *mode);
    if (*mode != SSL_SESSIONS_MODE)
    {
        // 如果当前是透传模式，必须先设置为非透传模式才能启用多路连接 否则指令报错
        // 查询模式
        *mode = 0xff;
        at_ret = AT_cmd_send(AT_commands[AT_QIMODE], Ql_strlen(AT_commands[AT_QIMODE]),AT_resp_parser_QIMODE, mode, AT_RESP_TIMEOUT_GENERAL);
        if (at_ret != RIL_AT_SUCCESS)
        {
            return __LINE__;
        }
        at_printl("imode=%d", *mode);
        if (*mode != NORMAL_MODE)
        {
            at_printl("set to NORMAL_MODE");
            // 配置模式
            at_ret = AT_cmd_send(AT_commands[AT_QIMODE0],Ql_strlen(AT_commands[AT_QIMODE0]), AT_resp_parser_QIMODE, NULL, AT_RESP_TIMEOUT_GENERAL);
            if (at_ret != RIL_AT_SUCCESS)
            {
                return __LINE__;
            }
        }
        //启用多路连接
        at_printl("MULTIPLE_ENABLE");
        at_ret = AT_cmd_send(AT_commands[AT_QIMUX_1], Ql_strlen(AT_commands[AT_QIMUX_1]),AT_resp_parser_generic, NULL, AT_RESP_TIMEOUT_GENERAL);
        if (at_ret != RIL_AT_SUCCESS)
        {
            return __LINE__;
        }
    }
#elif (SSL_SESSIONS_MODE == MULTIPLE_DISABLE)
    // 查询多路连接配置
    *mode = 0xff;
    at_ret = AT_cmd_send(AT_commands[AT_QIMUX_status], Ql_strlen(AT_commands[AT_QIMUX_status]),AT_resp_parser_QIMUX, mode, AT_RESP_TIMEOUT_GENERAL);
    if (at_ret != RIL_AT_SUCCESS)
    {
        return __LINE__;
    }
    at_printl("muxmode=%d", *mode);
    if (*mode != SSL_SESSIONS_MODE)
    {
        at_printl("MULTIPLE_DISABLE");
        // 禁用多路连接
        at_ret = AT_cmd_send(AT_commands[AT_QIMUX_0], Ql_strlen(AT_commands[AT_QIMUX_0]), AT_resp_parser_generic, NULL, AT_RESP_TIMEOUT_GENERAL);
        if (at_ret != RIL_AT_SUCCESS)
        {
            return __LINE__;
        }
    }
#else
#warning TCPIP_SESSIONS_MODE Not defined
    at_printl("多路连接未配置");
#endif
#if (SSL_TRANSFER_MODE == NORMAL_MODE)
    // 查询模式
    *mode = 0xff;
    at_ret = AT_cmd_send(AT_commands[AT_QIMODE],  Ql_strlen(AT_commands[AT_QIMODE]),AT_resp_parser_QIMODE, mode, AT_RESP_TIMEOUT_GENERAL);
    if (at_ret != RIL_AT_SUCCESS)
    {
        return __LINE__;
    }
    at_printl("imode=%d", *mode);
    if (*mode != SSL_TRANSFER_MODE)
    {
        at_printl("NORMAL_MODE");
        // 配置模式
        at_ret = AT_cmd_send(AT_commands[AT_QIMODE0], Ql_strlen(AT_commands[AT_QIMODE0]), AT_resp_parser_QIMODE, NULL, AT_RESP_TIMEOUT_GENERAL);
        if (at_ret != RIL_AT_SUCCESS)
        {
            return __LINE__;
        }
    }
#elif (SSL_TRANSFER_MODE == TRANSPARENT_MODE)
    // 先查询模式
    *mode = 0xff;
    at_ret = AT_cmd_send(AT_commands[AT_QIMODE],  Ql_strlen(AT_commands[AT_QIMODE]),AT_resp_parser_QIMODE, mode, AT_RESP_TIMEOUT_GENERAL);
    if (at_ret != 0)
    {
        return __LINE__;
    }
    at_printl("imode=%d", *mode);
    if (*mode != TCPIP_TRANSFER_MODE)
    {
        at_printl("TRANSPARENT_MODE");
        // 配置模式
        at_ret = AT_cmd_send(AT_commands[AT_QIMODE1], Ql_strlen(AT_commands[AT_QIMODE1]), AT_resp_parser_QIMODE, NULL, AT_RESP_TIMEOUT_GENERAL);
        if (at_ret != RIL_AT_SUCCESS)
        {
            return __LINE__;
        }
    }
#else
#warning TCPIP_TRANSFER_MODE Not defined
    at_printl("TCP传输模式未配置");
#endif
    return 0;
}

int32_t GSM_GPRS_activate_pdp()
{
	s32 at_ret;
    int32_t i;
    int32_t ret;
    uint32_t checksum = 0;
    uint32_t ca_cert_len = 0;
    at_printl("%s", __FUNCTION__);

    // Deactivate GPRS PDP context.
    at_ret = AT_cmd_send(AT_commands[AT_QIDEACT], Ql_strlen(AT_commands[AT_QIDEACT]),AT_resp_parser_QIDEACT, NULL, AT_RESP_TIMEOUT_40S);
   // if (at_ret != RIL_AT_SUCCESS)
   // {
   //		 at_printl("at_ret=%d", at_ret);
   //     return __LINE__;
   // }

    // 查询信号质量
    for (i = 0; i < 10; i++)
    {
        at_ret = AT_cmd_send(AT_commands[AT_CSQ],Ql_strlen(AT_commands[AT_CSQ]), AT_resp_parser_CSQ, NULL, AT_RESP_TIMEOUT_GENERAL);
        if (at_ret == RIL_AT_SUCCESS)
        {
            break;
        }
    }
    // 信号质量不通过
    if (i >= 10)
    {
        return __LINE__;
    }
    // 查询sim卡状态
    at_ret = AT_cmd_send(AT_commands[AT_CPIN], Ql_strlen(AT_commands[AT_CPIN]),AT_resp_parser_CPIN, NULL, AT_RESP_TIMEOUT_5S);
    if (at_ret != RIL_AT_SUCCESS)
    {
        return __LINE__;
    }
	
#if 0	
    // 查询网络注册状态
    if (GSM_CheckCellularNetwork())
    {
        return __LINE__;
    }
	
#endif
	
    //Set context 0 as foreground context.
    // OK
    at_ret = AT_cmd_send(AT_commands[AT_QIFGCNT0], Ql_strlen(AT_commands[AT_QIFGCNT0]), AT_resp_parser_generic, NULL, AT_RESP_TIMEOUT_GENERAL);
    if (at_ret != RIL_AT_SUCCESS)
    {
        return __LINE__;
    }
    //Set bearer type as GPRS and the APN is
    // “CMNET", no username and password for the
    // APN.
    // OK
    at_ret = AT_cmd_send(AT_commands[AT_QICSGP], Ql_strlen(AT_commands[AT_QICSGP]),AT_resp_parser_generic, NULL, AT_RESP_TIMEOUT_GENERAL);
    if (at_ret != RIL_AT_SUCCESS)
    {
        return __LINE__;
    }
    //Register to TCP/IP stack.
    // OK
    at_ret = AT_cmd_send(AT_commands[AT_QIREGAPP], Ql_strlen(AT_commands[AT_QIREGAPP]),AT_resp_parser_generic, NULL, AT_RESP_TIMEOUT_GENERAL);
    if (at_ret != RIL_AT_SUCCESS)
    {
        return __LINE__;
    }

	Ql_Sleep(300);
    //Activate GPRS PDP context.
    // OK
    at_ret = AT_cmd_send(AT_commands[AT_QIACT],Ql_strlen(AT_commands[AT_QIACT]), AT_resp_parser_generic, NULL, AT_RESP_TIMEOUT_150S);
   if (at_ret != RIL_AT_SUCCESS)
    {
        return __LINE__;
   	}

#if 0	
    if (0 != GSM_AT_sync(0))
    {
        return __LINE__;
    }
#endif
	
    //Query the local IP address.
    at_ret = AT_cmd_send(AT_commands[AT_QILOCIP],Ql_strlen(AT_commands[AT_QILOCIP]), AT_resp_parse_QILOCIP, NULL, AT_RESP_TIMEOUT_GENERAL);
    if (at_ret != RIL_AT_SUCCESS)
    {
        return __LINE__;
    }
    return 0;
}


int32_t GSM_SSL_config()
{
    int32_t ret;
    int32_t at_ret;
    uint32_t checksum = 0;
    uint32_t ca_cert_len = 0;
    uint32_t at_resp_len;
    int32_t status;
    at_printl("%s", __FUNCTION__);
    at_ret = AT_cmd_send(AT_commands[AT],Ql_strlen(AT_commands[AT]), AT_resp_parser_generic, NULL, AT_RESP_TIMEOUT_GENERAL);
    if (at_ret != RIL_AT_SUCCESS)
    {
        return __LINE__;
    }
    //Configure SSL version
    // context 0
    // TLS1.2
    // OK
    at_ret = AT_cmd_send(AT_commands[AT_QSSLCFG_sslversion], Ql_strlen(AT_commands[AT_QSSLCFG_sslversion]),AT_resp_parser_generic, NULL, AT_RESP_TIMEOUT_GENERAL);
    if (at_ret != RIL_AT_SUCCESS)
    {
        return __LINE__;
    }
    // AT+QSSLCFG="ciphersuite",0,"0XFFFF" //Configure ciphersuite
    at_ret = AT_cmd_send(AT_commands[AT_QSSLCFG_ciphersuite],Ql_strlen(AT_commands[AT_QSSLCFG_ciphersuite]), AT_resp_parser_generic, NULL, AT_RESP_TIMEOUT_GENERAL);
    if (at_ret != RIL_AT_SUCCESS)
    {
        return __LINE__;
    }
    // Configure the authentication mode
    at_ret = AT_cmd_send(AT_commands[AT_QSSLCFG_seclevel], Ql_strlen(AT_commands[AT_QSSLCFG_seclevel]),AT_resp_parser_generic, NULL, AT_RESP_TIMEOUT_GENERAL);
    if (at_ret != RIL_AT_SUCCESS)
    {
        return __LINE__;
    }
#if defined(SSL_USE_SERVER_CERT) && (SSL_USE_SERVER_CERT == 1)
    //Check CA certificate is correct or not.
    at_ret = AT_cmd_send(AT_commands[AT_QSECREAD_cacert],Ql_strlen(AT_commands[AT_QSECREAD_cacert]), AT_resp_parser_generic, NULL, AT_RESP_TIMEOUT_GENERAL);
    if (at_ret != RIL_AT_SUCCESS)
    {
        return __LINE__;
    }
    // Configure CA certificate
    at_ret = AT_cmd_send(AT_commands[AT_QSSLCFG_cacert], Ql_strlen(AT_commands[AT_QSSLCFG_cacert]),AT_resp_parser_generic, NULL, AT_RESP_TIMEOUT_GENERAL);
    if (at_ret != RIL_AT_SUCCESS)
    {
        return __LINE__;
    }
#endif
#if defined(SSL_USE_CLIENT_CERT) && (SSL_USE_CLIENT_CERT == 1)
    // Check client certificate is correct or not.
    at_ret = AT_cmd_send(AT_commands[AT_QSECREAD_clientcert],Ql_strlen(AT_commands[AT_QSECREAD_clientcert]), AT_resp_parser_generic, NULL, AT_RESP_TIMEOUT_GENERAL);
    if (at_ret != RIL_AT_SUCCESS)
    {
        return __LINE__;
    }
    //Configure client certificate.
    at_ret = AT_cmd_send(AT_commands[AT_QSSLCFG_clientcert], Ql_strlen(AT_commands[AT_QSSLCFG_clientcert]),AT_resp_parser_generic, NULL, AT_RESP_TIMEOUT_GENERAL);
    if (at_ret != RIL_AT_SUCCESS)
    {
        return __LINE__;
    }
#endif
#if defined(SSL_USE_CLIENT_KEY) && (SSL_USE_CLIENT_KEY == 1)
    // Check client private key is correct or not.
    at_ret = AT_cmd_send(AT_commands[AT_QSECREAD_clientkey],Ql_strlen(AT_commands[AT_QSECREAD_clientkey]), AT_resp_parser_generic, NULL, AT_RESP_TIMEOUT_GENERAL);
    if (at_ret != RIL_AT_SUCCESS)
    {
        return __LINE__;
    }
    //Configure client key.
    at_ret = AT_cmd_send(AT_commands[AT_QSSLCFG_clientkey], Ql_strlen(AT_commands[AT_QSSLCFG_clientkey]),AT_resp_parser_generic, NULL, AT_RESP_TIMEOUT_GENERAL);
    if (at_ret != RIL_AT_SUCCESS)
    {
        return __LINE__;
    }
#endif
    return 0;
}


int32_t GSM_SSL_open(const char* host, uint16_t port)
{
	 s32 at_ret;
    int32_t ret;
    uint32_t checksum = 0;
    uint32_t status = 0;
    uint32_t* mode = (uint32_t*)at_ret_data;
    tcpip_recv.total_length = 0;
    tcpip_recv.data_ptr = NULL;
    tcpip_recv.act_length = 0;
    tcp_printd("%s", __FUNCTION__);
    if (is_ip_domain(host) != 0)
    {
        // 设置输入为域名
        at_ret = AT_cmd_send(AT_commands[AT_QIDNSIP_domain], Ql_strlen(AT_commands[AT_QIDNSIP_domain]),AT_resp_parser_QIDNSIP, NULL, AT_RESP_TIMEOUT_GENERAL);
        if (at_ret != RIL_AT_SUCCESS)
        {
            return __LINE__;
        }
    }
    else
    {
        // 设置输入为ip
        at_ret = AT_cmd_send(AT_commands[AT_QIDNSIP_ip],Ql_strlen(AT_commands[AT_QIDNSIP_ip]), AT_resp_parser_QIDNSIP, NULL, AT_RESP_TIMEOUT_GENERAL);
        if (at_ret != RIL_AT_SUCCESS)
        {
            return __LINE__;
        }
    }
    // param:
    // Secure socket
    // SSL context index
    // Transparent mode
    // 120 second timeout. The default value is 90 seconds.
#if (SSL_TRANSFER_OPEN_MODE == NORMAL_MODE)
    ret = Ql_snprintf(at_cmd_buf, AT_CMD_LEN_MAX, AT_commands[AT_QSSLOPEN], host, port, NORMAL_MODE);
#else
#error SSL_TRANSFER_OPEN_MODE not support now.
#endif
    at_ret = AT_cmd_send(at_cmd_buf,Ql_strlen(at_cmd_buf), AT_resp_parser_generic, NULL, AT_RESP_TIMEOUT_75S);
    if (at_ret != RIL_AT_SUCCESS)
    {
        return __LINE__;
    }
    // 检查连接状态码
  	at_ret = WaitURCSSLRe(1<<1,AT_RESP_TIMEOUT_75S);
    if (at_ret != 0)
    {
    	at_printd("打开错误");
        return __LINE__;
    }

#if 0
    if (*mode != 0)
    {
        return __LINE__;
    }
#endif
    return 0;
}


// SSL 发送数据
// 出错返回负值
int32_t GSM_SSL_transmit(const char* buf, uint32_t len, uint32_t timeout_ms)
{
    uint32_t i;
#if (SSL_TRANSFER_MODE == NORMAL_MODE)
    int32_t ret;
    int32_t at_ret;
    uint32_t checksum = 0;
    uint32_t status = 0;
    uint32_t at_resp_len;
    tcp_printd("%s", __FUNCTION__);
    if (0 != GSM_AT_sync(0))
    {
        tcp_printl("error %d", __LINE__);
        return -5;
    }
    // sslsend command
    ret = Ql_snprintf(at_cmd_buf, AT_CMD_LEN_MAX, AT_commands[AT_QSSLSEND], len);
    if ((ret < 0) || (ret > AT_CMD_LEN_MAX))
    {
        return -1;
    }
    at_printd("qsslsend cmd");
    at_ret = AT_cmd_send(at_cmd_buf,Ql_strlen(at_cmd_buf), AT_resp_parser_QISEND_Prepare, NULL, AT_RESP_TIMEOUT_GENERAL);
    if (at_ret != 0)
    {
        tcp_printl("error %d", __LINE__);
        return -2;
    }
    // 发送数据
    at_printd("send data %d bytes...", len);
    at_ret = Ql_RIL_WriteDataToCore(buf, len);
    if (at_ret <0)
    {
        tcp_printl("error %d", __LINE__);
        return -4;
    }
    at_printd("check resp");
    at_ret = WaitURCSSLRe(1<<2,AT_RESP_TIMEOUT_5S);
    if (at_ret != 0)
    {
        tcp_printl("error %d", __LINE__);
        return -3;
    }
#elif (SSL_TRANSFER_MODE == TRANSPARENT_MODE)
#error SSL TRANSPARENT_MODE not support.
#endif
    tcp_printd("qsslsend %d", len);
    return len;
}
// SSL 接收指定字节数据
int32_t GSM_SSL_retrieve(char* buf, uint32_t len, uint32_t timeout_ms)
{
    int i = 0;
    tcp_printd("%s", __FUNCTION__);
    tcp_printd("tmo=%d", timeout_ms);
    tcp_printd("len=%d", len);
#if (SSL_TRANSFER_MODE == NORMAL_MODE)
    int32_t ret;
    int32_t at_ret;
    uint32_t checksum = 0;
    uint32_t status = 0;
    uint32_t at_resp_len;
    if (tcpip_recv.total_length == 0)
    {
        // 等待收到数据
        tcp_printd("wait SSL-recv");
        at_ret = WaitQSSLURCRe(1<<4,1<<5,AT_RESP_TIMEOUT_75S);
        if (AT_URC_DISCONNECT == at_ret)
        {
            // TCP connection error occur.
            tcp_printl("connection error %d", __LINE__);
            return -2;
        }
        if (at_ret != AT_URC_SSL_RECV)
        {
            tcp_printl("error %d %d", at_ret, __LINE__);
            return 0;
        }
        tcpip_recv.total_length = 1;
    }

    if (0 != GSM_AT_sync(0))
    {
        tcp_printl("error %d", __LINE__);
        return -1;
    }

    tcp_printd("QSSLRECV cmd");
    // 读取数据 指令
    ret = Ql_snprintf(at_cmd_buf, AT_CMD_LEN_MAX, AT_commands[AT_QSSLRECV], 500);
    if ((ret < 0) || (ret > AT_CMD_LEN_MAX))
    {
        tcp_printl("gen cmd error %d", __LINE__);
        return -2;
    }
    tcpip_recv.data_ptr = buf;
    at_ret = AT_cmd_send(at_cmd_buf,Ql_strlen(at_cmd_buf), AT_resp_parser_QSSLRECV, &tcpip_recv, AT_RESP_TIMEOUT_GENERAL);
    if (at_ret != AT_SUCCESS)
    {
        tcp_printl("error %d %d", at_ret, __LINE__);
        tcpip_recv.total_length = 0;
        return -1;
    }
    if (tcpip_recv.act_length == 0)
    {
        tcpip_recv.total_length = 0;
    }
    return tcpip_recv.act_length;
#elif (SSL_TRANSFER_MODE == TRANSPARENT_MODE)
#error SSL TRANSPARENT_MODE not support.
#endif
}


int32_t GSM_SSL_close()
{
    int32_t ret;
    int32_t at_ret;
    uint32_t checksum = 0;
    uint32_t status = 0;
    uint32_t at_resp_len;
    tcp_printd("%s", __FUNCTION__);

    if (0 != GSM_AT_sync(0))
    {
        return __LINE__;
    }

    tcpip_recv.total_length = 0;
    // 查询连接状态
    at_ret = AT_cmd_send(AT_commands[AT_QSSLSTATE], Ql_strlen(AT_commands[AT_QSSLSTATE]),AT_resp_parser_QISTAT, NULL, AT_RESP_TIMEOUT_GENERAL);
    if (at_ret != 0)
    {
        return __LINE__;
    }
    // Close socket index
    at_ret = AT_cmd_send(AT_commands[AT_QSSLCLOSE_idx0], Ql_strlen(AT_commands[AT_QSSLCLOSE_idx0]), AT_resp_parser_QICLOSE, NULL, AT_RESP_TIMEOUT_5S);
    if (at_ret != 0)
    {
        //return __LINE__;
    }
    tcp_printd("ssl close ok");
    return 0;
}


int32_t GSM_SSL_delete_cert()
{
    int32_t ret;
    int32_t at_ret;
    uint32_t checksum = 0;
    uint32_t status = 0;
    uint32_t at_resp_len;
    tcp_printd("%s", __FUNCTION__);
#if defined(SSL_USE_SERVER_CERT) && (SSL_USE_SERVER_CERT == 1)
    // 删除 ca
    at_ret = AT_cmd_send(AT_commands[AT_QSECDEL_cacert], Ql_strlen(AT_commands[AT_QSECDEL_cacert]),AT_resp_parser_generic, NULL, AT_RESP_TIMEOUT_GENERAL);
    if (at_ret != 0)
    {
        //return __LINE__;
    }
#endif
#if defined(SSL_USE_CLIENT_CERT) && (SSL_USE_CLIENT_CERT == 1)
    // 删除 证书
    at_ret = AT_cmd_send(AT_commands[AT_QSECDEL_clientcert],  Ql_strlen(AT_commands[AT_QSECDEL_clientcert]),AT_resp_parser_generic, NULL, AT_RESP_TIMEOUT_GENERAL);
    if (at_ret != 0)
    {
        //return __LINE__;
    }
#endif
#if defined(SSL_USE_CLIENT_KEY) && (SSL_USE_CLIENT_KEY == 1)
    // 删除私钥
    at_ret = AT_cmd_send(AT_commands[AT_QSECDEL_clientkey],  Ql_strlen(AT_commands[AT_QSECDEL_clientkey]),AT_resp_parser_generic, NULL, AT_RESP_TIMEOUT_GENERAL);
    if (at_ret != 0)
    {
        //return __LINE__;
    }
#endif
    return 0;
}

int32_t GSM_deactivate_pdp()
{
    int32_t ret;
    int32_t at_ret;
    uint32_t checksum = 0;
    uint32_t status = 0;
    uint32_t at_resp_len;
    at_printl("%s", __FUNCTION__);

    if (0 != GSM_AT_sync(0))
    {
        return __LINE__;
    }

    // Deactivate GPRS PDP context.
    at_ret = AT_cmd_send(AT_commands[AT_QIDEACT], Ql_strlen(AT_commands[AT_QIDEACT]),AT_resp_parser_QIDEACT, NULL, AT_RESP_TIMEOUT_40S);
    if (at_ret != 0)
    {
        return __LINE__;
    }
    at_printl("Deactive ok");
    return 0;
}


int32_t GSM_GPRS_prepare(uint8_t* po_imei, uint8_t* po_ccid, uint8_t* po_imsi)
{
    int32_t ret;
    int32_t at_ret;
    uint32_t checksum = 0;
    uint32_t ca_cert_len = 0;
    uint32_t at_resp_len;
    int32_t i;
    uint8_t* p_ret_data = at_ret_data;

    if (GSM_AT_sync(0))
    {
        return __LINE__;
    }
    at_printl("%s", __FUNCTION__);

    // imei
    at_ret = AT_cmd_send(AT_commands[AT_GSN], Ql_strlen(AT_commands[AT_GSN]), AT_resp_parser_GSN, p_ret_data, AT_RESP_TIMEOUT_GENERAL);
    if (at_ret != 0)
    {
        return __LINE__;
    }
    at_printl("imei=%s", p_ret_data);
    if (po_imei)
        Ql_strncpy(po_imei, p_ret_data, strlen(p_ret_data));

    // 查询sim卡状态
    at_ret = AT_cmd_send(AT_commands[AT_CPIN],  Ql_strlen(AT_commands[AT_CPIN]),AT_resp_parser_CPIN, NULL, AT_RESP_TIMEOUT_5S);
    if (at_ret != 0)
    {
        return __LINE__;
    }

    // CCID
    at_ret = AT_cmd_send(AT_commands[AT_QCCID], Ql_strlen(AT_commands[AT_QCCID]), AT_resp_parser_QCCID, p_ret_data, AT_RESP_TIMEOUT_GENERAL);
    if (at_ret != 0)
    {
        return __LINE__;
    }
    at_printl("ccid=%s", p_ret_data);
    if (po_ccid)
        Ql_strncpy(po_ccid, p_ret_data, strlen(p_ret_data));

    // IMSI
    at_ret = AT_cmd_send(AT_commands[AT_CIMI], Ql_strlen(AT_commands[AT_CIMI]), AT_resp_parser_CIMI, p_ret_data, AT_RESP_TIMEOUT_GENERAL);
    if (at_ret != 0)
    {
        return __LINE__;
    }
    at_printl("imsi=%s", p_ret_data);
    if (po_imsi)
        Ql_strncpy(po_imsi, p_ret_data, strlen(p_ret_data));

    return 0;
}


