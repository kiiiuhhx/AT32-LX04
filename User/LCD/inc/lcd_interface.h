// File Name: lcd_interface.h
// Author: kukukukuku~
// Date: 2025-03-05

#ifndef __LCD_INTERFACE_H
#define __LCD_INTERFACE_H

#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"

#include "./LCD/inc/lcd_macros.h"

//LCD显示方向
typedef enum LCD_DIR_T
{
    LCD_DIR_NORMAL = 0, //正常显示
    LCD_DIR_REVERSE,    //反转显示
}LCD_DIR_T, *pLCD_DIR_T;

//LCD英文字体大小
typedef enum LCD_EN_FONT_SIZE_T
{
    LCD_EN_FONT_SIZE_8X16 = 0,
    LCD_EN_FONT_SIZE_16X16,
    LCD_EN_FONT_SIZE_16X24,
    LCD_EN_FONT_SIZE_16X32,
    LCD_EN_FONT_SIZE_24X32,
}LCD_EN_FONT_SIZE_T, *pLCD_EN_FONT_SIZE_T;

//LCD GPIO对象
typedef struct LCD_GPIO_OBJ_T
{
    gpio_type *mosiPort;
    gpio_type *sckPort;
    gpio_type *csPort;
    gpio_type *dcPort;
    gpio_type *resPort;
    gpio_type *blkPort;
    uint32_t mosiPin;
    uint32_t sckPin;
    uint32_t csPin;
    uint32_t dcPin;
    uint32_t resPin;
    uint32_t blkPin;
}LCD_GPIO_OBJ_T, *pLCD_GPIO_OBJ_T;

//LCD对象
typedef struct LCD_OBJ_T
{
    LCD_GPIO_OBJ_T *gpioObj;
    spi_type *spi;
    SemaphoreHandle_t updateSemaphoreHandle;
    uint16_t width;
    uint16_t height;
    LCD_DIR_T dir;
}LCD_OBJ_T, *pLCD_OBJ_T;


/**
 * @brief   LCD GPIO对象初始化
 * @param   lcdGpioObj: LCD GPIO对象
 * @param   mosiPort:   MOSI端口
 * @param   sckPort:    SCK端口
 * @param   csPort:     CS端口
 * @param   dcPort:     DC端口
 * @param   resPort:    RES端口
 * @param   blkPort:    BLK端口
 * @param   mosiPin:    MOSI引脚
 * @param   sckPin:     SCK引脚
 * @param   csPin:      CS引脚
 * @param   dcPin:      DC引脚
 * @param   resPin:     RES引脚
 * @param   blkPin:     BLK引脚
 * @retval  None
 * @note    None
*/
void lcd_gpio_obj_init(LCD_GPIO_OBJ_T * const lcdGpioObj, gpio_type * const mosiPort, gpio_type * const sckPort,
    gpio_type * const csPort, gpio_type * const dcPort, gpio_type * const resPort, gpio_type * const blkPort,
    const uint32_t mosiPin, const uint32_t sckPin, const uint32_t csPin, const uint32_t dcPin, const uint32_t resPin, const uint32_t blkPin);


/**
 * @brief   LCD对象初始化
 * @param   lcdObj:     LCD对象
 * @param   gpioObj:    LCD GPIO对象
 * @param   spi:        SPI外设
 * @param   width:      LCD屏幕宽度
 * @param   height:     LCD屏幕高度
 * @param   dir:        LCD显示方向
 * @retval  None
 * @note    None
*/
void lcd_obj_init(LCD_OBJ_T * const lcdObj, LCD_GPIO_OBJ_T * const gpioObj, spi_type * const spi, const uint8_t width, const uint8_t height, const LCD_DIR_T dir);


/**
 * @brief   LCD复位
 * @param   lcdObj: LCD对象
 * @retval  None
 * @note    None
*/
void lcd_reset(LCD_OBJ_T * const lcdObj);


/**
 * @brief   LCD上电指令初始化
 * @param   lcdObj: LCD对象
 * @retval  None
 * @note    None
*/
void lcd_power_on_code_init(LCD_OBJ_T * const lcdObj);


