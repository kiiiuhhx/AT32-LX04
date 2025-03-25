// File Name: servo_init.c
// Author: kukukukuku~
// Date: 2025-03-10

#include "./Servo/inc/servo_init.h"

/**
 * @brief   Servo GPIO配置
 * @param   None
 * @retval  None
 * @note    静态
*/
static void servo_gpio_config(void)
{
    gpio_init_type gpioInitStruct = 
    {
        .gpio_pins           = SERVO_GPIO_PIN,
        .gpio_out_type       = GPIO_OUTPUT_PUSH_PULL,
        .gpio_pull           = GPIO_PULL_NONE,
        .gpio_mode           = GPIO_MODE_MUX,
        .gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER,
    };
    
    gpio_init(SERVO_GPIO_PORT, &gpioInitStruct);
    gpio_pin_mux_config(SERVO_GPIO_PORT, SERVO_GPIO_MUX_PIN_SOURCE, SERVO_GPIO_MUX);
}


/**
 * @brief   Servo TMR配置
 * @param   None
 * @retval  None
 * @note    静态
*/
static void servo_tmr_config(void)
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
    
    tmr_base_init(SERVO_TMR, SERVO_TMR_ARR, SERVO_TMR_PSC);
    tmr_cnt_dir_set(SERVO_TMR, TMR_COUNT_UP);
    tmr_clock_source_div_set(SERVO_TMR, TMR_CLOCK_DIV1);
    tmr_period_buffer_enable(SERVO_TMR, TRUE);
    
    tmr_output_channel_config(SERVO_TMR, SERVO_TMR_CHANNEL, &tmrOcInitStructure);
    tmr_channel_value_set(SERVO_TMR, SERVO_TMR_CHANNEL, SERVO_TMR_PULSE);
    tmr_output_channel_buffer_enable(SERVO_TMR, SERVO_TMR_CHANNEL, TRUE);
    
    tmr_output_enable(SERVO_TMR, TRUE);
    
    tmr_counter_enable(SERVO_TMR, TRUE);
}


/**
 * @brief   Servo初始化
 * @param   None
 * @retval  None
 * @note    None
*/
void servo_init(void)
{
    servo_gpio_config();
    servo_tmr_config();
}
