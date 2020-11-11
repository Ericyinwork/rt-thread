/*
 * @Author: your name
 * @Date: 2020-11-10 11:38:46
 * @LastEditTime: 2020-11-10 18:47:57
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

/*----------------------------------------------------------------------------*
**                             Dependencies                                   *
**----------------------------------------------------------------------------*/
/* <Header file of standard Library> */
/*----------------------------------------------------------------------------*/
/* "Header file of nonstandard Library" */
#include "vcom_send.h"


/**---------------------------------------------------------------------------*
 **                            Debugging Flag                                 *
 **---------------------------------------------------------------------------*/
//define VCOM_SEND_DEBUG
#ifdef VCOM_SEND_DEBUG
#define VCOM_SEND_TRACE rt_kprinf
#else   
#define VCOM_SEND_TRACE(...)
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

/*----------------------------------------------------------------------------*/





/*----------------------------------------------------------------------------*
**                             Data Structures                                *
**----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
**                             Local Vars                                     *
**----------------------------------------------------------------------------*/
/* define event */
#define SENSOR_EVENT_RX_IND 0x00000001
#define VCOM_EVENT_RX_IND   0x00000002
#define VCOM_EVENT_TX_DONE  0x00000004
/* event for application */
static rt_event_t vcom_event = RT_NULL;
	/* vcom device */
static rt_device_t vcom_dev =RT_NULL;
/*----------------------------------------------------------------------------*/
/* vcom send buffer size */
#define VCOM_SEND_BUF_SIZE (1024)
/* vcom device name */
#define VCOM_DEVICE_NAME "vcom"
/* default vcom priority */
static COM_MODE_E com_mode = COM_MODE_VCOM;

/* vcom receive buffer */
static uint8_t vcom_data_buf[128] = {0};
/* vcom data send cyclequeue */
static CycleQueue_T s_tVcomSendQ = {NULL};
/* vcom send cyclequeue buffer */
static uint8_t s_pu8VcomSendQueBuf[VCOM_SEND_BUF_SIZE * 4] = {0};
/* vcon send buffer */
static uint8_t s_pu8VcomSendBuf[VCOM_SEND_BUF_SIZE] = {0};
/* vcom send status */
static bool s_bVcomSending = false;
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
* Function:vcom_rx_ind
* Description:
* Author:
* Returns: 
* Parameter:
* History:
*************************************************/
rt_err_t vcom_rx_ind(rt_device_t dev, rt_size_t size)
{
    if (size > 0)
    {
        rt_event_send(vcom_event, VCOM_EVENT_RX_IND);
    }

    return RT_EOK;    
}

/*************************************************
* Function:vcom_rx_ind
* Description:
* Author:
* Returns: 
* Parameter:
* History:
*************************************************/
rt_err_t vcom_tx_done(rt_device_t dev, void *buffer)
{
    rt_event_send(vcom_event, VCOM_EVENT_TX_DONE);

    return RT_EOK;
}

/*************************************************
* Function: com_send_data
* Description: config (BT/VCOM) send data
* Author: 
* Returns: return byte
* Parameter:
* History:
*************************************************/
uint32_t com_send_data(const uint8_t* data, uint32_t len)
{
    uint32_t sent_len = 0;
    switch (com_mode)
    {
    case COM_MODE_VCOM:
        {
            /* vcom priority */
            sent_len = vcom_send_data(data, len);
            if (0 == sent_len)
            {
                /* sent fail */
                /* rety send data in BT */
              //  sent_len = bt_send_data(data, len);
            }
            break;
        }
    case COM_MODE_BT:
    {
        /* BT priority */
      //  sent_len = bt_send_data(data, len);
        if (0 == sent_len)
        {
            /* sent fail */
            /* rety send data in vcom */
        }
        break;        
    }
    
    default:
        break;
    }

    return sent_len;
}

/*************************************************
* Function: set_com_mode
* Description: config communication mode(BT优先/VCOM优先)
* Author: 
* Returns: 
* Parameter:
* History:
*************************************************/
bool set_com_mode(COM_MODE_E mode)
{
    VCOM_SEND_TRACE("set_com_mode() mode=%d\r\n", mode);
    if ((COM_MODE_BT != mode)
        && (COM_MODE_VCOM != mode))
    {
        /* Invalid mode */
        VCOM_SEND_TRACE("set_com_mode() failed , invaild mode(%d)!\r\n", mode);
        return false;
    }

    /* set mode */
    com_mode = mode;

    return true;
}

/*************************************************
* Function: get_com_mode
* Description: read communication mode(BT优先/VCOM优先)
* Author: 
* Returns: 
* Parameter:
* History:
*************************************************/
COM_MODE_E get_com_mode(void)
{
    return com_mode;
}

