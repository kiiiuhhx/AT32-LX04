// File Name: lcd_init.c
// Author: kukukukuku~
// Date: 2025-03-05

#include "./LCD/inc/lcd_init.h"

/**
 * @brief   LCD GPIO配置
 * @param   None
 * @retval  None
 * @note    静态
*/
static void lcd_gpio_config(void)
{
    gpio_init_type gpioInitStruct = 
    {
        .gpio_pins           = LCD_MOSI_GPIO_PIN,
        .gpio_out_type       = GPIO_OUTPUT_PUSH_PULL,
        .gpio_pull           = GPIO_PULL_NONE,
        .gpio_mode           = GPIO_MODE_OUTPUT,
        .gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER,
    };
    
    //MOSI
    gpio_init(LCD_MOSI_GPIO_PORT, &gpioInitStruct);
    gpio_bits_set(LCD_MOSI_GPIO_PORT, LCD_MOSI_GPIO_PIN);
    
    //SCK
    gpioInitStruct.gpio_pins = LCD_SCK_GPIO_PIN;
    gpio_init(LCD_SCK_GPIO_PORT, &gpioInitStruct);
    gpio_bits_set(LCD_SCK_GPIO_PORT, LCD_SCK_GPIO_PIN);
    
    //CS
    gpioInitStruct.gpio_pins = LCD_CS_GPIO_PIN;
    gpio_init(LCD_CS_GPIO_PORT, &gpioInitStruct);
    gpio_bits_set(LCD_CS_GPIO_PORT, LCD_CS_GPIO_PIN);
    
    //DC
    gpioInitStruct.gpio_pins = LCD_DC_GPIO_PIN;
    gpio_init(LCD_DC_GPIO_PORT, &gpioInitStruct);
    gpio_bits_set(LCD_DC_GPIO_PORT, LCD_DC_GPIO_PIN);
    
    //RES
    gpioInitStruct.gpio_pins = LCD_RES_GPIO_PIN;
    gpio_init(LCD_RES_GPIO_PORT, &gpioInitStruct);
    gpio_bits_set(LCD_RES_GPIO_PORT, LCD_RES_GPIO_PIN);
    
    //BLK
    gpioInitStruct.gpio_pins = LCD_BLK_GPIO_PIN;
    gpio_init(LCD_BLK_GPIO_PORT, &gpioInitStruct);
    gpio_bits_reset(LCD_BLK_GPIO_PORT, LCD_BLK_GPIO_PIN);
}


/**
 * @brief   LCD初始化
 * @param   None
 * @retval  None
 * @note    None
*/
void lcd_init(void)
{
    lcd_gpio_config();
}
