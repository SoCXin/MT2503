/**
 * NOTE:
 *
 * HAL_TCP_xxx API reference implementation: wrappers/os/ubuntu/HAL_TCP_linux.c
 *
 */
#include "ql_type.h"
#include "infra_types.h"
#include "infra_defs.h"
#include "wrappers_defs.h"
#include "ql_memory.h"
#include "ql_stdlib.h"
#include "ql_error.h"
#include "ql_trace.h"
#include "ql_uart.h"
#include "ql_system.h"
#include "ql_gprs.h"
#include "ql_socket.h"
#include "sys.h"
#include "new_socket.h"
#include "user_mqtt.h"
#include "uart.h"
#include "mqtt_wrapper.h"

#define DEBUG


#ifdef DEBUG
    #define PLATFORM_SOCK_LOG(format, ...) mprintf(format, /*__FILE__,*/ ##__VA_ARGS__)
#else
    #define PLATFORM_SOCK_LOG(format, ...) 
#endif


static s32 pdpCntxtId=1;
static u8  m_ipaddress[5];

static s32 m_GprsActState = 0;         
static s32 m_SocketConnState = 0; 
s32 m_SocketId = -1; 


static ST_GprsConfig m_GprsConfig =
{
    "CMNET",    // APN name
    "",         // User name for APN
    "",         // Password for APN
    0,
    NULL,
    NULL,
};


static void Callback_GPRS_Actived(u8 contexId, s32 errCode, void* customParam)
{
    s32 ret;
    
    if(errCode == SOC_SUCCESS)
    {
        PLATFORM_SOCK_LOG("<--CallBack: active GPRS successfully.-->\r\n");
        m_GprsActState=1;
        
    }else
    {
        PLATFORM_SOCK_LOG("<--CallBack: active GPRS failed,errCode=%d-->\r\n",errCode);
    }      
}


//
//
// This callback function is invoked when GPRS drops down. The cause is in "errCode".
//
static void Callback_GPRS_Deactived(u8 contextId, s32 errCode, void* customParam)
{
    if (errCode == SOC_SUCCESS)
    {
        PLATFORM_SOCK_LOG("<--CallBack: deactivated GPRS successfully.-->\r\n");
    }
    else
    {
        PLATFORM_SOCK_LOG("<--CallBack: fail to deactivate GPRS, cause=%d)-->\r\n", errCode);
    }
    if (1 == m_GprsActState)
    {
        m_GprsActState = 0;
        PLATFORM_SOCK_LOG("<-- GPRS drops down -->\r\n");
    }
}
//
//
// This callback function is invoked when the socket connection is disconnected by server or network.
//
static void Callback_Socket_Close(s32 socketId, s32 errCode, void* customParam)
{
    if (errCode == SOC_SUCCESS)
    {
        PLATFORM_SOCK_LOG("<--CallBack: close socket successfully.-->\r\n");
    }
    else if (errCode == SOC_BEARER_FAIL)
    {
        PLATFORM_SOCK_LOG("<--CallBack: fail to close socket,(socketId=%d,error_cause=%d)-->\r\n", socketId, errCode);
    }
    else
    {
        PLATFORM_SOCK_LOG("<--CallBack: close socket failure,(socketId=%d,error_cause=%d)-->\r\n", socketId, errCode);
    }
    if (1 == m_SocketConnState)
    {
        PLATFORM_SOCK_LOG("<-- Socket connection is disconnected -->\r\n");
        PLATFORM_SOCK_LOG("<-- Close socket at module side -->\r\n");
        Ql_SOC_Close(socketId);
        m_SocketConnState = 0;
    }
}



//
// This callback function is invoked in the following case:
// The return value is less than the data length to send when calling Ql_SOC_Send(), which indicates
// the socket buffer is full. Application should stop sending socket data till this callback function
// is invoked, which indicates application can continue to send data to socket.
static void Callback_Socket_Write(s32 socketId, s32 errCode, void* customParam)
{
    if (errCode < 0)
    {
        PLATFORM_SOCK_LOG("<-- Socket error(error code:%d), close socket.-->\r\n", errCode);
        Ql_SOC_Close(socketId);
        m_SocketId = -1;
    }
    else
    {
        PLATFORM_SOCK_LOG("<-- You can continue to send data to socket -->\r\n");
    }
}


