/*
 * @Author: your name
 * @Date: 2020-10-22 11:54:38
 * @LastEditTime: 2020-11-10 11:37:30
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \stm32f407vet6-handheld\applications\vcom_cmd.c
 */
/****************************************************************************
 *
 * File Name£º
 *  
 * Author£º
 *  
 * Date£º
 * 
 * Descriptions:
 * 
 *
 ******************************************************************************/
/*----------------------------------------------------------------------------*
**                             Dependencies                                   *
**----------------------------------------------------------------------------*/
/* <Header file of standard Library> */
/*----------------------------------------------------------------------------*/
/* "Header file of nonstandard Library" */
#include "vcom_cmd.h"
/**---------------------------------------------------------------------------*
 **                            Debugging Flag                                 *
 **---------------------------------------------------------------------------*/
/* VCOM_CMD_TRACE */
//define VCOM_CMD_TRACE
#ifdef VCOM_CMD_TRACE
    #define VCOM_CMD_TRACE rt_kprintf
#else
    #define VCOM_CMD_TRACE(...)
#endif

/**---------------------------------------------------------------------------*
**                             Compiler Flag                                  *
**----------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif

/*----------------------------------------------------------------------------*
**                             Mcaro Definitions                              *
**----------------------------------------------------------------------------*/
/* cmd packet max len */
#define CMD_PACKET_MAX_LEN (64)
/* cmd packet head byte */
#define CMD_PACKET_HEAD_BYTE ((uint8_t)'[')
/* cmd packet tall byte */
#define CMD_PACKET_TALL_BYTE ((uint8_t)']')
/*----------------------------------------------------------------------------*
**                             Data Structures                                *
**----------------------------------------------------------------------------*/
/* cmd packet buffer status */
typedef enum
{
    /* empty status */
    CMD_PACKET_EMPTY = 0,
    /* receive packet head status */
    CMD_PACKET_HEAD,
    /* receive packet completed status */
    CMD_PACKET_COMPLETED,
}CMD_PACKET_BUF_STATUS_E;
/*----------------------------------------------------------------------------*/
/* cmd packet buffer */
typedef struct 
{
    /* packet buffer status */
    CMD_PACKET_BUF_STATUS_E ePacketStatus;
    /* u32 Packet Len */
    uint32_t u32PacketLen;
    /* u8 packet buffer */
    uint8_t  pu8PacketBuf[CMD_PACKET_MAX_LEN];
}CmdPacketBuf_T;
/*----------------------------------------------------------------------------*/
/* cmd handler function tpye define */
typedef void (*CMD_HANDLER_FUNC)(const StrConstRef_T* pctStrRefPrarm);
/*----------------------------------------------------------------------------*/
/* cmd handler list struct */
typedef struct 
{
    /* cmd name */
    const char* pcszCmdName;
    /* cmd lend */
    uint32_t u32CmdNameLen;
    /* cmd handler function */
    CMD_HANDLER_FUNC handlerFunc;
}CmdHandlerFunc_T;

/*----------------------------------------------------------------------------*
**                             Local Vars                                     *
**----------------------------------------------------------------------------*/
/* cmd packet buffer */
static CmdPacketBuf_T s_tCmdPacketBuf = {CMD_PACKET_EMPTY};

/*************************************************
 *  cmd   format :
 *  read command : [CMD]
 *  set  command : [CMD=Param]
*************************************************/
static void _CMD_HandlerVER(const StrConstRef_T* pctStrRefParam);
static void _CMD_HandlerBUILD(const StrConstRef_T* pctStrRefParam);


const static CmdHandlerFunc_T s_tCmdHandlerTbl[] = 
{
    {STR_ITEM("VER"), _CMD_HandlerVER },
    {STR_ITEM("BUILD"), _CMD_HandlerVER },
};

/*----------------------------------------------------------------------------*
**                             Extern Function                                *
**----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
**                             Local Function                                 *
**----------------------------------------------------------------------------*/
/*************************************************
* Function:_CMD_AssemblePacket
* Description: assenble packet get pack by using cmd packet format
* Author:
* Returns: 
* Parameter:
* History:
*************************************************/
static uint32_t _CMD_AssemblePacket(const uint8_t* pcu8Data, uint32_t u32DataLen)
{
/* define cmd paket buffer buf/len/status */
#define pu8PacketBuf  s_tCmdPacketBuf.pu8PacketBuf
#define u32PacketLen  s_tCmdPacketBuf.u32PacketLen
#define ePacketStatus s_tCmdPacketBuf.ePacketStatus
    uint32_t i =0;
    for ( i = 0;
        ( i < u32DataLen )
        && (CMD_PACKET_COMPLETED != ePacketStatus)
        && (u32DataLen < sizeof(pu8PacketBuf));
        ++i)
    {
       
        uint8_t u8Data = pcu8Data[i];
        switch (ePacketStatus)
        {
        case CMD_PACKET_EMPTY:
            if(CMD_PACKET_HEAD_BYTE == u8Data)
            {
                ePacketStatus = CMD_PACKET_HEAD;
                pu8PacketBuf[u32PacketLen++] = u8Data;
            }
            break;
        case CMD_PACKET_HEAD:
            if (CMD_PACKET_TALL_BYTE == u8Data)
            {
                ePacketStatus = CMD_PACKET_COMPLETED;
            }
            pu8PacketBuf[u32PacketLen++] = u8Data;
        case CMD_PACKET_COMPLETED :
            break;              
        default:
            break;
        }
    }
#undef pu8PacketBuf
#undef u32PacketLen
#undef ePacketStatus

    return i;
}

