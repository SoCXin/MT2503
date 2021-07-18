/******************************************************************************

                  版权所有 (C) 2018, 天创金农科技

 ******************************************************************************
  文件   : user_mqtt.c
  版本   : 初稿
  作者   : LiCM
  日期   : 2018年02月24日
  内容   : 创建文件
  描述   : 智云服MQTT版本连接服务器

  修改记录:
  日期   : 2018年02月24日
    作者   : LiCM
    内容   : 创建文件

******************************************************************************/

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
#include "user_lmq.h"

#include "MQTTPacket.h"
#include "transport.h"


#define USER_MQTT_DEBUG


#ifdef USER_MQTT_DEBUG
    #define user_mqtt_info(format, ...)  mprintf( format "\r\n", ##__VA_ARGS__)
    #define user_mqtt_error(format, ...) mprintf( "[error]%s() %d " format "\r\n", /*__FILE__,*/ __FUNCTION__, __LINE__, ##__VA_ARGS__)
#else
    #define user_mqtt_info(format, ...)
    #define user_mqtt_error(format, ...)
#endif


char bleaddr[13] = {0};


unsigned char buf[1024];
int buflen = sizeof(buf);


user_ble_mqtt_info_t ble_mqtt_info;
ble_info_t ble_infos;


int User_PahoMqtt_Test(u8 dow)
{
	u8 topicbuf[32]={0};
	MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
	int rc = 0;
	int mysock = 0;
	int msgid = 1;
	MQTTString topicString = MQTTString_initializer;
	int req_qos = 0;
	char* payload = BleHeartbeatToJson(bleaddr,"2018-3-8/T-16-21","4.100V","2.000A","15.000V","4.000A");
	int payloadlen = Ql_strlen(payload);
	int len = 0;
	char *host = "post-cn-v0h0dlzxa02.mqtt.aliyuncs.com";
	int port = 1883;

	user_mqtt_info("payload=%s",payload);

retary:
	
	mysock = transport_open(host, port);

	if(mysock < 0)
		return mysock;


	user_mqtt_info("Sending to hostname %s port %d\n", host, port);

	data.clientID.cstring = "GID_1988@@@4526";
	data.keepAliveInterval = 300;
	data.cleansession = 1;
	data.username.cstring = "LTAIbX60YQNt7UTf";
	data.password.cstring = "3UyyQbjklF8U/QnQlBYxHNZTBiU=";

	len = MQTTSerialize_connect(buf, buflen, &data);

	user_mqtt_info("MQTTSerialize_connect =%d\n",len);

	rc = transport_sendPacketBuffer(mysock, buf, len);

	user_mqtt_info("transport_sendPacketBuffer =%d\n",rc);

	if (MQTTPacket_read(buf, buflen, transport_getdata) == CONNACK)
	{
		unsigned char sessionPresent, connack_rc;

		if (MQTTDeserialize_connack(&sessionPresent, &connack_rc, buf, buflen) != 1 || connack_rc != 0)
		{
			user_mqtt_info("Unable to connect, return code %d\n", connack_rc);
			transport_close(mysock);
			goto retary;
		}
	}
	else
		{
			transport_close(mysock);
			goto retary;
		}

	Ql_sprintf(topicbuf,"xxtopic/%s/cmd",bleaddr);
	topicString.cstring = topicbuf;

	user_mqtt_info("topicString.cstring =%s\n",topicString.cstring);

	len = MQTTSerialize_subscribe(buf, buflen, 0, msgid, 1, &topicString, &req_qos);

	user_mqtt_info("MQTTSerialize_subscribe =%d\n",len);


	rc = transport_sendPacketBuffer(mysock, buf, len);

	user_mqtt_info("transport_sendPacketBuffer =%d\n",rc);
	
	if (MQTTPacket_read(buf, buflen, transport_getdata) == SUBACK) 	/* wait for suback */
	{
		unsigned short submsgid;
		int subcount;
		int granted_qos;

		rc = MQTTDeserialize_suback(&submsgid, 1, &subcount, &granted_qos, buf, buflen);
		if (granted_qos != 0)
		{
			user_mqtt_info("granted qos != 0, %d\n", granted_qos);
			goto exit;
		}
	}
	else
		goto exit;

	Ql_sprintf(topicbuf,"xxtopic/%s/sta",bleaddr);
	topicString.cstring = topicbuf;
	
	user_mqtt_info("MQTTDeserialize_suback OK\n");

	ble_mqtt_info.mqttsta=1;
	
	//while (0)
	//{
		/* transport_getdata() has a built-in 1 second timeout,
		your mileage will vary */
#if 0
		if (MQTTPacket_read(buf, buflen, transport_getdata) == PUBLISH)
		{
			unsigned char dup;
			int qos;
			unsigned char retained;
			unsigned short msgid;
			int payloadlen_in;
			unsigned char* payload_in;
			int rc;
			MQTTString receivedTopic;

			rc = MQTTDeserialize_publish(&dup, &qos, &retained, &msgid, &receivedTopic,
					&payload_in, &payloadlen_in, buf, buflen);
			user_mqtt_info("message arrived %.*s\n", payloadlen_in, payload_in);
		}
#endif
		if(dow==0)
			{
				user_mqtt_info("publishing reading\n");
				len = MQTTSerialize_publish(buf, buflen, 0, 0, 0, 0, topicString, (unsigned char*)payload, payloadlen);
				user_mqtt_info("MQTTSerialize_publish=%d\n",len);
				rc = transport_sendPacketBuffer(mysock, buf, len);
				user_mqtt_info("transport_sendPacketBuffer=%d\n",rc);
				Ql_Sleep(5000);
			}
	//}

	//user_mqtt_info("disconnecting\n");
	//len = MQTTSerialize_disconnect(buf, buflen);
	//rc = transport_sendPacketBuffer(mysock, buf, len);

exit:
	//transport_close(mysock);

	return 0;
}