static void Callback_Socket_read(s32 socketId, s32 errCode, void* customParam )
{
	s32 ret;
    u32 packetLen=0;
    PLATFORM_SOCK_LOG("!!!!Data from M2M!!!\r\n");
	CheckAliReData();
}

static void callback_socket_connect(s32 socketId, s32 errCode, void* customParam )
{
    if (errCode == SOC_SUCCESS)
    {
    	m_SocketConnState = 1;
        PLATFORM_SOCK_LOG("<--Callback: socket connect successfully.-->\r\n");          
       
    }else
    {
        PLATFORM_SOCK_LOG("<--Callback: socket connect failure,(socketId=%d),errCode=%d-->\r\n",socketId,errCode);
       
    }
    
}

static void callback_socket_accept(s32 listenSocketId, s32 errCode, void* customParam )
{
    PLATFORM_SOCK_LOG("callback_socket_accept\r\n");
}


static ST_PDPContxt_Callback callback_gprs_func =
{
    Callback_GPRS_Actived,
    Callback_GPRS_Deactived
};
static ST_SOC_Callback callback_soc_func =
{
    callback_socket_connect,
    Callback_Socket_Close,
    callback_socket_accept,
    Callback_Socket_read,
    Callback_Socket_Write
};



/**
 * @brief Deallocate memory block
 *
 * @param[in] ptr @n Pointer to a memory block previously allocated with platform_malloc.
 * @return None.
 * @see None.
 * @note None.
 */
void HAL_Free(void *ptr)
{
	Ql_MEM_Free(ptr);
}


/**
 * @brief Get device name from user's system persistent storage
 *
 * @param [ou] device_name: array to store device name, max length is IOTX_DEVICE_NAME_LEN
 * @return the actual length of device name
 */
int HAL_GetDeviceName(char device_name[IOTX_DEVICE_NAME_LEN + 1])
{
	Ql_sprintf(device_name,systemset.SN);
	return (int)1;
}


/**
 * @brief Get device secret from user's system persistent storage
 *
 * @param [ou] device_secret: array to store device secret, max length is IOTX_DEVICE_SECRET_LEN
 * @return the actual length of device secret
 */
int HAL_GetDeviceSecret(char device_secret[IOTX_DEVICE_SECRET_LEN + 1])
{
	Ql_sprintf(device_secret,systemset.secret);
	return (int)1;
}


/**
 * @brief Get firmware version
 *
 * @param [ou] version: array to store firmware version, max length is IOTX_FIRMWARE_VER_LEN
 * @return the actual length of firmware version
 */
int HAL_GetFirmwareVersion(char *version)
{
	Ql_sprintf(version,"19032201");
	return (int)1;
}


/**
 * @brief Get product key from user's system persistent storage
 *
 * @param [ou] product_key: array to store product key, max length is IOTX_PRODUCT_KEY_LEN
 * @return  the actual length of product key
 */
int HAL_GetProductKey(char product_key[IOTX_PRODUCT_KEY_LEN + 1])
{
	Ql_sprintf(product_key,systemset.key);
	return (int)1;
}


/**
 * @brief Allocates a block of size bytes of memory, returning a pointer to the beginning of the block.
 *
 * @param [in] size @n specify block size in bytes.
 * @return A pointer to the beginning of the block.
 * @see None.
 * @note Block value is indeterminate.
 */
void *HAL_Malloc(uint32_t size)
{
	return Ql_MEM_Alloc(size);
}


/**
 * @brief Create a mutex.
 *
 * @retval NULL : Initialize mutex failed.
 * @retval NOT_NULL : The mutex handle.
 * @see None.
 * @note None.
 */
void *HAL_MutexCreate(void)
{
	return (void*)1;
}


/**
 * @brief Destroy the specified mutex object, it will release related resource.
 *
 * @param [in] mutex @n The specified mutex.
 * @return None.
 * @see None.
 * @note None.
 */
