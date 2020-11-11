/*
 * @Author: your name
 * @Date: 2020-10-22 11:54:38
 * @LastEditTime: 2020-11-10 15:25:04
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \stm32f407vet6-handheld\applications\bosh_bme280.c
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

#include <bosh_bme280.h>

/**---------------------------------------------------------------------------*
 **                            Debugging Flag                                 *
 **---------------------------------------------------------------------------*/
/* BME280_DEBUG */
//#define BME280_DEBUG
#ifdef BME280_DEBUG
    #define BME280_TRACE rt_kprintf
#else
    #define BME280_TRACE(...)
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
**                             Data Structures                                *
**----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
**                             Local Vars                                     *
**----------------------------------------------------------------------------*/
// /* bosh bme280sensor i2c device name */
// #define BOSH_BME280_I2C_BUS_NAME    "i2c1" 
// /* i2c bus device handle */
// struct rt_i2c_bus_device *i2c_bus;
// i2c_bus = (struct rt_i2c_bus_device *)rt_device_find(BOSH_BME280_I2C_BUS_NAME);

/* bme280 sensor device */
static rt_device_t temp_bme280_dev = RT_NULL; // temp device
static rt_device_t humi_bme280_dev = RT_NULL; // humi device
static rt_device_t baro_bme280_dev = RT_NULL; // baro device

/*----------------------------------------------------------------------------*
**                             Extern Function                                *
**----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
**                             Local Function                                 *
**----------------------------------------------------------------------------*/
/*************************************************
* Function:bme280_init
* Description:
* Author:
* Returns: 
* Parameter:
* History:
*************************************************/
bool bosh_bme280_init(void)
{   /* SDO PIN LOW use the default address */
    rt_pin_mode(BME280_SDO_PIN,PIN_MODE_OUTPUT);
    rt_pin_write(BME280_SDO_PIN,PIN_LOW);
 
    /* temperature */
    temp_bme280_dev = rt_device_find("temp_bme280");// temperature device find
    if(RT_NULL == temp_bme280_dev)
    {
        BME280_TRACE(" bme280_init() call rt_device_find(temp_bme280) failed!\r\n ");
        return false;
    }
    rt_err_t ret = rt_device_open(temp_bme280_dev,RT_DEVICE_FLAG_RDONLY);// temperature device open
    if (RT_EOK != ret)
    {
        BME280_TRACE(" bme280_init() call rt_device_open(temp_bme280) failed!\r\n ");
        return false;
    }

    /* relative humidity */
    humi_bme280_dev = rt_device_find("humi_bme280");// humidity device find
    if(RT_NULL == humi_bme280_dev)
    {
        BME280_TRACE(" bme280_init() call rt_device_find(humi_bme280) failed!\r\n ");
        return false;
    }
    ret = rt_device_open(humi_bme280_dev,RT_DEVICE_FLAG_RDONLY);// humidity device open
    if (RT_EOK != ret)
    {   
        BME280_TRACE(" bme280_init() call rt_device_open(humi_bme280) failed!\r\n ");
        return false;
    }

    /* barometer */
    baro_bme280_dev = rt_device_find("baro_bme280"); // barometer device find
    if (RT_NULL == baro_bme280_dev)     
    {
        BME280_TRACE(" bme280_init() call rt_device_find(baro_bme280) failed!\r\n ");
        return false;
    }
    ret = rt_device_open(baro_bme280_dev,RT_DEVICE_FLAG_RDONLY);// barometer device open
    if (RT_EOK != ret)
    {
        BME280_TRACE(" bme280_init() call rt_device_open(baro_bme280) failed!\r\n ");
        return false;
    } 

    return true; 
}

/*************************************************
* Function:bme280_init
* Description:
* Author:
* Returns: 
* Parameter:
* History:
*************************************************/
void bosh_bme280_deinit(void)
{
    BME280_TRACE(" bme280_deinit()\r\n ");

    /* temperature */
    if (RT_NULL != temp_bme280_dev)     
    {
        rt_device_close(temp_bme280_dev); //temperature device close
        temp_bme280_dev = RT_NULL;
    }

    /* relative humidity */
    if (RT_NULL != humi_bme280_dev)
    {
        rt_device_close(humi_bme280_dev); //humidity device close
        humi_bme280_dev =RT_NULL;
    }

    /* barometer */
    if (RT_NULL != baro_bme280_dev)
    {
        rt_device_close(baro_bme280_dev); //barometer device close
        baro_bme280_dev =RT_NULL;
    }
    
}

/*************************************************
* Function:bme280_get_temp
* Description:
* Author:
* Returns: (float) sensor_data.data.temp
* Parameter:
* History:
*************************************************/
float bosh_bme280_get_temp(void)
{
    /* temperature data get failed debug*/
    if (temp_bme280_dev == RT_NULL)
    {
        BME280_TRACE(" bme280_get_temp() failed, temp_bme280_dev is null ! \r\n ");
        return 0.0f;
    }
    struct rt_sensor_data sensor_data = { 0 };

    /* read the temperature date size 1 */
    rt_size_t read_len = rt_device_read(temp_bme280_dev,0,&sensor_data,1);
    if (1 != read_len) //check size 1 ?
    {
        return 0.0f;
    }

    return (float)(sensor_data.data.temp) / 10.0f; //return the temperature sensor date    
}

/*************************************************
* Function:bme280_get_humi
* Description:
* Author:
* Returns: 
* Parameter:
* History:
*************************************************/
float bosh_bme280_get_humi(void)
{
    /* ralative humidity data get failed debug*/
    if (humi_bme280_dev == RT_NULL)
    {
        BME280_TRACE(" bme280_get_humi() failed, humi_bme280_dev is null ! \r\n ");
        return 0.0f;
    }
    struct rt_sensor_data sensor_data = { 0 };

    /* read the temperature date size 1 */
    rt_size_t read_len = rt_device_read(humi_bme280_dev,0,&sensor_data,1);
    if (1 != read_len) //check size 1 ?
    {
        return 0.0f;
    }

    return (float)(sensor_data.data.humi) / 10.0f; //return the relative humidity sensor date    
}

/*************************************************
* Function:bme280_get_baro
* Description:
* Author:
* Returns: 
* Parameter:
* History:
*************************************************/
float bosh_bme280_get_baro(void)
{
    /* baromoter data get failed debug*/
    if (baro_bme280_dev == RT_NULL)
    {
        BME280_TRACE(" bme280_get_baro() failed, baro_bme280_dev is null ! \r\n ");
        return 0.0f;
    }
    struct rt_sensor_data sensor_data = { 0 };

    /* read the temperature date size 1 */
    rt_size_t read_len = rt_device_read(baro_bme280_dev,0,&sensor_data,1);
    if (1 != read_len) //check size 1 ?
    {
        return 0.0f;
    }

    return (float)(sensor_data.data.baro) / 10.0f; //return the barometer sensor date    
}

/*----------------------------------------------------------------------------*
**                             Public Function                                *
**----------------------------------------------------------------------------*/
/*************************************************
* Function:
* Description:
* Author:
* Returns: 
* Parameter:
* History:
*************************************************/




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




/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
// End of xxx.c

