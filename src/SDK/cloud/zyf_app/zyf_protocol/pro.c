#include "ql_system.h"
#include "ql_gpio.h"
#include "ql_stdlib.h"
#include "ql_uart.h"
#include "sys.h"
#include "pro.h"
#include "uart.h"
#include "gps.h"
#include "tiem.h"
#include "new_socket.h"
#include "lcd.h"
#include "stdlib.h"
#include "PRD.h"

u8 Tetsbuf[50];



static unsigned char auchCRCHi[] = {
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81,
0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40
};
static unsigned char auchCRCLo[] = {
0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05, 0xC5, 0xC4,
0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD,
0x1D, 0x1C, 0xDC, 0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32, 0x36, 0xF6, 0xF7,
0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE,
0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1, 0x63, 0xA3, 0xA2,
0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB,
0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0, 0x50, 0x90, 0x91,
0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88,
0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80,
0x40
};


u16 SDI_CRC16(unsigned char *puchMsg, unsigned short usDataLen)
{
	unsigned char uchCRCHi = 0xFF ; /* 初始化高字节*/
	unsigned char uchCRCLo = 0xFF ; /* 初始化低字节*/
	unsigned int uIndex ; /*把CRC表*/
	while(usDataLen--)
	{
		uIndex = uchCRCHi ^ *puchMsg++ ; /*计算CRC */
		uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex] ;
		uchCRCLo = auchCRCLo[uIndex] ;
	}
	return (uchCRCLo << 8 | uchCRCHi) ;
}




u8 Dec2Hex(u8 num)
{
	u8 numbuf[2]={0};
	u8 hex;
	Ql_sprintf((char*)numbuf,"%02d",num);
	hex=strtol((const char*)numbuf,NULL,16);
	return hex;
}

u8 Hex2Dec(u8 num)
{
	u8 numbuf[2]={0};
	u8 dec;
	Ql_sprintf((char*)numbuf,"%02x",num);
	dec=strtol((const char*)numbuf,NULL,10);
	return dec;
}


///////////////////////////////////Protcol///////////////////////////////////


/*
*
*
*	Global variable
*
*
*/


_pro_seral ProSeNum;
u8 PROBUF[PORALEN];
u16 ProNum=0;
u8 CurrtLen=0;



/*
*
*	Agreement head
*
*/


void Head_Init(u16 func)
{
	u8 i;
	ProNum++;
	PROBUF[0]=0x7E;
	PROBUF[1]=0x7E;
	PROBUF[2]=(PROVERSION&0xff000000)>>24;
	PROBUF[3]=(PROVERSION&0xff0000)>>16;
	PROBUF[4]=(PROVERSION&0xff00)>>8;
	PROBUF[5]=PROVERSION&0xff;
	PROBUF[6]=(func&0xff00)>>8;
	PROBUF[7]=func&0xff;
	PROBUF[8]=(ProNum&0xff00)>>8;
	PROBUF[9]=ProNum&0xff;
	for(i=0;i<6;i++)PROBUF[10+i]=mycoredata.time[i];
	PROBUF[16]=0x00;
	PROBUF[17]=0x00;
	CurrtLen=18;
}


void Length_Init(u8 prolen)
{
	PROBUF[16]=(prolen&0xff00)>>8;
	PROBUF[17]=prolen&0xff;
}

void Tlv_Init(u8 hl,tlv *tlvtable,u8 *prolen)
{
	u8 i;
	PROBUF[hl]=(tlvtable->type&0xff00)>>8;
	PROBUF[hl+1]=tlvtable->type&0xff;
	PROBUF[hl+2]=(tlvtable->length&0xff00)>>8;
	PROBUF[hl+3]=tlvtable->length&0xff;
	for(i=0;i<tlvtable->length;i++)PROBUF[hl+4+i]=tlvtable->value[i];
	*prolen=hl+4+i;
}


void ProIdInit(void)
{
	u8 i;
	tlv Id;
	Id.type=0x0001;
	Id.length=0x0006;
	for(i=0;i<Id.length;i++)Id.value[i]=Send_packets.packets_Send_snone[i];
	Tlv_Init(CurrtLen,&Id,&CurrtLen);
}


