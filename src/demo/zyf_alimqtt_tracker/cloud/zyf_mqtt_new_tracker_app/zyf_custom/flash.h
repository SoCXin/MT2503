#ifndef __FLASH_H__
#define __FLASH_H__


#define  MYPATH_ROOT    ((u8 *)"myroot")  
#define  SYSFILE 		"sys3_config.txt"
#define  SAVEDATAFILE   "data_config.txt"

#define FILEPATH   "myroot\\sys3_config.txt\0"
#define FILEPATH   "myroot\\data_config.txt\0"

extern u32 s_iflashMutex;

void ReadFlashParamsNew(_system_setings *sysset);
void SaveFlashParamsNew(_system_setings *sysset);
void ReadFlashSaveData(_save_data_point* savedata);
void SaveFlashSaveData(_save_data_point* savedata);

#endif
