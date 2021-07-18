#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "ql_trace.h"
#include "ql_uart.h"
#include "ql_system.h"
#include "ql_stdlib.h"
#include "ql_error.h"
#include "ql_gpio.h"
#include "sys.h"
#include "ril.h"
#include "ql_socket.h"
#include "m203c_ble.h"
#include "m203c_module_cfg.h"
#include "m203c_module_at.h"
#include "uart.h"
#include "ril_bluetooth.h"
#include "ril_ble.h"
#include "motor.h"

#define _BLE_SCANF_DEBUG_

#define ble_at_print(format, ...) mprintf( format, ##__VA_ARGS__)

#ifdef _BLE_SCANF_DEBUG_
#define ble_scan_print(format, ...) mprintf( format, ##__VA_ARGS__)
#else
#define ble_scan_print(format, ...)
#endif

#define SYSTEM_DELAY_MS(nms) Ql_Sleep(nms)

ble_gap_set_adv   ble_adv_data;

ble_gap_evt_adv_report_t   adv_report;
ST_BLE_Server BikeBleServer= {0};
BLE_SERVER_STATE_t ble_server_sta=BLESERVER_DEVICE_INIT0;

u8 Bike_BLERD_Buffer[64];
u8 Bike_BLERD_Length = 0;


static s32 ATRsp_QBTGatsreg_Hdlr_203C(char* line, u32 len, void* param)
{

	ble_scan_print("%s",line);
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

s32 RIL_BT_Gatsreg_203C(u8 op , u8 *name)
{
    char strAT[100];
    s32 ret;

 
    Ql_memset(strAT, 0x0, sizeof(strAT));
    
    Ql_sprintf(strAT, "AT+QBTGATCREG=%d,\"%s\"", op,name);

    ret = Ql_RIL_SendATCmd(strAT, Ql_strlen(strAT), ATRsp_QBTGatsreg_Hdlr_203C,NULL,0);
    
    return ret;
    
}

void Ble_NameSet(void)
{
	s32 ret;
	u8 bufp[20]={0};
	u8 *p=bufp;
	if(systemset.snuser)
		{
			Ql_strcpy(bufp,systemset.SN);
		}
	else
		{
			ret =RIL_GetIMEI(bufp);
			if(ret!=RIL_AT_SUCCESS)
			 	{
			 		ble_at_print("RIL_GetIMEI Error");
			 	}
			if(bufp[0]==0x0D&&bufp[1]==0x0A)
				{
					p+=4;
				}
			else
				{
					p+=2;
				}
		}
	ret = RIL_BT_SetName(p,Ql_strlen(p));
    if(RIL_AT_SUCCESS == ret)
        {
            ble_at_print("BT device name set successful.\r\n");
        }
    else
        {
            ble_at_print("BT device name set failed,ret=%d.\r\n",ret);
        }

}


void Ble_PowerOn(void)
{
	s32 cur_pwrstate = 0 ;
	s32 ret = RIL_AT_SUCCESS ;
	ret = RIL_BT_GetPwrState(&cur_pwrstate);
	if(RIL_AT_SUCCESS != ret) 
    {
        ble_at_print("Get BT device power status failed.\r\n");
        return;
    }
	if(1 == cur_pwrstate)
    {
        ble_at_print("BT device already power on.\r\n");
    }
    else if(0 == cur_pwrstate)
    {
       ret = RIL_BT_Switch(1);
       if(RIL_AT_SUCCESS != ret)
       {
            ble_at_print("BT power on failed,ret=%d.\r\n",ret);
            return;
       }
       ble_at_print("BT device power on.\r\n");
    }

    RIL_BT_GetPwrState(&cur_pwrstate);
    mprintf("BT power  cur_pwrstate=%d.\r\n",cur_pwrstate);
	
}

void Ble_PowerOff(void)
{
	s32 cur_pwrstate = 0 ;
	s32 ret = RIL_AT_SUCCESS ;
	ret = RIL_BT_GetPwrState(&cur_pwrstate);
	if(RIL_AT_SUCCESS != ret) 
    {
        ble_at_print("Get BT device power status failed.\r\n");
        return;
    }
	if(1 == cur_pwrstate)
    {
        ble_at_print("BT device already power on.\r\n");
		ret = RIL_BT_Switch(0);
       if(RIL_AT_SUCCESS != ret)
       {
            mprintf("BT power on failed,ret=%d.\r\n",ret);
            return;
       }
       ble_at_print("BT device power off.\r\n");
    }
}

u8 ScanBlecom(u8 op , u8 *name,u32 timeout)
{
	u8 i;
	char strAT[100];
    s32 ret;
    Ql_memset(strAT, 0x0, sizeof(strAT));
	Ql_sprintf(strAT, "AT+QBTGATCSCAN=%d,\"%s\"", op,name);
	ble_scan_print("%s",strAT);
	ret = Ql_RIL_SendATCmd(strAT, Ql_strlen(strAT), ATRsp_QBTGatsreg_Hdlr_203C,NULL,0);
	return 0;
}



void GetBleInfos(u8 *buf)
{
	
	
}

/*
*
+QBTGATCSCAN: "ABCD",D2D1E69805C9,89,02010608FF58000000877DC608093838373935353811079FCADC240EE5A9E093F3A3B50100406E

*/

void ble_adv_report_encode(ble_gap_evt_adv_report_t *adv_data,u8 *bufs)
{
	u16 i=0,j=0;
	s32 rssi=0;
	u8 uuidtstrbuf[500];
	u8 uuidthexbuf[300];
	Ql_memset(uuidtstrbuf,0,sizeof(uuidtstrbuf));
	Ql_memset(uuidthexbuf,0,sizeof(uuidthexbuf));
	Ql_sscanf(bufs, "%*[^:]: %*[^,],%*[^,],%d,%*[^\r\n]\r\n",&rssi);
	Ql_sscanf(bufs, "%*[^:]: %*[^,],%[^,],%*[^,],%*[^\r\n]\r\n",adv_data->peer_addr.addr);
	Ql_sscanf(bufs, "%*[^:]: %*[^,],%*[^,],%*[^,],%[^\r\n]\r\n",uuidtstrbuf);
	adv_data->rssi=rssi-127;
	StrToHex(uuidthexbuf,uuidtstrbuf,Ql_strlen(uuidtstrbuf)/2);
	ble_scan_print("adv_data->peer_addr.addr=%s\r\n",adv_data->peer_addr.addr);
	ble_scan_print("uuidtstrbuf=%s\r\n",uuidtstrbuf);
	ble_scan_print("rssi=%d\r\n",adv_data->rssi);
	ble_scan_print("\r\n");
	for(i=0;i<Ql_strlen(uuidtstrbuf)/2;i++)ble_scan_print("%02X ",uuidthexbuf[i]);
	ble_at_print("\r\n");
	i=0;
	adv_data->ble_gap_adv_info_num=0;
	while(i<(Ql_strlen(uuidtstrbuf)/2))
		{
			ble_scan_print("\r\n");
			adv_data->peer_adv_info[adv_data->ble_gap_adv_info_num].dlen=uuidthexbuf[i++];
			adv_data->peer_adv_info[adv_data->ble_gap_adv_info_num].type=uuidthexbuf[i++];
			if(adv_data->peer_adv_info[adv_data->ble_gap_adv_info_num].dlen>BLE_GAP_ADV_MAX_SIZE)break;
			ble_scan_print("adv_data->peer_adv_info[%d].type=%02X\r\n",adv_data->ble_gap_adv_info_num,adv_data->peer_adv_info[adv_data->ble_gap_adv_info_num].type);
			ble_scan_print("adv_data->peer_adv_info[%d].dlen=%02X\r\n",adv_data->ble_gap_adv_info_num,adv_data->peer_adv_info[adv_data->ble_gap_adv_info_num].dlen);
			adv_data->peer_adv_info[adv_data->ble_gap_adv_info_num].dlen-=1;
			for(j=0;j<adv_data->peer_adv_info[adv_data->ble_gap_adv_info_num].dlen;j++)
				{
					adv_data->peer_adv_info[adv_data->ble_gap_adv_info_num].data[j]=uuidthexbuf[i++];
					ble_scan_print("%02X ",adv_data->peer_adv_info[adv_data->ble_gap_adv_info_num].data[j]);
				}
			adv_data->ble_gap_adv_info_num++;
			ble_scan_print("\r\n");
		}
}


/*
*	蓝牙扫描结果通过URC上报回来
*/

void OnURCHandler_BLEScan(const char* strURC, void* reserved)
{
	u8 *p;
	bool isFind=FALSE;
	ble_scan_print("%s",strURC);
	p=(u8*)strstr((const char *)strURC,"+QBTGATCSCAN: \"ABCD\",");
	if(p!=NULL)
		{
			ble_adv_report_encode(&adv_report,strURC);
			
		}
}


/*
*	蓝牙连接结果\接收数据结果\请求结果
*/


static void Bike_BLE_Callback(s32 event, s32 errCode, void* param1, void* param2)
{
	u8 s_index,c_index;
    
      switch(event)
    {
        case MSG_BLE_CONNECT :
            {
                ST_BLE_ConnStatus *conn = (ST_BLE_ConnStatus *)param2;
                if(Ql_StrPrefixMatch((char *)param1, BikeBleServer.gserv_id))
                {
                    Ql_memcpy(&BikeBleServer.conn_status,conn,sizeof(ST_BLE_ConnStatus));
                }
            ble_at_print("sever_id:%s,connect_id:%d,connect_status=%d\r\n",BikeBleServer.gserv_id,BikeBleServer.conn_status.connect_id,BikeBleServer.conn_status.connect_status);    
            break;
           }
        case MSG_BLE_WREG_IND :   
            if(Ql_StrPrefixMatch((char*)param1, BikeBleServer.gserv_id))
                { 
                    Ql_memcpy(&BikeBleServer.wrreq_param,(ST_BLE_WRreq*)param2,sizeof(ST_BLE_WRreq));
                    Ql_memset(Bike_BLERD_Buffer, 0, sizeof(Bike_BLERD_Buffer));
		            Bike_BLERD_Length = Ql_strlen(BikeBleServer.wrreq_param.value) / 2;
		            StrToHex(Bike_BLERD_Buffer, BikeBleServer.wrreq_param.value, Bike_BLERD_Length);
		            ble_at_print("data from clien:%s\r\n", BikeBleServer.wrreq_param.value);
		            ble_server_sta = BLESERVER_DEVICE_GATT_WREG_RESP;
					SendMsg2KernelForBLEStart();
                }
            break;

        case MSG_BLE_RREG_IND:
           if(Ql_StrPrefixMatch((char*)param1, BikeBleServer.gserv_id))
            {
                Ql_memcpy(&BikeBleServer.wrreq_param,(ST_BLE_WRreq*)param2,sizeof(ST_BLE_WRreq));
                ble_server_sta=BLESERVER_DEVICE_GATT_RREG_RESP;
				SendMsg2KernelForBLEStart();
            }

            break;
        case MSG_BLE_PXP_CONNECT:
            ble_at_print("dev addr:%s\r\n",(char*)param1);
            break;

        case MSG_BLE_FMP_CONNECT:
            ble_at_print("dev addr:%s\r\n",(char*)param1);
            break;
		
         case MSG_BLE_EWREG_IND:
            
            break;
                  
        default :
            break;
    }
}


/*
*	蓝牙Client模式初始化,初始化完成后开机自动扫描广播
*/

void Ble_Client_App(void)
{
	u8 res;
	res=ScanBlecom(1,"ABCD",200);
	if(res==0)ScanBlecom(0,"ABCD",150);
}

void Ble_Client_Test(void)
{	
	u8 op=1;
	u8 *name="ABCD";
	char strAT[100];
    s32 ret;
	Ble_PowerOn();
	Ql_memset(strAT, 0x0, sizeof(strAT));
	Ql_sprintf(strAT, "AT+QBTGATCSCAN=%d,\"%s\"", op,name);
	ble_scan_print("%s",strAT);
	ret = Ql_RIL_SendATCmd(strAT, Ql_strlen(strAT), ATRsp_QBTGatsreg_Hdlr_203C,NULL,0);
	ble_at_print("Ble_Client_Test ret=%d\r\n",ret);
}

void Ble_Init_AdvData(u16 devicesinfo,u8 locksta,u8* uuids)
{
    RIL_BT_Gatsl(0, &BikeBleServer);
    ble_adv_data.manufacture_data.devicesinfo = devicesinfo;
    ble_adv_data.manufacture_data.locksta = locksta;
	 ble_adv_data.services_data = "\0";
    ble_adv_data.appearance = 25;
    ble_adv_data.Complete_UUIDs[0] = uuids[0];
    ble_adv_data.Complete_UUIDs[1] = uuids[1];
    ble_server_sta = BLESERVER_DEVICE_SETADV;
}

s32 Ble_Set_AdvData(void)
{
    u8 temp_data[31] = {0};
    Ql_sprintf(temp_data, "%04X",ble_adv_data.manufacture_data.locksta);
    return RIL_BT_QGatSetadv(BikeBleServer.gserv_id, ble_adv_data.appearance, 0, temp_data, ble_adv_data.services_data, ble_adv_data.Complete_UUIDs[0] << 8 | ble_adv_data.Complete_UUIDs[1]);

}

/*
*   设置蓝牙广播包名字开关op=1打开 op=0关闭
*/

void Ble_Set_AdvNameVisib(u8 op)
{
	u8 at_data[20] = {0};
	Ql_sprintf(at_data,"AT+QBTCFG=\"advname\",%d",op);
	Ql_RIL_SendATCmd(at_data, Ql_strlen(at_data), NULL, NULL, 0);
}



void Ble_InitConnect_Start(void)
{
	u32 Gatsind_cnt=0;
	u8 s_i=0,c_i=0,d_id=0;
	s32 ret = RIL_AT_SUCCESS ;
	u8 num[2] = {0xFC, 0x32};
	do {
			switch (ble_server_sta)
	 	{
	 		case BLESERVER_DEVICE_PREINIT0:
				Ble_PowerOff();
				ble_server_sta=BLESERVER_DEVICE_INIT0;
				break;
	 		case BLESERVER_DEVICE_INIT0:
				Ql_memset(&BikeBleServer,0,sizeof(BikeBleServer));
				Ble_PowerOn();
				Ble_NameSet();
				ret = RIL_BLE_Initialize(Bike_BLE_Callback);
				if(RIL_AT_SUCCESS != ret) 
			    {
			        ble_at_print("BT initialization failed.\r\n");
			        break;
			    }
			    ble_at_print("BT callback function register successful.\r\n");
			    BikeBleServer.gserv_id[0]='A';
			    BikeBleServer.gserv_id[1]='B';
			    BikeBleServer.gserv_id[2]='\0';
			    BikeBleServer.sid=0;  
			    BikeBleServer.service_id[BikeBleServer.sid].service_uuid=0x6618;
			    BikeBleServer.service_id[BikeBleServer.sid].num_handles=5;
			    BikeBleServer.service_id[BikeBleServer.sid].is_primary=1;
			    BikeBleServer.service_id[BikeBleServer.sid].inst=15;
			    BikeBleServer.service_id[BikeBleServer.sid].cid=0;
			    c_i= BikeBleServer.service_id[BikeBleServer.sid].cid;
			    BikeBleServer.service_id[BikeBleServer.sid].char_id[c_i].char_uuid=0x232a;
			    BikeBleServer.service_id[BikeBleServer.sid].char_id[c_i].inst=60;
			    BikeBleServer.service_id[BikeBleServer.sid].char_id[c_i].prop=58;
			    BikeBleServer.service_id[BikeBleServer.sid].char_id[c_i].permission=17;
			    BikeBleServer.service_id[BikeBleServer.sid].char_id[c_i].did=0;
			    d_id =BikeBleServer.service_id[BikeBleServer.sid].char_id[c_i].did;
			    BikeBleServer.service_id[BikeBleServer.sid].char_id[c_i].desc_id[d_id].desc_uuid=0x1329;
			    BikeBleServer.service_id[BikeBleServer.sid].char_id[c_i].desc_id[d_id].inst=30;
			    BikeBleServer.service_id[BikeBleServer.sid].char_id[c_i].desc_id[d_id].permission=17;
			    BikeBleServer.service_id[BikeBleServer.sid].transport=0;
				ble_server_sta=BLESERVER_DEVICE_INIT;
				break;
			  case BLESERVER_DEVICE_INIT:
            ret = RIL_BT_Gatsreg(1, &BikeBleServer);
            if (RIL_AT_SUCCESS != ret)
            {
                ble_at_print("RIL_BT_Gatsreg failed.\r\n");
				Ble_PowerOff();
				ble_server_sta=BLESERVER_DEVICE_INIT0;
                break;
            }
            ret = RIL_BT_Gatss(1, &BikeBleServer);
            if (RIL_AT_SUCCESS != ret)
            {
                ble_at_print("RIL_BT_Gatss failed.\r\n");
				Ble_PowerOff();
				ble_server_sta=BLESERVER_DEVICE_INIT0;
                break;
            }
            ret = RIL_BT_Gatsc(1, &BikeBleServer);
            if (RIL_AT_SUCCESS != ret)
            {
                ble_at_print("RIL_BT_Gatsc failed.\r\n");
				Ble_PowerOff();
				ble_server_sta=BLESERVER_DEVICE_INIT0;
                break;
            }
            ret = RIL_BT_Gatsd(1, &BikeBleServer);
            if (RIL_AT_SUCCESS != ret)
            {
                ble_at_print("RIL_BT_Gatsd failed.\r\n");
				Ble_PowerOff();
				ble_server_sta=BLESERVER_DEVICE_INIT0;
                break;
            }
            ble_server_sta = BLESERVER_DEVICE_GATST;
            break;
        case BLESERVER_DEVICE_GATST:
            ret = RIL_BT_Gatsst(1, &BikeBleServer);
            if (RIL_AT_SUCCESS != ret)
            {
                ble_at_print("RIL_BT_Gatsst failed.\r\n");
				Ble_PowerOff();
				ble_server_sta=BLESERVER_DEVICE_INIT0;
                break;
            }
            ble_server_sta = BLESERVER_DEVICE_LOOP;
			if(systemset.snuser)Gatsind_cnt=0xA1;
			else Gatsind_cnt=0xA0;
            Ble_Init_AdvData(0x5800, Gatsind_cnt,num);     //组包  自己加入设备命
            break;
        case BLESERVER_DEVICE_SETADV:
			Ble_Set_AdvNameVisib(1);
            ret = Ble_Set_AdvData();
            if (RIL_AT_SUCCESS != ret)
            {
                ble_at_print("Ble_Set_AdvData failed.\r\n");
                break;
            }
            ret = RIL_BT_Gatsl(1, &BikeBleServer);
            ble_at_print("RIL_BT_Gatsl =%d\r\n", ret);
            ble_server_sta = BLESERVER_DEVICE_LOOP;
            break;
        case  BLESERVER_DEVICE_GATT_WREG_RESP:
	        {
	            char testdata[32] = "1234";
	            BikeBleServer.wrreq_param.need_rsp = 0;
	            Ql_memcpy(BikeBleServer.wrreq_param.value, testdata, sizeof(testdata));
	            ret = RIL_BT_Gatsrsp(&BikeBleServer);
	            if (RIL_AT_SUCCESS != ret)ble_at_print("RIL_BT_Gatsrsp %d,%d\r\n", ret, __LINE__);
	            nus_data_handler(Bike_BLERD_Buffer, Bike_BLERD_Length);
	        }
	        if(ble_server_sta==BLESERVER_DEVICE_GATT_WREG_RESP)ble_server_sta = BLESERVER_DEVICE_LOOP;
			 ble_at_print("BLESERVER_DEVICE_GATT_WREG_RESP OK\r\n");
              break;
		default:
			 	ble_at_print("ble_server_sta= %d,%d", __LINE__,ble_server_sta);
            	break;
	 	}
		SYSTEM_DELAY_MS(10);
		
	}while(ble_server_sta!=BLESERVER_DEVICE_LOOP);
}


void ble_nus_string_send(u8* sendbuf, u8 len)
{
    s32 ret = RIL_AT_SUCCESS;
    Hex2Str(sendbuf, BikeBleServer.wrreq_param.value, len);
	ble_at_print("%s",BikeBleServer.wrreq_param.value);
    BikeBleServer.sid = 0;
    BikeBleServer.service_id[BikeBleServer.sid].cid = 0;
    BikeBleServer.wrreq_param.attr_handle = BikeBleServer.service_id[BikeBleServer.sid].char_id[BikeBleServer.service_id[BikeBleServer.sid].cid].char_handle;
    BikeBleServer.wrreq_param.need_cnf = 0;
    if (BikeBleServer.conn_status.connect_id == 1)
    {
        ret = RIL_BT_Gatsind(&BikeBleServer);
        if (RIL_AT_SUCCESS != ret)
        {
            ble_at_print("\r\n<--RIL_BT_Gatsind: ret=%d,%d -->\r\n", ret, __LINE__);
        }
        else
        {
            ble_at_print("\r\n<ble_nus_string_send ok>\r\n");
        }
    }
}



/*
****************************************************************************************
*函数原形: void nus_data_handler(ble_nus_t * p_nus, uint8_t * p_data, uint16_t length)
*入口参数: p_nus    Nordic UART Service structure.
*                       p_data   Data to be send to UART module.    length   Length of the data.
*出口参数: 无
*功能描述: 蓝牙接收数据后会调用这个函数
****************************************************************************************
*/
void nus_data_handler(uint8_t* p_data, uint16_t length)
{
	Ble_zyf_checkble_data(p_data,length);
}