void ProTimeInit(void)
{
	GetSysTime(&time);
	Pro.time[0]=Dec2Hex(time.year-2000);
	Pro.time[1]=Dec2Hex(time.month);
	Pro.time[2]=Dec2Hex(time.day);
	Pro.time[3]=Dec2Hex(time.hour);
	Pro.time[4]=Dec2Hex(time.minute);
	Pro.time[5]=Dec2Hex(time.second);
}

void ProSigStInit(void)
{
	tlv ProSt;
	ProSt.type=0x0005;
	ProSt.length=0x0001;
	ProSt.value[0]=0x02;
	Tlv_Init(CurrtLen,&ProSt,&CurrtLen);
}

void ProSigEndInit(void)
{
	tlv ProSt;
	ProSt.type=0x0006;
	ProSt.length=0x0001;
	ProSt.value[0]=0x03;
	Tlv_Init(CurrtLen,&ProSt,&CurrtLen);
}


void ProCrcInit(void)
{
	u16 crcd;
	tlv ProV;
	ProV.type=0x0007;
	ProV.length=0x02;
	crcd=SDI_CRC16(PROBUF,CurrtLen);
	ProV.value[0]=crcd>>8;
	ProV.value[1]=crcd&0xff;
	Tlv_Init(CurrtLen,&ProV,&CurrtLen);
}

void ProPasswdInit(u8 len,u8 rellen)
{
	u8 mylen,i;
	mylen=strlen(systemset.Passwd);
	if(mylen>=rellen)mylen=rellen;
	for(i=0;i<mylen;i++)PROBUF[len+i]=PROBUF[len+i]^systemset.Passwd[i];
}


void PkInit(void)
{
	u32 temp=0;
	u8 tebuf[7]={0};
	u8 *pkbuf=PRODUCTKEY;
	mid(tebuf,pkbuf,6,0);
	temp=strtol((const char*)tebuf,NULL,16);
	ProSeNum.pk[0]=(temp&0xff0000)>>16;
	ProSeNum.pk[1]=(temp&0xff00)>>8;
	ProSeNum.pk[2]=temp&0xff;
	mid(tebuf,pkbuf,6,6);
	temp=strtol((const char*)tebuf,NULL,16);
	ProSeNum.pk[3]=(temp&0xff0000)>>16;
	ProSeNum.pk[4]=(temp&0xff00)>>8;
	ProSeNum.pk[5]=temp&0xff;
}


/*
*
*	Automatic generated
*
*/


void ProValuesInit(void)
{
	u8 i,j;
	tlv ProV;
	for(i=0;i<READONLYNUM;i++)
		{
			ProV.type= AutomaticRDTlv[i].type;
			ProV.length= AutomaticRDTlv[i].length;
			for(j=0;j<ProV.length;j++)ProV.value[i]=AutomaticRDTlv[i].value[j];
			Tlv_Init(CurrtLen,&ProV,&CurrtLen);
		}
}

void SnsInit(void)
{
	u8 i;
	for(i=0;i<6;i++)
		{
			ProSeNum.custSn[i]=Send_packets.packets_Send_snone[i];
			ProSeNum.deviceSn[i]=Send_packets.packets_Send_snone[i];
		}
}
void ProKeyInit(void)
{
	u8 i;
	tlv ProV;
	ProV.type=0xF401;
	ProV.length=0x0006;
	for(i=0;i<ProV.length;i++)ProV.value[i]=ProSeNum.pk[i];
	Tlv_Init(CurrtLen,&ProV,&CurrtLen);
	ProV.type=0xF402;
	ProV.length=0x0006;
	for(i=0;i<ProV.length;i++)ProV.value[i]=ProSeNum.custSn[i];
	Tlv_Init(CurrtLen,&ProV,&CurrtLen);
	ProV.type=0xF403;
	ProV.length=0x0006;
	for(i=0;i<ProV.length;i++)ProV.value[i]=ProSeNum.deviceSn[i];
	Tlv_Init(CurrtLen,&ProV,&CurrtLen);
	ProV.type=0xF410;
	ProV.length=0x000F;
	for(i=0;i<ProV.length;i++)ProV.value[i]=ProSeNum.imei[i];
	Tlv_Init(CurrtLen,&ProV,&CurrtLen);
	ProV.type=0xF412;
	ProV.length=0x0014;
	for(i=0;i<ProV.length;i++)ProV.value[i]=ProSeNum.iccid[i];
	Tlv_Init(CurrtLen,&ProV,&CurrtLen);
	ProV.type=0xF414;
	ProV.length=0x0004;
	ProV.value[0]=(SWVSION&0xff000000)>>24;
	ProV.value[1]=(SWVSION&0xff0000)>>16;
	ProV.value[2]=(SWVSION&0xff00)>>8;
	ProV.value[3]=SWVSION&0xff;
	Tlv_Init(CurrtLen,&ProV,&CurrtLen);
	
}


