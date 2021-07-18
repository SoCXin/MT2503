
#ifndef _ALIOT_COMMON_DEBUG_H_
#define _ALIOT_COMMON_DEBUG_H_

#include "aliot_log.h"

#ifdef ALIOT_DEBUG

#define DEBUG_PUTS(fmt, ...)                         \
    do {                                             \
        aliot_platform_printf(fmt, ##__VA_ARGS__); \
    } while (0)

#define ASSERT_FAILED_DO()                                \
    do {                                                  \
        while (1) {                                       \
            aliot_platform_msleep(1000);                  \
            aliot_platform_printf("assert failed\r\n"); \
        }                                                 \
    } while (0)

#define ALIOT_ASSERT(expr, fmt, ...)                                                     \
    do {                                                                                 \
        if (!(expr)) {                                                                   \
            DEBUG_PUTS("###ASSERT FAILED###, file=%s, line=%d\r\n", __FILE__, __LINE__); \
            if (NULL != fmt) {                                                           \
                DEBUG_PUTS(fmt, ##__VA_ARGS__);                                          \
                aliot_platform_printf("\r\n");                                         \
            }                                                                            \
            ASSERT_FAILED_DO();                                                          \
        }                                                                                \
    } while (0)

#define ALIOT_TRACE(fmt, ...) \
    DEBUG_PUTS(fmt, ##__VA_ARGS__)
#else

#define TRACE(fmt, ...) \
    do {                \
    } while (0)
#define ALIOT_ASSERT(expr, fmt, ...) \
    do {                             \
    } while (0)
#endif

#endif /* _ALIOT_COMMON_DEBUG_H_ */
