#include "custom_feature_def.h"
#include "ql_stdlib.h"
#include "ql_common.h"
#include "ql_type.h"
#include "ql_trace.h"
#include "ql_error.h"
#include "ql_uart.h"
#include "ql_uart.h"
#include "ql_gprs.h"
#include "ql_socket.h"
#include "ql_timer.h"
#include "ril_sim.h"
#include "ril_network.h"
#include "ril.h"
#include "ril_util.h"
#include "ril_telephony.h"
#include "sys.h"
#include "uart.h"
#include "new_socket.h"


u8 my_tcp_state = STATE_NW_GET_SIMSTATE;
static ST_GprsConfig  m_gprsCfg;
static u8  m_ipaddress[5]; 
u8 my_GprsStatue=1;
s32 new_socketid = -1; 
u8 myiplist[32]={0};


u8 GSM_InitModule(void)
{
	s32 ret;
	s32 pdpCntxtId=0;
	switch (my_tcp_state)
		{
			 case STATE_NW_GET_SIMSTATE:
			 	{
					 s32 simStat = 0;
					 RIL_SIM_GetSimState(&simStat);
					 if (simStat == SIM_STAT_READY)
					  	{
					  		 my_tcp_state = STATE_NW_QUERY_STATE;
							 mprintf("SIM card status is normal!\r\n");
							 
					  	}
					  else
					  	{
					  		 mprintf("SIM card status is unnormal!\r\n");
					  	}
					 break;
			 	}
			   case STATE_NW_QUERY_STATE:
			   	{
					 s32 creg = 0;
					 s32 cgreg = 0;
					 ret = RIL_NW_GetGSMState(&creg);
					 ret = RIL_NW_GetGPRSState(&cgreg);
					 mprintf("Network State:creg=%d,cgreg=%d\r\n",creg,cgreg);
					   if((cgreg == NW_STAT_REGISTERED)||(cgreg == NW_STAT_REGISTERED_ROAMING))
					   	{
					   		my_tcp_state = STATE_GPRS_REGISTER;
					   	}
					   break;
			   	}
			default:
                break;
		}
	
}


s8 Check_SystemGprsSta(s32 timeout)
{
	 s32 scond=timeout*2;
	 my_tcp_state=STATE_NW_GET_SIMSTATE;
	 GSM_InitModule();
	 while (my_tcp_state != STATE_GPRS_REGISTER&&scond>0)
	 	{
	 	   mprintf("Search the net....");
		   GSM_InitModule();
		   Ql_Sleep(500);
		   scond--;
	 	}
	 if(my_tcp_state != STATE_GPRS_REGISTER)return -1;
	 return 0;
}





