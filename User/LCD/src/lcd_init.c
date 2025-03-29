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
        .gpio_mode           = GPIO_MODE_MUX,
        .gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER,
    };
    
    //MOSI
    gpio_init(LCD_MOSI_GPIO_PORT, &gpioInitStruct);
    gpio_pin_mux_config(LCD_MOSI_GPIO_PORT, LCD_MOSI_GPIO_MUX_PIN_SOURCE, LCD_MOSI_GPIO_MUX);
    
    //SCK
    gpioInitStruct.gpio_pins = LCD_SCK_GPIO_PIN;
    gpio_init(LCD_SCK_GPIO_PORT, &gpioInitStruct);
    gpio_pin_mux_config(LCD_SCK_GPIO_PORT, LCD_SCK_GPIO_MUX_PIN_SOURCE, LCD_SCK_GPIO_MUX);
    
    //CS
    gpioInitStruct.gpio_mode = GPIO_MODE_OUTPUT;
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
 * @brief   LCD SPI配置
 * @param   None
 * @retval  None
 * @note    静态
*/
static void lcd_spi_config(void)
{
    spi_init_type spiInitStruct = 
    {
        .transmission_mode      = SPI_TRANSMIT_HALF_DUPLEX_TX,
        .master_slave_mode      = SPI_MODE_MASTER,
        .mclk_freq_division     = SPI_MCLK_DIV_2,
        .first_bit_transmission = SPI_FIRST_BIT_MSB,
        .frame_bit_num          = SPI_FRAME_8BIT,
        .clock_polarity         = SPI_CLOCK_POLARITY_HIGH,
        .clock_phase            = SPI_CLOCK_PHASE_2EDGE,
        .cs_mode_selection      = SPI_CS_SOFTWARE_MODE,
    };
    
    spi_init(LCD_SPI, &spiInitStruct);
    spi_enable(LCD_SPI, TRUE);
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
    lcd_spi_config();
}
