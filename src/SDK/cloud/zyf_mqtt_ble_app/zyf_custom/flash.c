#include "string.h"
#include "ql_type.h"
#include "ql_trace.h"
#include "ql_uart.h"
#include "ql_fs.h"
#include "ql_error.h"
#include "ql_system.h"
#include "ql_stdlib.h"
#include "sys.h"
#include "flash.h"
#include "uart.h"


void ReadFlashParams(_system_setings* sysset)
{
    u8 readenLen;
    u8 file_content[100];
    static s32 handle = -1;
    handle = Ql_FS_Open(FILEPATH, QL_FS_READ_WRITE);
    if (handle < 0)
    {
        sysset->saveflag = 0;
    }
    else
    {
        Ql_memset(file_content, 0, sizeof(file_content));
        Ql_FS_Seek(handle, 0, QL_FS_FILE_BEGIN);
        Ql_FS_Read(handle, file_content, 100 - 1, &readenLen);
        Ql_FS_Close(handle);
    }
}


void SaveFlashParams(_system_setings* sysset)
{
    static s32 handle = -1;

}

//////////////////////////////////////////////////////////


void ReadFlashParamsNew(_system_setings* sysset)
{
    s32 ret;
    s32 handle = -1;
    u8 filePath[100] = {0};
    ret = Ql_FS_CheckDir(MYPATH_ROOT);
    if (ret != QL_RET_OK)
    {
        ret  = Ql_FS_CreateDir(MYPATH_ROOT);
    }
    Ql_sprintf(filePath, "%s\\%s\0", MYPATH_ROOT, SYSFILE);
    handle = Ql_FS_Open(filePath, QL_FS_READ_WRITE);
    if (handle < 0)
    {

        handle = Ql_FS_Open(filePath, QL_FS_CREATE_ALWAYS);
        if (handle > 0)
        {
            LoadDefualtCfg();
            Ql_FS_Write(handle, sysset, sizeof(_system_setings), NULL);
            Ql_FS_Close(handle);
        }
    }
    else
    {
        Ql_FS_Read(handle, sysset, sizeof(_system_setings), NULL);
        Ql_FS_Close(handle);
    }
}

void SaveFlashParamsNew(_system_setings* sysset)
{
    s32 handle = -1;
    s32 ret;
    u8 filePath[100] = {0};
    Ql_sprintf(filePath, "%s\\%s\0", MYPATH_ROOT, SYSFILE);
    handle = Ql_FS_Open(filePath, QL_FS_READ_WRITE);
    Ql_FS_Write(handle, sysset, sizeof(_system_setings), NULL);
    Ql_FS_Close(handle);
}


void WFlash()
{
   
}

