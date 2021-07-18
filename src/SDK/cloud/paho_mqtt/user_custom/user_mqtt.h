
#ifndef __USER_MQTT__
#define __USER_MQTT__


#include "ril.h"
#include "ril_util.h"
#include "ril_telephony.h"
#include "ql_stdlib.h"
#include "ql_error.h"
#include "ql_trace.h"
#include "ql_uart.h"
#include "ql_system.h"

#define BLE_LIST_MAX_NUM	800

#define BLE_SEND_LIST		5


typedef struct 
{
	u8 mqttsta;
	u8 blesta;
	u32 heartping;
}user_ble_mqtt_info_t;

typedef struct 
{
	 u8 isUsd;
	 u8 name[20];
	 u8 mac[20];
	 u8 rssi[5];
}ble_mac_info_t;

typedef struct 
{
	u32 index;
	ble_mac_info_t ble_mac_info[10];
}ble_info_t;


typedef struct 
{
	u32 index;
	ble_mac_info_t ble_mac_info[BLE_LIST_MAX_NUM];
}ble_listinfo_t;



extern  ble_listinfo_t ble_info_list;

extern user_ble_mqtt_info_t ble_mqtt_info;
extern ble_info_t ble_infos;

extern char bleaddr[13];

int User_PahoMqtt_Test(u8 dow);
void Add_adv_report2List(u8 *name,u8 *mac,u8 *rssi);
void BleInfoPayloadData(u8 *cyc,u8 *num);
void publishSta(u8 *payload,u16 length);
void RdsubData(void);
#endif

