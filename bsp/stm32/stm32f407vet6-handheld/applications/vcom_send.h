/*
 * @Author: your name
 * @Date: 2020-11-10 11:38:46
 * @LastEditTime: 2020-11-10 18:47:49
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \stm32f407vet6-handheld\applications\device_define.h
 */
/****************************************************************************
 *
 * File Name:
 *  
 * Author:
 *  
 * Date:
 * 
 * Descriptions:
 * 
 *
 ******************************************************************************/
#ifndef __DEVICE_DEFINE_H__
#define __DEVICE_DEFINE_H__
/*----------------------------------------------------------------------------*
**                             Dependencies                                   *
**----------------------------------------------------------------------------*/
/* <Header file of standard Library> */
#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include <stdint.h>
#include <stdbool.h>
/*----------------------------------------------------------------------------*/
/* "Header file of nonstandard Library" */
#include "cyclequeue.h"
#include "vcom_cmd.h"

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


/*----------------------------------------------------------------------------*
**                             Data Structures                                *
**----------------------------------------------------------------------------*/
/* Communication mode */
typedef enum
{
    /* vcom priority */
    COM_MODE_VCOM = 0,
    /* BT priority */
    COM_MODE_BT,
}COM_MODE_E;


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
/*************************************************
* Function: com_send_data
* Description: config (BT/VCOM) send data
* Author: 
* Returns: return byte
* Parameter:
* History:
*************************************************/
uint32_t com_send_data(const uint8_t* data, uint32_t len);

/*************************************************
* Function: set_com_mode
* Description: config communication mode(BT优先/VCOM优先)
* Author: 
* Returns: 
* Parameter:
* History:
*************************************************/
bool set_com_mode(COM_MODE_E mode);

/*************************************************
* Function: get_com_mode
* Description: read communication mode(BT优先/VCOM优先)
* Author: 
* Returns: 
* Parameter:
* History:
*************************************************/
COM_MODE_E get_com_mode(void);

/*************************************************
* Function: vcom_dev_find
* Description: 
* Author: 
* Returns: 
* Parameter:
* History:
*************************************************/
uint32_t vcom_dev_find(void);

/*************************************************
* Function: get_com_device
* Description: communication device now(BT/VCOM)
* Author:
* Returns: 
* Parameter:
* History:
*************************************************/
rt_device_t get_com_device(void);
	
/*************************************************
* Function: vcom_send_data
* Description: send data in vcom
* Author: 
* Returns: return the output byte
* Parameter:
* History:
*************************************************/
uint32_t vcom_send_data(const uint8_t* data, uint32_t len);

/*************************************************
* Function: bt_send_data
* Description: send data in BT
* Author: 
* Returns: return the output data
* Parameter:
* History:
*************************************************/
//uint32_t bt_send_data(const uint8_t* data, uint32_t len);

/*************************************************
* Function: vcom_module_init
* Description: 
* Author: 
* Returns: 
* Parameter:
* History:
*************************************************/
void vcom_module_init(void);

/*************************************************
* Function: event_recved
* Description: 
* Author: 
* Returns: 
* Parameter:
* History:
*************************************************/
void vcom_event_recved(void);


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
// End of xxx.c
#endif  
// End of xxx.H