void FirstonthelineInit(u8 *len,u16 funcid)
{
	u8 elen;
	u8 plen;
	PkInit();
	SnsInit();
	ProTimeInit();
	Head_Init(funcid);
	ProIdInit();
	ProSigStInit();
	plen=CurrtLen;
	ProKeyInit();
	Tlv_Init(CurrtLen,&ReCenterTlV,&CurrtLen);
	elen=CurrtLen-plen;
	ProPasswdInit(plen,elen);
	ProSigEndInit();
	Length_Init(CurrtLen+6);
	ProCrcInit();
	*len=CurrtLen;
	
}


void CoreDataInit(u8 *len,u16 funcid)
{
	u8 elen;
	u8 plen;
	ProTimeInit();
	Head_Init(funcid);
	ProIdInit();
	ProSigStInit();
	plen=CurrtLen;
	ProValuesInit();
	elen=CurrtLen-plen;
	ProPasswdInit(plen,elen);
	ProSigEndInit();
	Length_Init(CurrtLen+6);
	ProCrcInit();
	*len=CurrtLen;
}




/////////////////////////////////////DownstreamData/////////////////////////////



u8 TlvIsBusy=0;

tlv CenterIDTlV;
tlv ReCenterTlV;


_pro_setings Pro;
_NumberInstruct  ProNumberInstruct;

u32	Center2ModuleInfo=0; 
u8    Center2ModuleLen=0;
u8 ProRtcSetBit=0;


void TlvInitList(void)
{
	u8 i;
	for(i=0;i<NUMINSTRCT;i++)ProNumberInstruct.sta[i]=0;
}

u8  JsVersion(u8 *buf)
{
	if(buf[2]==((PROVERSION&0xff000000)>>24)&&buf[3]==((PROVERSION&0xff0000)>>16)&&buf[4]==((PROVERSION&0xff00)>>8)&&buf[5]==(PROVERSION&0xff))return 0;
	else return 1;
}

u8 JsLength(u8 *buf,u8 len)
{
	if(buf[16]==((len&0xff00)>>8)&&buf[17]==(len&0xff))return 0;
	else return 1;
}

u8 JsCrcCode(u8 *buf,u8 len)
{
	u16 crcd;
	crcd=SDI_CRC16(buf,len-6);
	if(buf[len-2]==(crcd>>8)&&buf[len-1]==(crcd&0xff))return 0;
	else return 1;
}

u8 JsSysSerNum(u8 *buf)
{
	u8 len,i;
	if(buf[18]==0x00&&buf[19]==0x01)
		{
			len=(buf[20]<<8)|buf[21];
			for(i=0;i<len;i++)
				{
					if(Send_packets.packets_Send_snone[i]!=buf[22+i])return 1;
				}
			return 0;
		}
	return 1;
}


void Tlv_IdAnalys(u8 curlen,u8 *buf, u8 *Nextlen)
{
	u8 len;
	len=(buf[20]<<8)|buf[21];
	*Nextlen=curlen+4+len;
}

void Tlv_StartAnalys(u8 curlen,u8 *buf, u8 *Nextlen,u8 *tatal,u8 *currtnum)
{
	u8 len;
	if(buf[curlen]==0x00&&buf[curlen+1]==0x05)
		{
			len=(buf[curlen+2]<<8)|buf[curlen+3];
			if(len==0x03)
				{
					*tatal=buf[curlen+5];
					*currtnum=buf[curlen+6];
					*Nextlen=curlen+7;
				}
			if(len==0x01)
				{
					*tatal=0xff;
					*currtnum=0xff;
					*Nextlen=curlen+5;
				}
		}
}