/**
 * @brief   LCD填充
 * @param   lcdObj: LCD对象
 * @param   xStart: 起始横坐标
 * @param   yStart: 起始纵坐标
 * @param   xEnd:   结束横坐标
 * @param   yEnd:   结束纵坐标
 * @param   color:  填充颜色
 * @retval  None
 * @note    None
*/
void lcd_fill(LCD_OBJ_T * const lcdObj, const uint16_t xStart, const uint16_t yStart, const uint16_t xEnd, const uint16_t yEnd, const uint16_t color);


/**
 * @brief   LCD清屏
 * @param   lcdObj: LCD对象
 * @param   bColor: 背景颜色
 * @retval  None
 * @note    None
*/
void lcd_clear(LCD_OBJ_T * const lcdObj, const uint16_t bColor);


/**
 * @brief   LCD显示字符
 * @param   lcdObj:     LCD对象
 * @param   x:          横坐标
 * @param   y:          纵坐标
 * @param   ch:         要显示的字符
 * @param   fontSize:   字体大小
 * @param   fontSorce:  字库
 * @param   fColor:     字体颜色
 * @param   bColor:     背景颜色
 * @retval  None
 * @note    None
*/
void lcd_show_char(LCD_OBJ_T * const lcdObj, const uint16_t x, const uint16_t y, const uint8_t ch, const LCD_EN_FONT_SIZE_T fontSize,
    const uint8_t * const fontSorce, const uint16_t fColor, const uint16_t bColor);


/**
 * @brief   LCD显示字符串
 * @param   lcdObj:     LCD对象
 * @param   x:          横坐标
 * @param   y:          纵坐标
 * @param   str:        要显示的字符串
 * @param   fontSize:   字体大小
 * @param   fontSorce:  字库
 * @param   fColor:     字体颜色
 * @param   bColor:     背景颜色
 * @retval  None
 * @note    None
*/
void lcd_show_str(LCD_OBJ_T * const lcdObj, const uint16_t x, const uint16_t y, const uint8_t *str, const LCD_EN_FONT_SIZE_T fontSize,
    const uint8_t * const fontSorce, const uint16_t fColor, const uint16_t bColor);


/**
 * @brief   LCD显示整数
 * @param   lcdObj:     LCD对象
 * @param   x:          横坐标
 * @param   y:          纵坐标
 * @param   integer:    要显示的整数
 * @param   format:     显示格式
 * @param   fontSize:   字体大小
 * @param   fontSorce:  字库
 * @param   fColor:     字体颜色
 * @param   bColor:     背景颜色
 * @retval  None
 * @note    None
*/
void lcd_show_integer(LCD_OBJ_T * const lcdObj, const uint16_t x, const uint16_t y, const uint32_t integer, const char * const format,
    const LCD_EN_FONT_SIZE_T fontSize, const uint8_t * const fontSorce, const uint16_t fColor, const uint16_t bColor);


/**
 * @brief   LCD显示浮点数
 * @param   lcdObj:     LCD对象
 * @param   x:          横坐标
 * @param   y:          纵坐标
 * @param   floatNum:   要显示的浮点数
 * @param   format:     显示格式
 * @param   fontSize:   字体大小
 * @param   fontSorce:  字库
 * @param   fColor:     字体颜色
 * @param   bColor:     背景颜色
 * @retval  None
 * @note    None
*/
void lcd_show_float(LCD_OBJ_T * const lcdObj, const uint16_t x, const uint16_t y, const double floatNum, const char * const format,
    const LCD_EN_FONT_SIZE_T fontSize, const uint8_t * const fontSorce, const uint16_t fColor, const uint16_t bColor);


/**
 * @brief   LCD显示图片
 * @param   lcdObj:         LCD对象
 * @param   x:              横坐标
 * @param   y:              纵坐标
 * @param   picWidth:       图片宽度
 * @param   picHeight:      图片高度
 * @param   picData:        图片数据
 * @param   picDataSize:    图片数据大小
 * @retval  None
 * @note    None
*/
void lcd_show_picture(LCD_OBJ_T * const lcdObj, const uint16_t x, const uint16_t y, const uint16_t picWidth, const uint16_t picHeight,
    const uint8_t * const picData, const uint16_t picDataSize);

#endif //__LCD_INTERFACE_H
