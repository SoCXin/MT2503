#include "203c_call.h"
#include "ril_sms.h"
#include "ql_type.h"
#include "uart.h"
#include "ril_ble.h"
#include "sys.h"
/*
typedef struct 
{
	u32 HandInter;			//设置握手时间间隔	//已经没用了，小程序已经没有了
	u32 Interval;				//设置自动上传时间间隔
	u8 SN[16];				//SN
	u8 snuser;				//SN是否已经设置
	u8 saveflag;
	u8 fotaflag;				//升级标志
	u8 fotaaddr[200];			//固件地址和信息
	u32 SysTime;
	u8 CallMode;				//预留
	u8 updateinfo;			//升级标志
}_system_setings;
*/

void blecmd_handle_event(ST_BLE_Server cmd_buf)
{
	s32 ret;
	char cmd_value[64] = {0};
	Ql_strcpy(cmd_value, cmd_buf.wrreq_param.value);
	//ret = Ql_strcmp(, const char * s2)
	//if()
}

void Callback_myATResponse(char* line, u32 len, void* userData)
{}

/*****************************************************************************
 * FUNCTION
 *  Telephony_CallOut
 *
 * DESCRIPTION
 *  CallOut telephone.
 *
 * PARAMETERS
 *  VOID
 *
 * RETURNS
 *  VOID
 *
 *****************************************************************************/
	
/*
void Telephony_CallOut(void)
{
    s32 ret;
    s32 callState;
	
    char callOutPhoneNum[20] = "";
	Ql_memset(callOutPhoneNum, 0, Ql_strlen(callOutPhoneNum));
	Ql_strcpy(callOutPhoneNum, systemset.callphone);
	
	s32 cmdbuf[16] = {0};
	Ql_sprintf(cmdbuf, "AT+VTS=%c", systemset.callbutton);
	mprintf("cmdbuf = %s\r\n", cmdbuf);
	

    ret = RIL_Telephony_Dial(0, callOutPhoneNum, &callState);
    if (RIL_AT_SUCCESS == ret && CALL_STATE_OK == callState)
    {
    	mprintf("<-- Dial \"%s\" successfully -->\r\n", callOutPhoneNum);
		Ql_Sleep(10000);
    	ret = Ql_RIL_SendATCmd("AT+VTS=#", Ql_strlen("AT+VTS=#"), Callback_myATResponse, NULL, NULL);
		if(ret != RIL_AT_SUCCESS)
			{
				mprintf("AT+VTS= send failed!\r\n");
				return;
			}
		mprintf("%s send successful\r\n", cmdbuf);
		Ql_Sleep(systemset.calltime*1000);
		ret = Ql_RIL_SendATCmd("ATH0", Ql_strlen("ATH0"), Callback_myATResponse, NULL, NULL);
		if(ret != RIL_AT_SUCCESS)
			{
				mprintf("ATH0 send failed!\r\n");
				return;
			}
		mprintf("ATH0 send successful	,	dial disconnected!\r\n");
		flag_call_ready = 1;
    } else {
        mprintf("<-- Fail to dial \"%s\", cause: %d -->\r\n", ret);
    }
}
*/


/*****************************************************************************
 * FUNCTION
 *  SMS_Initialize
 *
 * DESCRIPTION
 *  Initialize SMS environment.
 *  
 * PARAMETERS
 *  VOID
 *
 * RETURNS
 *  TRUE:  This function works SUCCESS.
 *  FALSE: This function works FAIL!
 *****************************************************************************/