void Tlv_PawssAnalys(u8 curlen,u8 *buf,u8 len)
{
	u8 clen;
	u8 mylen,i;
	clen=len-curlen-6-5;
	mylen=strlen(systemset.Passwd);
	if(mylen>=clen)mylen=clen;
	if(SystemDebug==6)mprintf("\r\n");
	if(SystemDebug==6)mprintf("Tlv_PawssAnalys:\r\n");
	for(i=0;i<mylen;i++)
		{
			buf[curlen+i]=buf[curlen+i]^systemset.Passwd[i];
			if(SystemDebug==5)mprintf("%02X",buf[curlen+i]);
		}
	if(SystemDebug==6)mprintf("\r\n");
}

u8 Tlv_BufAnalys(u8 len,u8 curlen,u8 *buf, u8 *Nextlen,tlv *tlvtable)
{
	u8 i;
	if(curlen>=len)return 1;
	else
		{
			tlvtable->type=buf[curlen]<<8|buf[curlen+1];
			tlvtable->length=buf[curlen+2]<<8|buf[curlen+3];
			if(tlvtable->length>VSLUELEN)return 2;
			for(i=0;i<tlvtable->length;i++)tlvtable->value[i]=buf[curlen+i+4];
			*Nextlen=curlen+4+i;
			return 0;
		}
}

void ProReadTimeValuesInit(void)
{
	u8 i;
	tlv ProV;
	ProV.type= 0x0004;
	ProV.length=0x0006;
	for(i=0;i<ProV.length;i++)ProV.value[i]=Pro.time[i];
	Tlv_Init(CurrtLen,&ProV,&CurrtLen);
}

void ProReadIDValuesInit(void)
{
	u8 i;
	tlv Id;
	Id.type=0x0001;
	Id.length=0x0006;
	for(i=0;i<Id.length;i++)Id.value[i]=Send_packets.packets_Send_snone[i];
	Tlv_Init(CurrtLen,&Id,&CurrtLen);
}

void ProReadSendTValuesInit(void)
{
	u8 i;
	tlv ProV;
	ProV.type=T_SENDT;
	ProV.length=0x0002;
	ProV.value[0]=systemset.HandInter>>8;
	ProV.value[1]=systemset.HandInter&0xff;
	Tlv_Init(CurrtLen,&ProV,&CurrtLen);
}
   

void AddTlvCmd2List(tlv *addkey)
{
	u8 i,j;
	for(i=0;i<NUMINSTRCT;i++)
		{
			if(ProNumberInstruct.sta[i]==0)break;
		}
	if(i<NUMINSTRCT)
		{
			Ql_memset(ProNumberInstruct.tlvs[i].value, 0, sizeof(ProNumberInstruct.tlvs[i].value));
			ProNumberInstruct.sta[i]=1;
			ProNumberInstruct.tlvs[i].type=addkey->type;
			ProNumberInstruct.tlvs[i].length=addkey->length;
			for(j=0;j<ProNumberInstruct.tlvs[i].length;j++)
				{
					ProNumberInstruct.tlvs[i].value[j]=addkey->value[j];
				}
		}
	mprintf("\r\n**************************************************************\r\n");
	mprintf("ProNumberInstruct.tlvs[%d].type=%x\r\n",i,ProNumberInstruct.tlvs[i].type);
	mprintf("ProNumberInstruct.tlvs[%d].length=%x\r\n",i,ProNumberInstruct.tlvs[i].length);
	mprintf("ProNumberInstruct.tlvs[%d].value=%s\r\n",i,ProNumberInstruct.tlvs[i].value);
	mprintf("\r\n**************************************************************\r\n");
	
	
}


void ProRWValuesInit(tlv *anser,u8 dowhat)
{
	u8 i;
	tlv ProV;
	ProV.type=anser->type;
	ProV.length=anser->length;
	if(dowhat==0)
		{
			for(i=0;i<ProV.length;i++)ProV.value[i]=0xFF;
		}
	Tlv_Init(CurrtLen,&ProV,&CurrtLen);
}

