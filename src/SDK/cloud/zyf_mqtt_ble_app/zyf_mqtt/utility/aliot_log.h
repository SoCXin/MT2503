

#ifndef _ALIOT_COMMON_LOG_H_
#define _ALIOT_COMMON_LOG_H_

#include <stdio.h>
#include <stdlib.h>
#include "aliot_platform.h"

typedef int aliot_log_level_t;

#define ALIOT_LOG_LEVEL_DEBUG 0
#define ALIOT_LOG_LEVEL_INFO 1
#define ALIOT_LOG_LEVEL_WARN 2
#define ALIOT_LOG_LEVEL_ERROR 3
#define ALIOT_LOG_LEVEL_FATAL 4
#define ALIOT_LOG_LEVEL_NONE 5

extern aliot_log_level_t g_iotLogLevel;

void aliot_log_set_level(aliot_log_level_t iotLogLevel);

#ifndef ALIOT_LOG_LEVEL
#if defined(_WIN32)
    #define ALIOT_LOG_LEVEL ALIOT_LOG_LEVEL_INFO
#else
    #define ALIOT_LOG_LEVEL ALIOT_LOG_LEVEL_INFO
#endif
#endif

#if defined(_WIN32)
    #include <windows.h>
    #define NRF_LOG_COLOR_DEFAULT
    #define NRF_LOG_COLOR_BLACK
    #define NRF_LOG_COLOR_RED
    #define NRF_LOG_COLOR_GREEN
    #define NRF_LOG_COLOR_YELLOW
    #define NRF_LOG_COLOR_BLUE
    #define NRF_LOG_COLOR_MAGENTA
    #define NRF_LOG_COLOR_CYAN
    #define NRF_LOG_COLOR_WHITE
    #define SETCONSOLETEXTATTRIBUTE(handle, color) SetConsoleTextAttribute((handle), color);
#elif defined(NRF51)
    #include "nrf_log.h"
    #define SETCONSOLETEXTATTRIBUTE(handle, color) /*((void*)(0))*/
#else
    #define NRF_LOG_COLOR_DEFAULT "\x1B[0m"
    #define NRF_LOG_COLOR_BLACK "\x1B[1;30m"
    #define NRF_LOG_COLOR_RED "\x1B[1;31m"
    #define NRF_LOG_COLOR_GREEN "\x1B[1;32m"
    #define NRF_LOG_COLOR_YELLOW "\x1B[1;33m"
    #define NRF_LOG_COLOR_BLUE "\x1B[1;34m"
    #define NRF_LOG_COLOR_MAGENTA "\x1B[1;35m"
    #define NRF_LOG_COLOR_CYAN "\x1B[1;36m"
    #define NRF_LOG_COLOR_WHITE "\x1B[1;37m"
    #define SETCONSOLETEXTATTRIBUTE(handle, color) /*((void*)(0))*/
#endif


#if (ALIOT_LOG_LEVEL <= ALIOT_LOG_LEVEL_DEBUG)
#define ALIOT_LOG_DEBUG(format, ...)                                                                                                       \
    do {                                                                                                               \
        SETCONSOLETEXTATTRIBUTE(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);                     \
        aliot_platform_printf(NRF_LOG_COLOR_MAGENTA "[debug]" format "\r\n", ##__VA_ARGS__);                           \
    } while (0);
#else
    #define ALIOT_LOG_DEBUG(format, ...)
#endif

#if (ALIOT_LOG_LEVEL <= ALIOT_LOG_LEVEL_INFO)
#define ALIOT_LOG_INFO(format, ...)                                                                                             \
    do {                                                                                                               \
        SETCONSOLETEXTATTRIBUTE(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);                                             \
        aliot_platform_printf(NRF_LOG_COLOR_GREEN "[info]" format "\r\n", ##__VA_ARGS__);                              \
        SETCONSOLETEXTATTRIBUTE(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);          \
    } while (0);
#else
    #define ALIOT_LOG_INFO(format, ...)
#endif

#if (ALIOT_LOG_LEVEL <= ALIOT_LOG_LEVEL_WARN)
#define ALIOT_LOG_WARN(format, ...)                                                                                                      \
    do {                                                                                                                    \
        SETCONSOLETEXTATTRIBUTE(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);              \
        aliot_platform_printf(NRF_LOG_COLOR_YELLOW "[warn]%s<%d>" format "\r\n", __FUNCTION__, __LINE__, ##__VA_ARGS__);    \
        SETCONSOLETEXTATTRIBUTE(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);                   \
    } while (0);
#else
    #define ALIOT_LOG_WARN(format, ...)
#endif

#if (ALIOT_LOG_LEVEL <= ALIOT_LOG_LEVEL_ERROR)
#define ALIOT_LOG_ERROR(format, ...)                                                                                                  \
    do {                                                                                                               \
        SETCONSOLETEXTATTRIBUTE(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);                              \
        aliot_platform_printf(NRF_LOG_COLOR_RED "[error]%s<%d>" format "\r\n", __FUNCTION__, __LINE__, ##__VA_ARGS__); \
        SETCONSOLETEXTATTRIBUTE(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);                \
    } while (0);
#else
    #define ALIOT_LOG_ERROR(format, ...)
#endif

#if (ALIOT_LOG_LEVEL <= ALIOT_LOG_LEVEL_FATAL)
#define ALIOT_LOG_FATAL(format, ...)                                                                                                   \
    do {                                                                                                                \
        SETCONSOLETEXTATTRIBUTE(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);                               \
        aliot_platform_printf(NRF_LOG_COLOR_BLUE "[fatal]%s<%d>" format "\r\n", __FUNCTION__, __LINE__, ##__VA_ARGS__); \
        SETCONSOLETEXTATTRIBUTE(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);                 \
    } while (0);
#else
    #define ALIOT_LOG_FATAL(format, ...)
#endif

#if (ALIOT_LOG_LEVEL <= ALIOT_LOG_LEVEL_DEBUG)
    #define ALIOT_LOG_HEXDUMP(title, data_ptr, len) aliot_platform_printhex(title, data_ptr, len)
#else
    #define ALIOT_LOG_HEXDUMP(title, data_ptr, len)
#endif

#endif
