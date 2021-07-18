

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
#include "user_lmq.h"



USER_LMQ_CONFIG_T user_lmq_config=
{
	"post-cn-v0h0dlzxa02.mqtt.aliyuncs.com",
	"GID_1988",
	"4526",
	"LTAIbX60YQNt7UTf",
	"R2s9K54grf64t2U2hOLQRKfolMdJNZ",
	1883,
	1
};





/////////////////////////////////////////需要的编解码代码实现///////////////////////////////



const char * base64char = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
const char padding_char = '=';


/*编码代码
* const unsigned char * sourcedata， 源数组
* char * base64 ，码字保存
*/
int base64_encode(const unsigned char * sourcedata, char * base64)
{
    int i=0, j=0;
    unsigned char trans_index=0;    // 索引是8位，但是高两位都为0
    const int datalength = Ql_strlen((const char*)sourcedata);
    for (; i < datalength; i += 3){
        // 每三个一组，进行编码
        // 要编码的数字的第一个
        trans_index = ((sourcedata[i] >> 2) & 0x3f);
        base64[j++] = base64char[(int)trans_index];
        // 第二个
        trans_index = ((sourcedata[i] << 4) & 0x30);
        if (i + 1 < datalength){
            trans_index |= ((sourcedata[i + 1] >> 4) & 0x0f);
            base64[j++] = base64char[(int)trans_index];
        }else{
            base64[j++] = base64char[(int)trans_index];

            base64[j++] = padding_char;

            base64[j++] = padding_char;

            break;   // 超出总长度，可以直接break
        }
        // 第三个
        trans_index = ((sourcedata[i + 1] << 2) & 0x3c);
        if (i + 2 < datalength){ // 有的话需要编码2个
            trans_index |= ((sourcedata[i + 2] >> 6) & 0x03);
            base64[j++] = base64char[(int)trans_index];

            trans_index = sourcedata[i + 2] & 0x3f;
            base64[j++] = base64char[(int)trans_index];
        }
        else{
            base64[j++] = base64char[(int)trans_index];

            base64[j++] = padding_char;

            break;
        }
    }

    base64[j] = '\0'; 

    return 0;
}


unsigned long int ft(  
                    int t,  
                    unsigned long int x,  
                    unsigned long int y,  
                    unsigned long int z  
                    )  
{  
unsigned long int a,b,c;  
  
    if (t < 20)  
    {  
        a = x & y;  
        b = (~x) & z;  
        c = a ^ b;  
    }  
    else if (t < 40)  
    {  
        c = x ^ y ^ z;  
    }  
    else if (t < 60)  
    {  
        a = x & y;  
        b = a ^ (x & z);  
        c = b ^ (y & z);  
    }  
    else if (t < 80)  
    {  
        c = (x ^ y) ^ z;  
    }  
  
    return c;  
}  



unsigned long int k(int t)  
{  
unsigned long int c;  
  
    if (t < 20)  
    {  
        c = 0x5a827999;  
    }  
    else if (t < 40)  
    {  
        c = 0x6ed9eba1;  
    }  
    else if (t < 60)  
    {  
        c = 0x8f1bbcdc;  
    }  
    else if (t < 80)  
    {  
        c = 0xca62c1d6;  
    }  
  
    return c;  
}  
  
unsigned long int rotr(int bits, unsigned long int a)  
{  
unsigned long int c,d,e,f,g;  
    c = (0x0001 << bits)-1;  
    d = ~c;  
  
    e = (a & d) >> bits;  
    f = (a & c) << (32 - bits);  
  
    g = e | f;  
  
    return (g & 0xffffffff );  
  
}  



unsigned long int rotl(int bits, unsigned long int a)  
{  
unsigned long int c,d,e,f,g;  
    c = (0x0001 << (32-bits))-1;  
    d = ~c;  
  
    e = (a & c) << bits;  
    f = (a & d) >> (32 - bits);  
  
    g = e | f;  
  
    return (g & 0xffffffff );  
  
}  
  

