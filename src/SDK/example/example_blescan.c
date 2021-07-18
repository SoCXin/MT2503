#include "custom_feature_def.h"
#ifdef __OCPU_RIL_BLE_SUPPORT__
#ifdef __EXAMPLE_BLUETOOTH_BLESCAN__

#include "ql_stdlib.h"
#include "ql_uart.h"
#include "ql_trace.h"
#include "ql_type.h"
#include "ql_system.h"
#include "ril.h"
#include "ril_bluetooth.h"
#include "ril_ble.h"



#define DEBUG_ENABLE 1
#if DEBUG_ENABLE > 0
#define DEBUG_PORT  UART_PORT1
#define DBG_BUF_LEN   2048
static char DBG_BUFFER[DBG_BUF_LEN];
#define APP_DEBUG(FORMAT,...) {\
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
#define APP_DEBUG(FORMAT,...) 
#endif

#define SERIAL_RX_BUFFER_LEN  (2048)
#define BL_RX_BUFFER_LEN       (1024+1)

static u8 m_RxBuf_Uart1[SERIAL_RX_BUFFER_LEN];
static u8 m_RxBuf_BL[BL_RX_BUFFER_LEN];


static char bt_name[BT_NAME_LEN] = "ble_scantest";

    
static void CallBack_UART_Hdlr(Enum_SerialPort port, Enum_UARTEventType msg, bool level, void* customizedPara);
static s32 ATResponse_Handler(char* line, u32 len, void* userData);


static s32 ATRsp_QBTGatsreg_Hdlr_M20(char* line, u32 len, void* param)
{

	APP_DEBUG("%s",line);
    if (Ql_RIL_FindLine(line, len, "OK"))
    {  
        return  RIL_ATRSP_SUCCESS;
    }
    else if (Ql_RIL_FindLine(line, len, "ERROR"))
    {  
        return  RIL_ATRSP_FAILED;
    }
    else if (Ql_RIL_FindString(line, len, "+CME ERROR"))
    {
        return  RIL_ATRSP_FAILED;
    }
    else if (Ql_RIL_FindString(line, len, "+CMS ERROR:"))
    {
        return  RIL_ATRSP_FAILED;
    }
    return RIL_ATRSP_CONTINUE; //continue wait
}


s32 RIL_BT_Gatsreg_M20(u8 op , u8 *name)
{
    char strAT[100];
    s32 ret;
	
 
    Ql_memset(strAT, 0x0, sizeof(strAT));
    
    Ql_sprintf(strAT, "AT+QBTGATCREG=%d,\"%s\"", op,name);

	APP_DEBUG("%s",strAT);

    ret = Ql_RIL_SendATCmd(strAT, Ql_strlen(strAT), ATRsp_QBTGatsreg_Hdlr_M20,NULL,0);
    
    return ret;
    
}


s32 RIL_BT_Setcfg_M20(u8 num , u32 tt)
{
    char strAT[100];
    s32 ret;
	
 
    Ql_memset(strAT, 0x0, sizeof(strAT));
    
    Ql_sprintf(strAT, "at+qbtcfg=\"blescan\",%d,%d", num,tt);

	APP_DEBUG("%s",strAT);

    ret = Ql_RIL_SendATCmd(strAT, Ql_strlen(strAT), ATRsp_QBTGatsreg_Hdlr_M20,NULL,0);
    
    return ret;
    
}




void Ble_PowerOn(void)
{
	char strAT[100];
    s32 cur_pwrstate = 0 ;
    s32 ret = RIL_AT_SUCCESS ;
	
    ret = RIL_BT_GetPwrState(&cur_pwrstate);
    if (RIL_AT_SUCCESS != ret)
    {
        APP_DEBUG("Get BT device power status failed.\r\n");
        return;
    }
    if (1 == cur_pwrstate)
    {
        APP_DEBUG("BT device already power on.\r\n");
    }
    else if (0 == cur_pwrstate)
    {
        ret = RIL_BT_Switch(1);
        if (RIL_AT_SUCCESS != ret)
        {
            APP_DEBUG("BT power on failed,ret=%d.\r\n", ret);
            return;
        }
        APP_DEBUG("BT device power on.\r\n");
    }

    RIL_BT_GetPwrState(&cur_pwrstate);
    APP_DEBUG("BT power  cur_pwrstate=%d.\r\n", cur_pwrstate);

#if 1


#if 0
    ret = RIL_BT_SetName(bt_name, 16);
    if (RIL_AT_SUCCESS == ret)
    {
        APP_DEBUG("BT device name set successful^.\r\n");
    }
    else
    {
        APP_DEBUG("BT device name set failed,ret=%d.\r\n", ret);
    }

#endif

    RIL_BT_Gatsreg_M20(1, "ABCD");
	//RIL_BT_SetLeTxPwr(7);
#endif

	//RIL_BT_SetVisble(BT_VISIBLE_FOREVER,0);


}