void HAL_MutexDestroy(void *mutex)
{
	return;
}


/**
 * @brief Waits until the specified mutex is in the signaled state.
 *
 * @param [in] mutex @n the specified mutex.
 * @return None.
 * @see None.
 * @note None.
 */
void HAL_MutexLock(void *mutex)
{
	return;
}


/**
 * @brief Releases ownership of the specified mutex object..
 *
 * @param [in] mutex @n the specified mutex.
 * @return None.
 * @see None.
 * @note None.
 */
void HAL_MutexUnlock(void *mutex)
{
	return;
}


/**
 * @brief Writes formatted data to stream.
 *
 * @param [in] fmt: @n String that contains the text to be written, it can optionally contain embedded format specifiers
     that specifies how subsequent arguments are converted for output.
 * @param [in] ...: @n the variable argument list, for formatted and inserted in the resulting string replacing their respective specifiers.
 * @return None.
 * @see None.
 * @note None.
 */

#if 0
void HAL_Printf(const char *fmt, ...)
{
	va_list ap;
	char DBG_BUFFER[1024];
	Ql_memset(DBG_BUFFER, 0, 1024);
	va_start(ap,fmt);
	vsprintf(DBG_BUFFER,fmt,ap);
	va_end(ap);
	Ql_UART_Write(UART_PORT1, (u8*)(DBG_BUFFER), Ql_strlen((const char *)(DBG_BUFFER)));
}

#endif


/**
 * @brief Sleep thread itself.
 *
 * @param [in] ms @n the time interval for which execution is to be suspended, in milliseconds.
 * @return None.
 * @see None.
 * @note None.
 */
void HAL_SleepMs(uint32_t ms)
{
	Ql_Sleep(ms);
}


/**
 * @brief Writes formatted data to string.
 *
 * @param [out] str: @n String that holds written text.
 * @param [in] len: @n Maximum length of character will be written
 * @param [in] fmt: @n Format that contains the text to be written, it can optionally contain embedded format specifiers
     that specifies how subsequent arguments are converted for output.
 * @param [in] ...: @n the variable argument list, for formatted and inserted in the resulting string replacing their respective specifiers.
 * @return bytes of character successfully written into string.
 * @see None.
 * @note None.
 */

/**
 * @brief Destroy the specific TCP connection.
 *
 * @param [in] fd: @n Specify the TCP connection by handle.
 *
 * @return The result of destroy TCP connection.
 * @retval < 0 : Fail.
 * @retval   0 : Success.
 */
int HAL_TCP_Destroy(uintptr_t fd)
{
	 s32 ret;

  
    ret = Ql_SOC_Close(m_SocketId);
    PLATFORM_SOCK_LOG("<-- Close socket[%d], cause=%d --> \r\n", m_SocketId, ret);
	if(ret!=0)return ret;

    PLATFORM_SOCK_LOG("<-- Deactivating GPRS... -->\r\n");
    ret = Ql_GPRS_DeactivateEx(pdpCntxtId, TRUE);
    PLATFORM_SOCK_LOG("<-- Deactivated GPRS, cause=%d -->\r\n\r\n", ret);
	if(ret!=0)return ret;

    return 0;
}


