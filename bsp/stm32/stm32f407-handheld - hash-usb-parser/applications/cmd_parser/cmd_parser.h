/*
 * File Name:cmd_parser.h
 * Descriptions: uart and usb command parser /ARM/IAR/GCC compiler
 * Change Logs:
 * Date             Author       Notes
 * 2020-11-23       Eric      first implementation
*/

#ifndef _CMD_PARSER_H_
#define _CMD_PARSER_H_

/*----------------------------------------------------------------------------*
**                             Dependencies                                   *
**----------------------------------------------------------------------------*/
/* <Header file of standard Library> */
#include <stdio.h>
#include <rtthread.h>
///*----------------------------------------------------------------------------*/
///* "Header file of nonstandard Library" */
//#include "strref.h"

/**---------------------------------------------------------------------------*
 **                            Debugging Flag                                 *
 **---------------------------------------------------------------------------*/

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
#define CMD_HASH 0x0b72ab26 //my name and my birthday joaat result

/*----------------------------------------------------------------------------*/
#if defined(__CC_ARM) || defined(__CLANG_ARM)   /* ARM Compiler */
    #define SECTION(x)                  __attribute__((section(x)))
    #define CMD_USED                    __attribute__((used))

#elif defined (__IAR_SYSTEMS_ICC__)             /* IAR Compiler */
    #define SECTION(x)                  @ x
    #define CMD_USED                    __root
    
#elif defined (__GNUC__)                        /* GNU GCC Compiler */
    #define SECTION(x)                  __attribute__((section(x)))
    #define CMD_USED                    __attribute__((used))
#else
    #error "not supported tool chain..."
#endif
/*----------------------------------------------------------------------------*/
#define CMD_PACKET_MAX_LEN      (64)
#define CMD_TITEL_MAX_LEN       (64)
#define CMD_CONTENT_MAX_LEN     (64)

/*----------------------------------------------------------------------------*/
typedef void (*cmd_handler)(void);
typedef struct cmd {
    const char*     cmd;
    const char*     desc;
    unsigned int    hash;
    cmd_handler     handler;
} cmd_t;

/*----------------------------------------------------------------------------*/
typedef enum
{
    CMD_PACKET_EMPTY = 0, // empty status
    CMD_PACKET_HEAD, // receive head
    CMD_PACKET_COMPLETED // receive complete
} CMD_PACKET_BUF_STATUS_E;

/*----------------------------------------------------------------------------*/
typedef struct
{
    CMD_PACKET_BUF_STATUS_E ePacketStatus;
    uint32_t u32PacketLen;
    uint8_t pu8PacketBuf[CMD_PACKET_MAX_LEN];
} CmdPacketBuf_T;

#define REGISTER_CMD(cmd, handler,desc)                         \
    const char _register_##cmd##_cmd[] = #cmd;                  \
    const char _register_##cmd##_desc[] = #desc;                \
    CMD_USED cmd_t _register_##cmd SECTION("CMDS")=             \
    {                                                           \
        _register_##cmd##_cmd,                                  \
        _register_##cmd##_desc,                                 \
        (unsigned int)CMD_HASH,                                 \
        (cmd_handler)&handler                                   \
    };

/*----------------------------------------------------------------------------*
**                             Data Structures                                *
**----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
**                             Local Vars                                     *
**----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
**                             Extern Function                                *
**----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
**                             Local Function                                 *
**----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
**                             Public Function                                *
**----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
**                             Function Define                                *
**----------------------------------------------------------------------------*/
void cmd_init(void);
void cmd_parsing(char *str);
/* receive data and lenth*/
void CMD_OnRecvData(const uint8_t* pcu8Data, uint32_t u32DataLen);
void CMD_ClearPacketBuf(void);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
//End of xxx.c
#endif  
// End of xxx.H