void Reply2Server(u8 *len,u16 funcid,tlv *anser,u8 dowhat)
{
	u8 plen;
	u8 elen;
	ProTimeInit();
	Head_Init(funcid);
	ProIdInit();
	ProSigStInit();
	plen=CurrtLen;
	ProRWValuesInit(anser,dowhat);
	Tlv_Init(CurrtLen,&CenterIDTlV,&CurrtLen);
	elen=CurrtLen-plen;
	ProPasswdInit(plen,elen);
	ProSigEndInit();
	Length_Init(CurrtLen+6);
	ProCrcInit();
	*len=CurrtLen;
}


void ChckAutoReCmd(void)
{
	u8 i;
	u8 res;
	u8 doWhat=0;
	for(i=0;i<NUMINSTRCT;i++)
		{
			if(ProNumberInstruct.sta[i]==1)
				{
					u8 lens;
					ProNumberInstruct.sta[i]=0;
					res=SendMsg2HalDoHd(i,&doWhat);
					Reply2Server(&lens,TYGNM,&ProNumberInstruct.tlvs[i],doWhat);
					M203CSendDataNoAck(PROBUF,lens,0);
					break;
				}
		}
}



void BufAnalys(u8 len,u8 *buf,u16 fcunid)
{
	u8 i;
	tlv ProA;
	TlvIsBusy=1;
	while(!Tlv_BufAnalys(len,Center2ModuleLen,buf,&Center2ModuleLen,&ProA))
		{
			Ql_Sleep(10);
			if(SystemDebug==2)
				{	
					mprintf("\r\n");
					mprintf("ProA.type=%04X\r\n",ProA.type);
					mprintf("ProA.length=%04X\r\n",ProA.length);
					mprintf("ProA.value:\r\n");
					for(i=0;i<ProA.length;i++)mprintf("%02X",ProA.value[i]);
					mprintf("\r\n");
				}
			if(ProA.type==T_CENTTER)
				{
					CenterIDTlV.type=ProA.type;
					CenterIDTlV.length=ProA.length;
					for(i=0;i<ProA.length;i++)CenterIDTlV.value[i]=ProA.value[i];
				}
			if(fcunid==WRYHSZ)
				{
					if(ProA.type==T_TIME)
						{
							StRTC_Set(Hex2Dec(ProA.value[0])+2000,Hex2Dec(ProA.value[1]),Hex2Dec(ProA.value[2]),Hex2Dec(ProA.value[3]),Hex2Dec(ProA.value[4]),Hex2Dec(ProA.value[5]));
							StRTC_Get();
							ProRtcSetBit=1;
							Center2ModuleInfo|=1<<8;                                   //校时
							ReCenterTlV.type=ProA.type;
							ReCenterTlV.length=ProA.length;
							for(i=0;i<ProA.length;i++)ReCenterTlV.value[i]=0xFF;
						}
				}
			if(fcunid==WRYXCS)
				{
					if(ProA.type==T_SENDT)
						{
							systemset.HandInter=ProA.value[0]<<8|ProA.value[1];
							SaveFlashParamsNew(&systemset);
							Center2ModuleInfo|=1<<16;			//设置发送时间间隔
							ReCenterTlV.type=ProA.type;
							ReCenterTlV.length=ProA.length;
							for(i=0;i<ProA.length;i++)ReCenterTlV.value[i]=0xFF;
						}
				}
			if(fcunid==RDHCMD)
				{
					Center2ModuleInfo|=1<<9;  					//检测是否在线
				}
			if(fcunid==OTACMD)
				{
					ReCenterTlV.type=T_FOTA;
					ReCenterTlV.length=0x05;
					if(ProA.type==T_FOTA_ADDR)
						{
							for(i=0;i<ProA.length;i++)fotainfo.FTP_SVR_ADDR[i]=ProA.value[i];
							ReCenterTlV.value[0]=0xAA;
						}
					if(ProA.type==T_FOTA_PATH)
						{
							for(i=0;i<ProA.length;i++)fotainfo.FTP_SVR_PATH[i]=ProA.value[i];
							ReCenterTlV.value[1]=0xAA;
						}
					if(ProA.type==T_FOTA_USERNAME)
						{
							for(i=0;i<ProA.length;i++)fotainfo.FTP_USER_NAME[i]=ProA.value[i];
							ReCenterTlV.value[2]=0xAA;
						}
					if(ProA.type==T_FOTA_PASSWORD)
						{
							for(i=0;i<ProA.length;i++)fotainfo.FTP_PASSWORD[i]=ProA.value[i];
							ReCenterTlV.value[3]=0xAA;
						}
					if(ProA.type==T_FOTA_FILENAME)
						{
							for(i=0;i<ProA.length;i++)fotainfo.FTP_FILENAME[i]=ProA.value[i];
							ReCenterTlV.value[4]=0xAA;
						}
					Center2ModuleInfo|=1<<10;						//远程升级
				}
			if(fcunid==AUTOCMD)
				{
					for(i=0;i<READWRITE;i++)
						{
							if(ProA.type==AutomaticWRTlv[i].type)
								{
									AddTlvCmd2List(&ProA);
									break;
								}
						}
					
				}
			
		}
	TlvIsBusy=0;
}


