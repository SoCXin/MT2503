#ifndef __NEW_SOCKET_H__
#define __NEW_SOCKET_H__


#define REBUFLEN 200

typedef enum{
    STATE_NW_GET_SIMSTATE,
    STATE_NW_QUERY_STATE,
    STATE_GPRS_REGISTER,
    STATE_GPRS_CONFIG,
    STATE_GPRS_ACTIVATE,
    STATE_GPRS_ACTIVATING,
    STATE_GPRS_GET_DNSADDRESS,
    STATE_GPRS_GET_LOCALIP,
    STATE_CHACK_SRVADDR,
    STATE_SOC_REGISTER,
    STATE_SOC_CREATE,
    STATE_SOC_CONNECT,
    STATE_SOC_CONNECTING,
    STATE_SOC_SEND,
    STATE_SOC_SENDING,
    STATE_SOC_ACK,
    STATE_SOC_CLOSE,
    STATE_GPRS_DEACTIVATE,
    STATE_TOTAL_NUM
}Enum_TCPSTATE;

extern u8 my_tcp_state ;
extern u8 my_GprsStatue;
extern s32 new_socketid;

extern u8 my_tcp_state;
void GPRS_Send_Data(u8 *buf,u8 len);
s8 Check_SystemGprsSta(s32 timeout);

#endif


