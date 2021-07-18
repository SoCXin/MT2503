#include "ql_type.h"
#include "ql_trace.h"
#include "ql_uart.h"
#include "ql_system.h"
#include "ql_stdlib.h"
#include "ql_error.h"
#include "ql_gpio.h"
#include "sys.h"
#include "ql_socket.h"
#include "tiem.h"
#include "uart.h"
#include "pro.h"
#include "new_socket.h"
#include "lcd.h"
#include "ql_adc.h"
#include "flash.h"
#include "PRD.h"
#include "gps.h"
#include "ql_pwm.h"
#include "ql_memory.h"



/*!
 * @brief 上行数据打包,根据配置文件
 * \n
 *
 * @param Coredata  采集到的数据,格式化方式写入V值
 * \n
 * @see
 */


void Ch2Protocol(Coredata data)
{

	Ql_sprintf(AutomaticRDTlv[0].value,"%d",data.vol);
	Ql_sprintf(AutomaticRDTlv[1].value,"%d.%06d",data.gpsw/1000000,data.gpsw%1000000);
	Ql_sprintf(AutomaticRDTlv[2].value,"%d.%06d",data.gpsj/1000000,data.gpsj%1000000);
	Ql_sprintf(AutomaticRDTlv[3].value,"%d.%06d",data.lbsw/1000000,data.lbsw%1000000);
	Ql_sprintf(AutomaticRDTlv[4].value,"%d.%06d",data.lbsj/1000000,data.lbsj%1000000);
	Ql_sprintf(AutomaticRDTlv[5].value,"%04d",data.vbai);
	Ql_sprintf(AutomaticRDTlv[6].value,"%d",data.dbm);
	Ql_sprintf(AutomaticRDTlv[7].value,"%d",systemset.SysTime);

}



void fota_init(void)
{
	Ql_sprintf(fotainfo.FTP_SVR_ADDR,"211.159.175.43");
	Ql_sprintf(fotainfo.FTP_USER_NAME,"hello");
	Ql_sprintf(fotainfo.FTP_PASSWORD,"123456");
	Ql_sprintf(fotainfo.FTP_SVR_PATH,"/jt/");
	Ql_sprintf(fotainfo.FTP_FILENAME,"jt_266.bin");
}

/////////////////////////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief 下行数据处理,根据配置文件
 * \n
 *
 * @param index 下行指令集索引
 *		  *dowhat 处理完成数据后需要传递的消息
 * \n
 * @see
 */
 
u8 SendMsg2HalDoHd(u8 index,u8 *dowhat)
{
	u8 *p=NULL;
	u8 temstr[20]={0};
	*dowhat=0;
	if(ProNumberInstruct.tlvs[index].type==AutomaticWRTlv[0].type)
		{
			if(ProNumberInstruct.tlvs[index].value[0]==0x31)
				{
					 reverse_motor();
					 Ql_Sleep(2000);
					 sleep_motor();
					 
				}
			if(ProNumberInstruct.tlvs[index].value[0]==0x30)
				{
					 forward_motor();
					 Ql_Sleep(2000);
					 sleep_motor();
				}
		}
	else if(ProNumberInstruct.tlvs[index].type==AutomaticWRTlv[1].type)
		{
			mprintf("\r\n+++++++++++File upgrade+++++++++++\r\n");
			fota_init();
			systemset.fotaflag=1;
			Ql_sprintf(systemset.fotaaddr,"ftp://%s%s%s:%s@%s:%s",fotainfo.FTP_SVR_ADDR, fotainfo.FTP_SVR_PATH, fotainfo.FTP_FILENAME, FTP_SVR_PORT, fotainfo.FTP_USER_NAME, fotainfo.FTP_PASSWORD);
			SaveFlashParamsNew(&systemset);
		}
	else if(ProNumberInstruct.tlvs[index].type==AutomaticWRTlv[2].type)
		{
			Ql_sprintf(systemset.SN,"%s",ProNumberInstruct.tlvs[index].value);
			SaveFlashParamsNew(&systemset);
			InternetsetSN_AnalysisFor808(systemset.SN);
			mprintf("+SN %s\r\n",(u8*)systemset.SN);
			IsSnHavaSet();
		}
	return 0;
}



