#include "ql_type.h"
#include "ql_trace.h"
#include "ql_uart.h"
#include "ql_system.h"
#include "ql_stdlib.h"
#include "ql_error.h"
#include "ql_gpio.h"
#include "sys.h"
#include "ql_socket.h"
#include "uart.h"
#include "new_socket.h"
#include "ql_adc.h"
#include "ril.h"


_system_setings	systemset;


volatile u8 SocketBufRdBit=0;




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
	systemset.Interval=100;
	systemset.HandInter=60; 
	systemset.SysTime=0;
	systemset.saveflag=0x0A;
	systemset.fotaflag=0;
	systemset.snuser=0;
	systemset.CallMode=0; 
	Ql_sprintf((char*)systemset.SN,"FFFFFFFFF");
	
}


void ShowSysInfo(void)
{
	mprintf("ZFY_BLE_GSM_GPS_V3.0.0\r\n");
	mprintf("SN:%s\r\n",systemset.SN);
	mprintf("Snuser:%d\r\n",systemset.snuser);
	mprintf("Interval:%d\r\n",systemset.Interval);
	mprintf("HandInter:%d\r\n",systemset.HandInter);


}

void FlashTimeInit(void)
{
	
}



void SYS_Parameter_Init(void)
{
	
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




void M203C_BSP_init(void)
{
	 UartInit();
}





