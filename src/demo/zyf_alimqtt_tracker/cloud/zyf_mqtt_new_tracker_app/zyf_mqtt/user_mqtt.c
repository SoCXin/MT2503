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
#include "mqtt_api.h"
#include "user_mqtt.h"
#include "sys.h"
#include "uart.h"
#include "zyf_auto_config.h"
#include "zyf_protocol.h"
#include "tiem.h"
#include "dev_sign_internal.h"
#include "module_data.h"
#include "logfile.h"

#ifdef USER_MQTT_DEBUG
    #define user_mqtt_info(format, ...)  mprintf( format "\r\n", ##__VA_ARGS__)
    #define user_mqtt_error(format, ...) mprintf( "[error]%s() %d " format "\r\n", /*__FILE__,*/ __FUNCTION__, __LINE__, ##__VA_ARGS__)
#else
    #define user_mqtt_info(format, ...)
    #define user_mqtt_error(format, ...)
#endif

u32 SysOsTime=0;


iotx_sign_mqtt_t g_mqtt_signout;
iotx_mqtt_param_t mqtt_params;
iotx_mqtt_topic_info_t topic_msg;

char mqtt_msg_read_buf[MQTT_MSG_READ_BUF_SIZE];			// 订阅的消息 缓冲
char mqtt_msg_write_buf[MQTT_MSG_WRITE_BUF_SIZE];		// 需要发布的消息 缓冲
char mqtt_msg_pub[AMC_PUB_LENGTH_MAX];					// 需要发布的消息

char topic_for_pub[32];			//用户mqtt发布的主题
char topic_for_sub[32];			//用户mqtt发布的主题

char topic_for_alinkpub[128];			//alink_mqtt发布的主题
char topic_for_alinksub[128];			//alink_mqtt发布的主题
char topic_for_reg[128];        //注册主题


user_mqtt_info_t user_mqtt_yeinfo; //业务部分逻辑集合

USER_MQTT_CLINT_STA_T	user_mqtt_clint_sta=USER_MQTT_GPRS_INIT;		//用户mqtt客户端状态


void *app_pclient=NULL;


static int my_tolower(int c)  
{  
    if ((c >= 'A') && (c <= 'Z'))
    	{
        	return c + ('a' - 'A');
    	}
    return c;  
}


/*! 
 * @brief ALINK_MQTT发布主题信息初始化
 * \n
 *	/sys/a1yvpFa1g4N/015802212341/thing/event/property/post
 * @param NULL
 * @return NULL
 * \n
 * @see
 */
void alink_mqtt_topic_for_pub_init(void)
{
	char product_key[IOTX_PRODUCT_KEY_LEN] = {0};
    char device_name[IOTX_DEVICE_NAME_LEN] = {0};

	HAL_GetProductKey(product_key);
    HAL_GetDeviceName(device_name);
	Ql_memset(topic_for_alinkpub, 0, sizeof(topic_for_alinkpub));
	
#if 1
	Ql_strcat(topic_for_alinkpub, "/");
	Ql_strcat(topic_for_alinkpub, product_key);
	Ql_strcat(topic_for_alinkpub, "/");
	Ql_strcat(topic_for_alinkpub, device_name);
	Ql_strcat(topic_for_alinkpub, "/user/post");
#endif

#if 0
	Ql_strcat(topic_for_alinkpub, "/sys/");
	Ql_strcat(topic_for_alinkpub, product_key);
	Ql_strcat(topic_for_alinkpub, "/");
	Ql_strcat(topic_for_alinkpub, device_name);
	Ql_strcat(topic_for_alinkpub, "/thing/event/property/post");
#endif
}


/*!
 * @brief ALINK_MQTT订阅主题信息初始化
 * \n
 *	/sys/a1yvpFa1g4N/015802212341/thing/service/property/set
 * @param NULL
 * @return NULL
 * \n
 * @see
 */
