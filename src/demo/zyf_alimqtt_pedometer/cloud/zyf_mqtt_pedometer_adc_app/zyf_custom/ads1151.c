
#include "ql_type.h"
#include "ql_trace.h"
#include "ql_uart.h"
#include "ql_system.h"
#include "ql_stdlib.h"
#include "ql_error.h"
#include "ql_gpio.h"
#include "ql_iic.h"
#include "uart.h"
#include "sys.h"
#include "ads1151.h"


//unsigned char ReadBuffer[2];
/*******************************************************************************
* Function Name  : ads1151_i2c_init
* ?Ç??          : ?ùÂßã?ñi2c
* Attention  ?ôi2c?∞Êç*******************************************************************************/
int ads1151_i2c_init(void)
{
	s32 ret;
#if defined(USE_ADS1115) //&& !defined(USE_BOTH_MPU6050_ADS1115)
	ret = Ql_IIC_Init(1,PINNAME_RI,PINNAME_DCD,1);
	if(ret < 0)
	{
		mprintf("\r\n<--Failed!! IIC controller Ql_IIC_Init channel 1 fail ret=%d-->\r\n",ret);
		return ret;	
	}
	mprintf("\r\n<--pins(SCL=%d,SDA=%d) IIC controller Ql_IIC_Init channel 1 ret=%d-->\r\n",PINNAME_RI,PINNAME_DCD,ret);
#endif
    ret = Ql_IIC_Config(1,TRUE, 0x90, 300);
	if(ret < 0)
	{
		mprintf("\r\n<--Failed !! IIC controller Ql_IIC_Config channel 1 fail ret=%d-->\r\n",ret);
		return ret;
	}		
	mprintf("\r\n<--IIC controller Ql_IIC_Config channel 1 ret=%d-->\r\n",ret);
	return ret;
}
/*******************************************************************************
* Function Name  : ads_i2c_read
* ?Ç??      
                   addrÔºöÂØÑÂ≠ò?®Âú∞ÂùÄ
				   *data Â≠ò?®Ë				 
* Attention  ËØ?2c?∞Êç*******************************************************************************/
 int ads_i2c_read(u8 addr, u8 *data)
{
	int res = 0;
	res=Ql_IIC_Write(ADS1151_CHNUM,0x90,&addr,0x01);
	if(res< 0)
	{
		mprintf("\r\n<--Failed !! IIC controller Ql_IIC_Write channel 1 fail ret=%d-->\r\n",res);
		return res;	
	}
	res = Ql_IIC_Read(ADS1151_CHNUM, 0x90, data, 0x02);
	
	if(res < 0)
	{
		mprintf("\r\n<--Failed !! IIC controller Ql_IIC_Read channel 1 fail ret=%d-->\r\n",res);
		return data;	
	}
	return data;
}
/*******************************************************************************
* Function Name  : ads_i2c_write
* ?Ç??      
                   addrÔºöÂØÑÂ≠ò?®Âú∞ÂùÄ
				   *data Â≠ò?®Ë				 
* Attention  ËØ?2c?∞Êç*******************************************************************************/
int ads_i2c_write(u8 addr, u8 *data)
{
     u8 buf[3]={0,0,0};
	 buf[0]=addr;
	 buf[1]=data[0];
	 buf[2]=data[1];
	int res = 0;
	res=Ql_IIC_Write(ADS1151_CHNUM,0x90,buf,0x03);
		if(res< 0)
	{
		mprintf("\r\n<--Failed !! IIC controller Ql_IIC_Write channel 1 fail ret=%d-->\r\n",res);
		return res;	
	}
		return res;
	
}


/*******************************************************************************
* Function Name  : Confige1115
* ?Ç??          : ?ö?ì0/1/2/3
* Attention :?çÁΩ*******************************************************************************/

static void Confige1115 (unsigned char port)
{
    static unsigned char chnel, i;
	u8 databuf[2]={0,0}; 

	u8 buf_read[ADS1151_DATA_LEN] = {0};

	 int res = 0;
	 u8 mpuid;
    switch (port)
    {
      case 0:               //0?ö?ì
          chnel=0xC2; 
      break;
      
      case 1:               //1?ö?ì  
          chnel=0xD2;
      break;
          
      case 2:               //2?ö?ì  
          chnel=0xE2;
      break;
          
      case 3:               //3?ö?ì
          chnel=0xF2;
      break;
          
      default:
      break; 
    }   
	databuf[0]=chnel;
	databuf[1]=0x83;

	res = ads_i2c_write(CMD_CONF_REG,databuf);//?ô?•Êï∞Êç	ads_i2c_read(0x01, buf_read);
	if(res < 0)
	{
		return ADS1151_ERR_I2C;
	}
  
       
}

/*******************************************************************************
* ËØªÂèñADS1115?Ñ16‰Ωç?∞Êç*******************************************************************************/
static u16 ReadData (unsigned char chnnal1)
{
    unsigned char ReadBuffer[2]={0,0};
	u16  data=0;
	int res = 0;
	u8 buf[ADS1151_DATA_LEN] = {0};
	ads_i2c_read(0x00, buf);////ËØªÂèñ?∞Êç    Delayus(200);
	ReadBuffer[0]=buf[0];
	ReadBuffer[1]=buf[1];
	Delayus(200);
  	data = ReadBuffer[0]*256+ReadBuffer[1];  ////?∞Êç	return data;
}


/*******************************************************************************
* Function Name  : Get_ATOD
* Attention :?∑ÂèñADS1115Ê®°ÊãüËΩ*******************************************************************************/
float Get_ATOD (unsigned char channel)
{
	static unsigned char chn; 
	u16 data_get;
	chn = channel; 
	float vol; 
	Confige1115(channel); ////?çÁΩ	data_get = ReadData(chn);  ////‰ªé?ö?ì‰∏	/**?®‰?Êµã?èË¥ü?µÂéãÔºåË¥ü?µÂéã‰ªé8000~ffff,Ë¥ü?µÂéã‰∏éÊ≠£ÂÖ≥‰?0?âÁ±ª‰ººÂØπÁß∞?≥Á≥ªÔºå?â‰Ωç?ñ?ç?é+1?∏Âêå**/
	if(data_get>=0x8000)  
		vol=((float)(0xffff-data_get)/32768.0)*4.096;
	else
		vol=((float)data_get/32768.0)*4.096;
	vol=vol*1000;
				return vol;
}

