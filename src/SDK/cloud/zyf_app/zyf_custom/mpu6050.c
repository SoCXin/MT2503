
//#ifdef _MPU_USE_

#include "mpu6050.h"
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

int MPU_i2c_init(void)
{
	s32 ret;
	ret = Ql_IIC_Init(1,PINNAME_RI,PINNAME_DCD,1);
    if(ret < 0)
       {
           mprintf("\r\n<--Failed!! IIC controller Ql_IIC_Init channel 1 fail ret=%d-->\r\n",ret);
           return ret;
       }
    mprintf("\r\n<--pins(SCL=%d,SDA=%d) IIC controller Ql_IIC_Init channel 1 ret=%d-->\r\n",PINNAME_RI,PINNAME_DCD,ret);
	ret = Ql_IIC_Config(1,TRUE, MPU_ADDR, 40);
    if(ret < 0)
       {
          mprintf("\r\n<--Failed !! IIC controller Ql_IIC_Config channel 1 fail ret=%d-->\r\n",ret);
           return ret;
       }
     mprintf("\r\n<--IIC controller Ql_IIC_Config channel 1 ret=%d-->\r\n",ret);
	 return ret;
}

u8 MPU_Init(void)
{ 
	u8 res;
	u8 mpuid;
    Ql_Sleep(100);
	MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X80);	//复位MPU6050
    Ql_Sleep(300);
	MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X00);	//唤醒MPU6050 
	MPU_Write_Byte(MPU_GYRO_CFG_REG,0x18);//设置陀螺仪满量程范围
	MPU_Write_Byte(MPU_ACCEL_CFG_REG,0);//设置加速度传感器满量程范围  
	MPU_Set_Rate(50);						//设置采样率50Hz
	MPU_Write_Byte(MPU_INT_EN_REG,0X00);	//关闭所有中断
	MPU_Write_Byte(MPU_USER_CTRL_REG,0X00);	//I2C主模式关闭
	MPU_Write_Byte(MPU_FIFO_EN_REG,0X00);	//关闭FIFO
	//MPU_Write_Byte(MPU_INTBP_CFG_REG,0X80);	//INT引脚低电平有效
	res=MPU_Read_Byte(MPU_DEVICE_ID_REG, &mpuid);
	if(mpuid==0x68)//器件ID正确
	{
		MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X01);	//设置CLKSEL,PLL X轴为参考
		MPU_Write_Byte(MPU_PWR_MGMT2_REG,0X00);	//加速度与陀螺仪都工作
		MPU_Set_Rate(50);
		mprintf("\r\n<--IIC Device Addr=0x%02x-->\r\n",mpuid);
 	}else return 1;
	return 0;
}

u8 MPU_Sleep(void)
{
    Ql_Sleep(200);//等待采样
	MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X40);	//设置sleep
	return 0;
}

u8 MPU_Wakeup(void)
{
	MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X01);	//设置wakeup
    Ql_Sleep(200);//等待采样
	return 0;
}

u8 MPU_Set_LPF(u16 lpf)
{
	u8 data=0;
	if(lpf>=188)data=1;
	else if(lpf>=98)data=2;
	else if(lpf>=42)data=3;
	else if(lpf>=20)data=4;
	else if(lpf>=10)data=5;
	else data=6; 
	return MPU_Write_Byte(MPU_CFG_REG,data);//设置数字低通滤波器  
}

u8 MPU_Set_Rate(u16 rate)
{
	u8 data;
	if(rate>1000)rate=1000;//4~1000(Hz)
	if(rate<4)rate=4;
	data=1000/rate-1;
	data=MPU_Write_Byte(MPU_SAMPLE_RATE_REG,data);	//设置数字低通滤波器
 	return MPU_Set_LPF(rate/2);	//自动设置LPF为采样率的一半
}

