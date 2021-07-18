
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <time.h>
#include "aliot_platform.h"

#include "aliot_error.h"
#include "aliot_log.h"
#include "aliot_md5.h"
#include "aliot_hmac.h"
#include "aliot_httpclient.h"
#include "aliot_jsonparser.h"

#include "aliot_ca.h"
#include "aliot_auth.h"

#define SIGN_SOURCE_LEN (256)
#define SIGN_DIGEST_LEN (36)
#define HTTP_POST_MAX_LEN (500)
#define HTTP_RESP_MAX_LEN (500)

const static char* iot_auth_host = "http://iot-auth.cn-shanghai.aliyuncs.com/auth/devicename";

static int aliot_get_id_token(
    const char* auth_host,
    const char* product_key,
    const char* device_name,
    const char* device_secret,
    const char* client_id,
    const char* version,
    const char* timestamp,
    const char* resources,
    char* iot_id,
    char* iot_token,
    char* host,
    uint16_t* pport)
{
    int ret = -1, length;
    char* sign = NULL;
    char* buf = NULL, *post_buf = NULL, *response_buf = NULL;
    httpclient_t httpclient;
    httpclient_data_t httpclient_data;
    length = STRLEN(client_id);
    length += STRLEN(product_key);
    length += STRLEN(device_name);
    length += STRLEN(timestamp);
    length += 40; //40 chars space for key strings(clientId,deviceName,productKey,timestamp)
    if (length > SIGN_SOURCE_LEN) {
        ALIOT_LOG_ERROR("sign_source len = %d too long. client_id=%s, product_key=%s, device_name=%s, timestamp= %s",
                        length, client_id, product_key, device_name, timestamp);
        return ERROR_AUTH_DATA_LENGTH;
    }
	
    if (NULL == (buf = aliot_platform_malloc(length))) {
        ALIOT_LOG_FATAL("malloc %d bytes fail", length);
        return ERROR_AUTH_MEM_ALLOC;
    }
    //Calculate sign
    if (NULL == (sign = aliot_platform_malloc(SIGN_DIGEST_LEN))) {
        ret = ERROR_AUTH_MEM_ALLOC;
        ALIOT_LOG_FATAL("malloc %d bytes fail", SIGN_DIGEST_LEN);
        goto do_exit;
    }
    MEMSET(sign, 0, SIGN_DIGEST_LEN);
    ret = SNPRINTF(buf,
                   SIGN_SOURCE_LEN,
                   "clientId%sdeviceName%sproductKey%stimestamp%s",
                   client_id,
                   device_name,
                   product_key,
                   timestamp);
    if ((ret < 0) || (ret > SIGN_SOURCE_LEN)) {
        ALIOT_LOG_FATAL("generate sign_source");
        ret = ERROR_AUTH_GENERATE_DATA;
        goto do_exit;
    }
    ALIOT_LOG_DEBUG("sign_source=%s (%d bytes)", buf, STRLEN(buf));
    aliot_hmac_md5(buf, STRLEN(buf), sign, device_secret, STRLEN(device_secret));
    MEMSET(&httpclient, 0, sizeof(httpclient_t));
    httpclient.header = "Accept: text/xml,text/javascript,text/html,application/json\r\n";
    MEMSET(&httpclient_data, 0, sizeof(httpclient_data_t));
    post_buf = (char*)aliot_platform_malloc(HTTP_POST_MAX_LEN);
    if (NULL == post_buf) {
        ALIOT_LOG_ERROR("malloc %d bytes fail",HTTP_POST_MAX_LEN);
        ret = ERROR_AUTH_MEM_ALLOC;
        goto do_exit;
    }
    MEMSET(post_buf, 0, HTTP_POST_MAX_LEN);
    ret = SNPRINTF(post_buf,
                   HTTP_POST_MAX_LEN,
                   "productKey=%s&deviceName=%s&sign=%s&version=%s&clientId=%s&timestamp=%s&resources=%s",
                   product_key,
                   device_name,
                   sign,
                   version,
                   client_id,
                   timestamp,
                   resources);
    if ((ret < 0) || (ret >= HTTP_POST_MAX_LEN)) {
        ALIOT_LOG_ERROR("http message body is too long");
        ret = ERROR_AUTH_GENERATE_DATA;
        goto do_exit;
    }
    ALIOT_LOG_DEBUG("http content:%s (%d bytes)\n\r", post_buf, STRLEN(post_buf));
    ret = STRLEN(post_buf);
    response_buf = (char*)aliot_platform_malloc(HTTP_RESP_MAX_LEN);
    if (NULL == response_buf) {
        ALIOT_LOG_ERROR("malloc %d bytes fail", HTTP_RESP_MAX_LEN);
        ret = ERROR_AUTH_MEM_ALLOC;
        goto do_exit;
    }
    MEMSET(response_buf, 0, HTTP_RESP_MAX_LEN);
    httpclient_data.post_content_type = "application/x-www-form-urlencoded;charset=utf-8";
    httpclient_data.post_buf = post_buf;
    httpclient_data.post_buf_len = ret;
    httpclient_data.response_buf = response_buf;
    httpclient_data.response_buf_len = HTTP_RESP_MAX_LEN;
    ret = aliot_post(&httpclient,
                     auth_host,
                     443,
                     aliot_ca_get(),
                     10000,
                     &httpclient_data);
    if (ret != SUCCESS_RETURN) {
        ALIOT_LOG_FATAL("http post failed %d", ret);
        ret = ERROR_AUTH_HTTP_POST;
        goto do_exit;
    }
    ALIOT_LOG_DEBUG("http response:%s\n\r", httpclient_data.response_buf);
    //get iot-id and iot-token from response
    int type;
    const char* pvalue, *presrc;
    char port_str[6];
    //get iot-id
    if (NULL == (pvalue = json_get_value_by_fullname(
                              httpclient_data.response_buf,
                              STRLEN(httpclient_data.response_buf),
                              "data.iotId",
                              &length,
                              &type))) {
        ret = ERROR_AUTH_JSON_PARSE;
        ALIOT_LOG_FATAL("get id");
        goto do_exit;
    }
    memcpy(iot_id, pvalue, length);
    iot_id[length] = '\0';
    //get iot-token
    if (NULL == (pvalue = json_get_value_by_fullname(
                              httpclient_data.response_buf,
                              STRLEN(httpclient_data.response_buf),
                              "data.iotToken",
                              &length,
                              &type))) {
        ret = ERROR_AUTH_JSON_PARSE;
        ALIOT_LOG_FATAL("get token");
        goto do_exit;
    }
    memcpy(iot_token, pvalue, length);
    iot_token[length] = '\0';
    /*get host and port.*/
    if (NULL == (presrc = json_get_value_by_fullname(
                              httpclient_data.response_buf,
                              STRLEN(httpclient_data.response_buf),
                              "data.resources.mqtt",
                              &length,
                              &type))) {
        ret = ERROR_AUTH_JSON_PARSE;
        ALIOT_LOG_FATAL("get res-mqtt");
        goto do_exit;
    }
    //get host
    if (NULL == (pvalue = json_get_value_by_fullname(
                              presrc,
                              STRLEN(presrc),
                              "host",
                              &length,
                              &type))) {
        ret = ERROR_AUTH_JSON_PARSE;
        ALIOT_LOG_FATAL("get host");
        goto do_exit;
    }
    memcpy(host, pvalue, length);
    host[length] = '\0';
    //get port
    if (NULL == (pvalue = json_get_value_by_fullname(
                              presrc,
                              STRLEN(presrc),
                              "port",
                              &length,
                              &type))) {
        ret = ERROR_AUTH_JSON_PARSE;
        ALIOT_LOG_FATAL("get port");
        goto do_exit;
    }
    memcpy(port_str, pvalue, length);
    port_str[length] = '\0';
    *pport = atoi(port_str);
    ALIOT_LOG_INFO("get_id_token Success\niot-id=%s\niot-token=%s\nhost=%s\nport=%d\r\n",
                   iot_id,
                   iot_token,
                   host,
                   *pport);
    ret = SUCCESS_RETURN;
do_exit:
    if (NULL != buf) {
        aliot_platform_free(buf);
    }
    if (NULL != sign) {
        aliot_platform_free(sign);
    }
    if (NULL != post_buf) {
        aliot_platform_free(post_buf);
    }
    if (NULL != response_buf) {
        aliot_platform_free(response_buf);
    }
    return ret;
}