bool SMS_Initialize(void)
{
    s32 iResult = 0;
    u8  nCurrStorage = 0;
    u32 nUsed = 0;
    u32 nTotal = 0;
    
    // Set SMS storage:
    // By default, short message is stored into SIM card. You can change the storage to ME if needed, or
    // you can do it again to make sure the short message storage is SIM card.
    #if 0
    {
        iResult = RIL_SMS_SetStorage(RIL_SMS_STORAGE_TYPE_SM,&nUsed,&nTotal);
        if (RIL_ATRSP_SUCCESS != iResult)
        {
            mprintf("Fail to set SMS storage, cause:%d\r\n", iResult);
            return FALSE;
        }
        mprintf("<-- Set SMS storage to SM, nUsed:%u,nTotal:%u -->\r\n", nUsed, nTotal);

        iResult = RIL_SMS_GetStorage(&nCurrStorage, &nUsed ,&nTotal);
        if(RIL_ATRSP_SUCCESS != iResult)
        {
            mprintf("Fail to get SMS storage, cause:%d\r\n", iResult);
            return FALSE;
        }
        mprintf("<-- Check SMS storage: curMem=%d, used=%d, total=%d -->\r\n", nCurrStorage, nUsed, nTotal);
    }
    #endif

    // Enable new short message indication
    // By default, the auto-indication for new short message is enalbed. You can do it again to 
    // make sure that the option is open.
    #if 0
    {
        iResult = Ql_RIL_SendATCmd("AT+CNMI=2,1",Ql_strlen("AT+CNMI=2,1"),NULL,NULL,0);
        if (RIL_AT_SUCCESS != iResult)
        {
            mprintf("Fail to send \"AT+CNMI=2,1\", cause:%d\r\n", iResult);
            return FALSE;
        }
        mprintf("<-- Enable new SMS indication -->\r\n");
    }
    #endif

    // Delete all existed short messages (if needed)
    iResult = RIL_SMS_DeleteSMS(0, RIL_SMS_DEL_ALL_MSG);
    if (iResult != RIL_AT_SUCCESS)
    {
        mprintf("Fail to delete all messages, iResult=%d,cause:%d\r\n", iResult, Ql_RIL_AT_GetErrCode());
        return FALSE;
    }
    //mprintf("Delete all existed messages\r\n");
    
    return TRUE;
}

