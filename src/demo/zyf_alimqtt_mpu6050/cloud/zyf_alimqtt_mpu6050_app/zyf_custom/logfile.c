#include "string.h"
#include "ql_type.h"
#include "ql_trace.h"
#include "ql_uart.h"
#include "ql_fs.h"
#include "ql_error.h"
#include "ql_system.h"
#include "ql_stdlib.h"
#include "sys.h"
#include "uart.h"
#include "logfile.h"

void Logfilebufinit(void)
{
	s32 ret;
    s32 handle = -1;
    u8 filePath[100] = {0};
    ret = Ql_FS_CheckDir(LOGROOT);
    if (ret != QL_RET_OK)
    {
        ret  = Ql_FS_CreateDir(LOGROOT);
    }
    Ql_sprintf(filePath, "%s\\%s\0", LOGROOT, LOGFILE);
    handle = Ql_FS_Open(filePath, QL_FS_READ_WRITE);
    if (handle < 0)
    {
        handle = Ql_FS_Open(filePath, QL_FS_CREATE_ALWAYS);
        if (handle > 0)
        {
            Ql_FS_Close(handle);
        }
    }
    else
    {
        Ql_FS_Close(handle);
    }
}



void SaveLogbuf(u8 *buf,u16 len)
{
    s32 handle = -1;
    s32 offset;
    u8 filePath[100] = {0};
    Ql_sprintf(filePath, "%s\\%s\0", LOGROOT, LOGFILE);
	offset=Ql_FS_GetSize(filePath);
    handle = Ql_FS_Open(filePath, QL_FS_READ_WRITE);
	if((offset+len)>MAX_LOGFILESIZE)
		{
			Ql_FS_Truncate(handle);
			offset=0;
		}
	Ql_FS_Seek(handle,offset,QL_FS_FILE_BEGIN);
    Ql_FS_Write(handle,buf,len, NULL);
	Ql_FS_Flush(handle);
    Ql_FS_Close(handle);
}


void ClearLogbuf(void)
{
	s32 handle = -1;
    u8 filePath[100] = {0};
    Ql_sprintf(filePath, "%s\\%s\0", LOGROOT, LOGFILE);
    handle = Ql_FS_Open(filePath, QL_FS_READ_WRITE);
	Ql_FS_Truncate(handle);
    Ql_FS_Close(handle);
}


void LoadLogbuf(void)
{
	u8 *rxbuf;
	s32 handle = -1;
	s32 offset;
    u8 filePath[100] = {0};
    Ql_sprintf(filePath, "%s\\%s\0", LOGROOT, LOGFILE);
	offset=Ql_FS_GetSize(filePath);
	if(offset>0)
		{
			rxbuf=Ql_MEM_Alloc(offset);
		    handle = Ql_FS_Open(filePath, QL_FS_READ_WRITE);
			Ql_FS_Read(handle, rxbuf, offset, NULL);
		    Ql_FS_Close(handle);
			Ql_UART_Write(UART_PORT1, (u8*)(rxbuf), offset);
			Ql_MEM_Free(rxbuf);
		}
}