/*************************************************
* Function:
* Description: 
* Author:
* Returns: 
* Parameter:
* History:
*************************************************/
static uint32_t _CMD_Response(const char* pcszFmt, ...)
{
    /* turn the customize format use va_list/start/end*/
    char szCmdRspBuf[CMD_PACKET_MAX_LEN] = "";
    int iCmdRspLen = 0;
    va_list ap;
    va_start(ap, pcszFmt);
    iCmdRspLen = rt_vsnprintf(szCmdRspBuf, sizeof(szCmdRspBuf), pcszFmt, ap);
    va_end(ap);
    if ((iCmdRspLen > 0) && (iCmdRspLen <= (sizeof(szCmdRspBuf) - 2)))
    {
        szCmdRspBuf[iCmdRspLen++] = '\r';
        szCmdRspBuf[iCmdRspLen++] = '\n';
        /* select the channel (BT/VOM) send data */
        com_send_data((const uint8_t*)szCmdRspBuf, (uint32_t)iCmdRspLen);        
    }    
}

/*************************************************
* Function: _CMD_GetHandlerFunc
* Description: get cmd handler function
* Author: wangk
* Returns:
* Parameter:
* History:
*************************************************/
static CMD_HANDLER_FUNC _CMD_GetHandlerFunc(const StrConstRef_T* pctStrRefParam)
{
    /* strref is empty return null */
    if (STRREF_IsEmpty(pctStrRefParam))
    {
        return NULL;
    }

    CMD_HANDLER_FUNC handlerFunc = NULL ;
    /* find and handle the cmd */
    uint32_t u32TbLen = ARRAY_SIZE(s_tCmdHandlerTbl);
    uint32_t i = 0;
    for ( i = 0; i < u32TbLen; ++i )    
    {
        /* find cmd handle function, strict comparison of length and  */
        uint32_t u32CmdNameLen = s_tCmdHandlerTbl[i].u32CmdNameLen;
        if ((u32CmdNameLen == pctStrRefParam->u32Len)
            && (0 == memcmp(s_tCmdHandlerTbl[i].pcszCmdName
                ,pctStrRefParam->pcStr, u32CmdNameLen)))
        {
            /* code */
            handlerFunc = s_tCmdHandlerTbl[i].handlerFunc;
            break;
        }
        
    }
    
    return handlerFunc;   
}

/*************************************************
* Function: _CMD_PacketProcess
* Description: CMD Packet Process
* Author: 
* Returns:
* Parameter:
* History:
*************************************************/
static void _CMD_PacketProcess(const uint8_t* pcu8Packet, uint32_t u32PacketLen)
{
    /* check parameter */
    if((NULL == pcu8Packet) || (u32PacketLen < 2))
    {
        return;
    }

    /* packet content except head/tail */
    StrConstRef_T tPacketContent = {u32PacketLen - 2 , (const char*)(pcu8Packet + 1)};
    StrConstRef_T tStrRefSplitList[2] = {{0 , NULL }, };
    uint32_t u32SplitListLen = STRREF_Split(&tPacketContent, '=', tStrRefSplitList ,
                                         ARRAY_SIZE(tStrRefSplitList));
    if (0 == u32SplitListLen)
    {
        /* packet format error */
        _CMD_Response("[ERR]");
        return;
    }

    /* cmd handler function */
    CMD_HANDLER_FUNC handlerFunc = _CMD_GetHandlerFunc(& (tStrRefSplitList[0]));
    if (!handlerFunc)
    {
        /* not handler function */
        _CMD_Response("[ERR]");
        return;
    }

    /* CMD handle */
    switch (u32SplitListLen)
    {
    case 1:
        /* no parameter */
        {
            handlerFunc(NULL);
            break;
        }
    
    case 2:
        /* vaild parameter */
    {
        handlerFunc(&(tStrRefSplitList[1]));
        break;
    }
        
    default:
        /* packet format error */
        _CMD_Response("[ERR]");
        break;
    }

}

