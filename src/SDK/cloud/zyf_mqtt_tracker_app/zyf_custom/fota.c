#include "custom_feature_def.h"
#include "ql_type.h"
#include "ql_stdlib.h"
#include "ql_trace.h"
#include "ql_timer.h"
#include "ql_uart.h"
#include "ql_error.h"
#include "ril.h"
#include "ril_network.h"
#include "ql_gprs.h"
#include "ql_fs.h"
#include "ftp_download.h"
#include "new_socket.h"
#include "uart.h"
#include "hwhal.h"
#include "fota.h"
#include "sys.h"

u32 FotaUpdate_Start_TmrId =     (TIMER_ID_USER_START + 1);
s32 FotaUpdate_TimeOut_TmrId =   (TIMER_ID_USER_START + 2);

FOTAINFO fotainfo;


void ftp_downfile_timer(u32 timerId, void* param)
{
    s32 strLen;
    ST_GprsConfig apnCfg;
    Ql_memcpy(apnCfg.apnName,   FOTAAPN, Ql_strlen(FOTAAPN));
    Ql_memcpy(apnCfg.apnUserId, FOTAUSERID, Ql_strlen(FOTAUSERID));
    Ql_memcpy(apnCfg.apnPasswd, FOTAPASSWD, Ql_strlen(FOTAPASSWD));

    Ql_UART_Write(UART_PORT1, systemset.fotaaddr, strLen);

	SaveFlashParamsNew(&systemset);
    Ql_FOTA_StartUpgrade(systemset.fotaaddr, &apnCfg, NULL);
}
/*
	函数优化为传参函数
*/

void foto_init_param(char *addr,char *name,char *password,char *PATH,char *filename)
{
	    Ql_sprintf(fotainfo.FTP_SVR_ADDR,addr);
		Ql_sprintf(fotainfo.FTP_USER_NAME,name);
		Ql_sprintf(fotainfo.FTP_PASSWORD,password);
		Ql_sprintf(fotainfo.FTP_SVR_PATH,PATH);
		Ql_sprintf(fotainfo.FTP_FILENAME,filename);
		Ql_sprintf(systemset.fotaaddr,"ftp://%s%s%s:%s@%s:%s",fotainfo.FTP_SVR_ADDR, fotainfo.FTP_SVR_PATH, fotainfo.FTP_FILENAME, FTP_SVR_PORT, fotainfo.FTP_USER_NAME, fotainfo.FTP_PASSWORD);
		SaveFlashParamsNew(&systemset);
}

void fota_init(void)
{
	Ql_sprintf(fotainfo.FTP_SVR_ADDR,"211.159.175.43");
	Ql_sprintf(fotainfo.FTP_USER_NAME,"hello");
	Ql_sprintf(fotainfo.FTP_PASSWORD,"123456");
	Ql_sprintf(fotainfo.FTP_SVR_PATH,"/m203c/");
	Ql_sprintf(fotainfo.FTP_FILENAME,"1.bin");
	Ql_sprintf(systemset.fotaaddr,"ftp://%s%s%s:%s@%s:%s",fotainfo.FTP_SVR_ADDR, fotainfo.FTP_SVR_PATH, fotainfo.FTP_FILENAME, FTP_SVR_PORT, fotainfo.FTP_USER_NAME, fotainfo.FTP_PASSWORD);
	SaveFlashParamsNew(&systemset);
}

/*
函数优化：优化为传参
*/
s32 foto_app_param(char *addr,char *name,char *password,char *PATH,char *filename)
{
	s32 ret = 0;
	 u8  attempts = 0;
	 my_tcp_state=STATE_NW_GET_SIMSTATE;
	 GSM_InitModule();
	foto_init_param(addr,name,password,PATH,filename);
	while (my_tcp_state != STATE_GPRS_REGISTER)
		{
		   mprintf("Search the net....");
		   GSM_InitModule();
		   Ql_Sleep(200);
		}
	 ftp_downfile_timer(FotaUpdate_Start_TmrId, NULL);

}

s32 fota_app(void)
{
	 s32 ret = 0;
	 u8  attempts = 0;
	 my_tcp_state=STATE_NW_GET_SIMSTATE;
	 GSM_InitModule();
	 //fota_init();
	 while (my_tcp_state != STATE_GPRS_REGISTER)
		{
		   mprintf("Search the net....");
		   GSM_InitModule();
		   Ql_Sleep(200);
		}
	  ftp_downfile_timer(FotaUpdate_Start_TmrId, NULL);
	  
}









