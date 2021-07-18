/******************************************************************************

                  版权所有 (C) 2018, 天创金农科技

 ******************************************************************************
  文件   : MQTT_interface.c
  版本   : 初稿
  作者   : LuanYang
  日期   : 2018年03月5日
  内容   : 创建文件
  描述   : M203C3.0开发板单线程版本

  修改记录:
  日期   : 2018年03月5日
    作者   : LuanYang
    内容   : 创建文件

******************************************************************************/

#include "cJSON.h"
#include <math.h>
#include "ql_common.h"
#include "ql_system.h"
#include "ql_type.h"
#include "ql_trace.h"
#include "ql_timer.h"
#include "ql_stdlib.h"
#include "ql_uart.h"
#include "ql_memory.h"
#include "ql_error.h"

#include "ql_stdlib.h"
#include "uart.h"
#include "jsonAnaysis.h"


#define JSON_ANALYSIS_DEBUG



#ifdef JSON_ANALYSIS_DEBUG

    #define JSON_ANALYSIS_debug(format, ...) mprintf( format "\r\n", ##__VA_ARGS__)
#else
   	#define JSON_ANALYSIS_debug(format, ...)
#endif





typedef struct
{
	char sn[50];
	char timestamp[50];
	char cycle[50];
	char num[50];
}BLE_LIST_HEAD;


typedef struct BLE_LIST
{
	struct BLE_LIST *next, *previous;
	BLE_LIST_HEAD *ble_list;
	BLE_INFO_TYPE Ble_info;

}JSON_BLE_CMD_LINK_NODE;






/*!
 * @brief 链表指针传递仅用于本文件内部
 * \n
 *
 * @param p 需要进行传递的指针
 * @return 传入过的非空指针
 * \n
 * @see
 */
JSON_BLE_CMD_LINK_NODE * transfer_the_point(JSON_BLE_CMD_LINK_NODE * p)
{
	static JSON_BLE_CMD_LINK_NODE * pHead = NULL;
	if(p!=NULL)
	{
		pHead = p;
	}
	return pHead;
}



/*!
 * @brief 创建一个蓝牙信息链表
 * \n
 *
 * @param NULL
 * @return -1:创建失败可能的远因是内存分配失败。0:创建链表成功
 * \n
 * @see
 */
int CreateBleInfoList(void)
{
	JSON_BLE_CMD_LINK_NODE *ble_link_haed;
	static BLE_LIST_HEAD ble_list_head;
	ble_link_haed=(JSON_BLE_CMD_LINK_NODE*)Ql_MEM_Alloc(sizeof(JSON_BLE_CMD_LINK_NODE));
	if(ble_link_haed == NULL)
		{
		return (-1);
		JSON_ANALYSIS_debug("\r\n Create Ble InfoList failed \r\n");
		}
	ble_link_haed->next= NULL;
	ble_link_haed->previous = NULL;
	ble_link_haed->ble_list = &ble_list_head;

	transfer_the_point(ble_link_haed); //保存链表头指针
	return (0);

}

/*!
 * @brief 设置蓝牙信息的参数
 * \n
 *
 * @param pSN:收集蓝牙信息指令中需要传入的SN码的指针
 * @param pTimestamp:收集蓝牙信息指令中需要传入的时间戳的指针
 * @param pCycle:收集蓝牙信息指令中需要传入的pCycle的指针
 * @param pNum:收集蓝牙信息指令中需要传入的pNum的指针 
 * @return -2:设置失败,可能的原因是在CreateBleInfoList之前调用了该函数；0设置成功
 * \n
 * @see
 */
int SetBleInfoList(char *pSN , char *pTimestamp , char *pCycle, char *pNum)
{
	JSON_BLE_CMD_LINK_NODE * ble_link_haed_pro;

	ble_link_haed_pro=transfer_the_point(NULL);  //读取链表头指针
	if(ble_link_haed_pro == NULL)
	{
		return (-2);
		JSON_ANALYSIS_debug("\r\n BleLink is NULL  \r\n");
	}

	Ql_strcpy( (ble_link_haed_pro->ble_list->sn) , pSN);
	
	Ql_strcpy( (ble_link_haed_pro->ble_list->timestamp) , pTimestamp);

	Ql_strcpy( (ble_link_haed_pro->ble_list->cycle) ,pCycle);
	
	Ql_strcpy( (ble_link_haed_pro->ble_list->num) , pNum);	

	return (0);
	
}



