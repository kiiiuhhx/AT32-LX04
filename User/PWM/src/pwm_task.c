// File Name: pwm_task.c
// Author: kukukukuku~
// Date: 2025-03-07

#include "./PWM/inc/pwm_task.h"

//PWM任务句柄
TaskHandle_t pwmTaskHandle;


/**
 * @brief   PWM任务
 * @param   arg:    任务参数
 * @retval  None
 * @note    None
*/
void pwm_task(void *arg)
{
    volatile static UBaseType_t pwmTaskStackHighWaterMark;
    
    //PWM对象初始化
    pwm_obj_init(&pwmObj, PWM_TMR, PWM_TMR_CHANNEL);
    
    pwm_set_freq(&pwmObj, 10000);
    pwm_set_duty(&pwmObj, 50);
    
    pwm_freq_duty_update(&pwmObj);
    
    while(1)
    {
        pwmTaskStackHighWaterMark = uxTaskGetStackHighWaterMark(NULL);
        
        vTaskDelay(10);
    }
}
