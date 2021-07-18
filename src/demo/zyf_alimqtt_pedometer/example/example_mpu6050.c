/*****************************************************************************
 * Project:
 * --------
 *   OpenCPU
 *
 * Description:
 * ------------
 *   This example demonstrates how to use iic function with APIs in OpenCPU.
 *   Input the specified command through uart port1 and the result will be 
 *   output through the debug port.
 *   
 *
 * Usage:
 * ------
 *   Compile & Run:
 *
 *     Set "C_PREDEF=-D __EXAMPLE_MPU6050__" in gcc_makefile file. And compile the 
 *     app using "make clean/new".
 *     Download image bin to module to run.
 * 
 *============================================================================
 *             HISTORY
 *----------------------------------------------------------------------------
 * 
 ****************************************************************************/


#ifdef __EXAMPLE_MPU6050__


#include "ql_type.h"
#include "ql_trace.h"
#include "ql_uart.h"
#include "ql_system.h"
#include "ql_stdlib.h"
#include "ql_error.h"
#include "ql_gpio.h"
#include "ql_iic.h"
#include "example_mpu6050.h"


#define DEBUG_ENABLE 1
#if DEBUG_ENABLE > 0
#define DEBUG_PORT  UART_PORT1
#define DBG_BUF_LEN   512
static char DBG_BUFFER[DBG_BUF_LEN];
#define mprintf(FORMAT,...) {\
    Ql_memset(DBG_BUFFER, 0, DBG_BUF_LEN);\
    Ql_sprintf(DBG_BUFFER,FORMAT,##__VA_ARGS__); \
    if (UART_PORT2 == (DEBUG_PORT)) \
    {\
        Ql_Debug_Trace(DBG_BUFFER);\
    } else {\
        Ql_UART_Write((Enum_SerialPort)(DEBUG_PORT), (u8*)(DBG_BUFFER), Ql_strlen((const char *)(DBG_BUFFER)));\
    }\
}
#else
#define mprintf(FORMAT,...)
#endif


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



static void Callback_UART_Hdlr(Enum_SerialPort port, Enum_UARTEventType msg, bool level, void* customizedPara)
{
    mprintf("Callback_UART_Hdlr: port=%d, event=%d, level=%d, p=%x\r\n", port, msg, level, customizedPara);
}



void proc_main_task(s32 taskId)
{
    s32 ret;
    ST_MSG msg;

    ret = Ql_UART_Register(UART_PORT1, Callback_UART_Hdlr, NULL);
    if (ret < QL_RET_OK)
    {
        mprintf("Fail to register serial port[%d], ret=%d\r\n", UART_PORT1, ret);
    }
    ret = Ql_UART_Open(UART_PORT1, 115200, FC_NONE);

    if (ret < QL_RET_OK)
    {
        mprintf("Fail to open serial port[%d], ret=%d\r\n", UART_PORT1, ret);
    }
    
    mprintf("\r\n<-- OpenCPU: MPU6050 Example -->\r\n");

	MPU_i2c_init();
	MPU_Init();

    while (TRUE)
    {
        Ql_OS_GetMessage(&msg);

        switch(msg.message)
        {
            case MSG_ID_RIL_READY:
                Ql_RIL_Initialize();
                mprintf("<-- RIL is ready -->\r\n");
            break;

            case MSG_ID_USER_START:
                break;

            default:
                break;
        }
    }
}




void proc_subtask1(s32 TaskId)
{
	 Ql_Sleep(3000);
	 CountStepInit();
	 MPU_Wakeup();
	 while (1)
	 	{
	 		Ql_Sleep(10);
   			CountStep();
			
	 	}
}


SensorData GMeter;

