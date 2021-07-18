
#include "ql_type.h"
#include "ql_trace.h"
#include "ql_uart.h"
#include "ql_system.h"
#include "ql_stdlib.h"
#include "ql_error.h"
#include "ql_gpio.h"
#include "ql_iic.h"
#include "bma22x.h"
#include "uart.h"
#include "sys.h"


static bool sensor_power = TRUE;
static bool enable_status = FALSE;


int bma_i2c_init(void)
{
	s32 ret;
	ret = Ql_IIC_Init(1,PINNAME_RI,PINNAME_DCD,1);
    if(ret < 0)
       {
           mprintf("\r\n<--Failed!! IIC controller Ql_IIC_Init channel 1 fail ret=%d-->\r\n",ret);
           return ret;
       }
     mprintf("\r\n<--pins(SCL=%d,SDA=%d) IIC controller Ql_IIC_Init channel 1 ret=%d-->\r\n",PINNAME_RI,PINNAME_DCD,ret);
	  ret = Ql_IIC_Config(1,TRUE, BMA22X_I2C_SLAVE_WRITE_ADDR, 300);
    if(ret < 0)
       {
          mprintf("\r\n<--Failed !! IIC controller Ql_IIC_Config channel 1 fail ret=%d-->\r\n",ret);
           return ret;
       }
     mprintf("\r\n<--IIC controller Ql_IIC_Config channel 1 ret=%d-->\r\n",ret);
	 return ret;

}


static int bma_i2c_read_block(u8 slaveAddr, u8 addr, u8 *data, u8 len)
{
	int res = 0;
	u8 registerAdrr = addr;
#ifdef _USE_BMA_OLD_
	res=Ql_IIC_Write_Read(BMA22X_CHNUM, slaveAddr, &registerAdrr, 0x01 , data, len);
	if(res!=len)
		{
			 mprintf("\r\n<--Failed to  Ql_IIC_Write_Read channel %d fail ret=%d-->\r\n",BMA22X_CHNUM, res);
		}
#else
	res=Ql_IIC_Write(BMA22X_CHNUM,slaveAddr,&registerAdrr,0x01);
 	if(res< 0)
          {
              mprintf("\r\n<--Failed !! IIC controller Ql_IIC_Write channel 1 fail ret=%d-->\r\n",res);
			  return res;
          }
	 res = Ql_IIC_Read(BMA22X_CHNUM, slaveAddr, data, len);
     if(res < 0)
           {
              mprintf("\r\n<--Failed !! IIC controller Ql_IIC_Read channel 1 fail ret=%d-->\r\n",res);
              return res;
           }
#endif
	return res;
}

static int bma_i2c_write_block(u8 slaveAddr, u8 addr, u8 *data, u8 len)
{   
    int err, idx, num;
    char buf[8];
    err =0;

    num = 0;
    buf[num++] = addr;
    for (idx = 0; idx < len; idx++)
    {
        buf[num++] = data[idx];
    }

    err = Ql_IIC_Write(BMA22X_CHNUM,slaveAddr, buf, num);
    if (err < 0)
	{
        mprintf("send command error!!\n");
        return err;
    } 
    return err;
}

int hwmsen_write_byte(u8 slaveAddr, u8 addr, u8 data)
{
    u8 buf[] = {addr, data};
    int ret = 0;

    ret = Ql_IIC_Write(BMA22X_CHNUM,slaveAddr,(const char*)buf, sizeof(buf));
    if (ret < 0) {
        mprintf("send command error!!\n");
        return -1;
    }
    return 0;
}


int BMA22x_CheckDeviceID(void)
{
	int res = 0;
	u8 pBuffer[2] = { 0 };
	u8 registerAdrr = BMA222_REG_DEVID;
	res=Ql_IIC_Write_Read(BMA22X_CHNUM, BMA22X_I2C_SLAVE_WRITE_ADDR, &registerAdrr, 1 , pBuffer, 0x01);
	if(res!=1)
		{
			 mprintf("\r\n<--Failed to  Ql_IIC_Write_Read channel %d fail ret=%d-->\r\n",BMA22X_CHNUM, res);
		}
	else
		{
			mprintf("\r\n<--Ql_IIC_Write_Read ID 0x%02X-->\r\n",pBuffer[0]);
		}
	return res;
}

