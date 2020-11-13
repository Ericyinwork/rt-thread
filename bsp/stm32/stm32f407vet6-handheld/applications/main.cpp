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
#include "vcom_send.h"
#include "common.h"
#include "oled_gui.h"

/**---------------------------------------------------------------------------*
 **                            Debugging Flag                                 *
 **---------------------------------------------------------------------------*/
/* app debug */
//#define APP_DEBUG
#ifdef APP_DEBUG
	#define APP_TRACE rt_kprintf
#else
	#define APP_TRACE(...)
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




/*----------------------------------------------------------------------------*
**                             Local Vars                                     *
**----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
**                             Extern Function                                *
**----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
**                             Local Function                                 *
**----------------------------------------------------------------------------*/



void app_init(void)
{
	vcom_module_init();
}
/*----------------------------------------------------------------------------*
**                             Public Function                                *
**----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
**                             Function Define                                *
**----------------------------------------------------------------------------*/
/*************************************************
* Function:
* Description:
* Author:
* Returns: 
* Parameter:
* History:
*************************************************/
int main(void)
{
	int main_ret = 0;
	rt_kprintf("test!\r\n");

	/* app module init */
	//app_init();



	/* into loop */
	while (1)
	{	
		//vcom_event_recved();		
			
	}	
	
	return main_ret;
}



/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
// End of xxx.c

