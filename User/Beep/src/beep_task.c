// File Name: beep_task.c
// Author: kukukukuku~
// Date: 2025-03-06

#include "./Beep/inc/beep_task.h"

//Beep任务句柄
TaskHandle_t beepTaskHandle;


/**
 * @brief   Beep任务
 * @param   arg:    任务参数
 * @retval  None
 * @note    None
*/
void beep_task(void *arg)
{
    volatile static UBaseType_t beepTaskStackHighWaterMark;
    
    //Beep对象初始化
    beep_obj_init(&beepObj, BEEP_GPIO_PORT, BEEP_GPIO_PIN, BEEP_TMR, BEEP_POLARITY_HIGH);
    
    beep_ring(&beepObj, 50);
    
    while(1)
    {
        beepTaskStackHighWaterMark = uxTaskGetStackHighWaterMark(NULL);
        
        vTaskDelay(10);
    }
}


/**
 * @brief   Beep定时器中断服务函数
 * @param   None
 * @retval  None
 * @note    不可调用
*/
void BEEP_TMR_IRQ_HANDLER(void)
{
    //1ms
    if(tmr_interrupt_flag_get(BEEP_TMR, TMR_OVF_FLAG))
    {
        beep_tmr_irq_handler(&beepObj);
        
        tmr_flag_clear(BEEP_TMR, TMR_OVF_FLAG);
    }
}