void DataCore(u8 *databuf,u8 len)
{
	u8 res,cur;
	u16 funcid;
	if(databuf[0]==0x7E&&databuf[1]==0x7E)
		{
			res=JsLength(databuf,len);
			if(res)
				{
					Center2ModuleInfo|=1<<0;	       //数据长度错误
					mprintf("数据长度错误\r\n");
					return;
				}
			res=JsCrcCode(databuf,len);
			if(res)
				{
					Center2ModuleInfo|=1<<1;		//crc校验失败
					mprintf("crc校验失败\r\n");
					return;
				}
			res=JsVersion(databuf);
			if(res)
				{
					Center2ModuleInfo|=1<<2;		//版本号不一致
					mprintf("版本号不一致\r\n");
					return;
				}
#if 0
			res=JsSysSerNum(databuf);
			if(res)
				{
					Center2ModuleInfo|=1<<3;		//主ID错误
					mprintf("主ID错误\r\n");
					return;
				}
#endif

			Center2ModuleLen=18;
			Tlv_IdAnalys(Center2ModuleLen,databuf,&Center2ModuleLen);
			Tlv_StartAnalys(Center2ModuleLen,databuf,&Center2ModuleLen,&res,&cur);
			Tlv_PawssAnalys(Center2ModuleLen,databuf,len);
			funcid=databuf[6]<<8|databuf[7];
			if(SystemDebug==6)mprintf("funcid=%04X\r\n",funcid);
			BufAnalys(len,databuf,funcid);
		}
}

void ReSetDataInitNew(u8 *len,u16 funcid,tlv *tlvp)
{
	u8 plen;
	u8 elen;
	ProTimeInit();
	Head_Init(funcid);
	ProIdInit();
	ProSigStInit();
	plen=CurrtLen;
	Tlv_Init(CurrtLen,tlvp,&CurrtLen);
	Tlv_Init(CurrtLen,&CenterIDTlV,&CurrtLen);
	elen=CurrtLen-plen;
	ProPasswdInit(plen,elen);
	ProSigEndInit();
	Length_Init(CurrtLen+6);
	ProCrcInit();
	*len=CurrtLen;
}

u8 IsFtpUrlOK(u16 timeout)
{
	u8 i;
	while(--timeout)
		{
			for(i=0;i<5;i++)
				{
					if(ReCenterTlV.value[i]!=0xAA)
						{
							break;
						}
				}
			if(i==5)
				{
					break;
				}
			else
				{
					Ql_Sleep(10);
				}
		}
	if(timeout==0)return 1;
	return 0;
}

