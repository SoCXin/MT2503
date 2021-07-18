



#ifndef JSON_ANAYSIS_H_
#define JSON_ANAYSIS_H_


typedef enum
{
	SET_BLE_PRW=0,				//设置蓝牙发射功率命令
	SET_BLE_SEARCH_INTERVAL,	//设置蓝牙搜索间隔命令
	SET_BLE_SEARCH_WINDOW,		//设置蓝牙搜索窗口时间命令
	SET_BLE_REPORT_INTERVAL,	//设置蓝牙信息上报间隔命令
	SET_BLE_BATTERY_VOLTAGE,	//设置电池电压命令
	SET_BLE_BATTERY_CURRENT,	//设置电池电流命令
	SET_BLE_BATTERY_MAX_VOLTAGE,//设置外接电源电压上限命令
	SET_BLE_BATTERY_MAX_CURRENT,//设置外接电源电流上限命令
	SET_BLE_SLEEP_TIME,			//设置休眠时间命令
	SET_BLE_RESET,				//设备复位命令
	SET_BLE_MQTT_PARAMETER,		//设备mqtt通讯参数设置
	CMD_Error,					//指令错误
}E_S2C_CMD;//接受的指令码
	


typedef enum
{
	TRANSMIT_BLE_LIST=0,
	TRANSMIT_BLE_HEARTBEAT,
	TRANSMIT_ACK,
}E_C2S_CMD;//发送的指令码

typedef struct
{
	char name[20];
	char mac[20];
	char rssi[5];
	unsigned char RAWdata[2];

}BLE_INFO_TYPE;//蓝牙信息结构

typedef struct
{
	char *sleep;
	char *wakeup;
}
BLE_SLEEP_TIME;

typedef struct
{
	char *sslBrokerUrl;
	char *Topic;
	char *UserName;
	char *password;
}
BLE_MQTT_SET;	//mqtt通信设置的数据




typedef struct
{
	E_C2S_CMD cmd;
	char *StringType;
	BLE_SLEEP_TIME BleSleepType;
	BLE_MQTT_SET   BleMqttSetType;
}SJON_CMD_TYPE;	//解析后的json指令







int CreateBleInfoList(void);

int AddBleInfoToList(char *name , char *mac , char *rssi, char *RAWdata );

int DeleteBleInfoToList(const char * mac );

char *BleInfoToJson(void);

int SetBleInfoList(char *pSN , char *pTimestamp , char *pCycle, char *pNum);

char *BleHeartbeatToJson(char * mac,char *timestamp,char *bat_voltage,char *bat_current,char *pwr_voltage,char *pwr_current);

char *AckForJsonCmd(char *type, char *sn);

void FreeJsonString(char *pJsonStr);

SJON_CMD_TYPE*JsonCmdAnalysis(char *Payload);

void FreeAnalysisMem(SJON_CMD_TYPE *pMem);

void testJsonAnalysis(void);
int DeleteBleInfoP(void);








#endif


