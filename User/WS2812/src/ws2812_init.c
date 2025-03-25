// File Name: ws2812_init.c
// Author: kukukukuku~
// Date: 2025-03-06

#include "./WS2812/inc/ws2812_init.h"

/**
 * @brief   WS2812 GPIO配置
 * @param   None
 * @retval  None
 * @note    静态
*/
static void ws2812_gpio_config(void)
{
    gpio_init_type gpioInitStruct = 
    {
        .gpio_pins           = WS2812_GPIO_PIN,
        .gpio_out_type       = GPIO_OUTPUT_PUSH_PULL,
        .gpio_pull           = GPIO_PULL_NONE,
        .gpio_mode           = GPIO_MODE_MUX,
        .gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER,
    };
    
    gpio_init(WS2812_GPIO_PORT, &gpioInitStruct);
    gpio_pin_mux_config(WS2812_GPIO_PORT, WS2812_GPIO_MUX_PIN_SOURCE, WS2812_GPIO_MUX);
}


/**
 * @brief   WS2812 SPI配置
 * @param   None
 * @retval  None
 * @note    静态
*/
static void ws2812_spi_config(void)
{
    spi_init_type spiInitStruct = 
    {
        .transmission_mode      = SPI_TRANSMIT_HALF_DUPLEX_TX,
        .master_slave_mode      = SPI_MODE_MASTER,
        .mclk_freq_division     = SPI_MCLK_DIV_16,
        .first_bit_transmission = SPI_FIRST_BIT_MSB,
        .frame_bit_num          = SPI_FRAME_8BIT,
        .clock_polarity         = SPI_CLOCK_POLARITY_LOW,
        .clock_phase            = SPI_CLOCK_PHASE_2EDGE,
        .cs_mode_selection      = SPI_CS_SOFTWARE_MODE,
    };
    
    spi_init(WS2812_SPI, &spiInitStruct);
    spi_enable(WS2812_SPI, TRUE);
}


/**
 * @brief   WS2812初始化
 * @param   None
 * @retval  None
 * @note    None
*/
void ws2812_init(void)
{
    ws2812_gpio_config();
    ws2812_spi_config();
}
