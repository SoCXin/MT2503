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


u8 BootInfo=0;

void ReadFlashParamsNew(_system_setings *sysset)
{
	 s32 ret;
	 s32 handle = -1;
	 u8 filePath[100] = {0};
	 ret = Ql_FS_CheckDir(MYPATH_ROOT);
	 if(ret != QL_RET_OK)
	   	{
	   		ret  = Ql_FS_CreateDir(MYPATH_ROOT);
	   	}
	 Ql_sprintf(filePath,"%s\\%s\0",MYPATH_ROOT,SYSFILE);
	 handle = Ql_FS_Open(filePath,QL_FS_READ_WRITE);
	 if(handle<0)
	 	{
	 		
	 		handle=Ql_FS_Open(filePath,QL_FS_CREATE_ALWAYS);
			if(handle>0)
				{
					LoadDefualtCfg();
					Ql_FS_Write(handle, sysset, sizeof(_system_setings),NULL);
					Ql_FS_Close(handle);
					BootInfo=0;
				}
	 	}
	 else
	 	{
	 		 Ql_FS_Read(handle, sysset, sizeof(_system_setings),NULL);
			 Ql_FS_Close(handle); 
			 BootInfo=1;
	 	}
}

void SaveFlashParamsNew(_system_setings *sysset)
{
	 s32 handle = -1;
     s32 ret;
     u8 filePath[100] = {0};
	Ql_sprintf(filePath,"%s\\%s\0",MYPATH_ROOT,SYSFILE);
	handle = Ql_FS_Open(filePath,QL_FS_READ_WRITE );
	Ql_FS_Write(handle, sysset, sizeof(_system_setings),NULL);
	Ql_FS_Close(handle); 
}


void CreatNewFlashFile(void)
{
	s32 handle = -1;
    s32 ret;
    u8 filePath[100] = {0};
	ret = Ql_FS_CheckDir(MYPATH_ROOT);
	 if(ret != QL_RET_OK)
	   	{
	   		ret  = Ql_FS_CreateDir(MYPATH_ROOT);
	   	}
	 Ql_sprintf(filePath,"%s\\%s\0",MYPATH_ROOT,DATAFILE);
	 handle = Ql_FS_Open(filePath,QL_FS_READ_WRITE);
	 if(handle<0)
	 	{
	 		handle=Ql_FS_Open(filePath,QL_FS_CREATE_ALWAYS);
			Ql_FS_Close(handle);
			Ql_memset((Coredata*)&mycoredata,0,sizeof(Coredata));
	 	}
	 else
	 	{
	 		Ql_FS_Close(handle);
	 	}
	 Ql_sprintf(filePath,"%s\\%s\0",MYPATH_ROOT,DATACONFIG);
	 handle = Ql_FS_Open(filePath,QL_FS_READ_WRITE);
	 if(handle<0)
	 	{
	 		handle=Ql_FS_Open(filePath,QL_FS_CREATE_ALWAYS);
			Ql_FS_Close(handle);
			Position.saveposion=0;
			Position.sendpostion=0;
			SaveDataConfig2Flash(&Position);
	 	}
	 else
	 	{
	 		Ql_FS_Read(handle, &Position, sizeof(DataConfig),NULL);
	 		Ql_FS_Close(handle);
	 	}
}

void SaveDataConfig2Flash(DataConfig *dataconfig)
{
	s32 handle = -1;
    s32 ret;
    u8 filePath[100] = {0};
	Ql_sprintf(filePath,"%s\\%s\0",MYPATH_ROOT,DATACONFIG);
	handle = Ql_FS_Open(filePath,QL_FS_READ_WRITE );
	Ql_FS_Write(handle,dataconfig,sizeof(DataConfig),NULL);
	Ql_FS_Close(handle); 
}

void SaveData2Flash(Coredata *coredata)
{
	s32 handle = -1;
    s32 ret;
    u8 filePath[100] = {0};
	Ql_sprintf(filePath,"%s\\%s\0",MYPATH_ROOT,DATAFILE);
	handle = Ql_FS_Open(filePath,QL_FS_READ_WRITE );
	if(handle>=0)
		{
			Position.saveposion++;
			if(Position.saveposion>MAXDATANUM)
				{
					Position.saveposion=1;
					Ql_FS_Seek(handle,0,QL_FS_FILE_BEGIN);
				}
			else
				{
					Ql_FS_Seek(handle,(Position.saveposion-1)*sizeof(Coredata),QL_FS_FILE_BEGIN);
				}
			Ql_FS_Write(handle, coredata, sizeof(Coredata), NULL);
			Ql_FS_Flush(handle);
			Ql_FS_Close(handle);
			SaveDataConfig2Flash(&Position);
		}
}


void LoadDataFromFlash(Coredata *coredata,u8 Posion)
{
	s32 handle = -1;
    s32 ret;
    u8 filePath[100] = {0};
	Ql_sprintf(filePath,"%s\\%s\0",MYPATH_ROOT,DATAFILE);
	handle = Ql_FS_Open(filePath,QL_FS_READ_WRITE );
	if(handle>=0)
		{
			Ql_FS_Seek(handle,Posion*sizeof(Coredata),QL_FS_FILE_BEGIN);
			Ql_FS_Read(handle, coredata, sizeof(Coredata),NULL);
			Ql_FS_Close(handle);
		}
}