void alink_mqtt_topic_for_sub_init(void)
{
	char product_key[IOTX_PRODUCT_KEY_LEN] = {0};
    char device_name[IOTX_DEVICE_NAME_LEN] = {0};

	HAL_GetProductKey(product_key);
    HAL_GetDeviceName(device_name);
	Ql_memset(topic_for_alinksub, 0, sizeof(topic_for_alinksub));


#if 1
	Ql_strcat(topic_for_alinksub, "/");
	Ql_strcat(topic_for_alinksub, product_key);
	Ql_strcat(topic_for_alinksub, "/");
	Ql_strcat(topic_for_alinksub, device_name);
	Ql_strcat(topic_for_alinksub, "/user/set");
#endif

#if 0
	Ql_strcat(topic_for_alinksub, "/sys/");
	Ql_strcat(topic_for_alinksub, product_key);
	Ql_strcat(topic_for_alinksub, "/");
	Ql_strcat(topic_for_alinksub, device_name);
	Ql_strcat(topic_for_alinksub, "/thing/service/property/set");
#endif

	
}



/*!
 * @brief MQTT发布主题信息初始化
 * \n
 *
 * @param NULL
 * @return NULL
 * \n
 * @see
 */
void user_mqtt_topic_for_pub_init(void)
{
	char product_key[IOTX_PRODUCT_KEY_LEN] = {0};
    char device_name[IOTX_DEVICE_NAME_LEN] = {0};

	HAL_GetProductKey(product_key);
    HAL_GetDeviceName(device_name);
	
	Ql_memset(topic_for_pub, 0, sizeof(topic_for_pub));
	Ql_strcat(topic_for_pub, "/");
	Ql_strcat(topic_for_pub, product_key);
	Ql_strcat(topic_for_pub, "/");
	Ql_strcat(topic_for_pub, device_name);
	Ql_strcat(topic_for_pub, "/user/update");
}



/*!
 * @brief MQTT订阅主题信息初始化
 * \n
 *
 * @param NULL
 * @return NULL
 * \n
 * @see
 */
void user_mqtt_topic_for_sub_init(void)
{
	char product_key[IOTX_PRODUCT_KEY_LEN] = {0};
    char device_name[IOTX_DEVICE_NAME_LEN] = {0};

	HAL_GetProductKey(product_key);
    HAL_GetDeviceName(device_name);
	
	Ql_memset(topic_for_sub, 0, sizeof(topic_for_sub));
	Ql_strcat(topic_for_sub, "/");
	Ql_strcat(topic_for_sub, product_key);
	Ql_strcat(topic_for_sub, "/");
	Ql_strcat(topic_for_sub, device_name);
	Ql_strcat(topic_for_sub, "/user/get");
	
}

void alink_mqtt_topic_for_reg_init(void)
{
    char product_key[IOTX_PRODUCT_KEY_LEN] = {0};
    char device_name[IOTX_DEVICE_NAME_LEN] = {0};

	HAL_GetProductKey(product_key);
    HAL_GetDeviceName(device_name);
#if 1
	Ql_strcat(topic_for_reg, "/");
	Ql_strcat(topic_for_reg, product_key);
	Ql_strcat(topic_for_reg, "/");
	Ql_strcat(topic_for_reg, device_name);
	Ql_strcat(topic_for_reg, "/user/register");
#endif
}


s8 user_get_puser_info(void)
{
	u32 res = 0;
	iotx_dev_meta_info_t meta;
	memset(&meta,0,sizeof(iotx_dev_meta_info_t));
	memset(&g_mqtt_signout,0,sizeof(iotx_sign_mqtt_t));

	HAL_GetProductKey(meta.product_key);
	HAL_GetDeviceName(meta.device_name);
	HAL_GetDeviceSecret(meta.device_secret);

	res = IOT_Sign_MQTT(IOTX_CLOUD_REGION_SHANGHAI,&meta,&g_mqtt_signout);
	if (res == 0) 
		{
			user_mqtt_info("signout.hostname: %s",g_mqtt_signout.hostname);
			user_mqtt_info("signout.port    : %d",g_mqtt_signout.port);
			user_mqtt_info("signout.clientid: %s",g_mqtt_signout.clientid);
			user_mqtt_info("signout.username: %s",g_mqtt_signout.username);
			user_mqtt_info("signout.password: %s",g_mqtt_signout.password);
			
			memset(&mqtt_params,0,sizeof(iotx_mqtt_param_t));
			mqtt_params.port = g_mqtt_signout.port;
			mqtt_params.host = g_mqtt_signout.hostname;
			mqtt_params.client_id = g_mqtt_signout.clientid;
			mqtt_params.username = g_mqtt_signout.username;
			mqtt_params.password = g_mqtt_signout.password;

			return 0;
			
		}
	
	return -1;
}