/*************************************************
* Function: vcom_dev_find
* Description: 
* Author: 
* Returns: 
* Parameter:
* History:
*************************************************/
uint32_t vcom_dev_find(void)
{	
	uint32_t ret = 0;
	/* find vcom device */
	vcom_dev = rt_device_find(VCOM_DEVICE_NAME);
	if (RT_NULL == vcom_dev)
	{
		VCOM_SEND_TRACE("main() call rt_device_find(%s) failed!\r\n", VCOM_DEVICE_NAME);
		// main_ret = -RT_ERROR;
		// goto _END;
	}
	else
	{
		ret = rt_device_open(vcom_dev, RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX | RT_DEVICE_FLAG_DMA_TX);
		if (RT_EOK != ret)	
		{
			VCOM_SEND_TRACE("main() call rt_device_open(vcom_dev) failed, error(%d)!\r\n", ret);
			// main_ret = -RT_ERROR;
			// goto _END;
		}
		else
		{
			ret = rt_device_set_rx_indicate(vcom_dev, vcom_rx_ind);
			if (RT_EOK != ret)
			{
                VCOM_SEND_TRACE("main() call rt_device_set_rx_indicate(vcom_dev) failed, error(%d)!\r\n", ret);
                //main_ret = ret;
                //goto _END;
            }
            
            ret = rt_device_set_tx_complete(vcom_dev, vcom_tx_done);
            if (RT_EOK != ret)
            {
                VCOM_SEND_TRACE("main() call rt_device_set_tx_complete(vcom_dev) failed, error(%d)!\r\n", ret);
                //main_ret = ret;
                //goto _END;
            }
		}		
	}
	
	return ret;
}

/*************************************************
* Function: get_com_device
* Description: communication device now(BT/VCOM)
* Author:
* Returns: 
* Parameter:
* History:
*************************************************/
rt_device_t get_com_device(void)
{
    rt_device_t device =RT_NULL;
    switch (com_mode)
    {
    case COM_MODE_VCOM:
        {
            device = vcom_dev;
            break;
        }
    case COM_MODE_BT:
    {
       // device = bt_dev;
        break;
    }
    
    default:
        break;
    }

    return device;
}
	
/*************************************************
* Function: vcom_send_data
* Description: send data in vcom
* Author: 
* Returns: return the output byte
* Parameter:
* History:
*************************************************/
uint32_t vcom_send_data(const uint8_t* data, uint32_t len)
{
    if (RT_NULL == vcom_dev)
    {
        return 0;
    }
    uint32_t u32SendLen = CycleQueue_Insert(&s_tVcomSendQ, data, len);
    if (s_bVcomSending)
    {
        /* data sending */
        /* automatic sending data in buffer */
    }
    else
    {
        /* start sending */
        uint8_t* pu8SendBuf = s_pu8VcomSendBuf;
        uint32_t u32DataLen = CycleQueue_Delete(&s_tVcomSendQ, pu8SendBuf, VCOM_SEND_BUF_SIZE, NULL);
        if (u32DataLen > 0)
        {
            /* set sending status */
            s_bVcomSending = true;
            /* Request to send data in buffer */
            rt_device_write(vcom_dev, 0 ,pu8SendBuf, (rt_size_t)u32DataLen);
        }        
    }

    return u32SendLen;    
}
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
* Function: 
* Description: 
* Author: 
* Returns: 
* Parameter:
* History:
*************************************************/
void vcom_module_init(void)
{
	/* create vcom cycleque */
	CycleQueue_Create(&s_tVcomSendQ, s_pu8VcomSendQueBuf, sizeof(s_pu8VcomSendQueBuf));
	/* init cmd module */
	CMD_init();
	vcom_dev_find();
}

/*************************************************
* Function: event_recved
* Description: 
* Author: 
* Returns: 
* Parameter:
* History:
*************************************************/
void vcom_event_recved(void)
{
		/* create event */
	vcom_event = rt_event_create("app_event", RT_IPC_FLAG_FIFO);
	if (RT_NULL == vcom_event)
	{
		VCOM_SEND_TRACE("create app event failed!\r\n");
		//main_ret = -RT_ERROR;
		 goto _END;
	}	

	rt_err_t ret = RT_EOK;	
		rt_uint32_t event_recved = 0;
		ret = rt_event_recv(vcom_event, (VCOM_EVENT_RX_IND | VCOM_EVENT_TX_DONE ),
						  (RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR),
						  RT_WAITING_FOREVER, &event_recved);
		
		if (RT_EOK != ret)	
		{
			VCOM_SEND_TRACE("receive event failed(%d)!\r\n", ret);
	//		main_ret = ret;
	//		break;
		}

		if (event_recved & VCOM_EVENT_RX_IND)
		{	/* receive vcom data */
			if (vcom_dev == RT_NULL)
			{
		//		continue;
			}
			/* receive all data */
			while (1)
			{
				rt_size_t read_len = rt_device_read(vcom_dev, 0, vcom_data_buf, sizeof(vcom_data_buf));
				if (read_len > 0)
				{
					/* receive data trace */
					VCOM_SEND_TRACE("vcom read_len=%d\r\n", read_len);
					/* data into cmd module */
					CMD_OnRecvData(vcom_data_buf, (uint32_t)read_len);
				}
				else
				{
					/* data receive complete */
					break;
				}				
			}			
		}
	_END:
	/* release resource */
	if (RT_NULL != vcom_event)
	{
		rt_event_delete(vcom_event);
		vcom_event = RT_NULL;
	}
	if (RT_NULL != vcom_dev)
	{
		rt_device_close(vcom_dev);
		vcom_dev = RT_NULL;
		}			
}

/**---------------------------------------------------------------------------*
 **                           Compiler Flag                                   *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
// End of xxx.c