/*!
 * @brief 给蓝牙信息中添加一组蓝牙信息
 * \n
 *
 * @param name:蓝牙名的指针
 * @param mac:蓝牙的mac地址的指针
 * @param rssi:蓝牙信号灵敏度的指针
 * @param RAWdata:蓝牙scan_rsp报文内容
 * @return -2:设置失败,可能的原因是在CreateBleInfoList之前调用了该函数；0设置成功
 * \n
 * @see
 */
int AddBleInfoToList(char *name , char *mac , char *rssi, char *RAWdata )
{
	static JSON_BLE_CMD_LINK_NODE * ble_link_head;
	JSON_BLE_CMD_LINK_NODE * ble_link_haed_pro;
	char *temp;

	ble_link_haed_pro=transfer_the_point(NULL); //读取链表头指针
	if(ble_link_haed_pro == NULL)
	{
		JSON_ANALYSIS_debug("\r\n BleLink is NULL  \r\n");
		return (-2);
	}

	

	
	ble_link_head=(JSON_BLE_CMD_LINK_NODE*)Ql_MEM_Alloc(sizeof(JSON_BLE_CMD_LINK_NODE));
	if(ble_link_head==NULL)
	{
		JSON_ANALYSIS_debug("\r\n Memory allocation failure  \r\n");
		return (-1);
	}
//	ble_link_head->Ble_info.name = (char*)Ql_MEM_Alloc(Ql_strlen(name)+1);
//	ble_link_head->Ble_info.mac = (char*)Ql_MEM_Alloc(Ql_strlen(mac)+1);
//	ble_link_head->Ble_info.mac = (char*)Ql_MEM_Alloc(Ql_strlen(mac)+1);

	while(ble_link_haed_pro->next != NULL)
	{
		ble_link_haed_pro = ble_link_haed_pro->next;
	}

	
	ble_link_haed_pro->next = ble_link_head;
	ble_link_head->next = NULL;
	ble_link_head->previous = ble_link_haed_pro;
	ble_link_head->ble_list = NULL;
	
	Ql_strcpy( (ble_link_head->Ble_info.name) ,name);
	
	Ql_strcpy( (ble_link_head->Ble_info.mac) ,mac);

	Ql_strcpy( (ble_link_head->Ble_info.rssi) ,rssi);
	
	Ql_strcpy( (ble_link_head->Ble_info.RAWdata) ,RAWdata);	

	return (0);
	
}




/*!
 * @brief 从蓝牙信息中删除一组蓝牙信息
 * \n
 *
 * @param mac:需要删除的蓝牙信息中对应的mac地址
 * @return -2:设置失败,可能的原因是在CreateBleInfoList之前调用了该函数；-1:没有匹配的蓝牙; 0:删除成功
 * \n
 * @see
 */
int DeleteBleInfoP(void)
{
	JSON_BLE_CMD_LINK_NODE * ble_link_haed_pro;

	ble_link_haed_pro=transfer_the_point(NULL); //读取链表头指针
	if(ble_link_haed_pro == NULL)
	{
		JSON_ANALYSIS_debug("\r\n BleLink is NULL  \r\n");
		return (-2);
	}
	
	Ql_MEM_Free(ble_link_haed_pro);

	return 0;
	
}

int DeleteBleInfoToList(const char * mac )
{
	JSON_BLE_CMD_LINK_NODE * ble_link_head;
	JSON_BLE_CMD_LINK_NODE * ble_link_haed_pro;

	ble_link_haed_pro=transfer_the_point(NULL); //读取链表头指针
	if(ble_link_haed_pro == NULL)
	{
		JSON_ANALYSIS_debug("\r\n BleLink is NULL  \r\n");
		return (-2);
	}
	ble_link_haed_pro = ble_link_haed_pro->next;
	
	while( Ql_strcmp(ble_link_haed_pro->Ble_info.mac , mac) != 0 )
	{
		if(ble_link_haed_pro->next == NULL)
		{
			JSON_ANALYSIS_debug("\r\n Not found match mac  \r\n");
			return (-1);
		}
		ble_link_haed_pro = ble_link_haed_pro->next;
	}

	
	
	if(ble_link_haed_pro->next != NULL)
	{
		ble_link_haed_pro->previous->next = ble_link_haed_pro->next;
		ble_link_haed_pro->next->previous = ble_link_haed_pro->previous;
	}else
	{
		ble_link_haed_pro->previous->next = NULL;		
	}
    
	
	Ql_MEM_Free(ble_link_haed_pro);

	return 0;
	
}