/*!
 * @brief MQTT状态回调函数
 * \n
 *
 * @param pcontext pclient msg
 * @return NULL
 * \n
 * @see
 */


static void aliot_mqtt_event_handle(void* pcontext, void* pclient, iotx_mqtt_event_msg_pt msg)
{
   u32 packet_id = (u32)msg->msg;
    iotx_mqtt_topic_info_pt topic_info = (iotx_mqtt_topic_info_pt)msg->msg;
    switch (msg->event_type)
    {
    case IOTX_MQTT_EVENT_UNDEF:
        user_mqtt_error("undefined event occur.");
        break;
    case IOTX_MQTT_EVENT_DISCONNECT:
		user_mqtt_clint_sta = USER_MQTT_GPRS_INIT;
        SendMsg2KernelForMqttStart();
        user_mqtt_error("MQTT disconnect.");
        break;
    case IOTX_MQTT_EVENT_RECONNECT:
        user_mqtt_info("MQTT reconnected.");
        break;
    case IOTX_MQTT_EVENT_SUBCRIBE_SUCCESS:
        user_mqtt_info("subscribe success, packet-id=%u", packet_id);
        break;
    case IOTX_MQTT_EVENT_SUBCRIBE_TIMEOUT:
        user_mqtt_error("subscribe wait ack timeout, packet-id=%u", packet_id);
        break;
    case IOTX_MQTT_EVENT_SUBCRIBE_NACK:
        user_mqtt_error("subscribe nack, packet-id=%u", packet_id);
        break;
    case IOTX_MQTT_EVENT_UNSUBCRIBE_SUCCESS:
        user_mqtt_info("unsubscribe success, packet-id=%u", packet_id);
        break;
    case IOTX_MQTT_EVENT_UNSUBCRIBE_TIMEOUT:
        user_mqtt_error("unsubscribe timeout, packet-id=%u", packet_id);
        break;
    case IOTX_MQTT_EVENT_UNSUBCRIBE_NACK:
        user_mqtt_error("unsubscribe nack, packet-id=%u", packet_id);
        break;
    case IOTX_MQTT_EVENT_PUBLISH_SUCCESS:
        //user_mqtt_info("publish success, packet-id=%u", packet_id);
        break;
    case IOTX_MQTT_EVENT_PUBLISH_TIMEOUT:
        user_mqtt_error("publish timeout, packet-id=%u", packet_id);
        break;
    case IOTX_MQTT_EVENT_PUBLISH_NACK:
        user_mqtt_error("publish nack, packet-id=%u", packet_id);
        break;
    case IOTX_MQTT_EVENT_PUBLISH_RECEIVED:
        user_mqtt_info("arrived message but without any related handle,len=%d,payload=%s",
                  topic_info->payload_len,
                  topic_info->payload);
        break;
    default:
        user_mqtt_error("Should NOT arrive here.");
        break;
    }
}


/*!
 * @brief MQTT用户信息初始化
 * \n
 *
 * @param NULL
 * @return NULL
 * \n
 * @see
 */

void user_mqtt_autu_init(void)
{
	mqtt_params.request_timeout_ms = MQTT_REQUEST_TIMEOUT_MS;
	mqtt_params.clean_session = 0;
	mqtt_params.keepalive_interval_ms = MQTT_KEEPALIVE_INTERVAL_MS;
	mqtt_params.read_buf_size = MQTT_MSG_READ_BUF_SIZE;
	mqtt_params.write_buf_size = MQTT_MSG_WRITE_BUF_SIZE;
	mqtt_params.handle_event.h_fp = aliot_mqtt_event_handle;
	mqtt_params.handle_event.pcontext = NULL;
}


