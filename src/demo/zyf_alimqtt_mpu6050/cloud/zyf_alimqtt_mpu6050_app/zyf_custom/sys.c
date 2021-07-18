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
#include "new_socket.h"
#include "ql_adc.h"
#include "flash.h"
#include "gps.h"
#include "ril.h"
#include "hwhal.h"
#include "logfile.h"
#include "module_data.h"

u8 flag_sms=0;
u8 mpu6050SportLevel=0;
u8 SPORTLEVEL=3;
_mpu6050data mpu6050data;

_system_setings	systemset;
_save_data_point savedata;
volatile u8 nextfaststart = 0;
volatile int savemode = 0;

volatile u8 YorNupgrading = 0;

volatile u8 Chargsta=CHARG_OFF;


volatile u8 SocketBufRdBit=0;

Enum_PinName LED1=PINNAME_SIM2_CLK;
Enum_PinName LED2=PINNAME_SIM2_DATA;
Enum_PinName LED3=PINNAME_SIM2_RST;



void power_drv_init(void)
{
	 Ql_GPIO_Init(LED1, PINDIRECTION_OUT, 0, PINPULLSEL_PULLUP);
	 Ql_GPIO_Init(LED2, PINDIRECTION_OUT, 0, PINPULLSEL_PULLUP);
	

	 LED1_H;
	 LED2_H;
}


void Delayus(u32 data)
{
	u16 i;
	while(data--)
	{
		for(i=0;i<10;i++){}
	}
}



void  LoadDefualtCfg(void)
{
	systemset.Interval=300;
	systemset.HandInter=60; 
	systemset.SysTime=0;
	systemset.saveflag=0x0A;
	systemset.fotaflag=0;
	systemset.snuser=0;
	systemset.CallMode=0;
	systemset.updateinfo=0;
	Ql_sprintf((char*)systemset.SN,"FFFFFFFFF");
	
}


void ShowSysInfo(void)
{
	GetSysTime(&time);
	mprintf("zyf_alimqtt_mpu6050_app\r\n");
	mprintf("SN:%s\r\n",systemset.SN);
	mprintf("Snuser:%d\r\n",systemset.snuser);
	mprintf("Interval:%d\r\n",systemset.Interval);
	mprintf("HandInter:%d\r\n",systemset.HandInter);
	mprintf("updateinfo:%d\r\n",systemset.updateinfo);
	mprintf("+DataTime: %04d-%02d-%02d %02d:%02d:%02d\r\n",time.year, time.month, time.day, time.hour, time.minute, time.second);
	printfErr2F("+DataTime: %04d-%02d-%02d %02d:%02d:%02d\r\n",time.year, time.month, time.day, time.hour, time.minute, time.second);


}

void FlashTimeInit(void)
{
	GetSysTime(&time);
	
}


void SYS_Parameter_Init(void)
{
    Logfilebufinit();
	ReadFlashParamsNew(&systemset);
    ReadFlashSaveData(&savedata);
	ShowSysInfo();	
}

void Set_FirstData_location(u8 gorlflag,s32 llng,u32 llat)
{
    savedata.prellat = llat;    //上次可定位时的纬度
    savedata.prellng = llng;    //上次可定位时的经度
    savedata.preflag++;
    savedata.gorlflag = gorlflag;
    SaveFlashSaveData(&savedata);
}

void DisUpdateRTC(void)
{
	u8 buf[] = "AT+CTZU=0";
	
	Ql_RIL_SendATCmd(buf, Ql_strlen(buf), NULL, NULL, 0);
}



//字节流转换为十六进制字符串  
void ByteToHexStr(const unsigned char* source, char* dest, int sourceLen)  
{  
    short i;  
    unsigned char highByte, lowByte;  
  
    for (i = 0; i < sourceLen; i++)  
    {  
        highByte = source[i] >> 4;  
        lowByte = source[i] & 0x0f ;  
  
        highByte += 0x30;  
  
        if (highByte > 0x39)  
                dest[i * 2] = highByte + 0x07;  
        else  
                dest[i * 2] = highByte;  
  
        lowByte += 0x30;  
        if (lowByte > 0x39)  
            dest[i * 2 + 1] = lowByte + 0x07;  
        else  
            dest[i * 2 + 1] = lowByte;  
    }  
    return ;  
}  
  
//字节流转换为十六进制字符串的另一种实现方式  
void Hex2Str( const char *sSrc,  char *sDest, int nSrcLen )  
{  
    int  i;  
    char szTmp[3];  
  
    for( i = 0; i < nSrcLen; i++ )  
    {  
        Ql_sprintf( szTmp, "%02X", (unsigned char) sSrc[i] );  
        Ql_memcpy( &sDest[i * 2], szTmp, 2 );  
    }  
    return ;  
}  
  
