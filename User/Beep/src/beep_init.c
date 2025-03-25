// File Name: beep_init.c
// Author: kukukukuku~
// Date: 2025-03-06

#include "./Beep/inc/beep_init.h"

/**
 * @brief   Beep GPIO配置
 * @param   None
 * @retval  None
 * @note    静态
*/
static void beep_gpio_config(void)
{
    gpio_init_type gpioInitStruct = 
    {
        .gpio_pins           = BEEP_GPIO_PIN,
        .gpio_out_type       = GPIO_OUTPUT_PUSH_PULL,
        .gpio_pull           = GPIO_PULL_NONE,
        .gpio_mode           = GPIO_MODE_OUTPUT,
        .gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER,
    };
    
    gpio_init(BEEP_GPIO_PORT, &gpioInitStruct);
    gpio_bits_reset(BEEP_GPIO_PORT, BEEP_GPIO_PIN);
}


/**
 * @brief   Beep TMR配置
 * @param   None
 * @retval  None
 * @note    静态
*/
static void beep_tmr_config(void)
{
    tmr_base_init(BEEP_TMR, BEEP_TMR_ARR, BEEP_TMR_PSC);
    tmr_period_buffer_enable(BEEP_TMR, TRUE);
    
    tmr_flag_clear(BEEP_TMR, TMR_OVF_FLAG);
    tmr_interrupt_enable(BEEP_TMR, TMR_OVF_INT, TRUE);
    nvic_irq_enable(BEEP_TMR_IRQ, 1, 0);
    
    tmr_counter_enable(BEEP_TMR, FALSE);
}


/**
 * @brief   Beep初始化
 * @param   None
 * @retval  None
 * @note    None
*/
void beep_init(void)
{
    beep_gpio_config();
    beep_tmr_config();
}