/*!
 * @brief 将蓝牙链表中的信息转换为json串
 * \n
 *
 * @param NULL
 * @return 返回转换后的json串的指针，如果返回为NULL则表明转换失败，可能的原因是在CreateBleInfoList之前调用了该函数
 *		   输出的json串在使用完成后需要调用FreeJsonString()来释放json串所占用内存
 * \n
 * @see
 */
char * BleInfoToJson(void)
{
	char *json_ret;
	JSON_BLE_CMD_LINK_NODE * ble_link_head;
	JSON_BLE_CMD_LINK_NODE * ble_link_haed_pro;
	
	cJSON * root =  cJSON_CreateObject();
    cJSON * item =  cJSON_CreateObject();
	cJSON *rows,*row;

	ble_link_haed_pro=transfer_the_point(NULL); //读取链表头指针
	if(ble_link_haed_pro == NULL)
	{
		JSON_ANALYSIS_debug("\r\n BleLink is NULL  \r\n");
		return (NULL);
	}
			
	cJSON_AddItemToObject(root, "type", cJSON_CreateString("ble_list"));
	cJSON_AddItemToObject(root, "sn", cJSON_CreateString(ble_link_haed_pro->ble_list->sn ));
	cJSON_AddItemToObject(root, "timestamp", cJSON_CreateString(ble_link_haed_pro->ble_list->timestamp ));
	cJSON_AddItemToObject(root, "cycle", cJSON_CreateString(ble_link_haed_pro->ble_list->cycle ));
	cJSON_AddItemToObject(root, "num", cJSON_CreateString(ble_link_haed_pro->ble_list->num ));
	cJSON_AddItemToObject(root, "list", rows=cJSON_CreateArray());

	ble_link_head = ble_link_haed_pro->next;
	
	while(ble_link_head!= NULL)
	{
		cJSON_AddItemToArray(rows, row = cJSON_CreateObject());
		cJSON_AddItemToObject(row, "name", cJSON_CreateString(ble_link_head->Ble_info.name));  
		cJSON_AddItemToObject(row, "mac", cJSON_CreateString(ble_link_head->Ble_info.mac));  
		cJSON_AddItemToObject(row, "rssi", cJSON_CreateString(ble_link_head->Ble_info.rssi));
		//cJSON_AddItemToObject(row, "RAWdata", cJSON_CreateString(ble_link_head->Ble_info.RAWdata));		
		ble_link_head = ble_link_head->next;
		
	}
	
	json_ret=cJSON_Print(root);
	json_ret=(char *)Ql_MEM_Alloc(Ql_strlen(json_ret)+1);
	json_ret=Ql_strcpy(json_ret,cJSON_Print(root));
	cJSON_Delete(root);

	return (json_ret);
	
}


/*!
 * @brief 将心跳信息转换为json串
 * \n
 *
 * @param 		 mac :mac地址码的指针
 * @param   timestamp:时间戳的指针
 * @param bat_voltage:电池电压字符串的指针
 * @param bat_current:电池电流字符串的指针
 * @param pwr_voltage:电源电压字符串的指针
 * @param pwr_current:电源电流字符串的指针
 * @return  输出的json串在使用完成后需要调用FreeJsonString()来释放json串所占用内存  
 * \n
 * @see
 */
char *BleHeartbeatToJson(char * mac,char *timestamp,char *bat_voltage,char *bat_current,char *pwr_voltage,char *pwr_current)
{
	cJSON * root =  cJSON_CreateObject();
	char *json_ret,*buf_String;
	short len;
	
	cJSON_AddItemToObject(root, "type", cJSON_CreateString("ble_heartbeat"));
	cJSON_AddItemToObject(root, "mac", cJSON_CreateString(mac));
	cJSON_AddItemToObject(root, "timestamp", cJSON_CreateString(timestamp));
	cJSON_AddItemToObject(root, "batteryVoltage", cJSON_CreateString(bat_voltage));
	cJSON_AddItemToObject(root, "batteryCurrent", cJSON_CreateString(bat_current));
	cJSON_AddItemToObject(root, "powerVoltage", cJSON_CreateString(pwr_voltage));
	cJSON_AddItemToObject(root, "powerCurrent", cJSON_CreateString(pwr_current));
	
	buf_String=cJSON_Print(root);
	len = Ql_strlen(buf_String);
	json_ret=(char*)Ql_MEM_Alloc(len+1);
	if(json_ret != NULL)
	{
			Ql_strcpy(json_ret,buf_String);
	}else
	{
		JSON_ANALYSIS_debug("\r\n Memory allocation failure  \r\n");
	}
	cJSON_Delete(root);

	
	return (json_ret);		
	
}