/**
 * @brief Establish a TCP connection.
 *
 * @param [in] host: @n Specify the hostname(IP) of the TCP server
 * @param [in] port: @n Specify the TCP port of TCP server
 *
 * @return The handle of TCP connection.
   @retval   0 : Fail.
   @retval > 0 : Success, the value is handle of this TCP connection.
 */
 uintptr_t HAL_TCP_Establish(const char *host, uint16_t port)
{
	s32 ret;
	m_SocketId = -1;
	my_tcp_state=STATE_NW_GET_SIMSTATE;
	GSM_InitModule();
	while (my_tcp_state != STATE_GPRS_REGISTER)
		{
		   PLATFORM_SOCK_LOG("Search the net....");
		   GSM_InitModule();
		   HAL_SleepMs(200);
		}
	
    pdpCntxtId = 1;
  
    ret = Ql_GPRS_Register(pdpCntxtId, &callback_gprs_func, NULL);
    if (GPRS_PDP_SUCCESS == ret)
    {
        PLATFORM_SOCK_LOG("<-- Register GPRS callback function -->\r\n");
    }
    else  if (GPRS_PDP_ALREADY == ret)
    {
    	PLATFORM_SOCK_LOG("<--ALREADY to register GPRS-->\r\n");
    }
	else
	{
		PLATFORM_SOCK_LOG("<-- Fail to register GPRS, cause=%d. -->\r\n", ret);
       	return  NULL;
	}
 
    ret = Ql_GPRS_Config(pdpCntxtId, &m_GprsConfig);
    if (GPRS_PDP_SUCCESS == ret)
    {
        PLATFORM_SOCK_LOG("<-- Configure PDP context -->\r\n");
    }
    else
    {
        PLATFORM_SOCK_LOG("<-- Fail to configure GPRS PDP, cause=%d. -->\r\n", ret);
        return NULL;
    }

    PLATFORM_SOCK_LOG("<-- Activating GPRS... -->\r\n");
    ret = Ql_GPRS_ActivateEx(pdpCntxtId, TRUE);
    if (ret == GPRS_PDP_SUCCESS)
    {
        m_GprsActState = 1;
        PLATFORM_SOCK_LOG("<-- Activate GPRS successfully. -->\r\n\r\n");
    }
	else if (GPRS_PDP_ALREADY == ret)
	{
		 m_GprsActState = 1;
		PLATFORM_SOCK_LOG("<--GPRS callback function has already been registered,ret=%d,pdpCntxtId=%d-->\r\n",ret,pdpCntxtId);
					                   
	}
    else
    {
        PLATFORM_SOCK_LOG("<-- Fail to activate GPRS, cause=%d. -->\r\n\r\n", ret);
        return NULL;
    }

    ret = Ql_SOC_Register(callback_soc_func, NULL);
    if (SOC_SUCCESS == ret)
    {
        PLATFORM_SOCK_LOG("<-- Register socket callback function -->\r\n");
    }
	else if (SOC_ALREADY == ret)
    {
        PLATFORM_SOCK_LOG("<-- Register socket callback function -->\r\n");
    }
    else
    {
        PLATFORM_SOCK_LOG("<-- Fail to register socket callback, cause=%d. -->\r\n", ret);
        return NULL;
    }

    m_SocketId = Ql_SOC_Create(pdpCntxtId, SOC_TYPE_TCP);
    if (m_SocketId >= 0)
    {
        PLATFORM_SOCK_LOG("<-- Create socket successfully, socket id=%d. -->\r\n", m_SocketId);
    }
    else
    {
        PLATFORM_SOCK_LOG("<-- Fail to create socket, cause=%d. -->\r\n", m_SocketId);
        return NULL;
    }

    //6. Connect to server
    {
        //6.1 Convert IP format
        Ql_memset(m_ipaddress, 0, 5);
		PLATFORM_SOCK_LOG("host=%s",host);
        ret = Ql_IpHelper_ConvertIpAddr(host, (u32*)m_ipaddress);
        if (SOC_SUCCESS == ret)  
        {
            PLATFORM_SOCK_LOG("<-- Convert Ip Address successfully,m_ipaddress=%d,%d,%d,%d -->\r\n", m_ipaddress[0], m_ipaddress[1], m_ipaddress[2], m_ipaddress[3]);
        }
        else
        {
        	 s32 count=0;
             ret = Ql_IpHelper_GetIPByHostNameEx(pdpCntxtId, 0, host, &count,m_ipaddress);
			 if(ret == SOC_SUCCESS)
			      {
			           PLATFORM_SOCK_LOG("<--Get ip by hostname successfully.-->\r\n");
			      }
			  else
			      {
			           PLATFORM_SOCK_LOG("<--Get ip by hostname failure:ret=%d-->\r\n",ret);
			           return NULL;
			       }
        }

        //6.2 Connect to server
        PLATFORM_SOCK_LOG("<-- Connecting to server(IP:%d.%d.%d.%d, port:%d)... -->\r\n", m_ipaddress[0], m_ipaddress[1], m_ipaddress[2], m_ipaddress[3], port);
        ret = Ql_SOC_ConnectEx(m_SocketId, (u32)m_ipaddress, port, TRUE);
        if (SOC_SUCCESS == ret)
        {
            m_SocketConnState = 1;
            PLATFORM_SOCK_LOG("<-- Connect to server successfully -->\r\n");
			return __LINE__;
        }
        else
        {
            PLATFORM_SOCK_LOG("<-- Fail to connect to server, cause=%d -->\r\n", ret);
            return NULL;
        }
    }

    return NULL;
}