static int BMA222_SetBWRate(u8 client, u8 bwrate)
{
	u8 databuf[10]={0};    
	int res = 0;

	if((bma_i2c_read_block(client, BMA222_REG_BW_RATE, databuf, 0x01))<0)
	{
		mprintf("bma222 read rate failt \n");
		return BMA222_ERR_I2C;
	}
	Ql_Sleep(1);
	databuf[0] &= ~BMA222_BW_MASK;
	databuf[0] |= bwrate;
	

    res = bma_i2c_write_block(client,BMA222_REG_BW_RATE,databuf,0x1);
	if(res < 0)
	{
		return BMA222_ERR_I2C;
	}
	Ql_Sleep(1);
	
	return BMA222_SUCCESS;    
}


static int BMA222_SetDataFormat(u8 client, u8 dataformat)
{
	u8 databuf[10]={0};    
	int res = 0;
   
	if((bma_i2c_read_block(client, BMA222_REG_DATA_FORMAT, databuf, 0x01))<0)
	{
		mprintf("bma222 read Dataformat failt \n");
		return BMA222_ERR_I2C;
	}
	Ql_Sleep(1);
	databuf[0] &= ~BMA222_RANGE_MASK;
	databuf[0] |= dataformat;
	
	res = bma_i2c_write_block(client,BMA222_REG_DATA_FORMAT,databuf,0x1);
	if(res < 0)
	{
		return BMA222_ERR_I2C;
	}
	mprintf("BMA222_SetDataFormat OK! \n");
	Ql_Sleep(1);
	return res;    
}

static int BMA222_SetPowerMode(u8 client, bool enable)
{
	u8 databuf[2];    
	int res = 0;
	u8 addr = BMA222_REG_POWER_CTL;
	
	mprintf("enter Sensor power status is sensor_power = %d\n",sensor_power);

	if(enable == sensor_power )
	{
		mprintf("Sensor power status is newest!\n");
		return BMA222_SUCCESS;
	}

	if((bma_i2c_read_block(client, addr, databuf, 0x01))<0)
	{
		mprintf("read power ctl register err!\n");
		return BMA222_ERR_I2C;
	}
	mprintf("set power mode value = 0x%x!\n",databuf[0]);
	Ql_Sleep(1);
	if(enable == TRUE)
	{
		databuf[0] &= ~BMA222_MEASURE_MODE;
	}
	else
	{
		databuf[0] |= BMA222_MEASURE_MODE;
	}
	
	res = bma_i2c_write_block(client,BMA222_REG_POWER_CTL,databuf,0x1);
	if(res < 0)
	{
		mprintf("set power mode failed!\n");
		return BMA222_ERR_I2C;
	}
	else 
	{
		mprintf("set power mode ok %d!\n", databuf[1]);
	}
	
	sensor_power = enable;
	Ql_Sleep(1);
	return BMA222_SUCCESS;    
}

static int BMA222_ResetCalibration(u8 client)
{
	#ifndef SW_CALIBRATION 
	int err;
	u8 ofs[4]={0,0,0,0};
	#endif 
	
	#ifdef SW_CALIBRATION
		
	#else
		err = bma_i2c_write_block(client, BMA222_REG_OFSX, ofs, 4);
		if(err<0)
		{
			mprintf("error: %d\n", err);
			return err;
		}
	#endif

	return 0;    
}

static int BMA222_SetIntEnable(u8 client, u8 intenable)
{
			int res = 0;		
			res = hwmsen_write_byte(client, BMA222_INT_REG_1, 0x00);
			if(res != BMA222_SUCCESS) 
			{
				return res;
			}
			Ql_Sleep(1);
			res = hwmsen_write_byte(client, BMA222_INT_REG_2, 0x00);
			if(res != BMA222_SUCCESS) 
			{
				return res;
			}
			Ql_Sleep(1);
			return BMA222_SUCCESS;	  
}