//得到温度值
//返回值:温度值(扩大了100倍)
short MPU_Get_Temperature(void)
{
    u8 buf[2]; 
    short raw;
	float temp;
	MPU_Read_Len(MPU_ADDR,MPU_TEMP_OUTH_REG,2,buf); 
    raw=((u16)buf[0]<<8)|buf[1];  
    temp=36.53+((double)raw)/340;  
    return temp*100;;
}
//得到陀螺仪值(原始值)
//gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
//返回值:0,成功
//    其他,错误代码
u8 MPU_Get_Gyroscope(short *gx,short *gy,short *gz)
{
    u8 buf[6];  
    u8 res = 0;
	res=MPU_Read_Len(MPU_ADDR,MPU_GYRO_XOUTH_REG,6,buf);
	if(res==0)
	{
		*gx=((u16)buf[0]<<8)|buf[1];  
		*gy=((u16)buf[2]<<8)|buf[3];  
		*gz=((u16)buf[4]<<8)|buf[5];
		mprintf("\r\nGyroscope:gx=%d,gy=%d,gz=%d\r\n",*gx, *gy, *gz);
	} 	
    return res;
}
//得到加速度值(原始值)
//gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
//返回值:0,成功
//    其他,错误代码
u8 MPU_Get_Accelerometer(short *ax,short *ay,short *az)
{
    u8 buf[6],res;  
	res=MPU_Read_Len(MPU_ADDR,MPU_ACCEL_XOUTH_REG,6,buf);
	if(res==0)
	{
		*ax=((u16)buf[0]<<8)|buf[1];  
		*ay=((u16)buf[2]<<8)|buf[3];  
		*az=((u16)buf[4]<<8)|buf[5];
		mprintf("\r\nAccelerometer:ax=%d,ay=%d,az=%d\r\n",*ax, *ay, *az);
	} 	
    return res;
}
//IIC连续读
//addr:器件地址
//reg:要读取的寄存器地址
//len:要读取的长度
//buf:读取到的数据存储区
//返回值:0,正常
//    其他,错误代码
u8 MPU_Read_Len(u8 addr,u8 reg,u8 len,u8 *buf)
{
	int res = 0;
	res=Ql_IIC_Write(MPU_CHNUM,addr,&reg,0x01);
	if(res< 0)
	{
		mprintf("\r\n<--Failed !! IIC controller Ql_IIC_Write channel 1 fail ret=%d-->\r\n",res);
		return res;
	}
	res = Ql_IIC_Read(MPU_CHNUM, addr, buf, len);
	if(res < 0)
	{
		mprintf("\r\n<--Failed !! IIC controller Ql_IIC_Read channel 1 fail ret=%d-->\r\n",res);
		return res;
	}
	return 0;	
}
//IIC写一个字节 
//reg:寄存器地址
//data:数据
//返回值:0,正常
//    其他,错误代码
u8 MPU_Write_Byte(u8 reg,u8 data) 				 
{
	int err=0;
	u8 buf[5] = {0};

	buf[0] = reg;
	buf[1] = data;

    err = Ql_IIC_Write(MPU_CHNUM,MPU_ADDR, buf, 0x02);
    if (err < 0)
	{
        mprintf("send command error!!\n");
        return err;
    } 
	return 0;
}
//IIC读一个字节 
//reg:寄存器地址 
//返回值:读到的数据
u8 MPU_Read_Byte(u8 reg, u8 *data)
{
	u8 res;
	res=Ql_IIC_Write(MPU_CHNUM,MPU_ADDR,&reg,0x01);
	if(res< 0)
	{
		mprintf("\r\n<--Failed !! IIC controller Ql_IIC_Write channel 1 fail ret=%d-->\r\n",res);
		return res;
	}
	res = Ql_IIC_Read(MPU_CHNUM, MPU_ADDR, data, 0x01);
	if(res < 0)
	{
		mprintf("\r\n<--Failed !! IIC controller Ql_IIC_Read channel 1 fail ret=%d-->\r\n",res);
		return res;
	}
	return res;		
}

//#endif

