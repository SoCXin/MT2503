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


_system_setings	systemset;


volatile u8 SocketBufRdBit=0;

Enum_PinName LED1=PINNAME_SIM2_CLK;
Enum_PinName LED2=PINNAME_SIM2_DATA;
Enum_PinName LED3=PINNAME_SIM2_RST;


void power_drv_init(void)
{
	 Ql_GPIO_Init(LED1, PINDIRECTION_OUT, 0, PINPULLSEL_PULLUP);
	 Ql_GPIO_Init(LED2, PINDIRECTION_OUT, 0, PINPULLSEL_PULLUP);
	 Ql_GPIO_Init(LED3, PINDIRECTION_OUT, 0, PINPULLSEL_PULLUP);
	

	 LED1_L;
	 LED2_L;
	 LED3_L;
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
	systemset.Interval=600;
	systemset.HandInter=200; 
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
	mprintf("ZFY_BLE_GSM_GPS_V3.0.0\r\n");
	mprintf("SN:%s\r\n",systemset.SN);
	mprintf("Snuser:%d\r\n",systemset.snuser);
	mprintf("Interval:%d\r\n",systemset.Interval);
	mprintf("HandInter:%d\r\n",systemset.HandInter);
	mprintf("updateinfo:%d\r\n",systemset.updateinfo);
	mprintf("+DataTime: %04d-%02d-%02d %02d:%02d:%02d\r\n",time.year, time.month, time.day, time.hour, time.minute, time.second);


}

void FlashTimeInit(void)
{
	GetSysTime(&time);
	
}



void SYS_Parameter_Init(void)
{
	ReadFlashParamsNew(&systemset);
	ShowSysInfo();	
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
	SPK_AMP_EN_OFF;
	M203C_DATA_OFF;
	LED_B_OFF;
	LED_G_OFF;
	LED_R_OFF;
	BB_OFF;
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
	 mcu_drv_init();
	 UartInit();
}





