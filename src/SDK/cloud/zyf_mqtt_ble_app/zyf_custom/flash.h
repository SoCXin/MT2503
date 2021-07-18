#ifndef __FLASH_H__
#define __FLASH_H__


#define  MYPATH_ROOT    ((u8 *)"myroot")  
#define  SYSFILE 		"sys3_config.txt"

#define FILEPATH   "myroot\\sys3_config.txt\0"


void ReadFlashParamsNew(_system_setings *sysset);
void SaveFlashParamsNew(_system_setings *sysset);
#endif