/*****************************************************************************
 函数  : aliot_mqtt_msg_arrived
 描述  : 到达消息处理
 输入  : void* pcontext
       void* pclient
       aliot_mqtt_event_msg_pt msg
 输出  : 无
 返回  :
*****************************************************************************/
void aliot_mqtt_msg_arrived(void *pcontext, void *pclient, iotx_mqtt_event_msg_pt msg)
{
    iotx_mqtt_topic_info_t	   *topic_info = (iotx_mqtt_topic_info_pt) msg->msg;
	char topic[128] = {0};
	char payload[256] = {0};

    user_mqtt_info("msg->event_type : %d\n", msg->event_type);
	switch (msg->event_type) {
		case IOTX_MQTT_EVENT_PUBLISH_RECEIVED:
		{
			memcpy(topic,topic_info->ptopic,topic_info->topic_len);
			memcpy(payload,topic_info->payload,topic_info->payload_len);
			
			/* print topic name and topic message */
			user_mqtt_info("Message Arrived:");
			user_mqtt_info("Topic  : %s", topic);
			user_mqtt_info("Payload: %s", payload);
			alink_decode_cmd(payload,topic_info->payload_len);
		}
		break;
		default:
			break;
	}
}



void CheckAliReData(void)
{
	IOT_MQTT_Yield(app_pclient, 2000);
}


/*!
 * @brief MQTT连接任务,初始化MQTT参数并完成到服务器的连接
 * \n
 *
 * @param NULL
 * @return NULL
 * \n
 * @see
 */
void Mqtt_InitConnect_Start(void)
{
	u8 cent=0;
	s32 ret;
	int app_retcode=0;
	do {
			switch (user_mqtt_clint_sta)
				{
					  case USER_MQTT_GPRS_INIT:						    	//检查网络状态
						   ret=Check_SystemGprsSta(70); //35s 超时
						   if(ret!=0)
						   	{
						   		 user_mqtt_error("!!!MQTT GPRS CHECK ERROR!!!");
                                 printfErr2F("!!!MQTT GPRS CHECK ERROR!!!\r\n");
                                savemode = -1;
                                SendMsg2KernelForGetLocation(&savemode);
                                return ;
								 //break;
						   	}
						   LBSDataInit();
						   user_mqtt_clint_sta=USER_MQTT_PARMA_INIT;
						   break;
					  case USER_MQTT_PARMA_INIT:						    //初始化用户MQTT参数
					  		user_mqtt_yeinfo.heartping=0;
					  		alink_mqtt_topic_for_reg_init();
							alink_mqtt_topic_for_pub_init();
							alink_mqtt_topic_for_sub_init();
							user_mqtt_info("topic_for_sub=%s",topic_for_sub);
							user_mqtt_info("topic_for_pub=%s",topic_for_pub);
							user_mqtt_info("topic_for_alinkpub=%s",topic_for_alinkpub);
							user_mqtt_info("topic_for_alinksub=%s",topic_for_alinksub);
							user_mqtt_clint_sta=USER_MQTT_AUTH_INIT;
					  	break;
					 case USER_MQTT_AUTH_INIT:											//初始化mqtt用户登录信息
					 		ret=user_get_puser_info();
							if(ret==0)
								{
							  		user_mqtt_autu_init();
									user_mqtt_clint_sta=USER_MQTT_CONNECTING;
								}
					  	break;
					 case USER_MQTT_CONNECTING:								//mqtt连接服务器
					  		app_pclient = IOT_MQTT_Construct(&mqtt_params);
							if (NULL == app_pclient)
					            {
					                user_mqtt_error("MQTT construct failed");
					                break;
					            }
					        user_mqtt_info("MQTT construct OK.");
							app_retcode = IOT_MQTT_Subscribe(app_pclient, topic_for_alinksub, IOTX_MQTT_QOS1, aliot_mqtt_msg_arrived, NULL);
							if (app_retcode < 0)
								{
									IOT_MQTT_Destroy(&app_pclient);
									user_mqtt_error("subscribe failed, retcode = %d", app_retcode);
					                break;
								}
					         user_mqtt_clint_sta = USER_MQTT_PUB_REGISTER_MSG;
						     user_mqtt_info("MQTT connect OK.");
					  	break;
					  case USER_MQTT_PUB_REGISTER_MSG:								//发送注册信息
							  memset(&topic_msg, 0x0, sizeof(iotx_mqtt_topic_info_t));
							  topic_msg.qos = IOTX_MQTT_QOS1;
							  topic_msg.retain = 0;
							  topic_msg.dup = 0;
							  topic_msg.payload = (void*)mqtt_msg_pub;
							  topic_msg.payload_len = alink_msg_data(mqtt_msg_pub,0);
							  topic_msg.payload_len = zyf_msg_register_data(mqtt_msg_pub,0);
							  app_retcode = IOT_MQTT_Publish(app_pclient, topic_for_reg, &topic_msg);
							   if (app_retcode < 0)
					            {
					                user_mqtt_error("error %d when publish", app_retcode);
					                user_mqtt_clint_sta = USER_MQTT_CONNECTING;
					                break;
					            }
							 user_mqtt_clint_sta = USER_MQTT_CLINT_OK;
							 user_mqtt_info("USER_MQTT_CLINT_OK");
                             Ql_Sleep(1000);
                             SendMsg2KernelForSendSaveData();
							 break;
					  default:
					  	if(user_mqtt_clint_sta==USER_MQTT_CLINT_OK){
								user_mqtt_info("USER_MQTT_CLINT_OK");
					  		}
			            else {
								user_mqtt_error("should not goto here %d", __LINE__);
			            	}
			            break;
				}
	   }while(user_mqtt_clint_sta!=USER_MQTT_CLINT_OK);
}



