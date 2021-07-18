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
#include "aliot_platform.h"
#include "aliot_log.h"
#include "aliot_mqtt_client.h"
#include "aliot_auth.h"
#include "aliot_device.h"
#include "aliot_log.h"
#include "aliot_platform_network.h"
#include "user_mqtt.h"
#include "sys.h"
#include "uart.h"
#include "zyf_auto_config.h"
#include "zyf_protocol.h"
#include "user_lmq.h"


#ifdef USER_MQTT_DEBUG
    #define user_mqtt_info(format, ...)  mprintf( format "\r\n", ##__VA_ARGS__)
    #define user_mqtt_error(format, ...) mprintf( "[error]%s() %d " format "\r\n", /*__FILE__,*/ __FUNCTION__, __LINE__, ##__VA_ARGS__)
#else
    #define user_mqtt_info(format, ...)
    #define user_mqtt_error(format, ...)
#endif


char mqtt_msg_read_buf[MQTT_MSG_READ_BUF_SIZE];			// 订阅的消息 缓冲
char mqtt_msg_write_buf[MQTT_MSG_WRITE_BUF_SIZE];		// 需要发布的消息 缓冲
char mqtt_msg_pub[AMC_PUB_LENGTH_MAX];					// 需要发布的消息

char topic_for_pub[32];			//用户mqtt发布的主题
char topic_for_sub[32];			//用户mqtt发布的主题
char topic_for_upb[32];			//系统升级主题
char topic_for_reg[32];			//用户mqtt注册信息主题
char topic_for_red[32];			//回复数据
char topic_for_dis[32];			//正常离线



user_mqtt_info_t user_mqtt_yeinfo; //业务部分逻辑集合

aliot_user_info_t user_puser_info;										//用户登录MQTT需要的参数	

user_mqtt_param_t user_mqtt_param;										//用户mqtt参数和信息

USER_MQTT_CLINT_STA_T	user_mqtt_clint_sta=USER_MQTT_PARMA_INIT;		//用户mqtt客户端状态

void *app_pclient=NULL;


/*!
 * @brief MQTT注册主题初始化
 * \n
 *
 * @param NULL
 * @return NULL
 * \n
 * @see
 */


void user_mqtt_topic_for_red_init(void)
{
	Ql_memset(topic_for_red, 0, sizeof(topic_for_red));
	Ql_strcat(topic_for_red, "/");
	Ql_strcat(topic_for_red, "zyf");
	Ql_strcat(topic_for_red, "/");
	Ql_strcat(topic_for_red, "device");
	Ql_strcat(topic_for_red, "/result");

}



/*!
 * @brief MQTT正常离线
 * \n
 *
 * @param NULL
 * @return NULL
 * \n
 * @see
 */

void user_mqtt_topic_for_dis_init(void)
{
	Ql_memset(topic_for_dis, 0, sizeof(topic_for_dis));
	Ql_strcat(topic_for_dis, "/");
	Ql_strcat(topic_for_dis, "zyf");
	Ql_strcat(topic_for_dis, "/");
	Ql_strcat(topic_for_dis, "device");
	Ql_strcat(topic_for_dis, "/inout");

}






/*!
 * @brief MQTT注册主题初始化
 * \n
 *
 * @param NULL
 * @return NULL
 * \n
 * @see
 */
void user_mqtt_topic_for_reg_init(void)
{
	Ql_memset(topic_for_reg, 0, sizeof(topic_for_reg));
	Ql_strcat(topic_for_reg, "/");
	Ql_strcat(topic_for_reg, "zyf");
	Ql_strcat(topic_for_reg, "/");
	Ql_strcat(topic_for_reg, "device");
	Ql_strcat(topic_for_reg, "/register");

}