void Recive_OutData(void)
{
	u8 t;
	u8 res;
	if(!TlvIsBusy)
		{
			if(Center2ModuleInfo&(1<<8))			//回复校时
				{
					Center2ModuleInfo&=~(1<<8);
					ReSetDataInitNew(&res,TYGNM,&ReCenterTlV);
					M203CSendDataNoAck(PROBUF,res,0);
					
				}  
			if(Center2ModuleInfo&(1<<9))			//回复是否在线
				{
					Center2ModuleInfo&=~(1<<9);
					ReCenterTlV.type=0x4E23;
					ReSetDataInitNew(&res,TYGNM,&ReCenterTlV);
					M203CSendDataNoAck(PROBUF,res,0);
					
				}  
			if(Center2ModuleInfo&(1<<16))			//回复设置间隔
				{
					Center2ModuleInfo&=~(1<<16);
					ReSetDataInitNew(&res,TYGNM,&ReCenterTlV);
					M203CSendDataNoAck(PROBUF,res,0);
				}
			if(Center2ModuleInfo&(1<<10))
				{
					t=IsFtpUrlOK(300);
					Center2ModuleInfo&=~(1<<10);
					ReSetDataInitNew(&res,TYGNM,&ReCenterTlV);
					M203CSendDataNoAck(PROBUF,res,0);
					if(t==0)
						{
							systemset.fotaflag=1;
							Ql_sprintf(systemset.fotaaddr,"ftp://%s%s%s:%s@%s:%s",fotainfo.FTP_SVR_ADDR, fotainfo.FTP_SVR_PATH, fotainfo.FTP_FILENAME, FTP_SVR_PORT, fotainfo.FTP_USER_NAME, fotainfo.FTP_PASSWORD);
							SaveFlashParamsNew(&systemset);
							Ql_Sleep(3000);
			 				Ql_Reset(0);
						}
				}
			ChckAutoReCmd();
		}
}


/////////////////////////////////////////////////////////////////////////////////

/*
*
*处理字符串协议		
*
*
*/

void ProValuesInit_v2(void)
{
	u8 i,j;
	tlv ProV;
	for(i=0;i<READONLYNUM;i++)
		{
			ProV.type= AutomaticRDTlv[i].type;
			ProV.length= AutomaticRDTlv[i].length;
			for(j=0;j<ProV.length;j++)ProV.value[j]=0x00;
			if(strlen(AutomaticRDTlv[i].value)<=ProV.length)
				{
					for(j=0;j<(strlen(AutomaticRDTlv[i].value));j++)ProV.value[j]=AutomaticRDTlv[i].value[j];
				}
			Tlv_Init(CurrtLen,&ProV,&CurrtLen);
		}
}



void CoreDataInit_v2(u8 *len,u16 funcid)
{
	u8 elen;
	u8 plen;
	ProTimeInit();
	Head_Init(funcid);
	ProIdInit();
	ProSigStInit();
	plen=CurrtLen;
	ProValuesInit_v2();
	elen=CurrtLen-plen;
	ProPasswdInit(plen,elen);
	ProSigEndInit();
	Length_Init(CurrtLen+6);
	ProCrcInit();
	*len=CurrtLen;
}



void LTlv_Init(u8 hl,Ltlv *tlvtable,u8 *prolen)
{
	u8 i;
	PROBUF[hl]=(tlvtable->type&0xff00)>>8;
	PROBUF[hl+1]=tlvtable->type&0xff;
	PROBUF[hl+2]=(tlvtable->length&0xff00)>>8;
	PROBUF[hl+3]=tlvtable->length&0xff;
	for(i=0;i<tlvtable->length;i++)PROBUF[hl+4+i]=tlvtable->value[i];
	*prolen=hl+4+i;
}




void ProValuesInit_v3(void)
{
	u8 j;
	Ltlv ProV;
	ProV.type= 0x3E88;
	ProV.length=50;
	for(j=0;j<ProV.length;j++)ProV.value[j]=0x00;
	if(Ql_strlen(Tetsbuf)<=ProV.length)
			{
				for(j=0;j<Ql_strlen(Tetsbuf);j++)ProV.value[j]=Tetsbuf[j];
			}
	LTlv_Init(CurrtLen,&ProV,&CurrtLen);
}

void CoreDataInit_v3(u8 *len,u16 funcid)
{
	u8 elen;
	u8 plen;
	ProTimeInit();
	Head_Init(funcid);
	ProIdInit();
	ProSigStInit();
	plen=CurrtLen;
	ProValuesInit_v3();
	elen=CurrtLen-plen;
	ProPasswdInit(plen,elen);
	ProSigEndInit();
	Length_Init(CurrtLen+6);
	ProCrcInit();
	*len=CurrtLen;
}








