#ifndef __FLASH_H__
#define __FLASH_H__


#define  MYPATH_ROOT    ((u8 *)"myfile")  
#define  SYSFILE 		"sys6_config.txt"
#define  DATAFILE 		"sys5_config.txt"
#define  DATACONFIG 	"dconfig.txt"

#define FILEPATH   		"myfile\\sys6_config.txt\0"
#define DATAFILEPATH    "myfile\\sys5_config.txt\0"
#define DATACONFIGPATH    "myfile\\dconfig.txt\0"


#define MAXDATANUM 30	




extern u8 BootInfo;

void ReadFlashParamsNew(_system_setings *sysset);
void SaveFlashParamsNew(_system_setings *sysset);
void CreatNewFlashFile(void);
#endif
