// File Name: key_init.c
// Author: kukukukuku~
// Date: 2025-03-11

#include "./Key/inc/key_init.h"

/**
 * @brief   Key GPIO配置
 * @param   None
 * @retval  None
 * @note    静态
*/
static void key_gpio_config(void)
{
    gpio_init_type gpioInitStruct = 
    {
        .gpio_pins           = KEY_UP_GPIO_PIN,
        .gpio_out_type       = GPIO_OUTPUT_PUSH_PULL,
        .gpio_pull           = GPIO_PULL_UP,
        .gpio_mode           = GPIO_MODE_INPUT,
        .gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER,
    };
    
    //UP
    gpio_init(KEY_UP_GPIO_PORT, &gpioInitStruct);
    
    //DN
    gpioInitStruct.gpio_pins = KEY_DN_GPIO_PIN;
    gpio_init(KEY_DN_GPIO_PORT, &gpioInitStruct);
    
    //LT
    gpioInitStruct.gpio_pins = KEY_LT_GPIO_PIN;
    gpio_init(KEY_LT_GPIO_PORT, &gpioInitStruct);
    
    //RT
    gpioInitStruct.gpio_pins = KEY_RT_GPIO_PIN;
    gpio_init(KEY_RT_GPIO_PORT, &gpioInitStruct);
    
    //MD
    gpioInitStruct.gpio_pins = KEY_MD_GPIO_PIN;
    gpio_init(KEY_MD_GPIO_PORT, &gpioInitStruct);
}


/**
 * @brief   Key TMR配置
 * @param   None
 * @retval  None
 * @note    静态
*/
static void key_tmr_config(void)
{
    tmr_base_init(KEY_TMR, KEY_TMR_ARR, KEY_TMR_PSC);
    tmr_clock_source_div_set(KEY_TMR, TMR_CLOCK_DIV1);
    tmr_period_buffer_enable(KEY_TMR, TRUE);
    
    tmr_flag_clear(KEY_TMR, TMR_OVF_FLAG);
    tmr_interrupt_enable(KEY_TMR, TMR_OVF_INT, TRUE);
    nvic_irq_enable(KEY_TMR_IRQ, 1, 0);
    
    tmr_counter_enable(KEY_TMR, FALSE);
}


/**
 * @brief   Key初始化
 * @param   None
 * @retval  None
 * @note    None
*/
void key_init(void)
{
    key_gpio_config();
    key_tmr_config();
}
