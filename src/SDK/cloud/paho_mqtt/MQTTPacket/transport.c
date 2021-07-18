/*******************************************************************************
 * Copyright (c) 2014 IBM Corp.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * and the Eclipse Distribution License is available at
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Ian Craggs - initial API and implementation and/or initial documentation
 *    Sergio R. Caprile - "commonalization" from prior samples and/or documentation extension
 *******************************************************************************/

#if !defined(SOCKET_ERROR)
	/** error in socket operation */
	#define SOCKET_ERROR -1
#endif


#define INVALID_SOCKET -1
#include <stdint.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "ril.h"
#include "ril_util.h"
#include "ril_telephony.h"
#include "ql_stdlib.h"
#include "ql_error.h"
#include "ql_trace.h"
#include "ql_uart.h"
#include "ql_system.h"
#include "user_mqtt.h"
#include "sys.h"
#include "uart.h"
#include "new_socket.h"
#include "ql_gprs.h"
#include "ql_socket.h"

#define PAHO_DEBUG

#ifdef PAHO_DEBUG
    #define PAHO_LOG(format, ...) mprintf(format , ##__VA_ARGS__)
#else
    #define PAHO_LOG(format, ...) 
#endif


/**
This simple low-level implementation assumes a single connection for a single thread. Thus, a static
variable is used for that connection.
On other scenarios, the user must solve this by taking into account that the current implementation of
MQTTPacket_read() has a function pointer for a function call to get the data to a buffer, but no provisions
to know the caller or other indicator (the socket id): int (*getfn)(unsigned char*, int)
*/
static int mysock = INVALID_SOCKET;
static s32 pdpCntxtId=1;
static s32 m_GprsActState = 0;    // GPRS PDP activation state, 0= not activated, 1=activated
static s32 m_SocketConnState = 0; // Socket connection state, 0= disconnected, 1=connected
static u8  m_ipaddress[5];


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
        PAHO_LOG("<--CallBack: active GPRS successfully.-->\r\n");
        m_GprsActState=1;
        
    }else
    {
        PAHO_LOG("<--CallBack: active GPRS failed,errCode=%d-->\r\n",errCode);
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
        PAHO_LOG("<--CallBack: deactivated GPRS successfully.-->\r\n");
    }
    else
    {
        PAHO_LOG("<--CallBack: fail to deactivate GPRS, cause=%d)-->\r\n", errCode);
    }
    if (1 == m_GprsActState)
    {
        m_GprsActState = 0;
        PAHO_LOG("<-- GPRS drops down -->\r\n");
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
        PAHO_LOG("<--CallBack: close socket successfully.-->\r\n");
    }
    else if (errCode == SOC_BEARER_FAIL)
    {
        PAHO_LOG("<--CallBack: fail to close socket,(socketId=%d,error_cause=%d)-->\r\n", socketId, errCode);
    }
    else
    {
        PAHO_LOG("<--CallBack: close socket failure,(socketId=%d,error_cause=%d)-->\r\n", socketId, errCode);
    }
    if (1 == m_SocketConnState)
    {
        PAHO_LOG("<-- Socket connection is disconnected -->\r\n");
        PAHO_LOG("<-- Close socket at module side -->\r\n");
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
        PAHO_LOG("<-- Socket error(error code:%d), close socket.-->\r\n", errCode);
        Ql_SOC_Close(socketId);
        mysock = -1;
    }
    else
    {
        PAHO_LOG("<-- You can continue to send data to socket -->\r\n");
    }
}


static void Callback_Socket_read(s32 socketId, s32 errCode, void* customParam )
{
	s32 ret;
    u32 packetLen=0;
    PAHO_LOG("!!!!Data from M2M!!!\r\n");
	//RdsubData();
}

static void callback_socket_connect(s32 socketId, s32 errCode, void* customParam )
{
    if (errCode == SOC_SUCCESS)
    {
    	 m_SocketConnState = 1;
        PAHO_LOG("<--Callback: socket connect successfully.-->\r\n");          
       
    }else
    {
        PAHO_LOG("<--Callback: socket connect failure,(socketId=%d),errCode=%d-->\r\n",socketId,errCode);
       
    }
    
}