void sha1   (  
            unsigned char *message,  
            int message_length,  
            unsigned char *digest  
            )  
{  
int i;  
int num_blocks;  
int block_remainder;  
int padded_length;  
  
unsigned long int l;  
unsigned long int t;  
unsigned long int h[5];  
unsigned long int a,b,c,d,e;  
unsigned long int w[80];  
unsigned long int temp;  
  
  
    /* Calculate the number of 512 bit blocks */  
  
    padded_length = message_length + 8; /* Add length for l */  
    padded_length = padded_length + 1; /* Add the 0x01 bit postfix */  
  
    l = message_length * 8;  
  
    num_blocks = padded_length / 64;  
    block_remainder = padded_length % 64;  
  
  
    if (block_remainder > 0)  
    {  
        num_blocks++;  
    }  
  
    padded_length = padded_length + (64 - block_remainder);  
  
     /* clear the padding field */  
    for (i = message_length; i < (num_blocks * 64); i++)  
    {  
        message[i] = 0x00;             
    }  
  
    /* insert b1 padding bit */  
    message[message_length] = 0x80;  
      
    /* Insert l */  
    message[(num_blocks*64)-1] = (unsigned char)( l        & 0xff);  
    message[(num_blocks*64)-2] = (unsigned char)((l >> 8)  & 0xff);  
    message[(num_blocks*64)-3] = (unsigned char)((l >> 16) & 0xff);  
    message[(num_blocks*64)-4] = (unsigned char)((l >> 24) & 0xff);  
  
    /* Set initial hash state */  
    h[0] = 0x67452301;  
    h[1] = 0xefcdab89;  
    h[2] = 0x98badcfe;  
    h[3] = 0x10325476;  
    h[4] = 0xc3d2e1f0;  
  
  
    for (i = 0; i < num_blocks; i++)  
    {  
        /* Prepare the message schedule */  
        for (t=0; t < 80; t++)  
        {  
            if (t < 16)  
            {  
                w[t]  = (256*256*256) * message[(i*64)+(t*4)];  
                w[t] += (256*256    ) * message[(i*64)+(t*4) + 1];  
                w[t] += (256        ) * message[(i*64)+(t*4) + 2];  
                w[t] +=                 message[(i*64)+(t*4) + 3];  
            }  
            else if (t < 80)  
            {  
                w[t] = rotl(1,(w[t-3] ^ w[t-8] ^ w[t-14] ^ w[t-16]));  
            }  
        }  
        /* Initialize the five working variables */  
        a = h[0];  
        b = h[1];  
        c = h[2];  
        d = h[3];  
        e = h[4];  
  
        /* iterate a-e 80 times */  
  
        for (t = 0; t < 80; t++)  
        {  
            temp = (rotl(5,a) + ft(t,b,c,d)) & 0xffffffff;  
            temp = (temp + e) & 0xffffffff;  
            temp = (temp + k(t)) & 0xffffffff;  
            temp = (temp + w[t]) & 0xffffffff;  
            e = d;  
            d = c;  
            c = rotl(30,b);  
            b = a;  
            a = temp;  
  
        }  
  
        /* compute the ith intermediate hash value */  
        h[0] = (a + h[0]) & 0xffffffff;  
        h[1] = (b + h[1]) & 0xffffffff;  
        h[2] = (c + h[2]) & 0xffffffff;  
        h[3] = (d + h[3]) & 0xffffffff;  
        h[4] = (e + h[4]) & 0xffffffff;  
  
  
    }  
  
    digest[3]  = (unsigned char) ( h[0]       & 0xff);  
    digest[2]  = (unsigned char) ((h[0] >> 8) & 0xff);  
    digest[1]  = (unsigned char) ((h[0] >> 16) & 0xff);  
    digest[0]  = (unsigned char) ((h[0] >> 24) & 0xff);  
  
    digest[7]  = (unsigned char) ( h[1]       & 0xff);  
    digest[6]  = (unsigned char) ((h[1] >> 8) & 0xff);  
    digest[5]  = (unsigned char) ((h[1] >> 16) & 0xff);  
    digest[4]  = (unsigned char) ((h[1] >> 24) & 0xff);  
  
    digest[11]  = (unsigned char) ( h[2]       & 0xff);  
    digest[10]  = (unsigned char) ((h[2] >> 8) & 0xff);  
    digest[9] = (unsigned char) ((h[2] >> 16) & 0xff);  
    digest[8] = (unsigned char) ((h[2] >> 24) & 0xff);  
  
    digest[15] = (unsigned char) ( h[3]       & 0xff);  
    digest[14] = (unsigned char) ((h[3] >> 8) & 0xff);  
    digest[13] = (unsigned char) ((h[3] >> 16) & 0xff);  
    digest[12] = (unsigned char) ((h[3] >> 24) & 0xff);  
  
    digest[19] = (unsigned char) ( h[4]       & 0xff);  
    digest[18] = (unsigned char) ((h[4] >> 8) & 0xff);  
    digest[17] = (unsigned char) ((h[4] >> 16) & 0xff);  
    digest[16] = (unsigned char) ((h[4] >> 24) & 0xff);  
  
}  
  


