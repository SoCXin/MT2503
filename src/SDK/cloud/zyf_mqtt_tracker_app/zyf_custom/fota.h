#ifndef __FOAT_H__
#define __FOAT_H__
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif                          /* __cplusplus */

#include "ql_type.h"
#include "ql_stdlib.h"



#define FOTAAPN      "cmnet"
#define FOTAUSERID   ""
#define FOTAPASSWD   ""
#define FTP_SVR_PORT    "21"



typedef struct
{
    u8 FTP_SVR_ADDR[20];
	u8 FTP_SVR_PATH[20];
	u8 FTP_USER_NAME[20];
	u8 FTP_PASSWORD[20];
	u8 FTP_FILENAME[20];
} FOTAINFO;

extern FOTAINFO fotainfo;

extern u32 FotaUpdate_Start_TmrId;
extern s32 FotaUpdate_TimeOut_TmrId;
void foto_init_param(char *addr,char *name,char *password,char *PATH,char *filename);
s32 foto_app_param(char *addr,char *name,char *password,char *PATH,char *filename);

void ftp_downfile_timer(u32 timerId, void* param);




#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif 