//十六进制字符串转换为字节流  
void HexStrToByte(const char* source, unsigned char* dest, int sourceLen)  
{  
    short i;  
    unsigned char highByte, lowByte;  
      
    for (i = 0; i < sourceLen; i += 2)  
    {  
        highByte = Ql_toupper(source[i]);  
        lowByte  = Ql_toupper(source[i + 1]);  
  
        if (highByte > 0x39)  
            highByte -= 0x37;  
        else  
            highByte -= 0x30;  
  
        if (lowByte > 0x39)  
            lowByte -= 0x37;  
        else  
            lowByte -= 0x30;  
  
        dest[i / 2] = (highByte << 4) | lowByte;  
    }  
    return ;  
}  

void StrToHex(char *pbDest, char *pbSrc, int nLen)
{
	char h1,h2;
	char s1,s2;
	char tbuf[3];
	int i;

	for (i=0; i<nLen; i++)
	{
		h1 = pbSrc[2*i];
		h2 = pbSrc[2*i+1];
		Ql_sprintf(tbuf,"%c%c",h1,h2);
		pbDest[i] =strtol((const char*)tbuf,NULL,16);
	}
}


void PowerOff(void)
{
}


void CloseUart1(void)
{
	Ql_UART_RX_Disable(UART_PORT1);
	Ql_UART_Close(UART_PORT1);
}

void CloseUart2(void)
{
	Ql_UART_RX_Disable(UART_PORT2);
	Ql_UART_Close(UART_PORT2);
}


void M203C_BSP_init(void)
{
	 power_drv_init();
     #ifdef _USE_MCU_PRO_
	 mcu_drv_init();
     #endif
	 MPU_i2c_init();
	 MPU_Init();
	 UartInit();
}