void Ble_PowerOff(void)
{
    s32 cur_pwrstate = 0 ;
    s32 ret = RIL_AT_SUCCESS ;
    ret = RIL_BT_GetPwrState(&cur_pwrstate);
    if (RIL_AT_SUCCESS != ret)
    {
        APP_DEBUG("Get BT device power status failed.\r\n");
        return;
    }
    if (1 == cur_pwrstate)
    {
        APP_DEBUG("BT device already power on.\r\n");
        ret = RIL_BT_Switch(0);
        if (RIL_AT_SUCCESS != ret)
        {
            APP_DEBUG("BT power on failed,ret=%d.\r\n", ret);
            return;
        }
        APP_DEBUG("BT device power off.\r\n");
    }
}


u8 ScanBlecom(u8 op, u8* name, u32 timeout)
{
    u8 i;
    char strAT[100];
    s32 ret;
    if(op)Ble_PowerOn();
    Ql_memset(strAT, 0x0, sizeof(strAT));
    Ql_sprintf(strAT, "AT+QBTGATCSCAN=%d,\"%s\"", op, name);
    APP_DEBUG("%s", strAT);
    ret = Ql_RIL_SendATCmd(strAT, Ql_strlen(strAT), ATRsp_QBTGatsreg_Hdlr_M20, NULL, 0);
    if (op == 1)
    {
        if (timeout)
        {
            while (timeout)
            {
                Ql_Sleep(100);
                timeout--;
            }
        }
    }
    else
    {
       Ble_PowerOff();
    }
	
    return 0;
}


void OnURCHandler_BLEScan(const char* strURC, void* reserved)
{
	APP_DEBUG("%s", strURC);
}


void proc_subtask1(s32 taskId)
{
  

	Ql_Sleep(3000);

    while (TRUE)
    {
       ScanBlecom(1, "ABCD", 40);
	   ScanBlecom(0, "ABCD", 40);
    }
}


void proc_main_task(s32 taskId)
{
    s32 ret;
    ST_MSG msg;

	Ql_UART_Register(UART_PORT1, CallBack_UART_Hdlr, NULL);
    Ql_UART_Open(UART_PORT1, 115200, FC_NONE);


    APP_DEBUG("\r\n<-- OpenCPU: subtask -->\r\n")

	
    while (TRUE)
    {
        Ql_OS_GetMessage(&msg);
        switch(msg.message)
        {
            case MSG_ID_RIL_READY:
                APP_DEBUG("<-- RIL is ready -->\r\n");
                Ql_RIL_Initialize();
                RIL_BT_Setcfg_M20(2,50);
            break;
            default:
                break;
        }
    }
}



static s32 ReadSerialPort(Enum_SerialPort port, /*[out]*/u8* pBuffer, /*[in]*/u32 bufLen)
{
    s32 rdLen = 0;
    s32 rdTotalLen = 0;
    
    if (NULL == pBuffer || 0 == bufLen)
    {
        return -1;
    }
    
    Ql_memset(pBuffer, 0x0, bufLen);
    
    while (1)
    {
        rdLen = Ql_UART_Read(port, pBuffer + rdTotalLen, bufLen - rdTotalLen);
        if (rdLen <= 0)  // All data is read out, or Serial Port Error!
        {
            break;
        }
        rdTotalLen += rdLen;
        // Continue to read...
    }
    if (rdLen < 0) // Serial Port Error!
    {
        APP_DEBUG("Fail to read from port[%d]\r\n", port);
        return -99;
    }
    return rdTotalLen;
}


static void CallBack_UART_Hdlr(Enum_SerialPort port, Enum_UARTEventType msg, bool level, void* customizedPara)
{
    s32 ret = RIL_AT_SUCCESS ;


    switch (msg)
    {
    case EVENT_UART_READY_TO_READ:
        {
            if (UART_PORT1 == port)
            {
                s32 totalBytes = ReadSerialPort(port, m_RxBuf_Uart1, sizeof(m_RxBuf_Uart1));
                
                if (totalBytes <= 0)
                {
                    Ql_Debug_Trace("<-- No data in UART buffer! -->\r\n");
                    return;
                }

                APP_DEBUG("\r\n<-- m_RxBuf_Uart1:%s->\r\n",m_RxBuf_Uart1)
				
                if(0 == Ql_strncmp(m_RxBuf_Uart1,"AT+",3)) 
                {
                ret = Ql_RIL_SendATCmd((char*)m_RxBuf_Uart1, totalBytes, ATResponse_Handler, NULL, 0);
                break;
                }
          
            }
          
            break;
        }
    case EVENT_UART_READY_TO_WRITE:
        break;
    default:
        break;
    }
}

static s32 ATResponse_Handler(char* line, u32 len, void* userData)
{
    Ql_UART_Write(UART_PORT1, (u8*)line, len);
    
    if (Ql_RIL_FindLine(line, len, "OK"))
    {  
        return  RIL_ATRSP_SUCCESS;
    }
    else if (Ql_RIL_FindLine(line, len, "ERROR"))
    {  
        return  RIL_ATRSP_FAILED;
    }
    else if (Ql_RIL_FindString(line, len, "+CME ERROR"))
    {
        return  RIL_ATRSP_FAILED;
    }
    else if (Ql_RIL_FindString(line, len, "+CMS ERROR:"))
    {
        return  RIL_ATRSP_FAILED;
    }
    return RIL_ATRSP_CONTINUE; //continue wait
}

#endif

#endif
