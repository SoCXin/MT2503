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
#include "ril_audio.h"
#include "ril_util.h"
#include "hwhal.h"

#define DEBUG_FTP_APP

#ifdef DEBUG_FTP_APP

    #define ftp_debug(format, ...) mprintf( format "\r\n", ##__VA_ARGS__)
    #define ftp_info(format, ...) mprintf( format "\r\n", ##__VA_ARGS__)
    #define ftp_warn(format, ...) mprintf( format "\r\n", ##__VA_ARGS__)
    #define ftp_error(format, ...) mprintf( "[error]%s() %d " format "\r\n", /*__FILE__,*/ __FUNCTION__, __LINE__, ##__VA_ARGS__)
    #define ftp_msg(format, ...) mprintf( format "\r\n", ##__VA_ARGS__)
#else

    #define ftp_debug(format, ...)
    #define ftp_info(format, ...) 
    #define ftp_warn(format, ...) 
    #define ftp_error(format, ...) 
    #define ftp_msg(format, ...) 
#endif

ftp_config audio_ftp_config;
u8 AUDIO_file_path[29];        //设定一个全局变量存放语音文件的路径

static void FTP_Callback_Down(s32 result, s32 size)
{
    s32 ret;
    if (result)
    {
    	audio_ftp_config.init=1;
        ftp_debug("<-- Succeed in uploading/downloading image bin via FTP, file size:%d -->\r\n", size);
		
    }else{

		audio_ftp_config.init=-1;
        ftp_error("<-- Failed to upload/download file to FTP server, cause=%d -->\r\n", result);
		
    }
    ret = RIL_FTP_QFTPCLOSE();
    ftp_debug("<-- FTP close connection, ret=%d -->\r\n", ret);

    ftp_debug("<-- Deactivating PDP context -->\r\n");
    ret = RIL_FTP_QIDEACT();
    ftp_debug("<-- Released PDP context, ret=%d -->\r\n", ret);
}


void ftp_config_init(void)
{
	audio_ftp_config.init=0;
	audio_ftp_config.FTP_CONNECT_ATTEMPTS=5;
	Ql_sprintf(audio_ftp_config.APN,"cmnet");
	Ql_sprintf(audio_ftp_config.USERID,"");
	Ql_sprintf(audio_ftp_config.PASSWD,"");
	Ql_sprintf(audio_ftp_config.FTP_SVR_ADDR,"211.159.175.43");
	audio_ftp_config.FTP_SVR_PORT=21;
	Ql_sprintf(audio_ftp_config.FTP_USER_NAME,"hello");
	Ql_sprintf(audio_ftp_config.FTP_PASSWORD,"123456");
	Ql_sprintf(audio_ftp_config.FTP_SVR_PATH,"/mp3/");
	Ql_sprintf(audio_ftp_config.FTP_FILENAME,"zccg.mp3");
}



s32 ftp_downloadfile(void)
{
	 s32 ret = 0;
	 u8  attempts = 0;
	 my_tcp_state=STATE_NW_GET_SIMSTATE;
	 GSM_InitModule();
	 while (my_tcp_state != STATE_GPRS_REGISTER)
		{
		   ftp_debug("Search the net....");
		   GSM_InitModule();
		   Ql_Sleep(200);
		}
	 ret = RIL_NW_SetGPRSContext(Ql_GPRS_GetPDPContextId());
	 ftp_debug("<-- Set GPRS PDP context, ret=%d -->\r\n", ret);
	 ret = RIL_NW_SetAPN(1, audio_ftp_config.APN, audio_ftp_config.USERID, audio_ftp_config.PASSWD);
     ftp_debug("<-- Set GPRS APN, ret=%d -->\r\n", ret); 

	 do
    {
        ret = RIL_FTP_QFTPOPEN(audio_ftp_config.FTP_SVR_ADDR, audio_ftp_config.FTP_SVR_PORT,audio_ftp_config.FTP_USER_NAME,audio_ftp_config.FTP_PASSWORD, 1);
        ftp_debug("<-- FTP open connection, ret=%d -->\r\n", ret);
        if (RIL_AT_SUCCESS == ret)
        {
            attempts = 0;
            ftp_debug("<-- Open ftp connection -->\r\n");
            break;
        }
        attempts++;
        ftp_debug("<-- Retry to open FTP 2s later -->\r\n");
        Ql_Sleep(2000);
    } while (attempts < audio_ftp_config.FTP_CONNECT_ATTEMPTS);

	if (audio_ftp_config.FTP_CONNECT_ATTEMPTS == attempts)
		{
			 ftp_error("<-- Fail to open ftp connection for 5 times -->\r\n");
			 return -99;
		}
	 ret = RIL_FTP_QFTPCFG(4, AUDIOFILEPATH);
	 ftp_debug("<-- Set local storage, ret=%d -->\r\n", ret);
	 ret = RIL_FTP_QFTPPATH(audio_ftp_config.FTP_SVR_PATH);   
     ftp_debug("<-- Set remote path, ret=%d -->\r\n", ret);

	  ret = RIL_FTP_QFTPGET((u8 *)audio_ftp_config.FTP_FILENAME, 0, FTP_Callback_Down);
    if (ret < 0)
    {
        ftp_debug("<-- Failed to download, cause=%d -->\r\n", ret);
        
        ret = RIL_FTP_QFTPCLOSE();
        ftp_debug("<-- FTP close connection, ret=%d -->\r\n", ret);

        ret = RIL_FTP_QIDEACT();
        ftp_debug("<-- Release PDP context, ret=%d -->\r\n", ret);
        return -3;
    }
    ftp_debug("<-- Downloading file, please wait... -->\r\n");
    return 0;
	 
}