void SMS_TextMode_Send(void)
{
    s32 iResult;
    u32 nMsgRef;
	
    char strPhNum[20] = {0};
	Ql_memset(strPhNum, 0, Ql_strlen(strPhNum));
	Ql_strcpy(strPhNum, "10086");
	
    char strTextMsg[20]={0};
	Ql_strcpy(strTextMsg,"hello 10086!!!");
	
    char strConMsgSeg1[] = "Enginelf Module CON-SMS Test Start:<< GSM/GPRS modules are based on open standards and meet the requirements of applicable international standards and leg\0";
    char strConMsgSeg2[] = "islation. Our leading edge products and services that cherish the environment and support sustainable development are the result of our own know-how and \0";
    char strConMsgSeg3[] = "a perfect fit to the demands on the market. With compact size, low power consumption and extended temperature, our GSM/GPRS modules are used in a wide ra\0";
    char strConMsgSeg4[] = "nge of applications, such as automobile, VTS, Smart Metering, Wireless POS, security and a multitude of embedded devices in many industries. >> Test End.\0";
    char strConMsgChSeg1[] = "79FB8FDC6A215757957F77ED4FE16D4B8BD55F0059CBFF1A300A79FB8FDC901A4FE162E567099886514876844EA754C1548C670D52A1FF0C00470053004D002F004700500052005300206A21575757FA4E8E5F00653E548C7B26540856FD9645680751C66CD589C4768489816C42751F4EA78BBE8BA1FF0C517767097D2751D176845C3A5BF8\0";
    char strConMsgChSeg2[] = "30014F4E529F8017300162695C556E295EA67B494F1852BF0020300279FB8FDC901A4FE100470053004D002F00470050005200536A2157575E7F6CDB5E9475284E8E8F668F7D3001667A80FD8BA191CF30018FDC7A0B76D163A7300172694F538DDF8E2A300165E07EBF4ED86B3E30015B89516876D163A7300179FB52A88BA17B977B494F17\0";
    char strConMsgChSeg3[] = "591A884C4E1A76845D4C51655F0F8BBE59074E2D3002300B6D4B8BD57ED3675F\0";

    ST_RIL_SMS_SendExt sExt;

    //Initialize
    Ql_memset(&sExt,0x00,sizeof(sExt));

    mprintf("< Send Normal Text SMS begin... >\r\n");
   // RESEND:
    iResult = RIL_SMS_SendSMS_Text(strPhNum, Ql_strlen(strPhNum), LIB_SMS_CHARSET_GSM, strTextMsg, Ql_strlen(strTextMsg), &nMsgRef);
    if (iResult != RIL_AT_SUCCESS)
    {   
        mprintf("< Fail to send Text SMS, iResult=%d, cause:%d >\r\n", iResult, Ql_RIL_AT_GetErrCode());
		//goto RESEND;
        return;
    }
    mprintf("< Send Text SMS successfully, MsgRef:%u >\r\n", nMsgRef);
	mprintf("send message strPhNum : %s, send message Msg : %s\r\n", strPhNum, strTextMsg);
#if 1
    mprintf("< Send English Concatenate Text SMS begin... >\r\n");

    //The 1st segment of CON-SMS
    sExt.conPres = TRUE;
    sExt.con.msgType = 0xFF; //If it's 0xFF: use default CON-SMS-TYPE
    sExt.con.msgRef = 52; //Different CON-SMS shall have different <msgRef>
    sExt.con.msgTot = 4;
    sExt.con.msgSeg = 1;
    
    iResult = RIL_SMS_SendSMS_Text_Ext(strPhNum,Ql_strlen(strPhNum),LIB_SMS_CHARSET_GSM,strConMsgSeg1,Ql_strlen(strConMsgSeg1),&nMsgRef,&sExt);
    if (iResult != RIL_AT_SUCCESS)
    {   
        mprintf( 
            "< Fail to send Text SMS, cause:%d,cp:1,cy:%d,cr:%d,ct:%d,cs:%d >\r\n", 
            iResult,
            sExt.con.msgType,
            sExt.con.msgRef,
            sExt.con.msgTot,
            sExt.con.msgSeg
        );
        
        return;
    }
    mprintf(
        "< Send Text SMS successfully, MsgRef:%u,cp:1,cy:%d,cr:%d,ct:%d,cs:%d >\r\n", 
        nMsgRef,
        sExt.con.msgType,
        sExt.con.msgRef,
        sExt.con.msgTot,
        sExt.con.msgSeg
    ); 

    //The 2st segment of CON-SMS
    sExt.con.msgSeg = 2;
    iResult = RIL_SMS_SendSMS_Text_Ext(strPhNum,Ql_strlen(strPhNum),LIB_SMS_CHARSET_GSM,strConMsgSeg2,Ql_strlen(strConMsgSeg2),&nMsgRef,&sExt);
    if (iResult != RIL_AT_SUCCESS)
    {   
        mprintf(
            "< Fail to send Text SMS, cause:%d,cp:1,cy:%d,cr:%d,ct:%d,cs:%d >\r\n", 
            iResult,
            sExt.con.msgType,
            sExt.con.msgRef,
            sExt.con.msgTot,
            sExt.con.msgSeg
        );
        
        return;
    }
    mprintf(
        "< Send Text SMS successfully, MsgRef:%u,cp:1,cy:%d,cr:%d,ct:%d,cs:%d >\r\n", 
        nMsgRef,
        sExt.con.msgType,
        sExt.con.msgRef,
        sExt.con.msgTot,
        sExt.con.msgSeg
    ); 

    //The 3st segment of CON-SMS
    sExt.con.msgSeg = 3;
    iResult = RIL_SMS_SendSMS_Text_Ext(strPhNum,Ql_strlen(strPhNum),LIB_SMS_CHARSET_GSM,strConMsgSeg3,Ql_strlen(strConMsgSeg3),&nMsgRef,&sExt);
    if (iResult != RIL_AT_SUCCESS)
    {   
        mprintf(
            "< Fail to send Text SMS, cause:%d,cp:1,cy:%d,cr:%d,ct:%d,cs:%d >\r\n", 
            iResult,
            sExt.con.msgType,
            sExt.con.msgRef,
            sExt.con.msgTot,
            sExt.con.msgSeg
        );
        
        return;
    }
    mprintf(
        "< Send Text SMS successfully, MsgRef:%u,cp:1,cy:%d,cr:%d,ct:%d,cs:%d >\r\n", 
        nMsgRef,
        sExt.con.msgType,
        sExt.con.msgRef,
        sExt.con.msgTot,
        sExt.con.msgSeg
    ); 

    //The 4st segment of CON-SMS
    sExt.con.msgSeg = 4;
    iResult = RIL_SMS_SendSMS_Text_Ext(strPhNum,Ql_strlen(strPhNum),LIB_SMS_CHARSET_GSM,strConMsgSeg4,Ql_strlen(strConMsgSeg4),&nMsgRef,&sExt);
    if (iResult != RIL_AT_SUCCESS)
    {   
        mprintf( 
            "< Fail to send Text SMS, cause:%d,cp:1,cy:%d,cr:%d,ct:%d,cs:%d >\r\n", 
            iResult,
            sExt.con.msgType,
            sExt.con.msgRef,
            sExt.con.msgTot,
            sExt.con.msgSeg
        );
        
        return;
    }
    mprintf(
        "< Send Text SMS successfully, MsgRef:%u,cp:1,cy:%d,cr:%d,ct:%d,cs:%d >\r\n", 
        nMsgRef,
        sExt.con.msgType,
        sExt.con.msgRef,
        sExt.con.msgTot,
        sExt.con.msgSeg
    ); 

    mprintf("\r\n< Send Chinese Concatenate Text SMS begin... >\r\n");

    //The 1st segment of CH-CON-SMS
    sExt.conPres = TRUE;
    sExt.con.msgType = 0xFF; //If it's 0xFF: use default CON-SMS-TYPE
    sExt.con.msgRef = 53;  //Different CON-SMS shall have different <msgRef>
    sExt.con.msgTot = 3;
    sExt.con.msgSeg = 1;
    
    iResult = RIL_SMS_SendSMS_Text_Ext(strPhNum,Ql_strlen(strPhNum),LIB_SMS_CHARSET_UCS2,strConMsgChSeg1,Ql_strlen(strConMsgChSeg1),&nMsgRef,&sExt);
    if (iResult != RIL_AT_SUCCESS)
    {   
        mprintf(
            "< Fail to send Text SMS, cause:%d,cp:1,cy:%d,cr:%d,ct:%d,cs:%d >\r\n", 
            iResult,
            sExt.con.msgType,
            sExt.con.msgRef,
            sExt.con.msgTot,
            sExt.con.msgSeg
        );
        
        return;
    }
    mprintf(
        "< Send Text SMS successfully, MsgRef:%u,cp:1,cy:%d,cr:%d,ct:%d,cs:%d >\r\n", 
        nMsgRef,
        sExt.con.msgType,
        sExt.con.msgRef,
        sExt.con.msgTot,
        sExt.con.msgSeg
    ); 

    //The 2st segment of CH-CON-SMS
    sExt.con.msgSeg = 2;
    iResult = RIL_SMS_SendSMS_Text_Ext(strPhNum,Ql_strlen(strPhNum),LIB_SMS_CHARSET_UCS2,strConMsgChSeg2,Ql_strlen(strConMsgChSeg2),&nMsgRef,&sExt);
    if (iResult != RIL_AT_SUCCESS)
    {   
        mprintf(
            "< Fail to send Text SMS, cause:%d,cp:1,cy:%d,cr:%d,ct:%d,cs:%d >\r\n", 
            iResult,
            sExt.con.msgType,
            sExt.con.msgRef,
            sExt.con.msgTot,
            sExt.con.msgSeg
        );
        
        return;
    }
    mprintf(
        "< Send Text SMS successfully, MsgRef:%u,cp:1,cy:%d,cr:%d,ct:%d,cs:%d >\r\n", 
        nMsgRef,
        sExt.con.msgType,
        sExt.con.msgRef,
        sExt.con.msgTot,
        sExt.con.msgSeg
    ); 

    //The 3st segment of CH-CON-SMS
    sExt.con.msgSeg = 3;
    iResult = RIL_SMS_SendSMS_Text_Ext(strPhNum,Ql_strlen(strPhNum),LIB_SMS_CHARSET_UCS2,strConMsgChSeg3,Ql_strlen(strConMsgChSeg3),&nMsgRef,&sExt);
    if (iResult != RIL_AT_SUCCESS)
    {   
        mprintf(
            "< Fail to send Text SMS, cause:%d,cp:1,cy:%d,cr:%d,ct:%d,cs:%d >\r\n", 
            iResult,
            sExt.con.msgType,
            sExt.con.msgRef,
            sExt.con.msgTot,
            sExt.con.msgSeg
        );
        
        return;
    }
    mprintf(
        "< Send Text SMS successfully, MsgRef:%u,cp:1,cy:%d,cr:%d,ct:%d,cs:%d >\r\n", 
        nMsgRef,
        sExt.con.msgType,
        sExt.con.msgRef,
        sExt.con.msgTot,
        sExt.con.msgSeg
    ); 
#endif    
}