////////////////////////////////////////////////////////////////////////////////


ble_listinfo_t ble_info_list;


void ble_info_listInit(void)
{
	u16 i;
	for(i=0;i<BLE_LIST_MAX_NUM;i++)
		{
			ble_info_list.ble_mac_info[i].isUsd=0;
		}
	ble_info_list.index=0;
}
u8 isNeedAdd(u8 *name,u8 *mac,u8 *rssi)
{
	u16 i;
	for(i=0;i<ble_info_list.index;i++)
		{
			if(ble_info_list.ble_mac_info[i].isUsd)
				{
					if (0 == Ql_strcmp(ble_info_list.ble_mac_info[i].mac, mac))
						{
							if (0 != Ql_strcmp(ble_info_list.ble_mac_info[i].name, name))
								{
									Ql_sprintf(ble_info_list.ble_mac_info[i].name,"%s",name);
								}
							if (0 != Ql_strcmp(ble_info_list.ble_mac_info[i].rssi, rssi))
								{
									Ql_sprintf(ble_info_list.ble_mac_info[i].rssi,"%s",rssi);
								}
							return 0;
						}
				}
		}
	return 1;
}

void Add_adv_report2LongList(u8 *name,u8 *mac,u8 *rssi)
{
	u8 res;
	//判断存在相同的LIST
	res=isNeedAdd(name,mac,rssi);
	if(res)
		{
			ble_info_list.ble_mac_info[ble_info_list.index].isUsd=1;
			Ql_strcpy(ble_info_list.ble_mac_info[ble_info_list.index].name,name);
			Ql_strcpy(ble_info_list.ble_mac_info[ble_info_list.index].mac,mac);
			Ql_strcpy(ble_info_list.ble_mac_info[ble_info_list.index].rssi,rssi);
			//mprintf("ble_info_list.index=%d\r\n",ble_info_list.index);
			//mprintf("ble_info_list.ble_mac_info[%d].name=%s\r\n",ble_info_list.index,ble_info_list.ble_mac_info[ble_info_list.index].name);
			//mprintf("ble_info_list.ble_mac_info[%d].mac=%s\r\n",ble_info_list.index,ble_info_list.ble_mac_info[ble_info_list.index].mac);
			//mprintf("ble_info_list.ble_mac_info[%d].rssi=%s\r\n",ble_info_list.index,ble_info_list.ble_mac_info[ble_info_list.index].rssi);
			ble_info_list.index++;
			if(ble_info_list.index>=BLE_LIST_MAX_NUM)ble_info_list.index=BLE_LIST_MAX_NUM-1;
		}
}



