// File Name: ws2812_interface.c
// Author: kukukukuku~
// Date: 2025-03-06

#include "stddef.h"
#include "string.h"
#include "./WS2812/inc/ws2812_interface.h"

/**
 * @brief   WS2812信号时序对象初始化
 * @param   ws2812TimingObj:    WS2812信号时序对象
 * @param   spi:                SPI端口
 * @param   high:               高电平时序
 * @param   low:                低电平时序
 * @param   reset:              复位时序
 * @retval  None
 * @note    None
*/
void ws2812_timing_obj_init(WS2812_TIMING_OBJ_T * const ws2812TimingObj, spi_type * const spi, const uint8_t high, const uint8_t low, const uint8_t reset)
{
    if(ws2812TimingObj == NULL)
        return;
    
    ws2812TimingObj->spi   = spi;
    ws2812TimingObj->high  = high;
    ws2812TimingObj->low   = low;
    ws2812TimingObj->reset = reset;
}


/**
 * @brief   WS2812对象初始化
 * @param   ws2812Obj:          WS2812对象
 * @param   ws2812TimingObj:    WS2812信号时序对象
 * @retval  None
 * @note    None
*/
void ws2812_obj_init(WS2812_OBJ_T * const ws2812Obj, WS2812_TIMING_OBJ_T * const ws2812TimingObj)
{
    if(ws2812Obj == NULL || ws2812TimingObj == NULL)
        return;
    
    memset(ws2812Obj, 0, sizeof(WS2812_OBJ_T));
    ws2812Obj->timing = ws2812TimingObj;
}


/**
 * @brief   向WS2812发送一个bit
 * @param   ws2812Obj:  WS2812对象
 * @param   bit:        要发送的bit
 * @retval  None
 * @note    静态
*/
static void send_bit_to_ws2812(WS2812_OBJ_T * const ws2812Obj, const uint8_t bit)
{
    if(ws2812Obj == NULL || ws2812Obj->timing == NULL)
        return;
    
    while(!spi_i2s_flag_get(ws2812Obj->timing->spi, SPI_I2S_TDBE_FLAG));
    spi_i2s_data_transmit(ws2812Obj->timing->spi, bit ? ws2812Obj->timing->high : ws2812Obj->timing->low);
    while(!spi_i2s_flag_get(ws2812Obj->timing->spi, SPI_I2S_TDBE_FLAG));
}


/**
 * @brief   向WS2812发送一个byte
 * @param   ws2812Obj:  WS2812对象
 * @param   byte:       要发送的byte
 * @retval  None
 * @note    静态
*/
static void send_byte_to_ws2812(WS2812_OBJ_T * const ws2812Obj, const uint8_t byte)
{
    if(ws2812Obj == NULL || ws2812Obj->timing == NULL)
        return;
    
    for(uint8_t i = 0; i < 8; i++)
    {
        send_bit_to_ws2812(ws2812Obj, byte << i & 0x80);
    }
}


/**
 * @brief   向WS2812发送复位信号
 * @param   ws2812Obj:  WS2812对象
 * @retval  None
 * @note    静态
*/
static void send_reset_to_ws2812(WS2812_OBJ_T * const ws2812Obj)
{
    if(ws2812Obj == NULL || ws2812Obj->timing == NULL)
        return;
    
    for(uint16_t i = 0; i < 300; i++)
    {
        while(!spi_i2s_flag_get(ws2812Obj->timing->spi, SPI_I2S_TDBE_FLAG));
        spi_i2s_data_transmit(ws2812Obj->timing->spi, ws2812Obj->timing->reset);
        while(!spi_i2s_flag_get(ws2812Obj->timing->spi, SPI_I2S_TDBE_FLAG));
    }
}


/**
 * @brief   WS2812设置颜色
 * @param   ws2812Obj:  WS2812对象
 * @param   rgbData:    WS2812颜色数据
 * @param   index:      WS2812灯珠索引
 * @retval  None
 * @note    None
*/
void ws2812_set_rgb(WS2812_OBJ_T * const ws2812Obj, const uint32_t rgbData, const uint16_t index)
{
    if(ws2812Obj == NULL || ws2812Obj->timing == NULL || index >= sizeof(ws2812Obj->rgb) / sizeof(ws2812Obj->rgb[0]))
        return;

    ws2812Obj->rgb[index].data = rgbData;
}


/**
 * @brief   WS2812设置红色
 * @param   ws2812Obj:  WS2812对象
 * @param   rData:      WS2812红色数据
 * @param   index:      WS2812灯珠索引
 * @retval  None
 * @note    None
*/
void ws2812_set_red(WS2812_OBJ_T * const ws2812Obj, const uint8_t rData, const uint16_t index)
{
    if(ws2812Obj == NULL || ws2812Obj->timing == NULL || index >= sizeof(ws2812Obj->rgb) / sizeof(ws2812Obj->rgb[0]))
        return;

    ws2812Obj->rgb[index].red = rData;
}


/**
 * @brief   WS2812设置绿色
 * @param   ws2812Obj:  WS2812对象
 * @param   gData:      WS2812绿色数据
 * @param   index:      WS2812灯珠索引
 * @retval  None
 * @note    None
*/
void ws2812_set_green(WS2812_OBJ_T * const ws2812Obj, const uint8_t gData, const uint16_t index)
{
    if(ws2812Obj == NULL || ws2812Obj->timing == NULL || index >= sizeof(ws2812Obj->rgb) / sizeof(ws2812Obj->rgb[0]))
        return;
    
    ws2812Obj->rgb[index].green = gData;
}


/**
 * @brief   WS2812设置蓝色
 * @param   ws2812Obj:  WS2812对象
 * @param   bData:      WS2812蓝色数据
 * @param   index:      WS2812灯珠索引
 * @retval  None
 * @note    None
*/
void ws2812_set_blue(WS2812_OBJ_T * const ws2812Obj, const uint8_t bData, const uint16_t index)
{
    if(ws2812Obj == NULL || ws2812Obj->timing == NULL || index >= sizeof(ws2812Obj->rgb) / sizeof(ws2812Obj->rgb[0]))
        return;
    
    ws2812Obj->rgb[index].blue = bData;
}


/**
 * @brief   WS2812更新显示
 * @param   ws2812Obj:  WS2812对象
 * @retval  None
 * @note    None
*/
void ws2812_refresh(WS2812_OBJ_T * const ws2812Obj)
{
    if(ws2812Obj == NULL || ws2812Obj->timing == NULL)
        return;

    for(uint16_t i = 0; i < sizeof(ws2812Obj->rgb) / sizeof(ws2812Obj->rgb[0]); i++)
    {
        send_byte_to_ws2812(ws2812Obj, ws2812Obj->rgb[i].green);
        send_byte_to_ws2812(ws2812Obj, ws2812Obj->rgb[i].red);
        send_byte_to_ws2812(ws2812Obj, ws2812Obj->rgb[i].blue);
    }
    
    send_reset_to_ws2812(ws2812Obj);
}