/*****************************************************************************
 * FUNCTION
 *  Hdlr_RecvNewSMS
 *
 * DESCRIPTION
 *  The handler function of new received SMS.
 *  
 * PARAMETERS
 *  <nIndex>     The SMS index in storage,it starts from 1
 *  <bAutoReply> TRUE: The module should reply a SMS to the sender; 
 *               FALSE: The module only read this SMS.
 *
 * RETURNS
 *  VOID
 *
 * NOTE
 *  1. This is an internal function
 *****************************************************************************/

void Hdlr_RecvNewSMS(u32 nIndex, bool bAutoReply)
{
    s32 iResult = 0;
    u32 uMsgRef = 0;
	char *ret_str = NULL;
    ST_RIL_SMS_TextInfo *pTextInfo = NULL;
    ST_RIL_SMS_DeliverParam *pDeliverTextInfo = NULL;
    char aPhNum[RIL_SMS_PHONE_NUMBER_MAX_LEN] = {0};
	//Ql_strcpy(aPhNum, systemset.sms_replyphone);
    char aReplyCon[20] = {0};
	//Ql_strcpy(aReplyCon, systemset.sms_replycon);
	
    bool bResult = FALSE;
	char message_together[2048] = {0};
	
    pTextInfo = Ql_MEM_Alloc(sizeof(ST_RIL_SMS_TextInfo));
    if (NULL == pTextInfo)
    {
        mprintf("%s/%d:Ql_MEM_Alloc FAIL! size:%u\r\n", sizeof(ST_RIL_SMS_TextInfo), __func__, __LINE__);
        return;
    }
    Ql_memset(pTextInfo, 0x00, sizeof(ST_RIL_SMS_TextInfo));
    iResult = RIL_SMS_ReadSMS_Text(nIndex, LIB_SMS_CHARSET_GSM, pTextInfo);
    if (iResult != RIL_AT_SUCCESS)
    {
        Ql_MEM_Free(pTextInfo);
        mprintf("Fail to read text SMS[%d], cause:%d\r\n", nIndex, iResult);
        return;
    }        
    
    if ((LIB_SMS_PDU_TYPE_DELIVER != (pTextInfo->type)) || (RIL_SMS_STATUS_TYPE_INVALID == (pTextInfo->status)))
    {
        Ql_MEM_Free(pTextInfo);
        mprintf("WARNING: NOT a new received SMS.\r\n");    
        return;
    }
    
    pDeliverTextInfo = &((pTextInfo->param).deliverParam);    

    if(TRUE == pDeliverTextInfo->conPres)  //Receive CON-SMS segment
    {
        s8 iBufIdx = 0;
        u8 uSeg = 0;
        u16 uConLen = 0;
        iBufIdx = ConSMSBuf_GetIndex(g_asConSMSBuf,CON_SMS_BUF_MAX_CNT,&(pDeliverTextInfo->con));
        if(-1 == iBufIdx)
        {
            mprintf("Enter Hdlr_RecvNewSMS,WARNING! ConSMSBuf_GetIndex FAIL! Show this CON-SMS-SEG directly!\r\n");

            mprintf(
                "status:%u,type:%u,alpha:%u,sca:%s,oa:%s,scts:%s,data length:%u,cp:1,cy:%d,cr:%d,ct:%d,cs:%d\r\n",
                    (pTextInfo->status),
                    (pTextInfo->type),
                    (pDeliverTextInfo->alpha),
                    (pTextInfo->sca),
                    (pDeliverTextInfo->oa),
                    (pDeliverTextInfo->scts),
                    (pDeliverTextInfo->length),
                    pDeliverTextInfo->con.msgType,
                    pDeliverTextInfo->con.msgRef,
                    pDeliverTextInfo->con.msgTot,
                    pDeliverTextInfo->con.msgSeg
            );
            mprintf("data = %s\r\n",(pDeliverTextInfo->data));

            Ql_MEM_Free(pTextInfo);
        
            return;
        }
        bResult = ConSMSBuf_AddSeg(
                    g_asConSMSBuf,
                    CON_SMS_BUF_MAX_CNT,
                    iBufIdx,
                    &(pDeliverTextInfo->con),
                    (pDeliverTextInfo->data),
                    (pDeliverTextInfo->length)
        );
        if(FALSE == bResult)
        {
            mprintf("Enter Hdlr_RecvNewSMS,WARNING! ConSMSBuf_AddSeg FAIL! Show this CON-SMS-SEG directly!\r\n");

            mprintf(
                "status:%u,type:%u,alpha:%u,sca:%s,oa:%s,scts:%s,data length:%u,cp:1,cy:%d,cr:%d,ct:%d,cs:%d\r\n",
                (pTextInfo->status),
                (pTextInfo->type),
                (pDeliverTextInfo->alpha),
                (pTextInfo->sca),
                (pDeliverTextInfo->oa),
                (pDeliverTextInfo->scts),
                (pDeliverTextInfo->length),
                pDeliverTextInfo->con.msgType,
                pDeliverTextInfo->con.msgRef,
                pDeliverTextInfo->con.msgTot,
                pDeliverTextInfo->con.msgSeg
            );
            mprintf("data = %s\r\n",(pDeliverTextInfo->data));

            Ql_MEM_Free(pTextInfo);
        
            return;
        }
        bResult = ConSMSBuf_IsIntact(
                    g_asConSMSBuf,
                    CON_SMS_BUF_MAX_CNT,
                    iBufIdx,
                    &(pDeliverTextInfo->con)
        );
        if(FALSE == bResult)
        {
            mprintf(
                "Enter Hdlr_RecvNewSMS,WARNING! ConSMSBuf_IsIntact FAIL! Waiting. cp:1,cy:%d,cr:%d,ct:%d,cs:%d\r\n",
                pDeliverTextInfo->con.msgType,
                pDeliverTextInfo->con.msgRef,
                pDeliverTextInfo->con.msgTot,
                pDeliverTextInfo->con.msgSeg
            );

            Ql_MEM_Free(pTextInfo);

            return;
        }

        //Show the CON-SMS
        mprintf(
            "status:%u,type:%u,alpha:%u,sca:%s,oa:%s,scts:%s",
            (pTextInfo->status),
            (pTextInfo->type),
            (pDeliverTextInfo->alpha),
            (pTextInfo->sca),
            (pDeliverTextInfo->oa),
            (pDeliverTextInfo->scts)
        );
        
        uConLen = 0;
        for(uSeg = 1; uSeg <= pDeliverTextInfo->con.msgTot; uSeg++)
        {
            uConLen += g_asConSMSBuf[iBufIdx].asSeg[uSeg-1].uLen;
        }

        mprintf(",data length:%u",uConLen);
        mprintf("\r\n"); //Print CR LF

        for(uSeg = 1; uSeg <= pDeliverTextInfo->con.msgTot; uSeg++)
        {
            mprintf("data = %s\r\n ,len = %d\r\n",
                g_asConSMSBuf[iBufIdx].asSeg[uSeg-1].aData,
                g_asConSMSBuf[iBufIdx].asSeg[uSeg-1].uLen
            );
			Ql_strcat(message_together, (char *)g_asConSMSBuf[iBufIdx].asSeg[uSeg-1].aData);
        }
		
		//yq add
		mprintf("message_together = %s\r\n", message_together);
		mprintf("replay message aPhNum = %s\r\n", aPhNum);
		//8BF760A856DE590D0059786E8BA4529E7406	请您回复Y确认办理
		ret_str = Ql_strstr(message_together, "8BF76");
		if (NULL != ret_str)  // Not reply SMS from operator
        {
            mprintf("<-- Replying SMS... -->\r\n");
            iResult = RIL_SMS_SendSMS_Text(aPhNum, Ql_strlen(aPhNum),LIB_SMS_CHARSET_GSM,(u8*)aReplyCon,Ql_strlen(aReplyCon),&uMsgRef);
            if (iResult != RIL_AT_SUCCESS)
            {
                mprintf("RIL_SMS_SendSMS_Text FAIL! iResult:%u\r\n",iResult);
                return;
            }
            mprintf("<-- RIL_SMS_SendTextSMS OK.");
            //aReplyCon:%d -->\r\n", aReplyCon);
			
			//flag_sms_return = 1;
        }

        mprintf("exit 0\r\n"); //Print CR LF

        //Reset CON-SMS context
        bResult = ConSMSBuf_ResetCtx(g_asConSMSBuf,CON_SMS_BUF_MAX_CNT,iBufIdx);
        if(FALSE == bResult)
        {
            mprintf("Enter Hdlr_RecvNewSMS,WARNING! ConSMSBuf_ResetCtx FAIL! iBufIdx:%d\r\n",iBufIdx);
        }

        Ql_MEM_Free(pTextInfo);
        
        return;
    }

    mprintf("<-- RIL_SMS_ReadSMS_Text OK. eCharSet:LIB_SMS_CHARSET_GSM,nIndex:%u -->\r\n",nIndex);
    mprintf("status:%u,type:%u,alpha:%u,sca:%s,oa:%s,scts:%s,data length:%u\r\n",
        pTextInfo->status,
        pTextInfo->type,
        pDeliverTextInfo->alpha,
        pTextInfo->sca,
        pDeliverTextInfo->oa,
        pDeliverTextInfo->scts,
        pDeliverTextInfo->length);
    mprintf("data = %s\r\n",(pDeliverTextInfo->data));
	mprintf("replay message aPhNum = %s\r\n", aPhNum);
    if (bAutoReply)
    {
        if (Ql_strstr(pDeliverTextInfo->data, "8BF760A856DE590D0059786E8BA4529E7406"))  // Not reply SMS from operator
        {
            mprintf("<-- Replying SMS... -->\r\n");
            iResult = RIL_SMS_SendSMS_Text(aPhNum, Ql_strlen(aPhNum),LIB_SMS_CHARSET_GSM,(u8*)aReplyCon,Ql_strlen(aReplyCon),&uMsgRef);
            if (iResult != RIL_AT_SUCCESS)
            {
                mprintf("RIL_SMS_SendSMS_Text FAIL! iResult:%u\r\n",iResult);
                return;
            }
            mprintf("<-- RIL_SMS_SendTextSMS OK. ");
			//aReplyCon:%d -->\r\n", aReplyCon);
			//flag_sms_return = 1;
        }
    }
	Ql_strcpy(aPhNum, pDeliverTextInfo->oa);
    Ql_MEM_Free(pTextInfo);
    return;
}

