// File Name: ws2812_interface.h
// Author: kukukukuku~
// Date: 2025-03-06

#ifndef __WS2812_INTERFACE_H
#define __WS2812_INTERFACE_H

#include "./WS2812/inc/ws2812_macros.h"

//WS2812 RGB数据
typedef union WS2812_RGB_T
{
    uint32_t data;
    struct
    {
        uint8_t blue;
        uint8_t green;
        uint8_t red;
    };
    
}WS2812_RGB_T, *pWS2812_RGB_T;

//WS2812信号时序对象
typedef struct WS2812_TIMING_OBJ_T
{
    spi_type *spi;
    uint8_t high;
    uint8_t low;
    uint8_t reset;
}WS2812_TIMING_OBJ_T, *pWS2812_TIMING_OBJ_T;

//WS2812对象
typedef struct WS2812_OBJ_T
{
    WS2812_TIMING_OBJ_T *timing;
    WS2812_RGB_T rgb[WS2812_NUM];
}WS2812_OBJ_T, *pWS2812_OBJ_T;


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
void ws2812_timing_obj_init(WS2812_TIMING_OBJ_T * const ws2812TimingObj, spi_type * const spi, const uint8_t high, const uint8_t low, const uint8_t reset);


/**
 * @brief   WS2812对象初始化
 * @param   ws2812Obj:          WS2812对象
 * @param   ws2812TimingObj:    WS2812信号时序对象
 * @retval  None
 * @note    None
*/
void ws2812_obj_init(WS2812_OBJ_T * const ws2812Obj, WS2812_TIMING_OBJ_T * const ws2812TimingObj);


/**
 * @brief   WS2812设置颜色
 * @param   ws2812Obj:  WS2812对象
 * @param   rgbData:    WS2812颜色数据
 * @param   index:      WS2812灯珠索引
 * @retval  None
 * @note    None
*/
void ws2812_set_rgb(WS2812_OBJ_T * const ws2812Obj, const uint32_t rgbData, const uint16_t index);


/**
 * @brief   WS2812设置红色
 * @param   ws2812Obj:  WS2812对象
 * @param   rData:      WS2812红色数据
 * @param   index:      WS2812灯珠索引
 * @retval  None
 * @note    None
*/
void ws2812_set_red(WS2812_OBJ_T * const ws2812Obj, const uint8_t rData, const uint16_t index);


/**
 * @brief   WS2812设置绿色
 * @param   ws2812Obj:  WS2812对象
 * @param   gData:      WS2812绿色数据
 * @param   index:      WS2812灯珠索引
 * @retval  None
 * @note    None
*/
void ws2812_set_green(WS2812_OBJ_T * const ws2812Obj, const uint8_t gData, const uint16_t index);


/**
 * @brief   WS2812设置蓝色
 * @param   ws2812Obj:  WS2812对象
 * @param   bData:      WS2812蓝色数据
 * @param   index:      WS2812灯珠索引
 * @retval  None
 * @note    None
*/
void ws2812_set_blue(WS2812_OBJ_T * const ws2812Obj, const uint8_t bData, const uint16_t index);


/**
 * @brief   WS2812更新显示
 * @param   ws2812Obj:  WS2812对象
 * @retval  None
 * @note    None
*/
void ws2812_refresh(WS2812_OBJ_T * const ws2812Obj);

#endif //__WS2812_INTERFACE_H
