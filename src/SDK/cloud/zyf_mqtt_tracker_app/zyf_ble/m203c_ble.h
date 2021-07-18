
#ifndef __M203C_BLE_H__
#define __M203C_BLE_H__

#include <inttypes.h>
#include <stdint.h>
#include "ql_type.h"
#include <stdlib.h>

#define  BLE_GAP_128_UUIDS_SIZE       16

#define  BLE_GAP_ADV_INFO_MAX_SIZE       5

#define  BLE_GAP_ADV_MAX_SIZE       31

/** @brief BLE address length. */
#define BLE_GAP_ADDR_LEN            13


/** @defgroup BLE_GAP_AD_TYPE_DEFINITIONS GAP Advertising and Scan Response Data format
 *  @note Found at https://www.bluetooth.org/Technical/AssignedNumbers/generic_access_profile.htm
 * @{ */
#define BLE_GAP_AD_TYPE_FLAGS                               0x01 /**< Flags for discoverability. */
#define BLE_GAP_AD_TYPE_16BIT_SERVICE_UUID_MORE_AVAILABLE   0x02 /**< Partial list of 16 bit service UUIDs. */
#define BLE_GAP_AD_TYPE_16BIT_SERVICE_UUID_COMPLETE         0x03 /**< Complete list of 16 bit service UUIDs. */
#define BLE_GAP_AD_TYPE_32BIT_SERVICE_UUID_MORE_AVAILABLE   0x04 /**< Partial list of 32 bit service UUIDs. */
#define BLE_GAP_AD_TYPE_32BIT_SERVICE_UUID_COMPLETE         0x05 /**< Complete list of 32 bit service UUIDs. */
#define BLE_GAP_AD_TYPE_128BIT_SERVICE_UUID_MORE_AVAILABLE  0x06 /**< Partial list of 128 bit service UUIDs. */
#define BLE_GAP_AD_TYPE_128BIT_SERVICE_UUID_COMPLETE        0x07 /**< Complete list of 128 bit service UUIDs. */
#define BLE_GAP_AD_TYPE_SHORT_LOCAL_NAME                    0x08 /**< Short local device name. */
#define BLE_GAP_AD_TYPE_COMPLETE_LOCAL_NAME                 0x09 /**< Complete local device name. */
#define BLE_GAP_AD_TYPE_TX_POWER_LEVEL                      0x0A /**< Transmit power level. */
#define BLE_GAP_AD_TYPE_CLASS_OF_DEVICE                     0x0D /**< Class of device. */
#define BLE_GAP_AD_TYPE_SIMPLE_PAIRING_HASH_C               0x0E /**< Simple Pairing Hash C. */
#define BLE_GAP_AD_TYPE_SIMPLE_PAIRING_RANDOMIZER_R         0x0F /**< Simple Pairing Randomizer R. */
#define BLE_GAP_AD_TYPE_SECURITY_MANAGER_TK_VALUE           0x10 /**< Security Manager TK Value. */
#define BLE_GAP_AD_TYPE_SECURITY_MANAGER_OOB_FLAGS          0x11 /**< Security Manager Out Of Band Flags. */
#define BLE_GAP_AD_TYPE_SLAVE_CONNECTION_INTERVAL_RANGE     0x12 /**< Slave Connection Interval Range. */
#define BLE_GAP_AD_TYPE_SOLICITED_SERVICE_UUIDS_16BIT       0x14 /**< List of 16-bit Service Solicitation UUIDs. */
#define BLE_GAP_AD_TYPE_SOLICITED_SERVICE_UUIDS_128BIT      0x15 /**< List of 128-bit Service Solicitation UUIDs. */
#define BLE_GAP_AD_TYPE_SERVICE_DATA                        0x16 /**< Service Data. */
#define BLE_GAP_AD_TYPE_PUBLIC_TARGET_ADDRESS               0x17 /**< Public Target Address. */
#define BLE_GAP_AD_TYPE_RANDOM_TARGET_ADDRESS               0x18 /**< Random Target Address. */
#define BLE_GAP_AD_TYPE_APPEARANCE                          0x19 /**< Appearance. */
#define BLE_GAP_AD_TYPE_MANUFACTURER_SPECIFIC_DATA          0xFF /**< Manufacturer Specific Data. */
/** @} */


typedef enum
{
    BLESERVER_DEVICE_INIT0 = 0,
	BLESERVER_DEVICE_PREINIT0,
    BLESERVER_DEVICE_INIT,
    BLESERVER_DEVICE_GATST,
    BLESERVER_DEVICE_INITADV,
    BLESERVER_DEVICE_SETADV,
    BLESERVER_DEVICE_LOOP, 
    BLESERVER_DEVICE_GATT_WREG_RESP,
    BLESERVER_DEVICE_GATT_RREG_RESP,
} BLE_SERVER_STATE_t;


typedef struct
{
  u8 addr_type;                    
  u8 addr[BLE_GAP_ADDR_LEN];    
} ble_gap_addr_t;

typedef struct
{
  u8   type;                
  u8   dlen;
  u8   data[BLE_GAP_ADV_MAX_SIZE];
} ble_gap_adv_info;


typedef struct 
{
	ble_gap_addr_t   peer_addr;
	s8 rssi;
	u8 ble_gap_adv_info_num;
	ble_gap_adv_info peer_adv_info[BLE_GAP_ADV_INFO_MAX_SIZE];
}ble_gap_evt_adv_report_t;


typedef struct
{
 	u16 devicesinfo;
	u8 locksta;
	u32 qrcode;
} ManufacturerData;


typedef struct
{
 	ManufacturerData manufacture_data;
	u8 local_device_name[BLE_GAP_ADV_MAX_SIZE];
	u8 Complete_UUIDs[BLE_GAP_128_UUIDS_SIZE];
	u16 appearance;
	u8 *services_data;
} ble_gap_set_adv;

extern ble_gap_set_adv   ble_adv_data;

extern BLE_SERVER_STATE_t ble_server_sta;


extern ble_gap_evt_adv_report_t   adv_report;

void Ble_Client_App(void);
void OnURCHandler_BLEScan(const char* strURC, void* reserved);
void nus_data_handler(uint8_t* p_data, uint16_t length);

#endif /* __M203C_MODULE_H__ */