/*****************************************************************************
 * FUNCTION
 *  ConSMSBuf_GetIndex
 *
 * DESCRIPTION
 *  This function is used to get available index in <pCSBuf>
 *  
 * PARAMETERS
 *  <pCSBuf>     The SMS index in storage,it starts from 1
 *  <uCSMaxCnt>  TRUE: The module should reply a SMS to the sender; FALSE: The module only read this SMS.
 *  <pCon>       The pointer of 'ST_RIL_SMS_Con' data
 *
 * RETURNS
 *  -1:   FAIL! Can not get available index
 *  OTHER VALUES: SUCCESS.
 *
 * NOTE
 *  1. This is an internal function
 *****************************************************************************/
s8 ConSMSBuf_GetIndex(ConSMSStruct *pCSBuf,u8 uCSMaxCnt,ST_RIL_SMS_Con *pCon)
{
	u8 uIdx = 0;
	
    if(    (NULL == pCSBuf) || (0 == uCSMaxCnt) 
        || (NULL == pCon)
      )
    {
        mprintf("Enter ConSMSBuf_GetIndex,FAIL! Parameter is INVALID. pCSBuf:%x,uCSMaxCnt:%d,pCon:%x\r\n",pCSBuf,uCSMaxCnt,pCon);
        return -1;
    }

    if((pCon->msgTot) > CON_SMS_MAX_SEG)
    {
        mprintf("Enter ConSMSBuf_GetIndex,FAIL! msgTot:%d is larger than limit:%d\r\n",pCon->msgTot,CON_SMS_MAX_SEG);
        return -1;
    }
    
	for(uIdx = 0; uIdx < uCSMaxCnt; uIdx++)  //Match all exist records
	{
        if(    (pCon->msgRef == pCSBuf[uIdx].uMsgRef)
            && (pCon->msgTot == pCSBuf[uIdx].uMsgTot)
          )
        {
            return uIdx;
        }
	}

	for (uIdx = 0; uIdx < uCSMaxCnt; uIdx++)
	{
		if (0 == pCSBuf[uIdx].uMsgTot)  //Find the first unused record
		{
            pCSBuf[uIdx].uMsgTot = pCon->msgTot;
            pCSBuf[uIdx].uMsgRef = pCon->msgRef;
            
			return uIdx;
		}
	}

    mprintf("Enter ConSMSBuf_GetIndex,FAIL! No avail index in ConSMSBuf,uCSMaxCnt:%d\r\n",uCSMaxCnt);
    
	return -1;
}