static int my_tolower(int c)  
{  
    if ((c >= 'A') && (c <= 'Z'))
    	{
        	return c + ('a' - 'A');
    	}
    return c;  
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
	u8 i;
	u8 temp[16]={0};
	for(i=0;i<Ql_strlen(systemset.SN);i++)
		{
			temp[i]= my_tolower(systemset.SN[i]);
		}
	temp[i]=0;
	Ql_memset(topic_for_pub, 0, sizeof(topic_for_pub));
	Ql_strcat(topic_for_pub, "/");
	Ql_strcat(topic_for_pub, ZYFAutoconfig.pk);
	Ql_strcat(topic_for_pub, "/");
	Ql_strcat(topic_for_pub, temp);
	Ql_strcat(topic_for_pub, "/data");
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
	u8 i;
	u8 temp[16]={0};
	for(i=0;i<Ql_strlen(systemset.SN);i++)
		{
			temp[i]= my_tolower(systemset.SN[i]);
		}
	temp[i]=0;
	Ql_memset(topic_for_sub, 0, sizeof(topic_for_sub));
	Ql_strcat(topic_for_sub, "/");
	Ql_strcat(topic_for_sub, ZYFAutoconfig.pk);
	Ql_strcat(topic_for_sub, "/");
	Ql_strcat(topic_for_sub, temp);
	Ql_strcat(topic_for_sub, "/ins");
	
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
void user_mqtt_topic_for_upb_init(void)
{
	u8 i;
	u8 temp[16]={0};
	for(i=0;i<Ql_strlen(systemset.SN);i++)
		{
			temp[i]= my_tolower(systemset.SN[i]);
		}
	temp[i]=0;
	Ql_memset(topic_for_upb, 0, sizeof(topic_for_upb));
	Ql_strcat(topic_for_upb, "/");
	Ql_strcat(topic_for_upb, ZYFAutoconfig.pk);
	Ql_strcat(topic_for_upb, "/");
	Ql_strcat(topic_for_upb, temp);
	Ql_strcat(topic_for_upb, "/upb");
	
}




/*!
 * @brief 获取MQTT登录信息
 * \n
 *
 * @param 		parama 智云服登录信息 
 *				aliot_user_puser  MQTT需要的登录信息
 * @return 1 未开启SSL 直接返回成功
 * \n
 * @see
 */


s8 user_get_puser_info_zyf(ZYF_AUTO_CONFIG_T parama,aliot_user_info_t *aliot_user_puser)
{
	aliot_user_puser->port=parama.port;
	Ql_strcpy(aliot_user_puser->host_name,parama.host_name);
	Ql_strcpy(aliot_user_puser->client_id,systemset.SN);
	Ql_strcpy(aliot_user_puser->user_name,parama.user_name);
	Ql_strcpy(aliot_user_puser->password,parama.password);
	aliot_user_puser->pubKey=NULL;
	return 0;
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


static void aliot_mqtt_event_handle(void* pcontext, void* pclient, aliot_mqtt_event_msg_pt msg)
{
    u32 packet_id = (u32)msg->msg;
    aliot_mqtt_topic_info_pt topic_info = (aliot_mqtt_topic_info_pt)msg->msg;
    switch (msg->event_type)
    {
    case ALIOT_MQTT_EVENT_UNDEF:
        user_mqtt_error("undefined event occur.");
        break;
    case ALIOT_MQTT_EVENT_DISCONNECT:
		user_mqtt_clint_sta = USER_MQTT_GPRS_INIT;
        SendMsg2KernelForMqttStart();
        user_mqtt_error("MQTT disconnect.");
        break;
    case ALIOT_MQTT_EVENT_RECONNECT:
        user_mqtt_info("MQTT reconnected.");
        break;
    case ALIOT_MQTT_EVENT_SUBCRIBE_SUCCESS:
        user_mqtt_info("subscribe success, packet-id=%u", packet_id);
        break;
    case ALIOT_MQTT_EVENT_SUBCRIBE_TIMEOUT:
        user_mqtt_error("subscribe wait ack timeout, packet-id=%u", packet_id);
        break;
    case ALIOT_MQTT_EVENT_SUBCRIBE_NACK:
        user_mqtt_error("subscribe nack, packet-id=%u", packet_id);
        break;
    case ALIOT_MQTT_EVENT_UNSUBCRIBE_SUCCESS:
        user_mqtt_info("unsubscribe success, packet-id=%u", packet_id);
        break;
    case ALIOT_MQTT_EVENT_UNSUBCRIBE_TIMEOUT:
        user_mqtt_error("unsubscribe timeout, packet-id=%u", packet_id);
        break;
    case ALIOT_MQTT_EVENT_UNSUBCRIBE_NACK:
        user_mqtt_error("unsubscribe nack, packet-id=%u", packet_id);
        break;
    case ALIOT_MQTT_EVENT_PUBLISH_SUCCESS:
        user_mqtt_info("publish success, packet-id=%u", packet_id);
        break;
    case ALIOT_MQTT_EVENT_PUBLISH_TIMEOUT:
        user_mqtt_error("publish timeout, packet-id=%u", packet_id);
        break;
    case ALIOT_MQTT_EVENT_PUBLISH_NACK:
        user_mqtt_error("publish nack, packet-id=%u", packet_id);
        break;
    case ALIOT_MQTT_EVENT_PUBLISH_RECVEIVED:
        user_mqtt_error("arrived message but without any related handle,len=%d,payload=%s",
                  topic_info->payload_len,
                  topic_info->payload);
        // 重新连接
       	 user_mqtt_clint_sta = USER_MQTT_GPRS_INIT;
         SendMsg2KernelForMqttStart();
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

void user_mqtt_autu_init(aliot_user_info_pt app_puser_info)
{
   Ql_memset(&user_mqtt_param.mqtt_params, 0x0, sizeof(user_mqtt_param.mqtt_params));
   user_mqtt_param.mqtt_params.port = app_puser_info->port;
   user_mqtt_param.mqtt_params.host = app_puser_info->host_name;
   user_mqtt_param.mqtt_params.client_id = app_puser_info->client_id;
   user_mqtt_param.mqtt_params.user_name = app_puser_info->user_name;
   user_mqtt_param.mqtt_params.password = app_puser_info->password;
   user_mqtt_param.mqtt_params.pub_key = app_puser_info->pubKey;
   user_mqtt_param.mqtt_params.request_timeout_ms = MQTT_REQUEST_TIMEOUT_MS;
   user_mqtt_param.mqtt_params.clean_session = 0;
   user_mqtt_param.mqtt_params.keepalive_interval_ms = MQTT_KEEPALIVE_INTERVAL_MS;
   user_mqtt_param.mqtt_params.pread_buf = mqtt_msg_read_buf;
   user_mqtt_param.mqtt_params.read_buf_size = MQTT_MSG_READ_BUF_SIZE;
   user_mqtt_param.mqtt_params.pwrite_buf = mqtt_msg_write_buf;
   user_mqtt_param.mqtt_params.write_buf_size = MQTT_MSG_WRITE_BUF_SIZE;
   user_mqtt_param.mqtt_params.handle_event.h_fp = aliot_mqtt_event_handle;
   user_mqtt_param.mqtt_params.handle_event.pcontext = NULL;
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
void aliot_mqtt_msg_arrived(void* pcontext, void* pclient, aliot_mqtt_event_msg_pt msg)
{
    aliot_mqtt_topic_info_pt ptopic_info = (aliot_mqtt_topic_info_pt)msg->msg;
    user_mqtt_info("topic=%s", ptopic_info->ptopic);
    user_mqtt_info("payload_len=%d", ptopic_info->payload_len);
	user_mqtt_info("payload=%s", ptopic_info->payload);
	zyf_decode_cmd(ptopic_info->payload,ptopic_info->payload_len);
}

void upb_aliot_mqtt_msg_arrived(void* pcontext, void* pclient, aliot_mqtt_event_msg_pt msg)
{
	aliot_mqtt_topic_info_pt ptopic_info = (aliot_mqtt_topic_info_pt)msg->msg;
    user_mqtt_info("topic=%s", ptopic_info->ptopic);
    user_mqtt_info("payload_len=%d", ptopic_info->payload_len);
	user_mqtt_info("payload=%s", ptopic_info->payload);
	zyf_decode_upbcmd(ptopic_info->payload,ptopic_info->payload_len);
}


void CheckAliReData(void)
{
    aliot_mqtt_yield(app_pclient, MQTT_YIELD_INTERVAL_MS);
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
					  	   ret=Check_SystemGprsSta(180);
						   if(ret!=0)
						   	{
						   		 user_mqtt_error("!!!MQTT GPRS CHECK ERROR!!!");
								 break;
						   	}
						   user_mqtt_clint_sta=USER_MQTT_PARMA_INIT;
						   break;
					  case USER_MQTT_PARMA_INIT:						    //初始化用户MQTT参数
					  		user_mqtt_yeinfo.heartping=0;
							user_mqtt_topic_for_red_init();
					  		user_mqtt_topic_for_pub_init();
							user_mqtt_topic_for_sub_init();
							user_mqtt_topic_for_reg_init();
							user_mqtt_topic_for_upb_init();
							user_mqtt_topic_for_dis_init();
							user_mqtt_info("topic_for_sub=%s",topic_for_sub);
							user_mqtt_info("topic_for_pub=%s",topic_for_pub);
							user_mqtt_info("topic_for_reg=%s",topic_for_reg);
							user_mqtt_info("topic_for_upb=%s",topic_for_upb);
							user_mqtt_info("topic_for_red=%s",topic_for_red);
							user_mqtt_info("topic_for_dis=%s",topic_for_dis);
							user_mqtt_clint_sta=USER_MQTT_AUTH_INIT;
					  	break;
					 case USER_MQTT_AUTH_INIT:											//初始化mqtt用户登录信息
					 		ret=user_get_puser_info_zyf(ZYFAutoconfig,&user_puser_info);
							if(ret==0)
								{
									user_mqtt_info("user_puser_info.port=%d",user_puser_info.port);
									user_mqtt_info("user_puser_info.host_name=%s",user_puser_info.host_name);
									user_mqtt_info("user_puser_info.client_id=%s",user_puser_info.client_id);
									user_mqtt_info("user_puser_info.user_name=%s",user_puser_info.user_name);
									user_mqtt_info("user_puser_info.password=%s",user_puser_info.password);
							  		user_mqtt_autu_init(&user_puser_info);
									user_mqtt_clint_sta=USER_MQTT_CONNECTING;
								}
					  	break;
					 case USER_MQTT_CONNECTING:								//mqtt连接服务器
					  		app_pclient = aliot_mqtt_construct(&user_mqtt_param.mqtt_params);
							if (NULL == app_pclient)
					            {
					                user_mqtt_error("MQTT construct failed");
					                break;
					            }
					        user_mqtt_info("MQTT construct OK.");
						    app_retcode = aliot_mqtt_subscribe(app_pclient,topic_for_sub, ALIOT_MQTT_QOS1, aliot_mqtt_msg_arrived, NULL);
					        if (app_retcode < 0)
					            {
					                aliot_mqtt_deconstruct(app_pclient);
					                user_mqtt_error("subscribe failed, retcode = %d", app_retcode);
					                break;
					            }
							 app_retcode = aliot_mqtt_subscribe(app_pclient,topic_for_upb, ALIOT_MQTT_QOS1, upb_aliot_mqtt_msg_arrived, NULL);
					        if (app_retcode < 0)
					            {
					                aliot_mqtt_deconstruct(app_pclient);
					                user_mqtt_error("subscribe failed, retcode = %d", app_retcode);
					                break;
					            }
					         user_mqtt_clint_sta = USER_MQTT_PUB_REGISTER_MSG;
						     user_mqtt_info("MQTT connect OK.");
					  	break;
					  case USER_MQTT_PUB_REGISTER_MSG:								//发送注册信息
					  		  user_mqtt_param.topic_msg.qos = ALIOT_MQTT_QOS1;
					          user_mqtt_param.topic_msg.retain = 0;
					          user_mqtt_param.topic_msg.dup = 0;
					          user_mqtt_param.topic_msg.payload = (void*)mqtt_msg_pub;
							  user_mqtt_param.topic_msg.payload_len=zyf_msg_register_data(mqtt_msg_pub,0);
							  user_mqtt_info("MQTT connect OK.");
							  app_retcode = aliot_mqtt_publish(app_pclient,topic_for_reg, &user_mqtt_param.topic_msg);
					        if (app_retcode < 0)
					            {
					                user_mqtt_error("error %d when publish", app_retcode);
					                user_mqtt_clint_sta = USER_MQTT_CONNECTING;
					                break;
					            }
							 cent=100;
							 while(cent)
							 	{
							 		cent--;
							 		Ql_Sleep(100);
							 		CheckAliReData();
									if(ZYF_MQTT_CONFIG&(1<<0))
										{
											ZYF_MQTT_CONFIG&=~(1<<0);
											break;
										}
							 	}
							 user_mqtt_param.topic_msg.payload_len=zyf_msg_RegSensor_data(mqtt_msg_pub,0);
							 app_retcode = aliot_mqtt_publish(app_pclient, topic_for_pub, &user_mqtt_param.topic_msg);
							  if (app_retcode < 0)
					            {
					                user_mqtt_error("error %d when publish", app_retcode);
					                user_mqtt_clint_sta = USER_MQTT_CONNECTING;
					                break;
					            }
							 SendMsg2KernelForIotData();
							 user_mqtt_clint_sta = USER_MQTT_CLINT_OK;
							 user_mqtt_info("USER_MQTT_CLINT_OK");
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
	user_mqtt_info("MqttPing:%d", user_mqtt_yeinfo.heartping);
	ret=aliot_send_ping();
	if(ret<0)
		{
			user_mqtt_error("ping error");
			user_mqtt_clint_sta = USER_MQTT_GPRS_INIT;
            SendMsg2KernelForMqttStart();
		}
	user_mqtt_yeinfo.heartping++;
}


void MqttPubUserSensorData(void)
{
	int app_retcode = 0;
	user_mqtt_info("MqttPubUserSensorData");
	user_mqtt_param.topic_msg.payload_len=zyf_msg_Sensor_data(mqtt_msg_pub,0);
	app_retcode = aliot_mqtt_publish(app_pclient, topic_for_pub, &user_mqtt_param.topic_msg);
	if (app_retcode < 0)
		{
			user_mqtt_error("error %d when publish", app_retcode);
            user_mqtt_clint_sta = USER_MQTT_GPRS_INIT;
            SendMsg2KernelForMqttStart();
			return;
		}
}


void MqttPubUserDisData(void)
{
	int app_retcode = 0;
	user_mqtt_info("MqttPubUserDisData");
	user_mqtt_param.topic_msg.payload_len=zyf_msg_Dis_data(mqtt_msg_pub);
	app_retcode = aliot_mqtt_publish(app_pclient, topic_for_dis, &user_mqtt_param.topic_msg);
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
	user_mqtt_info("MqttPubUserReSensorData");
	user_mqtt_param.topic_msg.payload_len=zyf_msg_Iot_Redata(mqtt_msg_pub,ZYFRECMD[0],ZYFRECMD[1]);
	aliot_mqtt_publish(app_pclient, topic_for_red, &user_mqtt_param.topic_msg);
	if (app_retcode < 0)
		{
			user_mqtt_error("error %d when publish", app_retcode);
            user_mqtt_clint_sta = USER_MQTT_GPRS_INIT;
            SendMsg2KernelForMqttStart();
			return;
		}
}

void isAliveDevices(void)
{
	int app_retcode = 0;
	user_mqtt_info("isAliveDevices");
	user_mqtt_param.topic_msg.payload_len=zyf_msg_register_data(mqtt_msg_pub,0);
	app_retcode = aliot_mqtt_publish(app_pclient,topic_for_reg, &user_mqtt_param.topic_msg);
	if (app_retcode < 0)
		{
			user_mqtt_error("error %d when publish", app_retcode);
            user_mqtt_clint_sta = USER_MQTT_GPRS_INIT;
            SendMsg2KernelForMqttStart();
			return;
		}
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
	if(Ql_GPIO_GetLevel(LED1))
		{
			LED1_H;
		}
	else
		{
			LED1_L;
		}
	if(Need_Lbs_Data==1)
		{
			s32 ret;
			Need_Lbs_Data=0;
	 		ret=LBSDataInit();
			mprintf("RIL_GetLocation=%d\r\n",ret);
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
    Ql_Timer_Register(TIMER_ID, Callback_aliot_Timer, NULL);
    Ql_Timer_Start(TIMER_ID, ms, TRUE);

}