void Send_BleList2Server(void)
{
	
	u8 i=0,x=0;
	u8 j,k;
	u8 tem[10]={0};
	u8 num[10]={0};
	u8 tt;
	
	j=ble_info_list.index/BLE_SEND_LIST;
	k=ble_info_list.index%BLE_SEND_LIST;

	if(k==0)tt=j;
	else tt=j+1;

	Ql_sprintf(tem,"%d",ble_info_list.index);
	
	for(i=0;i<j;i++)
		{
			Ql_sprintf(num,"%d-%d",tt,i+1);
			ble_infos.index=0;
			for(x=0;x<BLE_SEND_LIST;x++)
				{
					Ql_strcpy(ble_infos.ble_mac_info[x].name,ble_info_list.ble_mac_info[x+BLE_SEND_LIST*i].name);
					Ql_strcpy(ble_infos.ble_mac_info[x].mac,ble_info_list.ble_mac_info[x+BLE_SEND_LIST*i].mac);
					Ql_strcpy(ble_infos.ble_mac_info[x].rssi,ble_info_list.ble_mac_info[x+BLE_SEND_LIST*i].rssi);
					ble_info_list.ble_mac_info[x+BLE_SEND_LIST*i].isUsd=0;
				}
			ble_infos.index=BLE_SEND_LIST;
			BleInfoPayloadData(tem,num);
		}
	if(k)
		{
			Ql_sprintf(num,"%d-%d",tt,tt);
			ble_infos.index=0;
			for(x=0;x<k;x++)
				{
					Ql_strcpy(ble_infos.ble_mac_info[x].name,ble_info_list.ble_mac_info[x+BLE_SEND_LIST*i].name);
					Ql_strcpy(ble_infos.ble_mac_info[x].mac,ble_info_list.ble_mac_info[x+BLE_SEND_LIST*i].mac);
					Ql_strcpy(ble_infos.ble_mac_info[x].rssi,ble_info_list.ble_mac_info[x+BLE_SEND_LIST*i].rssi);
					ble_info_list.ble_mac_info[x+BLE_SEND_LIST*i].isUsd=0;

				}
				ble_infos.index=k;
				BleInfoPayloadData(tem,num);
		}
	ble_info_list.index=0;
}









void Add_adv_report2List(u8 *name,u8 *mac,u8 *rssi)
{
	if(ble_mqtt_info.heartping<120)
		{
			Ql_strcpy(ble_infos.ble_mac_info[ble_infos.index].name,name);
			Ql_strcpy(ble_infos.ble_mac_info[ble_infos.index].mac,mac);
			Ql_strcpy(ble_infos.ble_mac_info[ble_infos.index].rssi,rssi);
			ble_infos.index++;
			if(ble_infos.index>=5)
				{
					ble_mqtt_info.heartping=200;
				}
		}
}