int32_t aliot_auth(aliot_device_info_pt pdevice_info, aliot_user_info_pt puser_info)
{
    int ret = 0;
#if !ALIOT_AUTH_SKIP
    char iot_id[ALIOT_AUTH_IOT_ID + 1], iot_token[ALIOT_AUTH_IOT_TOKEN + 1], host[HOST_ADDRESS_LEN + 1];
    uint16_t port;
#if MYDEVICE_NOT_SUPPORT_TIME
    const char timestamp[16] = "1502974619";
#else
    char timestamp[16];
    SNPRINTF(timestamp, sizeof(timestamp), "%u", time(NULL));
#endif
    ret = aliot_get_id_token(
              iot_auth_host,
              pdevice_info->product_key,
              pdevice_info->device_name,
              pdevice_info->device_secret,
              pdevice_info->device_id,
              "default",
              timestamp, //01 Jan 2050
              "mqtt",
              iot_id,
              iot_token,
              host,
              &port);
    if (SUCCESS_RETURN != ret) {
        ALIOT_LOG_FATAL("get id-token fail %d", ret);
        return ret;
    }
    strncpy(puser_info->user_name, iot_id, USER_NAME_LEN);
    strncpy(puser_info->password, iot_token, PASSWORD_LEN);
    strncpy(puser_info->host_name, host, HOST_ADDRESS_LEN);
    puser_info->port = port;
	ALIOT_LOG_INFO("AUTH request success!");
#else
    extern const char* g_al_iot_host;
    extern const uint16_t g_al_iot_port;
    extern const char* g_al_iot_id;
    extern const char* g_al_iot_token;
    // 跳过认证(测试用途)
    strncpy(puser_info->user_name, g_al_iot_id, USER_NAME_LEN);
    strncpy(puser_info->password, g_al_iot_token, PASSWORD_LEN);
    strncpy(puser_info->host_name, g_al_iot_host, HOST_ADDRESS_LEN);
    puser_info->port = g_al_iot_port;
	ALIOT_LOG_WARN("AUTH request skiped.");
#endif
#if ALIOT_MQTT_TCP
    puser_info->pubKey = NULL;
#else
    puser_info->pubKey = aliot_ca_get();
#endif
    if (NULL == puser_info->pubKey) {
        char pid[16];
        //Append string "nonesecure" if TCP connection be used.
        if (NULL == aliot_platform_module_get_pid(pid)) {
            ret = SNPRINTF(puser_info->client_id,
                           CLIENT_ID_LEN,
                           "%s|securemode=0|",
                           pdevice_info->device_id);
        } else {
            //Append "pid" if we have pid
            ret = SNPRINTF(puser_info->client_id,
                           CLIENT_ID_LEN,
                           "%s|securemode=0,pid=%s|",
                           pdevice_info->device_id,
                           pid);
        }
    } else {
        char pid[16];
        if (NULL == aliot_platform_module_get_pid(pid)) {
            ret = SNPRINTF(puser_info->client_id,
                           CLIENT_ID_LEN,
                           "%s",
                           pdevice_info->device_id);
        } else {
            //Append "pid" if we have pid
            ret = SNPRINTF(puser_info->client_id,
                           CLIENT_ID_LEN,
                           "%s|pid=%s|",
                           pdevice_info->device_id,
                           pid);
        }
    }
    if (ret >= CLIENT_ID_LEN) {
        ALIOT_LOG_ERROR("client_id is too long");
    } else if (ret < 0) {
        ALIOT_LOG_FATAL("generate client_id fail %d", ret);
        return ERROR_AUTH_GENERATE_CLIENT_ID;
    }
    return SUCCESS_RETURN;
}