/*****************************************************************************
 * FUNCTION
 *  ConSMSBuf_AddSeg
 *
 * DESCRIPTION
 *  This function is used to add segment in <pCSBuf>
 *  
 * PARAMETERS
 *  <pCSBuf>     The SMS index in storage,it starts from 1
 *  <uCSMaxCnt>  TRUE: The module should reply a SMS to the sender; FALSE: The module only read this SMS.
 *  <uIdx>       Index of <pCSBuf> which will be stored
 *  <pCon>       The pointer of 'ST_RIL_SMS_Con' data
 *  <pData>      The pointer of CON-SMS-SEG data
 *  <uLen>       The length of CON-SMS-SEG data
 *
 * RETURNS
 *  FALSE:   FAIL!
 *  TRUE: SUCCESS.
 *
 * NOTE
 *  1. This is an internal function
 *****************************************************************************/
bool ConSMSBuf_AddSeg(ConSMSStruct *pCSBuf,u8 uCSMaxCnt,u8 uIdx,ST_RIL_SMS_Con *pCon,u8 *pData,u16 uLen)
{
    u8 uSeg = 1;
    
    if(    (NULL == pCSBuf) || (0 == uCSMaxCnt) 
        || (uIdx >= uCSMaxCnt)
        || (NULL == pCon)
        || (NULL == pData)
        || (uLen > (CON_SMS_SEG_MAX_CHAR * 4))
      )
    {
        mprintf("Enter ConSMSBuf_AddSeg,FAIL! Parameter is INVALID. pCSBuf:%x,uCSMaxCnt:%d,uIdx:%d,pCon:%x,pData:%x,uLen:%d\r\n",pCSBuf,uCSMaxCnt,uIdx,pCon,pData,uLen);
        return FALSE;
    }

    if((pCon->msgTot) > CON_SMS_MAX_SEG)
    {
        mprintf("Enter ConSMSBuf_GetIndex,FAIL! msgTot:%d is larger than limit:%d\r\n",pCon->msgTot,CON_SMS_MAX_SEG);
        return FALSE;
    }

    uSeg = pCon->msgSeg;
    pCSBuf[uIdx].abSegValid[uSeg-1] = TRUE;
    Ql_memcpy(pCSBuf[uIdx].asSeg[uSeg-1].aData,pData,uLen);
    pCSBuf[uIdx].asSeg[uSeg-1].uLen = uLen;
    
	return TRUE;
}