void BleInfoPayloadData(u8 *cyc,u8 *num)
{
	u8 i;
	char* payload;
	CreateBleInfoList();
	SetBleInfoList(bleaddr,"2018-3/T09-02",cyc,num);
	for(i=0;i<ble_infos.index;i++)
		{
			AddBleInfoToList(ble_infos.ble_mac_info[i].name, ble_infos.ble_mac_info[i].mac, ble_infos.ble_mac_info[i].rssi, "");
		}
	payload = BleInfoToJson();
	cJSON_Minify(payload);
	mprintf("%s\r\n",payload);
	publishSta(payload,Ql_strlen(payload));
	Ql_MEM_Free(payload);
	for(i=0;i<ble_infos.index;i++)
		{
			DeleteBleInfoToList(ble_infos.ble_mac_info[i].mac);
		}
	DeleteBleInfoP();
	Ql_Sleep(300);
	
}

void publishSta(u8 *payload,u16 length)
{
	int len,mysock;
	int rc = 0;
	u8 topicbuf[32]={0};
	MQTTString topicString = MQTTString_initializer;
	Ql_sprintf(topicbuf,"xxtopic/%s/sta",bleaddr);
	topicString.cstring = topicbuf;
REPUB:
	user_mqtt_info("publishing reading\n");
	len = MQTTSerialize_publish(buf, buflen, 0, 0, 0, 0, topicString, (unsigned char*)payload, length);
	user_mqtt_info("MQTTSerialize_publish=%d\n",len);
	rc = transport_sendPacketBuffer(mysock, buf, len);
	user_mqtt_info("transport_sendPacketBuffer=%d\n",rc);
	if(rc<0)
		{
			User_PahoMqtt_Test(1);
			goto REPUB;
		}
}


s32 zyf_ays_cmd(char *src,char *key,char *value)
{
	u8 *p=NULL;
	p=Ql_strstr(src,key);
	if(p!=NULL)
		{
			while(*p!=':')p++;
			p++;
			while(*p!='"')p++;
			p++;
			while(*p!='"')
				{
					*value=*p;
					p++;
					value++;
				}
			*value=0;
			return 0;
		}
	return -1;
}

s32 zyf_decode_upbcmd(char *Payload,u16 len)
{
	s32 ret;
	u8 value[2][30]={0};
	Ql_memset(value[0], 0,sizeof(value[0]));
	zyf_ays_cmd(Payload,"type",value[0]);
	user_mqtt_info("value[0]=%s",value[0]);
	Ql_memset(value[1], 0,sizeof(value[1]));
	zyf_ays_cmd(Payload,"value",value[1]);
	user_mqtt_info("value[1]=%s",value[1]);
	ret=Ql_strcmp(value[0], "ble_report_interval");
	if(ret==0)
		{
			systemset.Interval=strtol((const char*)value[1],NULL,10);
			user_mqtt_info("systemset.Interval=%d\r\n",systemset.Interval);
			ble_mqtt_info.heartping=0;
		}
	ret=Ql_strcmp(value[0], "ble_reset");
	if(ret==0)
		{
			Ql_Sleep(1000);
			Ql_Reset(0);
		}
}


void RdsubData(void)
{
	int rc;
	if(ble_mqtt_info.mqttsta)
		{
			rc=MQTTPacket_read(buf, buflen, transport_getdata_1);
			if(rc==PUBLISH)
				{
					unsigned char dup;
					int qos;
					unsigned char retained;
					unsigned short msgid;
					int payloadlen_in;
					unsigned char* payload_in;
					int rc;
					MQTTString receivedTopic;
					user_mqtt_info("PUBLISH\n");
					rc = MQTTDeserialize_publish(&dup, &qos, &retained, &msgid, &receivedTopic,&payload_in, &payloadlen_in, buf, buflen);
					user_mqtt_info("message arrived %s\n", payload_in);
					zyf_decode_upbcmd(payload_in,payloadlen_in);
					
				}
			else if(rc==SUBACK)
				{
					user_mqtt_info("SUBACK\n");
				}
			else if(rc==PUBACK)
				{
					user_mqtt_info("PUBACK\n");
				}
		}
}