static void Callback_FtpAudPlay(s32 errCode)
{
    ftp_debug("<-- Audio playing indication: errCode=%d-->\r\n", errCode);
    if (AUD_PLAY_IND_OK == errCode)
    {
        ftp_debug("<-- Playing finished normally -->\r\n");
    }
    else if (AUD_PLAY_IND_INTERRUPT == errCode)
    {
        ftp_error("<-- Playing is interrupted -->\r\n");
    }
}

/*
	广告文件播放
	广告文件路径存放在ram 的路径中
	
*/

void Audio_file_App (u8 volume)
{
	//SPK_AMP_EN_ON;            //使能
	RIL_AUD_SetVolume(VOL_TYPE_MEDIA,volume);             //音量降低一半
	RIL_AUD_SetChannel(AUD_CHANNEL_LOUD_SPEAKER);    //设置通道
	Ql_Sleep(1000);
    RIL_AUD_RegisterPlayCB(Callback_FtpAudPlay);
	RIL_AUD_PlayFile(AUDIO_file_path,0);    //播放文件路径带文件名

}

s32 Audio_Ftp_App(void)
{
	s32 ret;
	u8 binfilePath[29];
	ftp_config_init();           //初始化下载广告文件的参数
	ret=ftp_downloadfile();
	if(ret==0)
		{
			while(audio_ftp_config.init!=1&&audio_ftp_config.init!=-1)
				{
					 ftp_debug("<-- Downloading file, please wait... -->\r\n");
					 Ql_Sleep(1000);
				}
			 Ql_Sleep(2000);
			  if (!Ql_strncmp(AUDIOFILEPATH,"UFS", 3))//UFS
				    {
				        Ql_sprintf((char*)AUDIO_file_path,"%s",(char*)audio_ftp_config.FTP_FILENAME);//文件存放在RAM下
						Ql_sprintf((char*)binfilePath,"%s",(char*)audio_ftp_config.FTP_FILENAME);
				    }
				    else
				    {
				         Ql_sprintf((char*)AUDIO_file_path,"%s:%s",AUDIOFILEPATH,(char*)audio_ftp_config.FTP_FILENAME);//文件存放在RAM下
						 Ql_sprintf((char*)binfilePath,"%s:%s",AUDIOFILEPATH,(char*)audio_ftp_config.FTP_FILENAME);
				    }
			 ret=Ql_FS_Check(binfilePath);
			 ftp_debug("<-- Ql_FS_Check ret=%d -->\r\n",ret);
			if(audio_ftp_config.init==1)             //下载成功播放一次
				{
					  ftp_debug("<-- Downloading file OK, SPK_AMP_EN_ON... -->\r\n");
					  //SPK_AMP_EN_ON;
					  RIL_AUD_SetChannel(AUD_CHANNEL_LOUD_SPEAKER);
					  Ql_Sleep(1000);
					  RIL_AUD_RegisterPlayCB(Callback_FtpAudPlay);
            		  ret = RIL_AUD_PlayFile(binfilePath,0);
					  ftp_debug("<-- RIL_AUD_PlayFile name=%s -->\r\n",audio_ftp_config.FTP_FILENAME);
				}
			else
				{
					return -99;
				}
		}
	return ret;
}