void MqttPubUserHeartData(void)
{
	s32 ret;
	user_mqtt_info("MqttPing:%ld", user_mqtt_yeinfo.heartping);
	ret=aliot_send_ping();
	if(ret<0)
		{
			user_mqtt_error("ping error");
			user_mqtt_clint_sta = USER_MQTT_GPRS_INIT;
            SendMsg2KernelForMqttStart();
		}
	user_mqtt_yeinfo.heartping++;
}

void MqttPubUserDisData(void)
{
	int app_retcode = 0;
	user_mqtt_info("MqttPubUserDisData");
	topic_msg.payload_len=zyf_msg_Dis_data(mqtt_msg_pub);
	app_retcode = IOT_MQTT_Publish(app_pclient, topic_for_alinkpub, &topic_msg);
	if (app_retcode < 0)
		{
			user_mqtt_error("error %d when publish", app_retcode);
            user_mqtt_clint_sta = USER_MQTT_GPRS_INIT;
            SendMsg2KernelForMqttStart();
			return;
		}
}


void MqttPubUserSensorData(void)
{
	int app_retcode = 0;
	topic_msg.payload_len=alink_msg_Sensor_data(mqtt_msg_pub,0);	
	app_retcode = IOT_MQTT_Publish(app_pclient, topic_for_alinkpub, &topic_msg);
	if (app_retcode < 0)
		{
			user_mqtt_error("error %d when publish", app_retcode);
            user_mqtt_clint_sta = USER_MQTT_GPRS_INIT;
            SendMsg2KernelForMqttStart();
			return;
		}
}


void MqttPubUserReSensorData(void)
{
	int app_retcode = 0;
	topic_msg.payload_len=alink_msg_Iot_Redata(mqtt_msg_pub,ZYFRECMD[0],ZYFRECMD[1]);
	app_retcode = IOT_MQTT_Publish(app_pclient, topic_for_alinkpub, &topic_msg);
	if (app_retcode < 0)
		{
			user_mqtt_error("error %d when publish", app_retcode);
            user_mqtt_clint_sta = USER_MQTT_GPRS_INIT;
            SendMsg2KernelForMqttStart();
			return;
		}
}


void MqttPubUserSaveSensorData(void)
{
	int app_retcode = 0;
    int i = 0;
    int max_num = savedata.save_data_num;
    user_mqtt_info("Check if any save data for publish.%d \r\n",max_num);
  
    if(savedata.save_data_num != 0 && savedata.save_data_num <= MAX_POINT_LEN)
    {
    	user_mqtt_info("MqttPubUserSaveSensorData");
        for(i = max_num - 1;i >= 0;i--)
        {
           // Ql_memset(mqtt_msg_pub, 0, sizeof(mqtt_msg_pub));
        	topic_msg.payload_len=alink_msg_Save_Sensor_data(mqtt_msg_pub,&i);
        	app_retcode = IOT_MQTT_Publish(app_pclient, topic_for_alinkpub, &topic_msg);
        	if (app_retcode < 0)
        		{
                    Ql_memset(savedata.save_point + i, 0, sizeof(_save_location_point) * (max_num - i - 1)); 
                    SaveFlashSaveData(&savedata);
        			user_mqtt_error("error %d when publish", app_retcode);
                    user_mqtt_clint_sta = USER_MQTT_GPRS_INIT;
                    SendMsg2KernelForMqttStart();
        			return;
        		}
            savedata.save_data_num--;
            Ql_Sleep(10);
            SaveFlashSaveData(&savedata);
        }
        savedata.faststart = 0;
        savedata.save_data_num = 0;
        Ql_memset(savedata.save_point, 0, sizeof(_save_location_point) * max_num); 
        SaveFlashSaveData(&savedata);
        
    }
    else if(savedata.save_data_num > MAX_POINT_LEN)
    {
        savedata.save_data_num = MAX_POINT_LEN;
        SaveFlashSaveData(&savedata);
    }
    else
    {
        user_mqtt_info("Have no save data for publish.\r\n");
    }
    //进入正常获取数据模式
    
}