static int BMA250_ReadData(u8 client, s16 data[BMA250_AXES_NUM])
{
	u8 addr = BMA222_REG_DATAXLOW;
	u8 buf[BMA250_DATA_LEN] = {0};
	int err = 0;
	int i;
	err = bma_i2c_read_block(client, addr, buf, 0x06);
	if(err<0)
		{
			mprintf("error: %d\n", err);
		}
	else
		{
			data[BMA250_AXIS_X] = (s16)((buf[BMA250_AXIS_X*2] >> 6) |
		         (buf[BMA250_AXIS_X*2+1] << 2));
			data[BMA250_AXIS_Y] = (s16)((buf[BMA250_AXIS_Y*2] >> 6) |
			         (buf[BMA250_AXIS_Y*2+1] << 2));
			data[BMA250_AXIS_Z] = (s16)((buf[BMA250_AXIS_Z*2] >> 6) |
			         (buf[BMA250_AXIS_Z*2+1] << 2));

				for(i=0;i<3;i++)				
			{								//because the data is store in binary complement number formation in computer system
				if ( data[i] == 0x0200 )	//so we want to calculate actual number here
					data[i]= -512;			//10bit resolution, 512= 2^(10-1)
				else if ( data[i] & 0x0200 )//transfor format
				{							//printk("data 0 step %x \n",data[i]);
					data[i] -= 0x1;			//printk("data 1 step %x \n",data[i]);
					data[i] = ~data[i];		//printk("data 2 step %x \n",data[i]);
					data[i] &= 0x01ff;		//printk("data 3 step %x \n\n",data[i]);
					data[i] = -data[i];		
				}
			}

			mprintf("\r\n[%08X %08X %08X] => [%5d %5d %5d] after\r\n", data[BMA250_AXIS_X], data[BMA250_AXIS_Y], data[BMA250_AXIS_Z],
		                               data[BMA250_AXIS_X], data[BMA250_AXIS_Y], data[BMA250_AXIS_Z]);

		}
}

int bma222_init_client(u8 client,int reset_cali)
{
	int res = 0;
	res=BMA22x_CheckDeviceID();
	if(res!=1)
		{
			return res;
		}
	mprintf("BMA250_CheckDeviceID ok \n");
	BMA222_SetBWRate(client, BMA222_BW_25HZ);
	BMA222_SetDataFormat(client, BMA222_RANGE_2G);
	BMA222_SetIntEnable(client, 0x00);
	BMA222_SetPowerMode(client, enable_status);

	if(0 != reset_cali)
	{ 
		res = BMA222_ResetCalibration(client);
		if(res != BMA222_SUCCESS)
		{
			return res;
		}
	}
	mprintf("bma222_init_client OK!\n");
	return BMA222_SUCCESS;	
}

int BMA250_ReadSensorData(u8 client, char *buf)
{
	u8  databuf[20];
	s16 data[BMA250_AXES_NUM+1];
	int acc[BMA250_AXES_NUM];
	int res = 0;
	Ql_memset(databuf, 0, sizeof(u8)*10);
	if(sensor_power == FALSE)
		{
			res = BMA222_SetPowerMode(client, TRUE);
			if(res<0)
			{
				mprintf("Power on bma250 error %d!\n", res);
			}
		}
	BMA250_ReadData(client, data);

#if 0
	data[BMA250_AXIS_X] = data[BMA250_AXIS_X] * GRAVITY_EARTH_1000 / obj->reso->sensitivity;
	data[BMA250_AXIS_Y] = data[BMA250_AXIS_Y] * GRAVITY_EARTH_1000 / obj->reso->sensitivity;
	data[BMA250_AXIS_Z] = data[BMA250_AXIS_Z] * GRAVITY_EARTH_1000 / obj->reso->sensitivity;

	obj->data[BMA250_AXIS_X] += obj->cali_sw[BMA250_AXIS_X];
	obj->data[BMA250_AXIS_Y] += obj->cali_sw[BMA250_AXIS_Y];
	obj->data[BMA250_AXIS_Z] += obj->cali_sw[BMA250_AXIS_Z];

	acc[obj->cvt.map[BMA250_AXIS_X]] = obj->cvt.sign[BMA250_AXIS_X]*obj->data[BMA250_AXIS_X];
	acc[obj->cvt.map[BMA250_AXIS_Y]] = obj->cvt.sign[BMA250_AXIS_Y]*obj->data[BMA250_AXIS_Y];
	acc[obj->cvt.map[BMA250_AXIS_Z]] = obj->cvt.sign[BMA250_AXIS_Z]*obj->data[BMA250_AXIS_Z];
	
#endif
	Ql_sprintf(mycoredata.xyz,"%05d%05d%05d", data[BMA250_AXIS_X], data[BMA250_AXIS_Y], data[BMA250_AXIS_Z]);

	return 0;

}

void BMA250_Sleep(void)
{
	int res = 0;
	if(sensor_power == TRUE)
		{
			res = BMA222_SetPowerMode(0x30, FALSE);
			if(res<0)
			{
				mprintf("Power on bma250 error %d!\r\n", res);
			}
			else
				{
					mprintf("\r\nPower Off bma250 s=%d!\r\n", res);
				}
		}
}


