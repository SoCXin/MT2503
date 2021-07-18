
#ifndef __FTP_DW_H__
#define __FTP_DW_H__
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif                          /* __cplusplus */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>


#define AUDIOFILEPATH	"UFS"

typedef struct
{
	int8_t init;
    uint8_t APN[10];      
    uint8_t USERID[15]; 
    uint8_t PASSWD[15];   
    uint8_t FTP_SVR_ADDR[18];   
    uint8_t FTP_SVR_PATH[31];
	uint8_t FTP_USER_NAME[31];   
    uint8_t FTP_PASSWORD[31];
	uint8_t FTP_FILENAME[31];
	uint8_t FTP_CONNECT_ATTEMPTS;
	uint32_t FTP_SVR_PORT;
} ftp_config;


extern ftp_config audio_ftp_config;

void Audio_file_App (u8 volume);




#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif 