void hmac_sha1(  
                unsigned char *key,  
                int key_length,  
                unsigned char *data,  
                int data_length,  
                unsigned char *digest  
                )  
  
{  
    int b = 64; /* blocksize */  
    unsigned char ipad = 0x36;  
  
    unsigned char opad = 0x5c;  
  
    unsigned char k0[64];  
    unsigned char k0xorIpad[64];  
    unsigned char step7data[64];  
    unsigned char step5data[512+128];  
    unsigned char step8data[64+20];  
    int i;  
  
    for (i=0; i<64; i++)  
    {  
        k0[i] = 0x00;  
    }  
  
  
  
    if (key_length != b)    /* Step 1 */  
    {  
        /* Step 2 */  
        if (key_length > b)        
        {  
            sha1(key, key_length, digest);  
            for (i=0;i<20;i++)  
            {  
                k0[i]=digest[i];  
            }  
        }  
        else if (key_length < b)  /* Step 3 */  
        {  
            for (i=0; i<key_length; i++)  
            {  
                k0[i] = key[i];  
            }  
        }  
    }  
    else  
    {  
        for (i=0;i<b;i++)  
        {  
            k0[i] = key[i];  
        }  
    }  
    /* Step 4 */  
    for (i=0; i<64; i++)  
    {  
        k0xorIpad[i] = k0[i] ^ ipad;  
    }  
    /* Step 5 */  
    for (i=0; i<64; i++)  
    {  
        step5data[i] = k0xorIpad[i];  
    }  
    for (i=0;i<data_length;i++)  
    {  
        step5data[i+64] = data[i];  
    }  
  
    /* Step 6 */  
    sha1(step5data, data_length+b, digest);  
  
  
    /* Step 7 */  
    for (i=0; i<64; i++)  
    {  
        step7data[i] = k0[i] ^ opad;  
    }  
    
    /* Step 8 */  
    for (i=0;i<64;i++)  
    {  
        step8data[i] = step7data[i];  
    }  
    for (i=0;i<20;i++)  
    {  
        step8data[i+64] = digest[i];  
    }  
  
  
    /* Step 9 */  
    sha1(step8data, b+20, digest);  
  
}  

/////////////////////////////////////////需要的编解码代码实现///////////////////////////////


#if 0
/*!
 * @brief 获取MQTT登录信息
 * \n
 *
 * @param 		parama LMQ登录信息
 *				aliot_user_puser  MQTT需要的登录信息
 * @return 1 未开启SSL 直接返回成功
 * \n
 * @see
 */


s8 user_get_puser_info_alilmq(USER_LMQ_CONFIG_T parama,aliot_user_info_t *aliot_user_puser)
{
	char tempData[100]={0};
	char resultData[100]={0};
	char clientIdUrl[64];

	Ql_sprintf(clientIdUrl, "%s@@@%s", parama.groupId, parama.deviceId);
	hmac_sha1(parama.secretKey,Ql_strlen(parama.secretKey),parama.groupId, Ql_strlen(parama.groupId),tempData);
	base64_encode(tempData,resultData);
	
	aliot_user_puser->port=parama.port;
	Ql_strcpy(aliot_user_puser->host_name,parama.host);
	Ql_strcpy(aliot_user_puser->client_id,clientIdUrl);
	Ql_strcpy(aliot_user_puser->user_name,parama.accessKey);
	Ql_strcpy(aliot_user_puser->password,resultData);
	aliot_user_puser->pubKey=NULL;
	return 0;
}

#endif








