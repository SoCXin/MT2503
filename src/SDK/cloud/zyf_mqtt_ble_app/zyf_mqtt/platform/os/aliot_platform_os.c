
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include "ql_system.h"
#include "ql_uart.h"
#include "aliot_platform.h"
#include "aliot_platform_os.h"
#include "sys.h"


#ifdef DEBUG
    #define PLATFORM_OS_PERROR(format, ...) aliot_platform_printf(NRF_LOG_COLOR_YELLOW "[OS]%u %s " format "\n", __LINE__, __FUNCTION__, ##__VA_ARGS__)
    #define PLATFORM_OS_PDEBUG(format, ...) aliot_platform_printf(NRF_LOG_COLOR_YELLOW "[OS]%s " format "\n", __FUNCTION__, ##__VA_ARGS__)
#else
    #define PLATFORM_OS_PERROR(format, ...)
    #define PLATFORM_OS_PDEBUG(format, ...)
#endif
/*
创建互斥锁，如果是无OS的平台，无需实现该接口
销毁互斥锁，如果是无OS的平台，无需实现该接口
锁住指定互斥锁，如果是无OS的平台，无需实现该接口
释放指定互斥锁，如果是无OS的平台，无需实现该接口
*/
void* aliot_platform_mutex_create(void)
{
    return NULL;
}
void aliot_platform_mutex_destroy(_IN_ void* mutex)
{
    return;
}
void aliot_platform_mutex_lock(_IN_ void* mutex)
{
    return;
}
void aliot_platform_mutex_unlock(_IN_ void* mutex)
{
    return;
}
/*
分配内存块
释放内存块
已改为静态分配内存，无需实现该接口
*/
void* aliot_platform_malloc(_IN_ uint32_t size)
{
    void* ptr = Ql_MEM_Alloc(size);
    PLATFORM_OS_PDEBUG("%#x %d bytes, used=%d.", ptr, size, lite_mem_used());
    return ptr;
}
void aliot_platform_free(_IN_ void* ptr)
{
    PLATFORM_OS_PDEBUG("%#X, used=%d.", ptr, lite_mem_used());
    Ql_MEM_Free(ptr);
    return;
}
/*
启动OTA，由于暂不支持OTA功能，该接口暂无需实现
写OTA固件，由于暂不支持OTA功能，该接口暂无需实现
完成OTA，由于暂不支持OTA功能，该接口暂无需实现
*/
int aliot_platform_ota_start(const char* md5, uint32_t file_size)
{
    return -1;
}
int aliot_platform_ota_write(_IN_ char* buffer, _IN_ uint32_t length)
{
    return -1;
}
int aliot_platform_ota_finalize(_IN_ int stat)
{
    return -1;
}
/* 获取系统时间（单位：ms），允许溢出 */
uint32_t aliot_platform_time_get_ms(void)
{
    // TODO
    return systemset.SysTime*1000;
}
/*
睡眠指定时间，如果是无OS的平台，将函数实现为延时指定时间即可
*/
void aliot_platform_msleep(_IN_ uint32_t ms)
{
    Ql_Sleep(ms);
}

/*
hex形式打印 u8 pdata[i] 的前n个元素
*/
void aliot_platform_printhex(const int8_t* title, uint8_t* pdata, uint32_t count)
{
    uint32_t i;
    if (count == 0)
        return;
    if (title != NULL)
        aliot_platform_printf("%s: ", title);
    for (i = 0; i < count; i++)
        aliot_platform_printf("%02X ", *(pdata + i));
    aliot_platform_printf("(%dB)\r\n", count);
}
char* aliot_platform_module_get_pid(char pid_str[])
{
    return NULL;
}
