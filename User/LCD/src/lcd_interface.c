// File Name: lcd_interface.c
// Author: kukukukuku~
// Date: 2025-03-05

#include "stddef.h"
#include "stdio.h"

#include "FreeRTOS.h"
#include "task.h"

#include "./LCD/inc/lcd_interface.h"

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
    const uint32_t mosiPin, const uint32_t sckPin, const uint32_t csPin, const uint32_t dcPin, const uint32_t resPin, const uint32_t blkPin)
{
    if(lcdGpioObj == NULL)
        return;
    
    lcdGpioObj->mosiPort = mosiPort;
    lcdGpioObj->sckPort  = sckPort;
    lcdGpioObj->csPort   = csPort;
    lcdGpioObj->dcPort   = dcPort;
    lcdGpioObj->resPort  = resPort;
    lcdGpioObj->blkPort  = blkPort;
    
    lcdGpioObj->mosiPin = mosiPin;
    lcdGpioObj->sckPin  = sckPin;
    lcdGpioObj->csPin   = csPin;
    lcdGpioObj->dcPin   = dcPin;
    lcdGpioObj->resPin  = resPin;
    lcdGpioObj->blkPin  = blkPin;
}


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
void lcd_obj_init(LCD_OBJ_T * const lcdObj, LCD_GPIO_OBJ_T * const gpioObj, spi_type * const spi, const uint8_t width, const uint8_t height, const LCD_DIR_T dir)
{
    if(lcdObj == NULL || gpioObj == NULL || spi == NULL)
        return;
    
    lcdObj->gpioObj               = gpioObj;
    lcdObj->spi                   = spi;
    lcdObj->updateSemaphoreHandle = xSemaphoreCreateBinary();
    lcdObj->width                 = width;
    lcdObj->height                = height;
    lcdObj->dir                   = dir;
}


/**
 * @brief   LCD毫秒级延时
 * @param   ms: 延时时间
 * @retval  None
 * @note    静态
*/
static void lcd_delay_ms(const uint16_t ms)
{
    if(ms == 0)
        return;
    
    vTaskDelay(ms);
}


/**
 * @brief   LCD CS使能
 * @param   lcdObj: LCD对象
 * @retval  None
 * @note    静态
*/
static void lcd_cs_enable(LCD_OBJ_T * const lcdObj)
{
    if(lcdObj == NULL || lcdObj->gpioObj == NULL)
        return;
    
    gpio_bits_reset(lcdObj->gpioObj->csPort, lcdObj->gpioObj->csPin);
}


/**
 * @brief   LCD CS失能
 * @param   lcdObj: LCD对象
 * @retval  None
 * @note    静态
*/
static void lcd_cs_disable(LCD_OBJ_T * const lcdObj)
{
    if(lcdObj == NULL || lcdObj->gpioObj == NULL)
        return;
    
    gpio_bits_set(lcdObj->gpioObj->csPort, lcdObj->gpioObj->csPin);
}


/**
 * @brief   LCD DC选择命令
 * @param   lcdObj: LCD对象
 * @retval  None
 * @note    静态
*/
static void lcd_dc_cmd(LCD_OBJ_T * const lcdObj)
{
    if(lcdObj == NULL || lcdObj->gpioObj == NULL)
        return;
    
    gpio_bits_reset(lcdObj->gpioObj->dcPort, lcdObj->gpioObj->dcPin);
}


/**
 * @brief   LCD DC选择数据
 * @param   lcdObj: LCD对象
 * @retval  None
 * @note    静态
*/
static void lcd_dc_data(LCD_OBJ_T * const lcdObj)
{
    if(lcdObj == NULL || lcdObj->gpioObj == NULL)
        return;
    
    gpio_bits_set(lcdObj->gpioObj->dcPort, lcdObj->gpioObj->dcPin);
}


/**
 * @brief   LCD RES使能
 * @param   lcdObj: LCD对象
 * @retval  None
 * @note    静态
*/
static void lcd_res_enable(LCD_OBJ_T * const lcdObj)
{
    if(lcdObj == NULL || lcdObj->gpioObj == NULL)
        return;
    
    gpio_bits_reset(lcdObj->gpioObj->resPort, lcdObj->gpioObj->resPin);
}


/**
 * @brief   LCD RES失能
 * @param   lcdObj: LCD对象
 * @retval  None
 * @note    静态
*/
static void lcd_res_disable(LCD_OBJ_T * const lcdObj)
{
    if(lcdObj == NULL || lcdObj->gpioObj == NULL)
        return;
    
    gpio_bits_set(lcdObj->gpioObj->resPort, lcdObj->gpioObj->resPin);
}


