// File Name: pwm_init.c
// Author: kukukukuku~
// Date: 2025-03-07

#include "./PWM/inc/pwm_init.h"

/**
 * @brief   PWM GPIO配置
 * @param   None
 * @retval  None
 * @note    静态
*/
static void pwm_gpio_config(void)
{
    gpio_init_type gpioInitStruct = 
    {
        .gpio_pins           = PWM_GPIO_PIN,
        .gpio_out_type       = GPIO_OUTPUT_PUSH_PULL,
        .gpio_pull           = GPIO_PULL_NONE,
        .gpio_mode           = GPIO_MODE_MUX,
        .gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER,
    };
    
    gpio_init(PWM_GPIO_PORT, &gpioInitStruct);
    gpio_pin_mux_config(PWM_GPIO_PORT, PWM_GPIO_MUX_PIN_SOURCE, PWM_GPIO_MUX);
}


/**
 * @brief   PWM TMR配置
 * @param   None
 * @retval  None
 * @note    静态
*/
static void pwm_tmr_config(void)
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
    
    tmr_base_init(PWM_TMR, PWM_TMR_ARR, PWM_TMR_PSC);
    tmr_clock_source_div_set(PWM_TMR, TMR_CLOCK_DIV1);
    tmr_period_buffer_enable(PWM_TMR, TRUE);
    
    tmr_output_channel_config(PWM_TMR, PWM_TMR_CHANNEL, &tmrOcInitStructure);
    tmr_channel_value_set(PWM_TMR, PWM_TMR_CHANNEL, PWM_TMR_PULSE);
    tmr_output_channel_buffer_enable(PWM_TMR, PWM_TMR_CHANNEL, TRUE);
    
    tmr_counter_enable(PWM_TMR, TRUE);
}


/**
 * @brief   PWM初始化
 * @param   None
 * @retval  None
 * @note    None
*/
void pwm_init(void)
{
    pwm_gpio_config();
    pwm_tmr_config();
}