void JudgeGpsAndGSM(void)
{
    u32 t1,t2;
    s32 ret;
    u8 count = 0;
//                    SendMsg2KernelForBLEStart();      //发送消息给BLE任务,处理BLE数据
RESEND:
    ret = RIL_NW_GetSignalQuality(&t1,&t2);
    if(ret == RIL_AT_SUCCESS)
    {
        if(t1 <= 31 && t1 >= 7)
        {
            if(gpspowersta == 0 && systemset.Interval < 3600 && systemset.Interval > 0)
            {
                ret = GpsOpen();
                if(ret == 0)
                    mprintf("GPS Open successed.\r\n");
                gpspowersta = 1;
            }
            SendMsg2KernelForMqttStart();
            mprintf("csq is %d.\r\n",t1);
        }
        else if(t1 < 7 && t1 >=4)
        {
            savemode = 1;
            SendMsg2KernelForGetLocation(&savemode);//信号质量极差 不要定位直接取上一次的数据点保存
            mprintf("csq is %d.\r\n",t1);
        }
        else if(t1 == 99) //信号质量差
        {
            if(count++ > 50) //25s
            {
                savemode = -1;
                SendMsg2KernelForGetLocation(&savemode);//信号质量极差 不要定位直接取上一次的数据点保存
                mprintf("csq is %d.\r\n",t1);

            }
            else
            {
                Ql_Sleep(500);
                goto RESEND;
            }
        }
        else
        {
            
            savemode = -1;
            SendMsg2KernelForGetLocation(&savemode);//信号质量极差 不要定位直接取上一次的数据点保存
            mprintf("csq is %d.\r\n",t1);
        }
    }
    else   
    {   
        //发送AT指令失败 直接关机
        mprintf("get csq fialed\r\n");
        nextfaststart = 1; //获取不到 csq 20分钟后开机重试
        SendMsg2KernelForMcuOffP(&nextfaststart);
    }

}
#define DEBUG_TEST (1)
void SaveGpsdatapoint(u32 parm)
{
    _save_location_point *point = savedata.save_point;
    int i,ret;
    ST_Time time;
    int mode = *(int *)parm;
    static int errorT = 0;
    
    GetModuleData();
	GetSysTime(&time);
    
    mprintf("mode is %d\r\n",mode);
    printfErr2F("mode is %d\r\n",mode);
    mprintf("saving data point num %d\r\n",savedata.save_data_num);
    printfErr2F("saving data point num %d\r\n",savedata.save_data_num);
    if(savedata.faststart > 0 && errorT != 1)
    {
        if(savedata.save_data_num > 0)
            savedata.save_data_num--; //如果是快速启动,更新上一个点
    }
    
    if(savedata.save_data_num != MAX_POINT_LEN)
    {
        if(savedata.save_data_num > MAX_POINT_LEN)
        {
            savedata.save_data_num = MAX_POINT_LEN - 1;
        }
    }
    else if(savedata.save_data_num == MAX_POINT_LEN) //如果保存的数据点等于最大值，舍弃最旧的
    {
        Ql_memset((void *)(&point[0]), 0, sizeof(_save_location_point));
        for(i = 0;i < MAX_POINT_LEN - 1;i++)
        {
            Ql_memcpy((void *)(&point[i]), (void *)(&point[i + 1]), sizeof(_save_location_point));  
        }

#if 0
        Ql_memset((void *)(point), 0, sizeof(_save_location_point));
        for(i = 0;i < MAX_POINT_LEN - 1;i++)
        {
            Ql_memcpy((void *)point + i, (void *)point + i + 1, sizeof(_save_location_point));  
        }
#endif

        savedata.save_data_num--;  //减一个数据点
    }

    
    if(mode == 0)
    {
        Set_FirstData_location(1,gpsx.longitude,gpsx.latitude); //订到位设置保存位置信息
        mprintf("gps success get localtion.\r\n");
    }
    else if(mode == 1)
    {
        mprintf("get lbs location.\r\n");
        printfErr2F("get lbs location.\r\n");
    }
    else //没有信号 
    {
        mprintf("gps fialed get location.\r\n");
    }

    
    u8 temp = savedata.save_data_num;
    if(mode == 1) //取lbs位置信息
    {
        ret=Check_SystemGprsSta(70); //35s 超时
//        ret = -1;
	    if(ret!=0)
	   	{
            savemode = -1;      //再次保存 使用上一个数据点
	   		mprintf("!!!save data point  GPRS CHECK ERROR!!!\r\n");
            printfErr2F("!!!save data point  GPRS CHECK ERROR!!!\r\n");
            errorT = 1;
            SendMsg2KernelForGetLocation(&savemode);
            return ;
	   	}
		ret = LBSDataInit();
		ret = WaitLbsGetLocation(200);
		if(ret == 0 && LbsJingdu != 0 && LbsWeidu != 0)
		{
            savedata.preflag = 0;//清除保存的位置信息
            Set_FirstData_location(2,LbsJingdu,LbsWeidu);           
            savedata.save_point[temp].latitude = LbsWeidu;
            savedata.save_point[temp].longitude = LbsJingdu;
            if(savedata.faststart >= 0)
            {
                savedata.faststart = 0;
                mode =0;                //无信号时，得到最优点 取消重复取点
            }
		}
        else
        {
            savemode = -1;
            errorT = 1;
            SendMsg2KernelForGetLocation(&savemode);
            return;
        }
    }
    else if(mode < 0) //定位失败
    {
        if(temp != 0)
        {
            if(savedata.preflag != 0) //上次联网gps lbs或者未联网gps 定到位
            {
                savedata.preflag = 0;//清除保存的位置信息
                savedata.save_point[temp].latitude = savedata.prellat;
                savedata.save_point[temp].longitude = savedata.prellng;
            }
            else ////上次联网gps lbs或者未联网gps 未定到位 不管直接取上次定位信息
            {
                savedata.save_point[temp].latitude = savedata.save_point[temp - 1].latitude;
                savedata.save_point[temp].longitude = savedata.save_point[temp - 1].longitude;
            }
        }
        else //若之前没有数据点 
        {
            savedata.preflag = 0;
            savedata.save_point[temp].latitude = savedata.prellat;
            savedata.save_point[temp].longitude = savedata.prellng;
        }
    }
    else
    {
        savedata.save_point[temp].latitude = gpsx.latitude;
        savedata.save_point[temp].longitude = gpsx.longitude;
    }
    
    savedata.save_point[temp].collecttime.year = time.year;
    savedata.save_point[temp].collecttime.mon = time.month;
    savedata.save_point[temp].collecttime.day = time.day;
    savedata.save_point[temp].collecttime.hour = time.hour;
    savedata.save_point[temp].collecttime.min = time.minute;
    savedata.save_point[temp].collecttime.sec = time.second;
    savedata.save_point[temp].csq = module_data.csq;
    savedata.save_point[temp].bvol = G_Bvol;
    savedata.save_point[temp].sysruntime = systemset.SysTime;
    savedata.save_data_num++;

#if DEBUG_TEST
    mprintf("save date:%d-%d-%d %d:%d:%d\r\n",point[temp].collecttime.year,point[temp].collecttime.mon,point[temp].collecttime.day,point[temp].collecttime.hour,point[temp].collecttime.min,point[temp].collecttime.sec);
    printfErr2F("save date:%d-%d-%d %d:%d:%d\r\n",point[temp].collecttime.year,point[temp].collecttime.mon,point[temp].collecttime.day,point[temp].collecttime.hour,point[temp].collecttime.min,point[temp].collecttime.sec);
    mprintf("save csq:%d\r\n",point[temp].csq);  
    printfErr2F("save csq:%d\r\n",point[temp].csq);
    mprintf("save bvol:%d\r\n",point[temp].bvol);
    mprintf("save sysruntime:%d\r\n",point[temp].sysruntime);
    mprintf("save latitude:%d\r\n",point[temp].latitude); 
    printfErr2F("save latitude:%d\r\n",point[temp].latitude);
    mprintf("save longitude:%d\r\n",point[temp].longitude);
    printfErr2F("save longitude:%d\r\n",point[temp].longitude);
#endif
        
    if(mode != 0)
    {   
        savedata.faststart++;
        if(savedata.faststart > 2) //3次 一个小时
        {
            
            savedata.faststart = 0;
            nextfaststart = 0; //如果重试 一个小时仍无法联网 正常运行
        }
        else
        {
            nextfaststart = 1;// default 20min  
        }
    }
    else
    {
        nextfaststart = 0;
    }
    SaveFlashSaveData(&savedata);//保存数据并关机
    SendMsg2KernelForMcuOffP(&nextfaststart);
    return;
}




