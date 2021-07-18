#ifndef	 	_LMQ_H_  
#define 	_LMQ_H_ 

#include "ql_type.h"
#include "aliot_device.h"


typedef struct {
	char *host;
	char *groupId;
    char *deviceId;
    char *accessKey;
    char *secretKey;
    int port;
    int qos;
}USER_LMQ_CONFIG_T;

extern USER_LMQ_CONFIG_T user_lmq_config;

s8 user_get_puser_info_alilmq(USER_LMQ_CONFIG_T parama,aliot_user_info_t *aliot_user_puser);

#endif