/*!
 * @brief 下发设置命令后，设备回复设置完成命令
 * \n
 *
 * @param type :就近接收到的指令的type内容的指针
 * @param sn   :sn的指针
 * @return  输出的json串在使用完成后需要调用FreeJsonString()来释放json串所占用内存  
 * \n
 * @see
 */
char *AckForJsonCmd(char *type, char *sn)
{
	cJSON * root =  cJSON_CreateObject();
	char *json_ret;
	
	cJSON_AddItemToObject(root, "type", cJSON_CreateString(type));
	cJSON_AddItemToObject(root, "sn", cJSON_CreateString(sn));
	
	json_ret=cJSON_Print(root);
	json_ret=(char *)Ql_MEM_Alloc(Ql_strlen(json_ret)+1);
	json_ret=Ql_strcpy(json_ret,cJSON_Print(root));
	cJSON_Delete(root);
	
	return (json_ret);
	
}



/*!
 * @brief 释放转换的json内存
 * \n
 *
 * @param pJsonStr :需要删除的json串的指针
 * @return void
 * \n
 * @see
 */
void FreeJsonString(char *pJsonStr)
{
	if(pJsonStr != NULL)
	{
		Ql_MEM_Free(pJsonStr);
		JSON_ANALYSIS_debug("\r\n Del successful \r\n");
	}
	else
	{
		JSON_ANALYSIS_debug("\r\n Del Error \r\n");
	}
}

/*!
 * @brief 解析json串，将结果以 SJON_CMD_TYPE类型指针返回
 * \n
 * @param  Payload :需要解析的json串的指针
 * @return SJON_CMD_TYPE :解析结果 根据解析出来的指令码来确定解析的数据是从	StringType，BleSleepType，BleMqttSetType
 *							中的哪一个获得。如数据是一个字符串则从	StringType中读取，
 *							如果指令是‘设置休眠时间命令’则从 BleSleepType中读取，
 *							如果指令是‘1.15设备mqtt通讯参数设置’则从BleMqttSetType中读取
 * \n
 * @see
 */