/*****************************************************************************
 * FUNCTION
 *  ConSMSBuf_IsIntact
 *
 * DESCRIPTION
 *  This function is used to check the CON-SMS is intact or not
 *  
 * PARAMETERS
 *  <pCSBuf>     The SMS index in storage,it starts from 1
 *  <uCSMaxCnt>  TRUE: The module should reply a SMS to the sender; FALSE: The module only read this SMS.
 *  <uIdx>       Index of <pCSBuf> which will be stored
 *  <pCon>       The pointer of 'ST_RIL_SMS_Con' data
 *
 * RETURNS
 *  FALSE:   FAIL!
 *  TRUE: SUCCESS.
 *
 * NOTE
 *  1. This is an internal function
 *****************************************************************************/
bool ConSMSBuf_IsIntact(ConSMSStruct *pCSBuf,u8 uCSMaxCnt,u8 uIdx,ST_RIL_SMS_Con *pCon)
{
    u8 uSeg = 1;
	
    if(    (NULL == pCSBuf) 
        || (0 == uCSMaxCnt) 
        || (uIdx >= uCSMaxCnt)
        || (NULL == pCon)
      )
    {
        mprintf("Enter ConSMSBuf_IsIntact,FAIL! Parameter is INVALID. pCSBuf:%x,uCSMaxCnt:%d,uIdx:%d,pCon:%x\r\n",pCSBuf,uCSMaxCnt,uIdx,pCon);
        return FALSE;
    }

    if((pCon->msgTot) > CON_SMS_MAX_SEG)
    {
        mprintf("Enter ConSMSBuf_GetIndex,FAIL! msgTot:%d is larger than limit:%d\r\n",pCon->msgTot,CON_SMS_MAX_SEG);
        return FALSE;
    }
        
	for (uSeg = 1; uSeg <= (pCon->msgTot); uSeg++)
	{
        if(FALSE == pCSBuf[uIdx].abSegValid[uSeg-1])
        {
            mprintf("Enter ConSMSBuf_IsIntact,FAIL! uSeg:%d has not received!\r\n",uSeg);
            return FALSE;
        }
	}
    
    return TRUE;
}

