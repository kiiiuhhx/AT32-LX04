// File Name: led_init.c
// Author: kukukukuku~
// Date: 2025-03-17

#include "./LED/inc/led_init.h"

/**
 * @brief   LED GPIO配置
 * @param   None
 * @retval  None
 * @note    静态
*/
static void led_gpio_config(void)
{
    gpio_init_type gpioInitStruct = 
    {
        .gpio_pins           = LED1_GPIO_PIN,
        .gpio_out_type       = GPIO_OUTPUT_PUSH_PULL,
        .gpio_pull           = GPIO_PULL_NONE,
        .gpio_mode           = GPIO_MODE_MUX,
        .gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER,
    };
    
    //LED1
    gpio_init(LED1_GPIO_PORT, &gpioInitStruct);
    gpio_pin_mux_config(LED1_GPIO_PORT, LED1_GPIO_MUX_PIN_SOURCE, LED1_GPIO_MUX);
    
    //LED2
    gpioInitStruct.gpio_mode = GPIO_MODE_OUTPUT;
    gpioInitStruct.gpio_pins = LED2_GPIO_PIN;
    gpio_init(LED2_GPIO_PORT, &gpioInitStruct);
    gpio_bits_reset(LED2_GPIO_PORT, LED2_GPIO_PIN);
}


/**
 * @brief   LED TMR配置
 * @param   None
 * @retval  None
 * @note    静态
*/
static void led_tmr_config(void)
{
    tmr_output_config_type tmrOcInitStructure = 
    {
        .oc_mode          = TMR_OUTPUT_CONTROL_PWM_MODE_A,
        .oc_idle_state    = TRUE,
        .occ_idle_state   = FALSE,
        .oc_polarity      = TMR_OUTPUT_ACTIVE_HIGH,
        .occ_polarity     = TMR_OUTPUT_ACTIVE_LOW,
        .oc_output_state  = TRUE,
        .occ_output_state = FALSE,
    };
    
    //LED1
    tmr_base_init(LED1_TMR, LED1_TMR_BREATH_ARR, LED1_TMR_PSC);
    tmr_clock_source_div_set(LED1_TMR, TMR_CLOCK_DIV1);
    tmr_period_buffer_enable(LED1_TMR, TRUE);
    
    tmr_output_channel_config(LED1_TMR, LED1_TMR_CHANNEL, &tmrOcInitStructure);
    tmr_channel_value_set(LED1_TMR, LED1_TMR_CHANNEL, LED1_TMR_PULSE);
    tmr_output_channel_buffer_enable(LED1_TMR, LED1_TMR_CHANNEL, TRUE);
    
    tmr_flag_clear(LED1_TMR, TMR_OVF_FLAG);
    tmr_interrupt_enable(LED1_TMR, TMR_OVF_INT, TRUE);
    nvic_irq_enable(LED1_TMR_IRQ, 1, 0);
    
    tmr_output_enable(LED1_TMR, TRUE);
    
    tmr_counter_enable(LED1_TMR, TRUE);
}


/**
 * @brief   LED初始化
 * @param   None
 * @retval  None
 * @note    None
*/
void led_init(void)
{
    led_gpio_config();
    led_tmr_config();
}