/**
 * @brief   LCD BLK使能
 * @param   lcdObj: LCD对象
 * @retval  None
 * @note    静态
*/
static void lcd_blk_enable(LCD_OBJ_T * const lcdObj)
{
    if(lcdObj == NULL || lcdObj->gpioObj == NULL)
        return;
    
    gpio_bits_set(lcdObj->gpioObj->blkPort, lcdObj->gpioObj->blkPin);
}


/**
 * @brief   LCD BLK失能
 * @param   lcdObj: LCD对象
 * @retval  None
 * @note    静态
*/
static void lcd_blk_disable(LCD_OBJ_T * const lcdObj)
{
    if(lcdObj == NULL || lcdObj->gpioObj == NULL)
        return;
    
    gpio_bits_reset(lcdObj->gpioObj->blkPort, lcdObj->gpioObj->blkPin);
}


/**
 * @brief   向LCD发送一个字节
 * @param   lcdObj: LCD对象
 * @param   byte:   要发送的字节
 * @retval  None
 * @note    静态
*/
static void send_byte_to_lcd(LCD_OBJ_T * const lcdObj, const uint8_t byte)
{
    if(lcdObj == NULL || lcdObj->spi == NULL)
        return;
    
    while(!spi_i2s_flag_get(lcdObj->spi, SPI_I2S_TDBE_FLAG));
    spi_i2s_data_transmit(lcdObj->spi, byte);
}


/**
 * @brief   向LCD发送命令
 * @param   lcdObj: LCD对象
 * @param   cmd:    要发送的命令
 * @retval  None
 * @note    静态
*/
static void send_cmd_to_lcd(LCD_OBJ_T * const lcdObj, const uint8_t cmd)
{
    if(lcdObj == NULL || lcdObj->gpioObj == NULL || lcdObj->spi == NULL)
        return;
    
    lcd_cs_enable(lcdObj);
    
    lcd_dc_cmd(lcdObj);
    send_byte_to_lcd(lcdObj, cmd);
    
    lcd_cs_disable(lcdObj);
}


/**
 * @brief   向LCD发送8位数据
 * @param   lcdObj: LCD对象
 * @param   data:   要发送的数据
 * @retval  None
 * @note    静态
*/
static void send_data_8bit_to_lcd(LCD_OBJ_T * const lcdObj, const uint8_t data)
{
    if(lcdObj == NULL || lcdObj->gpioObj == NULL || lcdObj->spi == NULL)
        return;
    
    lcd_cs_enable(lcdObj);
    
    lcd_dc_data(lcdObj);
    send_byte_to_lcd(lcdObj, data);
    
    lcd_cs_disable(lcdObj);
}


/**
 * @brief   向LCD发送16位数据
 * @param   lcdObj: LCD对象
 * @param   data:   要发送的数据
 * @retval  None
 * @note    静态
*/
static void send_data_16bit_to_lcd(LCD_OBJ_T * const lcdObj, const uint16_t data)
{
    if(lcdObj == NULL || lcdObj->gpioObj == NULL || lcdObj->spi == NULL)
        return;
    
    send_byte_to_lcd(lcdObj, data >> 8 & 0xFF);
    send_byte_to_lcd(lcdObj, data & 0xFF);
}


/**
 * @brief   LCD复位
 * @param   lcdObj: LCD对象
 * @retval  None
 * @note    None
*/
void lcd_reset(LCD_OBJ_T * const lcdObj)
{
    if(lcdObj == NULL || lcdObj->gpioObj == NULL || lcdObj->spi == NULL)
        return;
    
    lcd_res_disable(lcdObj);
    lcd_delay_ms(10);
    
    lcd_res_enable(lcdObj);
    lcd_delay_ms(10);
    
    lcd_res_disable(lcdObj);
    lcd_delay_ms(100);
}


