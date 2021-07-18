#ifndef _PRO_H_
#define _PRO_H_


#include "sys.h"

#define VSLUELEN	20
#define LVSLUELEN	50

#define PORALEN	255

#define NUMINSTRCT	10

#define PROVERSION	0x23235554

typedef struct 
{
	u16 type;
	u16 length;
	u8 value[VSLUELEN];
}tlv;

typedef struct 
{
	u16 type;
	u16 length;
	u8 value[LVSLUELEN];
}Ltlv;


typedef struct 
{
	u8 sta[NUMINSTRCT];
	tlv tlvs[NUMINSTRCT];
}_NumberInstruct;


typedef struct 
{
	u8 sn[6];
	u8 subsn[6];
	u8 time[6];
	u8 paswd[2];
	u16 tem;
	u16 hum;
	u16 ChargIn;
	u16 ChargOut;
	u16 hvol;
	u16 bvol;
	u8 hsta;
	u8 gpssta;
	u8 csta;
	u8 bleve;
}_pro_setings;


typedef struct 
{
	u8 sn[6];
	u8 pk[6];
	u8 custSn[6];
	u8 deviceSn[6];
	u8 imei[15];
	u8 iccid[20];
}_pro_seral;


extern _NumberInstruct  ProNumberInstruct;

extern _pro_setings Pro;

extern _pro_seral ProSeNum;

extern u8 ProRtcSetBit;

extern u8 Tetsbuf[50];

/////////////////////////////////////////////////////////////////////////////

#define SWVSION 0x18041601

#define PROVERSION	0x23235554



#define T_TIME		0x0004
#define T_CENTTER	0x0008
#define T_MAINID	0xA264
#define T_MAINIP	0xA265
#define T_MAINPOT	0xA266
#define T_SENDT		0xA268

#define RDHCMD	0x4E23
#define RDJBPZ	0x4E0A
#define WRJBPZ	0x4E09
#define RDYXCS	0x4E0C
#define WRYXCS	0x4E0B
#define WRYHSZ	0x4E11
#define RDYHSZ	0x4E13
#define CKSSSJ	0x4E06
#define TYGNM		0x4E15
#define OTACMD	0x4E14
#define AUTOCMD	0x4E19

#define FTP_SVR_PORT    "21"

#define B_FOTA	0xF101
#define T_FOTA	0xF001
#define T_FOTA_ADDR	0xF010
#define T_FOTA_PATH	0xF011
#define T_FOTA_USERNAME	0xF012
#define T_FOTA_PASSWORD	0xF013
#define T_FOTA_FILENAME	0xF014


extern u8 PROBUF[PORALEN];


extern tlv CenterIDTlV;
extern tlv ReCenterTlV;

void TlvInitList(void);


#endif