SJON_CMD_TYPE*JsonCmdAnalysis(char *Payload)
{
	cJSON *root , *value , *type;
	
	unsigned short index=0;
	static SJON_CMD_TYPE RetResult;
	static char *cmd[11]={
					"ble_power",
					"ble_search_interval",
					"ble_search_window",
					"ble_report_interval",
					"ble_battery_voltage",
					"ble_battery_current",
					"ble_power_max_voltage",
					"ble_power_max_current",
					"ble_sleep",
					"ble_reset",
					"ble_mqtt"
					};


					
    root = cJSON_Parse(Payload);  
    if (!root)  
    {  
        JSON_ANALYSIS_debug("Error before: [%s]\n",cJSON_GetErrorPtr());
		RetResult.cmd=CMD_Error;
		goto END;
    }
	else
	{
		
		 type = cJSON_GetObjectItem( root , "type");
		 
		 if(type->type == cJSON_String)
		 {
		 	mprintf("%s\r\n",type->valuestring);
		 	while(0!=Ql_strcmp(type->valuestring, cmd[index]))
	 		{
	 			
	 			if(index >=11)
	 			{
	 				RetResult.cmd=CMD_Error;
					goto END;
	 			}
				
				index++;
			
	 		}
			RetResult.cmd = index;
			
			JSON_ANALYSIS_debug("The cmd is %d\r\n",index);
			
			switch(index)
			{
				case SET_BLE_PRW:
				case SET_BLE_SEARCH_INTERVAL:
				case SET_BLE_SEARCH_WINDOW:
				case SET_BLE_REPORT_INTERVAL:
				case SET_BLE_BATTERY_VOLTAGE:
				case SET_BLE_BATTERY_CURRENT:
				case SET_BLE_BATTERY_MAX_VOLTAGE:
				case SET_BLE_BATTERY_MAX_CURRENT:
					{
						type = cJSON_GetObjectItem( root , "value");
						if(type->type==cJSON_String)
						{
							RetResult.StringType = Ql_MEM_Alloc(Ql_strlen(type->valuestring)+1);
							Ql_strcpy(RetResult.StringType,type->valuestring);
							JSON_ANALYSIS_debug(" value = %s\r\n",RetResult.StringType);
						}
						else
						{
							JSON_ANALYSIS_debug(" value Error %s\r\n");
							RetResult.cmd=CMD_Error;
							goto END;
						}
						
					}break;
					
				case SET_BLE_SLEEP_TIME:
					{
						type = cJSON_GetObjectItem( root , "sleep");
						if(type->type==cJSON_String)
						{
							RetResult.BleSleepType.sleep  = Ql_MEM_Alloc(Ql_strlen(type->valuestring)+1);
							Ql_strcpy(RetResult.BleSleepType.sleep ,type->valuestring);
							JSON_ANALYSIS_debug("\r\n sleep = %s \r\n",RetResult.BleSleepType.sleep);
						}
						else
						{
							JSON_ANALYSIS_debug("\r\n sleep Error\r\n");
							RetResult.cmd=CMD_Error;
							goto END;
						}						
						type = cJSON_GetObjectItem( root , "wakeup");
						if(type->type==cJSON_String)
						{
							RetResult.BleSleepType.wakeup  = Ql_MEM_Alloc(Ql_strlen(type->valuestring)+1);
							Ql_strcpy(RetResult.BleSleepType.wakeup ,type->valuestring);	
							JSON_ANALYSIS_debug("\r\n wakeup = %s\r\n",RetResult.BleSleepType.wakeup);
						}
						else
						{
							JSON_ANALYSIS_debug("\r\n wakeup Error\r\n");
							Ql_MEM_Free(RetResult.BleSleepType.sleep);
							RetResult.cmd=CMD_Error;
							goto END;
						}						
					}break;
						
				case SET_BLE_MQTT_PARAMETER:
					{
						type = cJSON_GetObjectItem( root , "sslBrokerUrl");
						if(type->type==cJSON_String)
						{
							RetResult.BleMqttSetType.sslBrokerUrl  = Ql_MEM_Alloc(Ql_strlen(type->valuestring)+1);
							Ql_strcpy(RetResult.BleMqttSetType.sslBrokerUrl,type->valuestring);
							JSON_ANALYSIS_debug("\r\n sslBrokerUrl = %s\r\n",RetResult.BleMqttSetType.sslBrokerUrl);
						}
						else
						{
							JSON_ANALYSIS_debug("\r\n sslBrokerUrl Error\r\n");
							RetResult.cmd=CMD_Error;
							goto END;
						}						
						
						type = cJSON_GetObjectItem( root , "Topic");
						if(type->type==cJSON_String)
						{
							RetResult.BleMqttSetType.Topic  = Ql_MEM_Alloc(Ql_strlen(type->valuestring)+1);
							Ql_strcpy(RetResult.BleMqttSetType.Topic,type->valuestring);
							JSON_ANALYSIS_debug("Topic=%s\r\n",RetResult.BleMqttSetType.Topic);
						}	
						else
						{
							JSON_ANALYSIS_debug("Topic Error\r\n");
							Ql_MEM_Free(RetResult.BleMqttSetType.sslBrokerUrl);
							RetResult.cmd=CMD_Error;
							goto END;
						}
						
						type = cJSON_GetObjectItem( root , "UserName");
						if(type->type==cJSON_String)
						{
							RetResult.BleMqttSetType.UserName  = Ql_MEM_Alloc(Ql_strlen(type->valuestring)+1);
							Ql_strcpy(RetResult.BleMqttSetType.UserName,type->valuestring);
							JSON_ANALYSIS_debug("UserName=%s\r\n",RetResult.BleMqttSetType.UserName);
						}
						else
						{
							JSON_ANALYSIS_debug("UserName Error\r\n");
							Ql_MEM_Free(RetResult.BleMqttSetType.sslBrokerUrl);
							Ql_MEM_Free(RetResult.BleMqttSetType.Topic);
							RetResult.cmd=CMD_Error;
							goto END;
						}	
						
						type = cJSON_GetObjectItem( root , "password");
						if(type->type==cJSON_String)
						{
							RetResult.BleMqttSetType.password  = Ql_MEM_Alloc(Ql_strlen(type->valuestring)+1);
							Ql_strcpy(RetResult.BleMqttSetType.password,type->valuestring);
							JSON_ANALYSIS_debug("\r\n password = %s\r\n",RetResult.BleMqttSetType.password);
						}
						else
						{
							JSON_ANALYSIS_debug("password Error\r\n");
							Ql_MEM_Free(RetResult.BleMqttSetType.sslBrokerUrl);
							Ql_MEM_Free(RetResult.BleMqttSetType.Topic);	
							Ql_MEM_Free(RetResult.BleMqttSetType.UserName);
							RetResult.cmd=CMD_Error;
							goto END;
						}
					}break;
				default :  RetResult.cmd=CMD_Error; JSON_ANALYSIS_debug("The cmd Error %d\r\n");break;
			}

			JSON_ANALYSIS_debug("The analysis end %d\r\n");

			
		 }
	}
END:
	cJSON_Delete(root);
	return (&RetResult);
}