/**
 * @brief   LCD上电指令初始化
 * @param   lcdObj: LCD对象
 * @retval  None
 * @note    None
*/
void lcd_power_on_code_init(LCD_OBJ_T * const lcdObj)
{
    if(lcdObj == NULL || lcdObj->gpioObj == NULL || lcdObj->spi == NULL)
        return;
    
    lcd_blk_disable(lcdObj);
    
    send_cmd_to_lcd(lcdObj, 0xFE);
    send_cmd_to_lcd(lcdObj, 0xEF);
    //内部寄存器使能80~8Fh
    send_cmd_to_lcd(lcdObj, 0x80);
    send_data_8bit_to_lcd(lcdObj, 0xFF);
    
    send_cmd_to_lcd(lcdObj, 0x81);
    send_data_8bit_to_lcd(lcdObj, 0xFF);
    
    send_cmd_to_lcd(lcdObj, 0x82);
    send_data_8bit_to_lcd(lcdObj, 0xFF);
    
    send_cmd_to_lcd(lcdObj, 0x83);
    send_data_8bit_to_lcd(lcdObj, 0xFF);
    
    send_cmd_to_lcd(lcdObj, 0x84);
    send_data_8bit_to_lcd(lcdObj, 0xFF);
    
    send_cmd_to_lcd(lcdObj, 0x85);
    send_data_8bit_to_lcd(lcdObj, 0xFF);
    
    send_cmd_to_lcd(lcdObj, 0x86);
    send_data_8bit_to_lcd(lcdObj, 0xFF);
    
    send_cmd_to_lcd(lcdObj, 0x87);
    send_data_8bit_to_lcd(lcdObj, 0xFF);
    
    send_cmd_to_lcd(lcdObj, 0x88);
    send_data_8bit_to_lcd(lcdObj, 0xFF);
    
    send_cmd_to_lcd(lcdObj, 0x89);
    send_data_8bit_to_lcd(lcdObj, 0xFF);
    
    send_cmd_to_lcd(lcdObj, 0x8A);
    send_data_8bit_to_lcd(lcdObj, 0xFF);
    
    send_cmd_to_lcd(lcdObj, 0x8B);
    send_data_8bit_to_lcd(lcdObj, 0xFF);
    
    send_cmd_to_lcd(lcdObj, 0x8C);
    send_data_8bit_to_lcd(lcdObj, 0xFF);
    
    send_cmd_to_lcd(lcdObj, 0x8D);
    send_data_8bit_to_lcd(lcdObj, 0xFF);
    
    send_cmd_to_lcd(lcdObj, 0x8E);
    send_data_8bit_to_lcd(lcdObj, 0xFF);
    
    send_cmd_to_lcd(lcdObj, 0x8F);
    send_data_8bit_to_lcd(lcdObj, 0xFF);
    
    send_cmd_to_lcd(lcdObj, 0x3A);
    send_data_8bit_to_lcd(lcdObj, 0x05);
    //设置翻转方式
    send_cmd_to_lcd(lcdObj, 0xEC);
    send_data_8bit_to_lcd(lcdObj, 0x11);
    
    //VGL大小
    send_cmd_to_lcd(lcdObj, 0x7E);
    send_data_8bit_to_lcd(lcdObj, 0x7a);
    
    //修改帧频
    send_cmd_to_lcd(lcdObj, 0x74);
    send_data_8bit_to_lcd(lcdObj, 0x02);
    send_data_8bit_to_lcd(lcdObj, 0x0E);
    send_data_8bit_to_lcd(lcdObj, 0x00);
    send_data_8bit_to_lcd(lcdObj, 0x00);
    send_data_8bit_to_lcd(lcdObj, 0x28);
    send_data_8bit_to_lcd(lcdObj, 0x00);
    send_data_8bit_to_lcd(lcdObj, 0x00);
    
    //内部电压调整
    send_cmd_to_lcd(lcdObj, 0x98);
    send_data_8bit_to_lcd(lcdObj, 0x3E);
    send_cmd_to_lcd(lcdObj, 0x99);
    send_data_8bit_to_lcd(lcdObj, 0x3E);
    
    //内部porch设置
    send_cmd_to_lcd(lcdObj, 0xB5);
    send_data_8bit_to_lcd(lcdObj, 0x0E);
    send_data_8bit_to_lcd(lcdObj, 0x0E);
    
    //gip timing start
    send_cmd_to_lcd(lcdObj, 0x60);
    send_data_8bit_to_lcd(lcdObj, 0x38);
    send_data_8bit_to_lcd(lcdObj, 0x09);
    send_data_8bit_to_lcd(lcdObj, 0x6D);
    send_data_8bit_to_lcd(lcdObj, 0x67);
    
    send_cmd_to_lcd(lcdObj, 0x63);
    send_data_8bit_to_lcd(lcdObj, 0x38);
    send_data_8bit_to_lcd(lcdObj, 0xAD);
    send_data_8bit_to_lcd(lcdObj, 0x6D);
    send_data_8bit_to_lcd(lcdObj, 0x67);
    send_data_8bit_to_lcd(lcdObj, 0x05);
    
    send_cmd_to_lcd(lcdObj, 0x64);
    send_data_8bit_to_lcd(lcdObj, 0x38);
    send_data_8bit_to_lcd(lcdObj, 0x0B);
    send_data_8bit_to_lcd(lcdObj, 0x70);
    send_data_8bit_to_lcd(lcdObj, 0xAB);
    send_data_8bit_to_lcd(lcdObj, 0x6D);
    send_data_8bit_to_lcd(lcdObj, 0x67);
    
    send_cmd_to_lcd(lcdObj, 0x66);
    send_data_8bit_to_lcd(lcdObj, 0x38);
    send_data_8bit_to_lcd(lcdObj, 0x0F);
    send_data_8bit_to_lcd(lcdObj, 0x70);
    send_data_8bit_to_lcd(lcdObj, 0xAF);
    send_data_8bit_to_lcd(lcdObj, 0x6d);
    send_data_8bit_to_lcd(lcdObj, 0x67);
    
    send_cmd_to_lcd(lcdObj, 0x6A);
    send_data_8bit_to_lcd(lcdObj, 0x00);
    send_data_8bit_to_lcd(lcdObj, 0x00);
    
    send_cmd_to_lcd(lcdObj, 0x68);
    send_data_8bit_to_lcd(lcdObj, 0x3B);
    send_data_8bit_to_lcd(lcdObj, 0x08);
    send_data_8bit_to_lcd(lcdObj, 0x04);
    send_data_8bit_to_lcd(lcdObj, 0x00);
    send_data_8bit_to_lcd(lcdObj, 0x04);
    send_data_8bit_to_lcd(lcdObj, 0x64);
    send_data_8bit_to_lcd(lcdObj, 0x67);
    
    send_cmd_to_lcd(lcdObj, 0x6C);
    send_data_8bit_to_lcd(lcdObj, 0x22);
    send_data_8bit_to_lcd(lcdObj, 0x02);
    send_data_8bit_to_lcd(lcdObj, 0x22);
    send_data_8bit_to_lcd(lcdObj, 0x02);
    send_data_8bit_to_lcd(lcdObj, 0x22);
    send_data_8bit_to_lcd(lcdObj, 0x22);
    send_data_8bit_to_lcd(lcdObj, 0x50);
    
    send_cmd_to_lcd(lcdObj, 0x6E);
    send_data_8bit_to_lcd(lcdObj, 0x00);
    send_data_8bit_to_lcd(lcdObj, 0x00);
    send_data_8bit_to_lcd(lcdObj, 0x00);
    send_data_8bit_to_lcd(lcdObj, 0x00);
    send_data_8bit_to_lcd(lcdObj, 0x07);
    send_data_8bit_to_lcd(lcdObj, 0x01);
    send_data_8bit_to_lcd(lcdObj, 0x13);
    send_data_8bit_to_lcd(lcdObj, 0x11);
    
    send_data_8bit_to_lcd(lcdObj, 0x0B);
    send_data_8bit_to_lcd(lcdObj, 0x09);
    send_data_8bit_to_lcd(lcdObj, 0x16);
    send_data_8bit_to_lcd(lcdObj, 0x15);
    send_data_8bit_to_lcd(lcdObj, 0x1D);
    send_data_8bit_to_lcd(lcdObj, 0x1E);
    send_data_8bit_to_lcd(lcdObj, 0x00);
    send_data_8bit_to_lcd(lcdObj, 0x00);
    
    send_data_8bit_to_lcd(lcdObj, 0x00);
    send_data_8bit_to_lcd(lcdObj, 0x00);
    send_data_8bit_to_lcd(lcdObj, 0x1E);
    
    send_data_8bit_to_lcd(lcdObj, 0x1D);
    send_data_8bit_to_lcd(lcdObj, 0x15);
    send_data_8bit_to_lcd(lcdObj, 0x16);
    send_data_8bit_to_lcd(lcdObj, 0x0A);
    send_data_8bit_to_lcd(lcdObj, 0x0C);
    
    send_data_8bit_to_lcd(lcdObj, 0x12);
    send_data_8bit_to_lcd(lcdObj, 0x14);
    send_data_8bit_to_lcd(lcdObj, 0x02);
    send_data_8bit_to_lcd(lcdObj, 0x08);
    send_data_8bit_to_lcd(lcdObj, 0x00);
    send_data_8bit_to_lcd(lcdObj, 0x00);
    send_data_8bit_to_lcd(lcdObj, 0x00);
    send_data_8bit_to_lcd(lcdObj, 0x00);
    //gip timing end
    
    //内部电压设定开始
    send_cmd_to_lcd(lcdObj, 0xA9);
    send_data_8bit_to_lcd(lcdObj, 0x1B);
    
    send_cmd_to_lcd(lcdObj, 0xA8);
    send_data_8bit_to_lcd(lcdObj, 0x6B);
    
    send_cmd_to_lcd(lcdObj, 0xA8);
    send_data_8bit_to_lcd(lcdObj, 0x6D);
    
    send_cmd_to_lcd(lcdObj, 0xA7);
    send_data_8bit_to_lcd(lcdObj, 0x40);
    
    send_cmd_to_lcd(lcdObj, 0xAD);
    send_data_8bit_to_lcd(lcdObj, 0x47);
    
    send_cmd_to_lcd(lcdObj, 0xAF);
    send_data_8bit_to_lcd(lcdObj, 0x73);
    
    send_cmd_to_lcd(lcdObj, 0xAF);
    send_data_8bit_to_lcd(lcdObj, 0x73);
    
    send_cmd_to_lcd(lcdObj, 0xAC);
    send_data_8bit_to_lcd(lcdObj, 0x44);
    
    send_cmd_to_lcd(lcdObj, 0xA3);
    send_data_8bit_to_lcd(lcdObj, 0x6C);
    
    send_cmd_to_lcd(lcdObj, 0xCB);
    send_data_8bit_to_lcd(lcdObj, 0x00);
    
    send_cmd_to_lcd(lcdObj, 0xCD);
    send_data_8bit_to_lcd(lcdObj, 0x22);
    
    send_cmd_to_lcd(lcdObj, 0xC2);
    send_data_8bit_to_lcd(lcdObj, 0x10);
    
    send_cmd_to_lcd(lcdObj, 0xC5);
    send_data_8bit_to_lcd(lcdObj, 0x00);
    
    send_cmd_to_lcd(lcdObj, 0xC6);
    send_data_8bit_to_lcd(lcdObj, 0x0E);
    
    send_cmd_to_lcd(lcdObj, 0xC7);
    send_data_8bit_to_lcd(lcdObj, 0x1f);
    
    send_cmd_to_lcd(lcdObj, 0xC8);
    send_data_8bit_to_lcd(lcdObj, 0x0E);
    //内部电压设定结束
    
    //选择single gate mode
    send_cmd_to_lcd(lcdObj, 0xbf);
    send_data_8bit_to_lcd(lcdObj, 0x00);
    //SOU相关调整
    send_cmd_to_lcd(lcdObj, 0xF9);
    send_data_8bit_to_lcd(lcdObj, 0x20);
    
    //vreg电压调整
    send_cmd_to_lcd(lcdObj, 0x9b);
    send_data_8bit_to_lcd(lcdObj, 0x3b);
    send_cmd_to_lcd(lcdObj, 0x93);
    send_data_8bit_to_lcd(lcdObj, 0x33);
    send_data_8bit_to_lcd(lcdObj, 0x7f);
    send_data_8bit_to_lcd(lcdObj, 0x00);
    
    //VGH/VGL CLK调整70，71h
    send_cmd_to_lcd(lcdObj, 0x70);
    send_data_8bit_to_lcd(lcdObj, 0x0E);
    send_data_8bit_to_lcd(lcdObj, 0x0f);
    send_data_8bit_to_lcd(lcdObj, 0x03);
    send_data_8bit_to_lcd(lcdObj, 0x0e);
    send_data_8bit_to_lcd(lcdObj, 0x0f);
    send_data_8bit_to_lcd(lcdObj, 0x03);
    
    send_cmd_to_lcd(lcdObj, 0x71);
    send_data_8bit_to_lcd(lcdObj, 0x0e);
    send_data_8bit_to_lcd(lcdObj, 0x16);
    send_data_8bit_to_lcd(lcdObj, 0x03);
    
    //内部电压调整
    send_cmd_to_lcd(lcdObj, 0x91);
    send_data_8bit_to_lcd(lcdObj, 0x0e);
    send_data_8bit_to_lcd(lcdObj, 0x09);
    
    //vreg电压调整
    send_cmd_to_lcd(lcdObj, 0xc3);
    send_data_8bit_to_lcd(lcdObj, 0x2c);
    send_cmd_to_lcd(lcdObj, 0xc4);
    send_data_8bit_to_lcd(lcdObj, 0x1a);
    
    //gamma F1~F3h
    send_cmd_to_lcd(lcdObj, 0xf0);
    send_data_8bit_to_lcd(lcdObj, 0x51);
    send_data_8bit_to_lcd(lcdObj, 0x13);
    send_data_8bit_to_lcd(lcdObj, 0x0c);
    send_data_8bit_to_lcd(lcdObj, 0x06);
    send_data_8bit_to_lcd(lcdObj, 0x00);
    send_data_8bit_to_lcd(lcdObj, 0x2f);
    
    send_cmd_to_lcd(lcdObj, 0xf2);
    send_data_8bit_to_lcd(lcdObj, 0x51);
    send_data_8bit_to_lcd(lcdObj, 0x13);
    send_data_8bit_to_lcd(lcdObj, 0x0c);
    send_data_8bit_to_lcd(lcdObj, 0x06);
    send_data_8bit_to_lcd(lcdObj, 0x00);
    send_data_8bit_to_lcd(lcdObj, 0x33);
    
    send_cmd_to_lcd(lcdObj, 0xf1);
    send_data_8bit_to_lcd(lcdObj, 0x3c);
    send_data_8bit_to_lcd(lcdObj, 0x94);
    send_data_8bit_to_lcd(lcdObj, 0x4f);
    send_data_8bit_to_lcd(lcdObj, 0x33);
    send_data_8bit_to_lcd(lcdObj, 0x34);
    send_data_8bit_to_lcd(lcdObj, 0xCf);
    
    send_cmd_to_lcd(lcdObj, 0xf3);
    send_data_8bit_to_lcd(lcdObj, 0x4d);
    send_data_8bit_to_lcd(lcdObj, 0x94);
    send_data_8bit_to_lcd(lcdObj, 0x4f);
    send_data_8bit_to_lcd(lcdObj, 0x33);
    send_data_8bit_to_lcd(lcdObj, 0x34);
    send_data_8bit_to_lcd(lcdObj, 0xCf);
    
    send_cmd_to_lcd(lcdObj, 0x36);
    send_data_8bit_to_lcd(lcdObj, lcdObj->dir == LCD_DIR_NORMAL ? 0x00 : 0xC0);
    send_cmd_to_lcd(lcdObj, 0x11);
    
    lcd_delay_ms(200);
    
    send_cmd_to_lcd(lcdObj, 0x29);
    send_cmd_to_lcd(lcdObj, 0x2C);
    
    lcd_blk_enable(lcdObj);
}