/**
 * @brief Read data from the specific TCP connection with timeout parameter.
 *        The API will return immediately if 'len' be received from the specific TCP connection.
 *
 * @param [in] fd @n A descriptor identifying a TCP connection.
 * @param [out] buf @n A pointer to a buffer to receive incoming data.
 * @param [out] len @n The length, in bytes, of the data pointed to by the 'buf' parameter.
 * @param [in] timeout_ms @n Specify the timeout value in millisecond. In other words, the API block 'timeout_ms' millisecond maximumly.
 *
 * @retval       -2 : TCP connection error occur.
 * @retval       -1 : TCP connection be closed by remote server.
 * @retval        0 : No any data be received in 'timeout_ms' timeout period.
 * @retval (0, len] : The total number of bytes be received in 'timeout_ms' timeout period.

 * @see None.
 */
 int32_t HAL_TCP_Read(uintptr_t fd, char *buf, uint32_t len, uint32_t timeout_ms)
{
	u8 errorT=0;
	int32_t res=0;
	if(fd)
		{
retary:
		      res = Ql_SOC_Recv(m_SocketId, buf, len);
			  if(res<0)
			  	{
			  		errorT=0;
	 				 if(res == -2)
	 				  	{
	 				  		
	 				  		return 0;
	 				  	}
					 else
					 	{
					 		return res;
					 	}
			  	}
			  else if (res==0)
			  	{
			  		HAL_SleepMs(10);
			  		errorT++;
					if(errorT>10)
						{
							return res;
						}
			  		goto retary;
			  	}
		   	  else
		   	  	{
		   	  		errorT=0;
		   	  		return res;
		   	  	}
		}
	else
		{
			 PLATFORM_SOCK_LOG("<--HAL_TCP_Read Error fd. --> \r\n");
		}
    return -1;
}


/**
 * @brief Write data into the specific TCP connection.
 *        The API will return immediately if 'len' be written into the specific TCP connection.
 *
 * @param [in] fd @n A descriptor identifying a connection.
 * @param [in] buf @n A pointer to a buffer containing the data to be transmitted.
 * @param [in] len @n The length, in bytes, of the data pointed to by the 'buf' parameter.
 * @param [in] timeout_ms @n Specify the timeout value in millisecond. In other words, the API block 'timeout_ms' millisecond maximumly.
 *
 * @retval      < 0 : TCP connection error occur..
 * @retval        0 : No any data be write into the TCP connection in 'timeout_ms' timeout period.
 * @retval (0, len] : The total number of bytes be written in 'timeout_ms' timeout period.

 * @see None.
 */
int32_t HAL_TCP_Write(uintptr_t fd, const char *buf, uint32_t len, uint32_t timeout_ms)
{
	s32 ret = 0;
	if(fd)
		{
			ret = Ql_SOC_Send(m_SocketId, (u8*)buf, len);
		    if (ret == len)
		    {
		        PLATFORM_SOCK_LOG("<-- Send socket data successfully. --> \r\n");
		    }
		    else
		    {
		        PLATFORM_SOCK_LOG("<-- Fail to send socket data. --> \r\n");
		        Ql_SOC_Close(m_SocketId);
		        return -1;
		    } 
			
    		return len;
		}
	return -1;
}


/**
 * @brief Retrieves the number of milliseconds that have elapsed since the system was boot.
 *
 * @return the number of milliseconds.
 * @see None.
 * @note None.
 */
uint64_t HAL_UptimeMs(void)
{
	return SysOsTime*1000;
}