/*************************************************
* Function: _CMD_HandlerVER
* Description: ICCID??????
* Author: 
* Returns:
* Parameter:
* History:
*************************************************/
static void _CMD_HandlerVER(const StrConstRef_T* pctStrRefParam)
{
	if (NULL == pctStrRefParam)
	{   /* read cmd */
		_CMD_Response("[VER=%s]", VERSION);
	}
	else
	{   /* set parameter */
		/* read only */
		_CMD_Response("[ERR]");
	}
}

/*************************************************
* Function: _CMD_HandlerBUILD
* Description: BUILD??????
* Author: 
* Returns:
* Parameter:
* History:
*************************************************/
static void _CMD_HandlerBUILD(const StrConstRef_T* pctStrRefParam)
{
	if (NULL == pctStrRefParam)
	{   /* read cmd */
		_CMD_Response("[BUILD=%s]", BUILD);
	}
	else
	{   /* set parameter */
		/* read only */
		_CMD_Response("[ERR]");
	}
}

/*************************************************
* Function: _CMD_ClearPacketBuf
* Description: Clear CMD Packet Buffer
* Author: 
* Returns:
* Parameter:
* History:
*************************************************/
static void _CMD_ClearPacketBuf(void)
{
    /* clear packetbuf */
    memset(&s_tCmdPacketBuf, 0, sizeof(s_tCmdPacketBuf));
    s_tCmdPacketBuf.ePacketStatus = CMD_PACKET_EMPTY;
    s_tCmdPacketBuf.u32PacketLen  = 0;

}

/*************************************************
* Function: _CMD_OnRecvData
* Description: receive uart cmd data use this function
* Author: 
* Returns: return handle byte
* Parameter:
* History:
*************************************************/
uint32_t _CMD_OnRecvData(const uint8_t* pcu8Data, uint32_t u32DataLen)
{
    uint32_t u32ProcessedLen = _CMD_AssemblePacket(pcu8Data, u32DataLen);
    if (CMD_PACKET_COMPLETED == s_tCmdPacketBuf.ePacketStatus)
    {   
        /* cmd receive complete packet */
        _CMD_PacketProcess(s_tCmdPacketBuf.pu8PacketBuf, s_tCmdPacketBuf.u32PacketLen);
        /* clear packet buffer */
        _CMD_ClearPacketBuf();
        /* receive complete */    
    }
    else if (s_tCmdPacketBuf.u32PacketLen >= sizeof(s_tCmdPacketBuf.pu8PacketBuf))
    {
        /* cmd packet buffer full */
        /* discard data in buffer */
        /* TODO */
        /* clear buffer, reset status */
        _CMD_ClearPacketBuf();
        /* report warning */
        VCOM_CMD_TRACE("_CMD_NORMALOnRecvData() warning, CMD packet buffer if full!\r\n");
        /* receive complete */
    }
    else
    {
        /* continue accumulating data in the buffer */
        /* uncomplete receive data */
    }

    return u32ProcessedLen;
}

/*----------------------------------------------------------------------------*
**                             Public Function                                *
**----------------------------------------------------------------------------*/
/*************************************************
* Function:VCOM_CMD_init
* Description:init vcom_cmd 
* Author:
* Returns: 
* Parameter:
* History:
*************************************************/
void CMD_init(void)
{
    /* Initialize all variables */
    #if 0
    menset(&s_tCmdPacketBuf, 0 ,sizeof(s_tCmdPacketBuf));
    #endif

    /* clear packetbuffer */
    CMD_ClearPacketBuf();
    
}

/*************************************************
* Function:VCOM_CMD_ClearPacketBuf
* Description:clear buffer
* Author:
* Returns: 
* Parameter:
* History:
*************************************************/
void CMD_ClearPacketBuf(void)
{
    _CMD_ClearPacketBuf();
}

/*************************************************
* Function:CMD_ClearPacketBuf
* Description:init vcom_cmd 
* Author:
* Returns: 
* Parameter:
* History:
*************************************************/
void CMD_OnRecvData(const uint8_t* pcu8Data, uint32_t u32DataLen)
{
    /* processed data lenth */
    uint32_t u32ProcessedLen = 0;
    /* Loop all received data */
    while (u32ProcessedLen < u32DataLen)
    {
        u32ProcessedLen += _CMD_OnRecvData((pcu8Data + u32ProcessedLen),
                                            (u32DataLen - u32ProcessedLen));
    }    
}


/*----------------------------------------------------------------------------*
**                             Function Define                                *
**----------------------------------------------------------------------------*/





/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
// End of xxx.c