/*****************************************************************************
 * FUNCTION
 *  ConSMSBuf_ResetCtx
 *
 * DESCRIPTION
 *  This function is used to reset ConSMSBuf context
 *  
 * PARAMETERS
 *  <pCSBuf>     The SMS index in storage,it starts from 1
 *  <uCSMaxCnt>  TRUE: The module should reply a SMS to the sender; FALSE: The module only read this SMS.
 *  <uIdx>       Index of <pCSBuf> which will be stored
 *
 * RETURNS
 *  FALSE:   FAIL!
 *  TRUE: SUCCESS.
 *
 * NOTE
 *  1. This is an internal function
 *****************************************************************************/
bool ConSMSBuf_ResetCtx(ConSMSStruct *pCSBuf,u8 uCSMaxCnt,u8 uIdx)
{
    if(    (NULL == pCSBuf) || (0 == uCSMaxCnt) 
        || (uIdx >= uCSMaxCnt)
      )
    {
        mprintf("Enter ConSMSBuf_ResetCtx,FAIL! Parameter is INVALID. pCSBuf:%x,uCSMaxCnt:%d,uIdx:%d\r\n",pCSBuf,uCSMaxCnt,uIdx);
        return FALSE;
    }
    
    //Default reset
    Ql_memset(&pCSBuf[uIdx],0x00,sizeof(ConSMSStruct));

    //TODO: Add special reset here
    
    return TRUE;
}
/*
static int my_strstr(const char *mother_str, const char *son_str)
{
	int i,j;
	for(i = 0;)
}
*/
/*
void ActiveCardTest()
{
	mprintf("\r\n22222222222222222222222\r\n");
	u8 i;
	flag_call_ready = 0;
	flag_sms_return = 0;
	
	systemset.HandInter = 1;
	//1  call 10086
	Telephony_CallOut();

	if(flag_call_ready == 1)
	{
		//2  send sms
		i = SMS_Initialize();         
	    if (!i)
	    {
	        mprintf("Fail to initialize SMS\r\n");
	    }
	    SMS_TextMode_Send();
	}
	flag_call = 0;
	flag_sms = 0;
}
*/