static void Callback_Heart_Timer(u32 timerId, void* param)
{
	SendMsg2KernelForIotHeart();
}

void Heart_Timer_init(u32 TIMER_ID, u32 ms)
{
    Ql_Timer_Register(TIMER_ID, Callback_Heart_Timer, NULL);
    Ql_Timer_Start(TIMER_ID,ms,TRUE);
}



u32 Heart_T=0;
u32 IotData_T=0;



static void Callback_Iot_Timer(u32 timerId, void* param)
{
	u32 t1=60/10;
	u32 t2=systemset.HandInter/10;
	if(t1<1)t1=1;
	if(t2<1)t2=1;
	if(user_mqtt_clint_sta==USER_MQTT_CLINT_OK)
		{
			Heart_T++;
			IotData_T++;
			if(Heart_T>=t1)

				{

					Heart_T=0;
					SendMsg2KernelForIotHeart();
				}
			if(IotData_T>=t2)

				{

					IotData_T=0;
					Heart_T=0;							//取消最近一次心跳
					SendMsg2KernelForIotData();
				}
		}
}


static void Callback_led_Timer(u32 timerId, void* param)
{
	if(Ql_GPIO_GetLevel(LED2))
		{
			LED2_H;
		}
	else
		{
			LED2_L;
		}
}

void Iotdata_Timer_init(u32 TIMER_ID, u32 ms)
{
    Ql_Timer_Register(TIMER_ID, Callback_Iot_Timer, NULL);
    Ql_Timer_Start(TIMER_ID,ms,TRUE);
}


void Iotdata_Timer_Start(u32 TIMER_ID, u32 ms)
{
	 Ql_Timer_Start(TIMER_ID,ms,TRUE);
}

void Iotdata_Timer_Stop(u32 TIMER_ID, u32 ms)
{
	 Ql_Timer_Stop(TIMER_ID);
}



void Led_Timer_init(u32 TIMER_ID, u32 ms)
{
    Ql_Timer_Register(TIMER_ID, Callback_led_Timer, NULL);
    Ql_Timer_Start(TIMER_ID,ms,TRUE);
}




/*!
 * 定时增加阿里云系统时间,回调函数，时间到后执行一次
 * \n
 *
 * @param secd
 * @return
 *
 * \n
 * @see
 */

static void Callback_aliot_Timer(u32 timerId, void* param)
{
	systemset.SysTime++;
    SysOsTime++;
    //GetModuleData();
    //mprintf(" timer : %d s.\r\n",systemset.SysTime);
}



/*!
 * 给阿里云提供的系统定时器1s钟增加一次
 * \n
 *
 * @param ms    要定时的毫秒数(需要大于10ms)
 * @return
 *
 * \n
 * @see
 */

void aliot_Timer_init(u32 TIMER_ID, u32 ms)
{
    Ql_Timer_RegisterFast(TIMER_ID, Callback_aliot_Timer, NULL);
    Ql_Timer_Start(TIMER_ID, ms, TRUE);

}

static void Callback_test_Timer(u32 timerId, void* param)
{
    s32 t1,t2;
	int ret = RIL_GetPowerSupply(&t1, &t2);
	if(ret!=RIL_AT_SUCCESS)
	 	{
	 		mprintf("RIL_GetPowerSupply Error");
	 	}
    if(G_Bvol < t1)
        G_Bvol = t1;
//	mprintf("capacity,%d,",t1);
//	mprintf("voltage,%d\r\n",t2);
}


void TEST_Timer_init(u32 TIMER_ID, u32 ms)
{
    Ql_Timer_Register(TIMER_ID, Callback_test_Timer, NULL);
//    Ql_Timer_Start(TIMER_ID,ms,TRUE);
}


