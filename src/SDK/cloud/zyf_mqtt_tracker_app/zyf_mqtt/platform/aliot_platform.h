
#ifndef _PLATFORM_ALIOT_PLATFORM_H_
#define _PLATFORM_ALIOT_PLATFORM_H_

// 选择设备通过TCP或TLS与MQTT服务端建立连接
// 1 -- TCP
// 0 -- SSL
#define ALIOT_MQTT_TCP 1

#if defined(_WIN32)
#define ALIOT_AUTH_SKIP	0	// 是否跳过认证(测试用途)
#else
#define ALIOT_AUTH_SKIP	0	// 是否跳过认证(测试用途)
#endif

#define MYDEVICE_NOT_SUPPORT_TIME	1	// 设备是否不支持 time()

#include "aliot_platform_datatype.h"
#include "os/aliot_platform_os.h"
#include "network/aliot_platform_network.h"
#include "ssl/aliot_platform_ssl.h"


#endif /* _PLATFORM_ALIOT_PLATFORM_H_ */
