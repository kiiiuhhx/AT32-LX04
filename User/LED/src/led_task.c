// File Name: led_task.c
// Author: kukukukuku~
// Date: 2025-03-19

#include "./LED/inc/led_task.h"

//LED任务句柄
TaskHandle_t ledTaskHandle;


/**
 * @brief   LED任务
 * @param   arg:    任务参数
 * @retval  None
 * @note    None
*/
void led_task(void *arg)
{
    //LED1对象初始化
    led_obj_init(&led1Obj, LED1_GPIO_PORT, LED1_GPIO_PIN, LED1_GPIO_MUX_PIN_SOURCE, LED1_GPIO_MUX, LED1_TMR, LED1_TMR_PULSE,
        LED1_TMR_BREATH_ARR, LED1_TMR_BLINK_ARR, 500, LED1_POLARITY, LED1_BREATH_STEP_MIN, LED1_BREATH_STEP_MAX);
    led_set_breath_step(&led1Obj, 20);
    led_set_blink_num(&led1Obj, 300);
    led_set_mode(&led1Obj, LED_MODE_BREATH);
    //led_set_mode(&led1Obj, LED_MODE_BLINK);
    
    //LED2对象初始化
    led_obj_init(&led2Obj, LED2_GPIO_PORT, LED2_GPIO_PIN, (gpio_pins_source_type)0, (gpio_mux_sel_type)0, NULL, 0, 0, 0, 0, LED2_POLARITY, 0, 0);
    led_set_mode(&led2Obj, LED_MODE_SWITCH);
    led_off(&led2Obj);
    
    while(1)
    {
        vTaskDelay(10);
    }
}


/**
 * @brief   LED定时器中断服务函数
 * @param   None
 * @retval  None
 * @note    不可调用
*/
void LED1_TMR_IRQ_HANDLER(void)
{
    if(tmr_interrupt_flag_get(LED1_TMR, TMR_OVF_FLAG))
    {
        led_tmr_irq_handler(&led1Obj);
        
        tmr_flag_clear(LED1_TMR, TMR_OVF_FLAG);
    }
}
