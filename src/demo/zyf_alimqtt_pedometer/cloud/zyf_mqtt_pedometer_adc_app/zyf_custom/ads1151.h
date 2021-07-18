#ifndef __ads_H__
#define __ads_H__
#define ADS1151_CHNUM		1                 //I2C通道
#define ADS1151_I2C_SLAVE_ADDR      0x48   //I2C地址
#define CMD_Write      0x90                   ////写入命令 1001 0000  前7位表示ADS1115地址，最后1位0表示写
#define CMD_Read       0x91      ////读取命令  1001 001   前7位表示ADS1115地址，最后1位1表示读
#define CMD_POINT_REG  0x00         ////指向寄存器配置
#define CMD_CONF_REG   0x01        /////配置寄存器配置
#define CONF_L    0xe3        ////低8位
#define ADS1151_ERR_I2C			-1
#define client     0x48
#define ADS1151_DATA_LEN        16
extern float Get_ATOD (unsigned char channel);
#endif