/**
 * @brief   LCD设置地址
 * @param   lcdObj: LCD对象
 * @param   xStart: 起始横坐标
 * @param   yStart: 起始纵坐标
 * @param   xEnd:   结束横坐标
 * @param   yEnd:   结束纵坐标
 * @retval  None
 * @note    静态
*/
static void lcd_set_addr(LCD_OBJ_T * const lcdObj, const uint16_t xStart, const uint16_t yStart, const uint16_t xEnd, const uint16_t yEnd)
{
    if(lcdObj == NULL || lcdObj->gpioObj == NULL || lcdObj->spi == NULL || xStart > xEnd || yStart > yEnd || xEnd >= lcdObj->width || yEnd >= lcdObj->height)
        return;
    
    //设置列地址
    send_cmd_to_lcd(lcdObj, 0x2A);
    send_data_8bit_to_lcd(lcdObj, yStart >> 8 & 0xFF);
    send_data_8bit_to_lcd(lcdObj, yStart & 0xFF);
    send_data_8bit_to_lcd(lcdObj, yEnd >> 8 & 0xFF);
    send_data_8bit_to_lcd(lcdObj, yEnd & 0xFF);
    //设置行地址
    send_cmd_to_lcd(lcdObj, 0x2B);
    send_data_8bit_to_lcd(lcdObj, (lcdObj->width - xEnd - 1) >> 8 & 0xFF);
    send_data_8bit_to_lcd(lcdObj, (lcdObj->width - xEnd - 1) & 0xFF);
    send_data_8bit_to_lcd(lcdObj, (lcdObj->width - xStart - 1) >> 8 & 0xFF);
    send_data_8bit_to_lcd(lcdObj, (lcdObj->width - xStart - 1) & 0xFF);
    //写储存器
    send_cmd_to_lcd(lcdObj, 0x2C);
}


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
void lcd_fill(LCD_OBJ_T * const lcdObj, const uint16_t xStart, const uint16_t yStart, const uint16_t xEnd, const uint16_t yEnd, const uint16_t color)
{
    if(lcdObj == NULL || lcdObj->gpioObj == NULL || lcdObj->spi == NULL || xStart > xEnd || yStart > yEnd || xEnd >= lcdObj->width || yEnd >= lcdObj->height)
        return;
    
    lcd_set_addr(lcdObj, xStart, yStart, xEnd, yEnd);
    
    lcd_cs_enable(lcdObj);
    lcd_dc_data(lcdObj);
    
    for(uint16_t i = xStart; i <= xEnd; i++)
    {
        for(uint16_t j = yStart; j <= yEnd; j++)
        {
            send_data_16bit_to_lcd(lcdObj, color);
        }
    }
    
    lcd_cs_disable(lcdObj);
}


