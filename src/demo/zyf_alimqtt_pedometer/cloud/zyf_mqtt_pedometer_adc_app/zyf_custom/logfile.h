#ifndef __LOGFILE_H__
#define __LOGFILE_H__

#include "ql_type.h"
#include "ql_trace.h"
#include "ql_system.h"
#include "ql_stdlib.h"
#include "ql_error.h"
#include "ql_uart.h"


#define  LOGROOT    	((u8 *)"log")  
#define  LOGFILE 		"log.txt"

#define  LOGFILEPATH   "log\\log.txt\0"

#define  MAX_LOGFILESIZE	 	1024*50

void Logfilebufinit(void);
void SaveLogbuf(u8 *buf,u16 len);
void ClearLogbuf(void);
void LoadLogbuf(void);

#define DEBUGLOGFILE_ENABLE 1
#if DEBUGLOGFILE_ENABLE > 0
static char LOGFILE_BUFFER[1024];
#define printfErr2F(FORMAT,...) {\
    Ql_memset(LOGFILE_BUFFER, 0, 1024);\
    Ql_sprintf(LOGFILE_BUFFER,FORMAT,##__VA_ARGS__); \
    SaveLogbuf(LOGFILE_BUFFER, Ql_strlen((const char *)(LOGFILE_BUFFER)));\
}

#else
#define printfErr2F(FORMAT,...)
#endif



#endif



