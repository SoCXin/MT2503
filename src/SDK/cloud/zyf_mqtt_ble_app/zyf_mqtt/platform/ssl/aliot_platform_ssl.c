// GSM/GPRS Module Series
// GSM_SSL_TCP_Application
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "stddef.h"
#include "aliot_platform_os.h"
#include "aliot_platform.h"


#define DEBUG


#ifdef DEBUG
#define SSL_LOG(format, ...)     aliot_platform_printf("[SSL]%s %d " format "\r\n", /*__FILE__,*/ __FUNCTION__, __LINE__, ##__VA_ARGS__)                                                                   
#else
#define SSL_LOG(format, ...)
#endif
int32_t aliot_platform_ssl_read(uintptr_t handle, char* buf, int len, int timeout_ms)
{
	if (handle)
        return GSM_SSL_retrieve(buf, len, timeout_ms);
    return -1;
}
int32_t aliot_platform_ssl_write(uintptr_t handle, const char* buf, int len, int timeout_ms)
{
	 if (handle)
        return GSM_SSL_transmit(buf, len, timeout_ms);
    return -1;
}
int32_t aliot_platform_ssl_destroy(uintptr_t handle)
{
	int32_t retcode = 0;
	 if (handle)
    {
        retcode = GSM_SSL_close();
        if (retcode != 0)
        {
            SSL_LOG("error %d", retcode);
            //return retcode;
        }
    }
    // 未连接 只需要deact pdp
    if (GSM_deactivate_pdp() != 0)
    {
        SSL_LOG("clean fail");
    }
    return 0;
}
uintptr_t aliot_platform_ssl_establish(const char* host,
                                       uint16_t port,
                                       const char* ca_crt,
                                       size_t ca_crt_len)
{
	int32_t retcode = 0;
    retcode = GSM_SSL_upload_CACert(ca_crt, ca_crt_len);
    if (retcode != 0)
    {
        SSL_LOG("error %d", retcode);
        return 0;
    }
    retcode = GSM_SSL_transfer_mode_set();
    if (retcode != 0)
    {
        SSL_LOG("error %d", retcode);
        return 0;
    }
    retcode = GSM_GPRS_activate_pdp();
    if (retcode != 0)
    {
        SSL_LOG("error %d", retcode);
        return 0;
    }
    retcode = GSM_SSL_config();
    if (retcode != 0)
    {
        SSL_LOG("error %d", retcode);
        return 0;
    }
    retcode = GSM_SSL_open(host, port);
    if (retcode != 0)
    {
        SSL_LOG("error %d", retcode);
        return 0;
    }
    return __LINE__;
}