/**
 * @brief   LCD清屏
 * @param   lcdObj: LCD对象
 * @param   bColor: 背景颜色
 * @retval  None
 * @note    None
*/
void lcd_clear(LCD_OBJ_T * const lcdObj, const uint16_t bColor)
{
    if(lcdObj == NULL || lcdObj->gpioObj == NULL || lcdObj->spi == NULL)
        return;
    
    lcd_fill(lcdObj, 0, 0, lcdObj->width - 1, lcdObj->height - 1, bColor);
}


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
    const uint8_t * const fontSorce, const uint16_t fColor, const uint16_t bColor)
{
    uint8_t xSize = 0, ySize = 0;
    uint16_t chByteSize = 0;
    uint16_t xx = x, yy = y;
    
    if(lcdObj == NULL || lcdObj->gpioObj == NULL || lcdObj->spi == NULL || fontSorce == NULL || ch > 127 || xx >= lcdObj->width || yy >= lcdObj->height)
        return;
    
    switch(fontSize)
    {
        case LCD_EN_FONT_SIZE_8X16:
            xSize = 8, ySize = 16;
            break;
        case LCD_EN_FONT_SIZE_16X16:
            xSize = 16, ySize = 16;
            break;
        case LCD_EN_FONT_SIZE_16X24:
            xSize = 16, ySize = 24;
            break;
        case LCD_EN_FONT_SIZE_16X32:
            xSize = 16, ySize = 32;
            break;
        case LCD_EN_FONT_SIZE_24X32:
            xSize = 24, ySize = 32;
            break;
        default:
            xSize = 8, ySize = 16;
            break;
    }
    
    if(xx + xSize > lcdObj->width)
    {
        xx = 0;
        yy += ySize;
    }
    
    if(yy + ySize > lcdObj->height)
        yy = 0;
    
    lcd_set_addr(lcdObj, xx, yy, xx + xSize - 1, yy + ySize - 1);
    
    lcd_cs_enable(lcdObj);
    lcd_dc_data(lcdObj);
    
    chByteSize = (xSize / 8) * ySize;
    
    for(uint16_t i = 0; i < chByteSize; i++)
    {
        for(uint8_t j = 0; j < 8; j++)
        {
            send_data_16bit_to_lcd(lcdObj, fontSorce[ch * chByteSize + i] >> j & 0x01 ? fColor : bColor);
        }
    }
    
    lcd_cs_disable(lcdObj);
}


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
    const uint8_t * const fontSorce, const uint16_t fColor, const uint16_t bColor)
{
    uint8_t xSize = 0, ySize = 0;
    uint16_t xx = x, yy = y;
    
    if(lcdObj == NULL || lcdObj->gpioObj == NULL || lcdObj->spi == NULL || fontSorce == NULL || str == NULL || xx >= lcdObj->width || yy >= lcdObj->height)
        return;
    
    switch(fontSize)
    {
        case LCD_EN_FONT_SIZE_8X16:
            xSize = 8, ySize = 16;
            break;
        case LCD_EN_FONT_SIZE_16X16:
            xSize = 16, ySize = 16;
            break;
        case LCD_EN_FONT_SIZE_16X24:
            xSize = 16, ySize = 24;
            break;
        case LCD_EN_FONT_SIZE_16X32:
            xSize = 16, ySize = 32;
            break;
        case LCD_EN_FONT_SIZE_24X32:
            xSize = 24, ySize = 32;
            break;
        default:
            xSize = 8, ySize = 16;
            break;
    }
    
    while(*str)
    {
        if(xx + xSize > LCD_WIDTH)
        {
            xx = 0;
            yy += ySize;
        }
        
        if(yy + ySize > LCD_HEIGHT)
            yy = 0;
        
        lcd_show_char(lcdObj, xx, yy, *str++, fontSize, fontSorce, fColor, bColor);
        
        xx += xSize;
    }
}


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
    const LCD_EN_FONT_SIZE_T fontSize, const uint8_t * const fontSorce, const uint16_t fColor, const uint16_t bColor)
{
    uint8_t xSize = 0, ySize = 0;
    uint16_t xx = x, yy = y;
    uint8_t str[20] = {0};
    uint8_t *pStr = str;
    
    if(lcdObj == NULL || lcdObj->gpioObj == NULL || lcdObj->spi == NULL || fontSorce == NULL || xx >= lcdObj->width || yy >= lcdObj->height)
        return;
    
    switch(fontSize)
    {
        case LCD_EN_FONT_SIZE_8X16:
            xSize = 8, ySize = 16;
            break;
        case LCD_EN_FONT_SIZE_16X16:
            xSize = 16, ySize = 16;
            break;
        case LCD_EN_FONT_SIZE_16X24:
            xSize = 16, ySize = 24;
            break;
        case LCD_EN_FONT_SIZE_16X32:
            xSize = 16, ySize = 32;
            break;
        case LCD_EN_FONT_SIZE_24X32:
            xSize = 24, ySize = 32;
            break;
        default:
            xSize = 8, ySize = 16;
            break;
    }
    
    sprintf((char *)str, format, integer);
    
    while(*pStr)
    {
        if(xx + xSize > LCD_WIDTH)
        {
            xx = 0;
            yy += ySize;
        }
        
        if(yy + ySize > LCD_HEIGHT)
            yy = 0;
        
        lcd_show_char(lcdObj, xx, yy, *pStr == ' ' ? 0 : *pStr - '0' + 2, fontSize, fontSorce, fColor, bColor);
        
        xx += xSize;
        pStr++;
    }
}


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
    const LCD_EN_FONT_SIZE_T fontSize, const uint8_t * const fontSorce, const uint16_t fColor, const uint16_t bColor)
{
    uint8_t xSize = 0, ySize = 0;
    uint16_t xx = x, yy = y;
    uint8_t str[20] = {0};
    uint8_t *pStr = str;
    
    if(lcdObj == NULL || lcdObj->gpioObj == NULL || lcdObj->spi == NULL || fontSorce == NULL || xx >= lcdObj->width || yy >= lcdObj->height)
        return;
    
    switch(fontSize)
    {
        case LCD_EN_FONT_SIZE_8X16:
            xSize = 8, ySize = 16;
            break;
        case LCD_EN_FONT_SIZE_16X16:
            xSize = 16, ySize = 16;
            break;
        case LCD_EN_FONT_SIZE_16X24:
            xSize = 16, ySize = 24;
            break;
        case LCD_EN_FONT_SIZE_16X32:
            xSize = 16, ySize = 32;
            break;
        case LCD_EN_FONT_SIZE_24X32:
            xSize = 24, ySize = 32;
            break;
        default:
            xSize = 8, ySize = 16;
            break;
    }
    
    sprintf((char *)str, format, floatNum);
    
    while(*pStr)
    {
        if(xx + xSize > LCD_WIDTH)
        {
            xx = 0;
            yy += ySize;
        }
        
        if(yy + ySize > LCD_HEIGHT)
            yy = 0;
        
        lcd_show_char(lcdObj, xx, yy, *pStr == ' ' ? 0 : (*pStr == '.' ? 1 : *pStr - '0' + 2), fontSize, fontSorce, fColor, bColor);
        
        xx += xSize;
        pStr++;
    }
}


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
    const uint8_t * const picData, const uint16_t picDataSize)
{
    if(lcdObj == NULL || lcdObj->gpioObj == NULL || lcdObj->spi == NULL || picData == NULL || x >= lcdObj->width || y >= lcdObj->height ||
        x + picWidth > lcdObj->width || y + picHeight > lcdObj->height)
        return;
    
    lcd_set_addr(lcdObj, x, y, x + picWidth - 1, y + picHeight - 1);
    
    lcd_cs_enable(lcdObj);
    lcd_dc_data(lcdObj);
    
    for(uint16_t i = 0; i < picDataSize / 2; i++)
    {
        send_data_16bit_to_lcd(lcdObj, picData[i * 2] << 8 | picData[i * 2 + 1]);
    }
    
    lcd_cs_disable(lcdObj);
}