unsigned char ucData[32] = {0};
unsigned short m = 0;
unsigned char n = 0;
DATATYPE DateBufferX[10] = {9000, 9000, 9000, 9000, 9000, 9000, 9000, 9000, 9000, 9000};
DATATYPE DateBufferY[10] = {9000, 9000, 9000, 9000, 9000, 9000, 9000, 9000, 9000, 9000};
DATATYPE DateBufferZ[10] = {9000, 9000, 9000, 9000, 9000, 9000, 9000, 9000, 9000, 9000};
unsigned short StepCount = 0;
unsigned short PreStepCount = 0;
unsigned char StepFlag  = 0;
unsigned char InitFlag = 0;
unsigned char m_count = 0;
unsigned char GMeterAmax = 0;
/*********************************************************************************************************
** Function name:           main
** Descriptions:            计步
**                          现象：记录人走路或跑步时的步数，并将数据打包通过串口和无线模块发送出去
** input parameters:        none
** output parameters:       none
** Returned value:          none
** Created by:              smallmount
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
void DataInit(SensorData *SData)
{
    SData->X.Max = -8192;
    SData->X.Min = +8192;
    SData->Y.Max = -8192;
    SData->Y.Min = +8192;
    SData->Z.Max = -8192;
    SData->Z.Min = +8192;
}
/*********************************************************************************************************
** Function name:           main
** Descriptions:            计步
**                          现象：记录人走路或跑步时的步数，并将数据打包通过串口和无线模块发送出去
** input parameters:        none
** output parameters:       none
** Returned value:          none
** Created by:              smallmount
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
unsigned char DataSelect(SensorData *GMeter)
{
    DATATYPE tempX = 0, tempY = 0, tempZ = 0, tempMax;
    unsigned char flag = 0;
    tempX = GMeter->X.Max - GMeter->X.Min;
    tempY = GMeter->Y.Max - GMeter->Y.Min;
    tempZ = GMeter->Z.Max - GMeter->Z.Min;
    if(tempX > tempY) {
        if(tempX > tempZ) {
            flag = 1;
            tempMax = tempX;
        } else {
            flag = 3;
            tempMax = tempZ;
        }
    } else {
        if(tempY > tempZ) {
            flag = 2;
            tempMax = tempY;
        } else {
            flag = 3;
            tempMax = tempZ;
        }
    }
    if(tempMax > 1000) {
        return flag;
    } else {
        return 0;
    }
}

void CountStepInit(void)
{
    DataInit(&GMeter);
}


void CountStep(void)
{
	unsigned char i = 0;
    signed short X = 0, Y = 0, Z = 0;
	//MPU_Read_Len(MPU_ADDR,MPU_ACCEL_XOUTH_REG,6,ucData);
	MPU_Read_Len(MPU_ADDR,MPU_GYRO_XOUTH_REG,6,ucData);
    ucData[6] = StepCount;
        X = ((ucData[0] << 8) | ucData[1]);
        Y = ((ucData[2] << 8) | ucData[3]);
        Z = ((ucData[4] << 8) | ucData[5]);
			mprintf("%d,%d,%d\r\n",X,Y,Z);

        if(m == DATASIZE) {
            m = 0;
            if(m_count == 0) {
                m_count = 1;
            }
        }
        if(n == 10) {
            n = 0;
        }
        DateBufferX[n] = X;
        DateBufferY[n] = Y;
        DateBufferZ[n] = Z;
        if(InitFlag < 9) {
            GMeter.X.Data[m] = X;
            GMeter.Y.Data[m] = Y;
            GMeter.Z.Data[m] = Z;
            InitFlag++;
        } else {
            GMeter.X.Data[m] = (DateBufferX[0] + DateBufferX[1] + DateBufferX[2] + DateBufferX[3] + DateBufferX[4] + DateBufferX[5] + DateBufferX[6] + DateBufferX[7] + DateBufferX[8] + DateBufferX[9]) / 10;
            GMeter.Y.Data[m] = (DateBufferY[0] + DateBufferY[1] + DateBufferY[2] + DateBufferY[3] + DateBufferY[4] + DateBufferY[5] + DateBufferY[6] + DateBufferY[7] + DateBufferY[8] + DateBufferY[9]) / 10;
            GMeter.Z.Data[m] = (DateBufferZ[0] + DateBufferZ[1] + DateBufferZ[2] + DateBufferZ[3] + DateBufferZ[4] + DateBufferZ[5] + DateBufferZ[6] + DateBufferZ[7] + DateBufferZ[8] + DateBufferZ[9]) / 10;
        }
        if(m_count == 1) {
            if(GMeter.X.MaxMark == m || GMeter.X.MinMark == m || GMeter.Y.MaxMark == m || GMeter.Y.MinMark == m || GMeter.Z.MaxMark == m || GMeter.Z.MinMark == m) {
                unsigned char tempXMaxMark = GMeter.X.MaxMark;
                unsigned char tempXMinMark = GMeter.X.MinMark;
                unsigned char tempYMaxMark = GMeter.Y.MaxMark;
                unsigned char tempYMinMark = GMeter.Y.MinMark;
                unsigned char tempZMaxMark = GMeter.Z.MaxMark;
                unsigned char tempZMinMark = GMeter.Z.MinMark;
                if(GMeter.X.MaxMark == m) {
                    GMeter.X.Max = -8192;
                }
                if(GMeter.X.MinMark == m) {
                    GMeter.X.Min = +8192;
                }
                if(GMeter.Y.MaxMark == m) {
                    GMeter.Y.Max = -8192;
                }
                if(GMeter.Y.MinMark == m) {
                    GMeter.Y.Min = +8192;
                }
                if(GMeter.Z.MaxMark == m) {
                    GMeter.Z.Max = -8192;
                }
                if(GMeter.Z.MinMark == m) {
                    GMeter.Z.Min = +8192;
                }
                for(i = 0; i < DATASIZE; i++) {
                    if(GMeter.X.MaxMark == m) {
                        if(GMeter.X.Data[i] >= GMeter.X.Max) {
                            GMeter.X.Max = GMeter.X.Data[i];
                            tempXMaxMark = i;
                        }
                    }
                    if(GMeter.X.MinMark == m) {
                        if(GMeter.X.Data[i] <= GMeter.X.Min) {
                            GMeter.X.Min = GMeter.X.Data[i];
                            tempXMinMark = i;
                        }
                    }
                    if(GMeter.Y.MaxMark == m) {
                        if(GMeter.Y.Data[i] >= GMeter.Y.Max) {
                            GMeter.Y.Max = GMeter.Y.Data[i];
                            tempYMaxMark = i;
                        }
                    }
                    if(GMeter.Y.MinMark == m) {
                        if(GMeter.Y.Data[i] <= GMeter.Y.Min) {
                            GMeter.Y.Min = GMeter.Y.Data[i];
                            tempYMinMark = i;
                        }
                    }
                    if(GMeter.Z.MaxMark == m) {
                        if(GMeter.Z.Data[i] >= GMeter.Z.Max) {
                            GMeter.Z.Max = GMeter.Z.Data[i];
                            tempZMaxMark = i;
                        }
                    }
                    if(GMeter.Z.MinMark == m) {
                        if(GMeter.Z.Data[i] <= GMeter.Z.Min) {
                            GMeter.Z.Min = GMeter.Z.Data[i];
                            tempZMinMark = i;
                        }
                    }
                }
                GMeter.X.MaxMark = tempXMaxMark;
                GMeter.X.MinMark = tempXMinMark;
                GMeter.Y.MaxMark = tempYMaxMark;
                GMeter.Y.MinMark = tempYMinMark;
                GMeter.Z.MaxMark = tempZMaxMark;
                GMeter.Z.MinMark = tempZMinMark;
            }
        }
        if(GMeter.X.Data[m] >= GMeter.X.Max) {
            GMeter.X.Max = GMeter.X.Data[m];
            GMeter.X.MaxMark = m;
        }
        if(GMeter.X.Data[m] <= GMeter.X.Min) {
            GMeter.X.Min = GMeter.X.Data[m];
            GMeter.X.MaxMark = m;
        }
        if(GMeter.Y.Data[m] >= GMeter.Y.Max) {
            GMeter.Y.Max = GMeter.Y.Data[m];
            GMeter.Y.MaxMark = m;
        }
        if(GMeter.Y.Data[m] <= GMeter.Y.Min) {
            GMeter.Y.Min = GMeter.Y.Data[m];
            GMeter.Y.MinMark = m;
        }
        if(GMeter.Z.Data[m] >= GMeter.Z.Max) {
            GMeter.Z.Max = GMeter.Z.Data[m];
            GMeter.Z.MaxMark = m;
        }
        if(GMeter.Z.Data[m] <= GMeter.Z.Min) {
            GMeter.Z.Min = GMeter.Z.Data[m];
            GMeter.Z.MinMark = m;
        }
        GMeter.X.Base      = (GMeter.X.Max + GMeter.X.Min) / 2;
        GMeter.Y.Base      = (GMeter.Y.Max + GMeter.Y.Min) / 2;
        GMeter.Z.Base      = (GMeter.Z.Max + GMeter.Z.Min) / 2;
        GMeter.X.UpLimit   = (GMeter.X.Base + GMeter.X.Max * 2) / 3;
        GMeter.Y.UpLimit   = (GMeter.Y.Base + GMeter.Y.Max * 2) / 3;
        GMeter.Z.UpLimit   = (GMeter.Z.Base + GMeter.Z.Max * 2) / 3;
        GMeter.X.DownLimit = (GMeter.X.Base + GMeter.X.Min * 2) / 3;
        GMeter.Y.DownLimit = (GMeter.Y.Base + GMeter.Y.Min * 2) / 3;
        GMeter.Z.DownLimit = (GMeter.Z.Base + GMeter.Z.Min * 2) / 3;
        GMeterAmax = DataSelect(&GMeter);
        switch(GMeterAmax) {
        case 1:
            if((GMeter.X.Data[m] > GMeter.X.UpLimit) && StepFlag == 0) {
                StepFlag = 1;
            }
            if((GMeter.X.Data[m] < GMeter.X.DownLimit) && StepFlag ==1) {
                StepFlag = 0;
                StepCount++;
                StepCount++;
            }
            break;
        case 2:
            if((GMeter.Y.Data[m] > GMeter.Y.UpLimit) && StepFlag == 0) {
                StepFlag = 1;
            }
            if((GMeter.Y.Data[m] < GMeter.Y.DownLimit) && StepFlag ==1) {
                StepFlag = 0;
                StepCount++;
                StepCount++;
            }
            break;
        case 3:
            if((GMeter.Z.Data[m] > GMeter.Z.UpLimit) && StepFlag == 0) {
                StepFlag = 1;
            }
            if((GMeter.Z.Data[m] < GMeter.Z.DownLimit) && StepFlag ==1) {
                StepFlag = 0;
                StepCount++;
                StepCount++;
            }
            break;
        default:
            break;
        }
        ucData[6]   = GMeter.X.Data[m] >> 8;
        ucData[7]   = GMeter.X.Data[m] & 0x00FF;
        ucData[8]   = GMeter.Y.Data[m] >> 8;
        ucData[9]   = GMeter.Y.Data[m] & 0x00FF;
        ucData[10]  = GMeter.Z.Data[m] >> 8;
        ucData[11]  = GMeter.Z.Data[m] & 0x00FF;
        ucData[12]  = GMeter.X.Base >> 8;
        ucData[13]  = GMeter.X.Base & 0x00FF;
        ucData[14]  = GMeter.X.UpLimit >> 8;
        ucData[15]  = GMeter.X.UpLimit & 0x00FF;
        ucData[16]  = GMeter.X.DownLimit >> 8;
        ucData[17]  = GMeter.X.DownLimit & 0x00FF;
        ucData[18]  = GMeter.X.Max >> 8;
        ucData[19]  = GMeter.X.Max & 0x00FF;
        ucData[20]  = GMeter.X.Min >> 8;
        ucData[21]  = GMeter.X.Min & 0x00FF;
        ucData[22]  = StepCount;
        ucData[23]  = StepFlag;
        ucData[24]  = 0;
        ucData[25]  = 0;
        m++;
        n++;
		if(PreStepCount!=StepCount)
			{
				PreStepCount=StepCount;
				mprintf("StepCount=%d\r\n",StepCount);
			}

}

#endif