static void callback_socket_accept(s32 listenSocketId, s32 errCode, void* customParam )
{
    PAHO_LOG("callback_socket_accept\r\n");
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
int transport_sendPacketBuffer(int sock, unsigned char* buf, int buflen)
{
	int ret = 0;
	
	ret = Ql_SOC_Send(mysock, (u8*)buf, buflen);
	return ret;
			
}


int transport_getdata(unsigned char* buf, int count)
{
	int res=-1;
	u32 errorT=0;
	while(res!=count)
		{
			res=Ql_SOC_Recv(mysock,buf,count);
			Ql_Sleep(10);
			errorT++;
			if(errorT>100*15)
				{
					break;
				}
		}
	return  res;
}


int transport_getdata_1(unsigned char* buf, int count)
{
	int res=-1;
	u32 errorT=0;
retary:
	res = Ql_SOC_Recv(mysock, buf, count);
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
			 Ql_Sleep(10);
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
	return  res;
}


int transport_getdatanb(void *sck, unsigned char* buf, int count)
{
	int sock = *((int *)sck); 	/* sck: pointer to whatever the system may use to identify the transport */
	/* this call will return after the timeout set on initialization if no bytes;
	   in your system you will use whatever you use to get whichever outstanding
	   bytes your socket equivalent has ready to be extracted right now, if any,
	   or return immediately */
	int rc = Ql_SOC_Recv(mysock,buf,count);
	if (rc == -2) {
		/* check error conditions from your system here, and return -1 */
		return 0;
	}
	return rc;
}

/**
return >=0 for a socket descriptor, <0 for an error code
@todo Basically moved from the sample without changes, should accomodate same usage for 'sock' for clarity,
removing indirections
*/
int transport_open(char* addr, int port)
{
	s32 ret;
	my_tcp_state=STATE_NW_GET_SIMSTATE;
	GSM_InitModule();
	while (my_tcp_state != STATE_GPRS_REGISTER)
		{
		   PAHO_LOG("Search the net....");
		   GSM_InitModule();
		   Ql_Sleep(200);
		}
    pdpCntxtId = 1;
	  if (GPRS_PDP_ERROR == pdpCntxtId)
    {
        PAHO_LOG("No PDP context is available\r\n");
        return INVALID_SOCKET;
    }
    ret = Ql_GPRS_Register(pdpCntxtId, &callback_gprs_func, NULL);
    if (GPRS_PDP_SUCCESS == ret)
    {
        PAHO_LOG("<-- Register GPRS callback function -->\r\n");
    }
    else  if (GPRS_PDP_ALREADY == ret)
    {
    	PAHO_LOG("<--ALREADY to register GPRS-->\r\n");
    }
	else
	{
		PAHO_LOG("<-- Fail to register GPRS, cause=%d. -->\r\n", ret);
       	return INVALID_SOCKET;
	}
    //2. Configure PDP
    ret = Ql_GPRS_Config(pdpCntxtId, &m_GprsConfig);
    if (GPRS_PDP_SUCCESS == ret)
    {
        PAHO_LOG("<-- Configure PDP context -->\r\n");
    }
    else
    {
        PAHO_LOG("<-- Fail to configure GPRS PDP, cause=%d. -->\r\n", ret);
       return INVALID_SOCKET;
    }

    //3. Activate GPRS PDP context
    PAHO_LOG("<-- Activating GPRS... -->\r\n");
    ret = Ql_GPRS_ActivateEx(pdpCntxtId, TRUE);
    if (ret == GPRS_PDP_SUCCESS)
    {
        m_GprsActState = 1;
        PAHO_LOG("<-- Activate GPRS successfully. -->\r\n\r\n");
    }
	else if (GPRS_PDP_ALREADY == ret)
	{
		 m_GprsActState = 1;
		PAHO_LOG("<--GPRS callback function has already been registered,ret=%d,pdpCntxtId=%d-->\r\n",ret,pdpCntxtId);
					                   
	}
    else
    {
        PAHO_LOG("<-- Fail to activate GPRS, cause=%d. -->\r\n\r\n", ret);
       return INVALID_SOCKET;
    }

    //4. Register Socket callback
    ret = Ql_SOC_Register(callback_soc_func, NULL);
    if (SOC_SUCCESS == ret)
    {
        PAHO_LOG("<-- Register socket callback function -->\r\n");
    }
	else if (SOC_ALREADY == ret)
    {
        PAHO_LOG("<-- Register socket callback function -->\r\n");
    }
    else
    {
        PAHO_LOG("<-- Fail to register socket callback, cause=%d. -->\r\n", ret);
        return NULL;
    }

    //5. Create socket
    mysock = Ql_SOC_Create(pdpCntxtId, SOC_TYPE_TCP);
    if (mysock >= 0)
    {
        PAHO_LOG("<-- Create socket successfully, socket id=%d. -->\r\n", mysock);
    }
    else
    {
        PAHO_LOG("<-- Fail to create socket, cause=%d. -->\r\n", mysock);
        return INVALID_SOCKET;
    }

    //6. Connect to server
    {
        //6.1 Convert IP format
        Ql_memset(m_ipaddress, 0, 5);
		PAHO_LOG("host=%s",addr);
        ret = Ql_IpHelper_ConvertIpAddr(addr, (u32*)m_ipaddress);
        if (SOC_SUCCESS == ret)   // ip address is xxx.xxx.xxx.xxx
        {
            PAHO_LOG("<-- Convert Ip Address successfully,m_ipaddress=%d,%d,%d,%d -->\r\n", m_ipaddress[0], m_ipaddress[1], m_ipaddress[2], m_ipaddress[3]);
        }
        else
        {
        	 s32 count=0;
             ret = Ql_IpHelper_GetIPByHostNameEx(pdpCntxtId, 0, addr, &count,m_ipaddress);
			 if(ret == SOC_SUCCESS)
			      {
			           PAHO_LOG("<--Get ip by hostname successfully.-->\r\n");
			      }
			  else
			      {
			           PAHO_LOG("<--Get ip by hostname failure:ret=%d-->\r\n",ret);
			           return INVALID_SOCKET;
			       }
        }

        //6.2 Connect to server
        PAHO_LOG("<-- Connecting to server(IP:%d.%d.%d.%d, port:%d)... -->\r\n", m_ipaddress[0], m_ipaddress[1], m_ipaddress[2], m_ipaddress[3], port);
        ret = Ql_SOC_ConnectEx(mysock, (u32)m_ipaddress, port, TRUE);
        if (SOC_SUCCESS == ret)
        {
            m_SocketConnState = 1;
            PAHO_LOG("<-- Connect to server successfully -->\r\n");
			return mysock;
        }
        else
        {
            PAHO_LOG("<-- Fail to connect to server, cause=%d -->\r\n", ret);
            return INVALID_SOCKET;
        }
    }
	return mysock;
}

int transport_close(int sock)
{
	return Ql_SOC_Close(sock);
}