/*!
 * @brief 释放JsonCmdAnalysis()解析时为返回的数据开的内存
 * \n
 *
 * @param pMem :需要释放的解析结果所对应的指针
 * @return void
 * \n
 * @see
 */
void FreeAnalysisMem(SJON_CMD_TYPE *pMem)
{
	unsigned short index;

	index = pMem->cmd;
	switch(index)
	{
		case SET_BLE_PRW:
		case SET_BLE_SEARCH_INTERVAL:
		case SET_BLE_SEARCH_WINDOW:
		case SET_BLE_REPORT_INTERVAL:
		case SET_BLE_BATTERY_VOLTAGE:
		case SET_BLE_BATTERY_CURRENT:
		case SET_BLE_BATTERY_MAX_VOLTAGE:
		case SET_BLE_BATTERY_MAX_CURRENT:
			{
				Ql_MEM_Free(pMem->StringType);
				JSON_ANALYSIS_debug("\r\n Free StringType %d\r\n");
			}break;
			
		case SET_BLE_SLEEP_TIME:
			{
				
				Ql_MEM_Free(pMem->BleSleepType.sleep);
				Ql_MEM_Free(pMem->BleSleepType.wakeup);	
				JSON_ANALYSIS_debug("\r\n Free BleSleepType %d\r\n");
			}break;
				
		case SET_BLE_MQTT_PARAMETER:
			{
				Ql_MEM_Free(pMem->BleMqttSetType.password);					
				Ql_MEM_Free(pMem->BleMqttSetType.sslBrokerUrl);		
				Ql_MEM_Free(pMem->BleMqttSetType.Topic);					
				Ql_MEM_Free(pMem->BleMqttSetType.UserName);	
				JSON_ANALYSIS_debug("\r\n Free BleMqttSetType %d\r\n");
			}break;
		default :  break;
	}

	

	
}


/*!
 * @brief 模块测试函数
 * \n
 *
 * @param NULL 
 * @return NULL
 * \n
 * @see
 */
void testJsonAnalysis(void)
{

	SJON_CMD_TYPE* JSON_CMD;
	char *json="{\"type\":\"ble_mqtt\",\"sslBrokerUrl\":\"ssl://post-cn-v0h0dlzxa02.mqtt.aliyuncs.com:8883\",\"Topic\":\"xxtopic\",\"UserName\":\"LTAIbX60YQNt7UTf\",\"password\":\"3UyyQbjklF8U/QnQlBYxHNZTBiU=\"}";
	char *mmce;
	int temp;

	CreateBleInfoList();
	SetBleInfoList("12334567","2018-3/T09-02","123","1");//(char *pSN , char *pTimestamp , char *pCycle, char *pNum);
	AddBleInfoToList("Thomas", "01af5bc8d5e9", "-12", "这个是一个测试");
	AddBleInfoToList("Thomas1", "01af5bc8d5ea", "-18", "这个是一个测试1");
	AddBleInfoToList("Thomas2", "01af5bc8d5eb", "-20", "这个是一个测试2");
	mprintf("%s\r\n",mmce = BleInfoToJson());
	
	temp=DeleteBleInfoToList("01af5bc8d5e9");
	if(temp==0)
	{
		mprintf("删除结果");
	}else
	{
		mprintf("删除失败 错误是 %d",temp);
	}
	temp=DeleteBleInfoToList("01a05bc8d5ea");
	if(temp==0)
	{
		mprintf("删除结果");
	}else
	{
		mprintf("删除失败 错误是 %d",temp);
	}	
	
	mprintf("%s\r\n",mmce = BleInfoToJson());

	mprintf("%s\r\n",mmce = BleHeartbeatToJson("123abd43d","2018-3-8/T-16-21","4.100V","2.000A","15.000V","4.000A"));
	
	FreeJsonString(mmce);

	mprintf("%s\r\n",mmce=AckForJsonCmd("ble_heartbeat","12343455"));

	FreeJsonString(mmce);

	JSON_CMD = JsonCmdAnalysis(json);

	FreeAnalysisMem(JSON_CMD);	
}




