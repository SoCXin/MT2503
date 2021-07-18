#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "ql_system.h"
#include "ql_gprs.h"
#include "ql_socket.h"
#include "aliot_platform_os.h"
#include "aliot_platform_network.h"
#include "sys.h"
#include "new_socket.h"
#include "user_mqtt.h"
#include "uart.h"


//#define DEBUG


#ifdef DEBUG
    #define PLATFORM_SOCK_LOG(format, ...) mprintf(format, /*__FILE__,*/ ##__VA_ARGS__)
#else
    #define PLATFORM_SOCK_LOG(format, ...) //aliot_platform_printlog("[SOCK]%d %s " format "\r\n", /*__FILE__,*/ __LINE__, __FUNCTION__, ##__VA_ARGS__)
#endif

static s32 pdpCntxtId=1;
static u8 m_SrvADDR[20] = "123.57.59.57\0";
static u32 m_SrvPort = 9006;
static u8  m_ipaddress[5];
static u8 ali_tcp_state = 0;

u8 TestMsg=0;

static s32 m_GprsActState = 0;    // GPRS PDP activation state, 0= not activated, 1=activated
static s32 m_SocketId = -1;       // Store socket Id that returned by Ql_SOC_Create()
static s32 m_SocketConnState = 0; // Socket connection state, 0= disconnected, 1=connected

/************************************************************************/
/* Definition for GPRS PDP context                                      */
/************************************************************************/
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


#if 1
intptr_t aliot_platform_tcp_establish(const char* host, uint16_t port)
{
    s32 ret;
	m_SocketId = -1;
	my_tcp_state=STATE_NW_GET_SIMSTATE;
	GSM_InitModule();
	while (my_tcp_state != STATE_GPRS_REGISTER)
		{
		   PLATFORM_SOCK_LOG("Search the net....");
		   GSM_InitModule();
		   aliot_platform_msleep(200);
		}
	
    pdpCntxtId = 1;
    if (GPRS_PDP_ERROR == pdpCntxtId)
    {
        PLATFORM_SOCK_LOG("No PDP context is available\r\n");
        return NULL;
    }
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
    //2. Configure PDP
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

    //3. Activate GPRS PDP context
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

    //4. Register Socket callback
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

    //5. Create socket
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
        if (SOC_SUCCESS == ret)   // ip address is xxx.xxx.xxx.xxx
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

#endif


int32_t aliot_platform_tcp_destroy(uintptr_t handle)
{
    s32 ret;
    //8. Close socket
    ret = Ql_SOC_Close(m_SocketId);
    PLATFORM_SOCK_LOG("<-- Close socket[%d], cause=%d --> \r\n", m_SocketId, ret);
	if(ret!=0)return ret;

    //9. Deactivate GPRS
    PLATFORM_SOCK_LOG("<-- Deactivating GPRS... -->\r\n");
    ret = Ql_GPRS_DeactivateEx(pdpCntxtId, TRUE);
    PLATFORM_SOCK_LOG("<-- Deactivated GPRS, cause=%d -->\r\n\r\n", ret);
	if(ret!=0)return ret;

    return 0;
}
int32_t aliot_platform_tcp_write(uintptr_t fd, const char* buf, uint32_t len, uint32_t timeout_ms)
{
    s32 ret = 0;
    u64 ackNum = 0;
	
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



int32_t aliot_platform_tcp_read(uintptr_t fd, char* buf, uint32_t len, uint32_t timeout_ms)
{
	u8 errorT=0;
	s32 offset = 0;
	u32 ackT = timeout_ms;
	int32_t res=0;
	if(fd)
		{
retary:
		      res = Ql_SOC_Recv(m_SocketId, buf, len);
			  if(res<0)
			  	{
			  		errorT=0;
			  		SocketBufRdBit=0;
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
			  		aliot_platform_msleep(10);
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
    return -1;
}



