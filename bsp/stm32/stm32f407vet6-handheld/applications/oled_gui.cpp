/*
 * @Author: your name
 * @Date: 2020-10-22 11:54:38
 * @LastEditTime: 2020-11-10 17:38:43
 * @LastEditors: your name
 * @Description: In User Settings Edit
 * @FilePath: \stm32f407vet6-handheld\applications\oled_gui.cpp
 */
/****************************************************************************
 *
 * File Name��
 *  
 * Author��
 *  
 * Date��
 * 
 * Descriptions:
 * 
 *
 ******************************************************************************/
/*----------------------------------------------------------------------------*
**                             Dependencies                                   *
**----------------------------------------------------------------------------*/
#include <U8g2lib.h>
#include <rtthread.h>
#include "drv_spi.h"
#include "oled_gui.h"
#include "board_config.h"




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
static U8G2_SH1107_PIMORONI_128X128_F_4W_HW_SPI u8g2(U8G2_R0,/* cs=*/ OLED_SPI_PIN_CS, /* dc=*/ OLED_SPI_PIN_DC, /* reset=*/ OLED_SPI_PIN_RES);


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
/*************************************************
* Function:rt_hw_spi_oled_init
* Description: oled spi init \add the 13.5V power on\ set pin higt active
* Author:Eric
* Returns: 
* Parameter:spi1 cs pin PA3 \ 13.5V power pin high active
* History:
*************************************************/
static int rt_hw_spi_oled_init(void)
{
	/* turn on the OLED 13.5V Power */
    rt_pin_mode(OLED_POWER_PIN, PIN_MODE_OUTPUT);
    rt_pin_write(OLED_POWER_PIN, PIN_HIGH);
    
	/* spi1 cs pin PA3 attach device */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    rt_hw_spi_device_attach("spi1", "spi10", GPIOA, GPIO_PIN_3);

    return RT_EOK;
}
INIT_COMPONENT_EXPORT(rt_hw_spi_oled_init);

/*************************************************
* Function:oled_gui_init
* Description: 
* Author:Eric
* Returns: 
* Parameter:
* History:
*************************************************/
static int oled_gui_init(void)
{
    
}
/*************************************************
* Function:oled_gui_draw_battry
* Description: 
* Author:Eric
* Returns: 
* Parameter:
* History:
*************************************************/
static int oled_gui_draw_battry(void)
{

}

/*************************************************
* Function:oled_gui_draw_device_logo
* Description: 
* Author:Eric
* Returns: 
* Parameter:
* History:
*************************************************/
static int oled_gui_draw_device_logo(void)
{

}

/*************************************************
* Function:oled_gui_draw_
* Description: 
* Author:Eric
* Returns: 
* Parameter:
* History:
*************************************************/

/*----------------------------------------------------------------------------*
**                             Public Function                                *
**----------------------------------------------------------------------------*/

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

