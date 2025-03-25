// File Name: key_task.c
// Author: kukukukuku~
// Date: 2025-03-11

#include "./Key/inc/key_task.h"

//Key任务句柄
TaskHandle_t keyTaskHandle;


/**
 * @brief   Key任务
 * @param   arg:    任务参数
 * @retval  None
 * @note    None
*/
void key_task(void *arg)
{
    volatile static UBaseType_t keyTaskStackHighWaterMark;
    static uint8_t j = 0;
    
    //Key GPIO对象初始化
    key_gpio_obj_init(&keyGpioObjList[KEY_UP_INDEX], KEY_UP_GPIO_PORT, KEY_UP_GPIO_PIN, KEY_PRESS_POLARITY);
    key_gpio_obj_init(&keyGpioObjList[KEY_DN_INDEX], KEY_DN_GPIO_PORT, KEY_DN_GPIO_PIN, KEY_PRESS_POLARITY);
    key_gpio_obj_init(&keyGpioObjList[KEY_LT_INDEX], KEY_LT_GPIO_PORT, KEY_LT_GPIO_PIN, KEY_PRESS_POLARITY);
    key_gpio_obj_init(&keyGpioObjList[KEY_RT_INDEX], KEY_RT_GPIO_PORT, KEY_RT_GPIO_PIN, KEY_PRESS_POLARITY);
    key_gpio_obj_init(&keyGpioObjList[KEY_MD_INDEX], KEY_MD_GPIO_PORT, KEY_MD_GPIO_PIN, KEY_PRESS_POLARITY);
    
    //Key状态机、Key对象初始化
    for(uint8_t i = 0; i < KEY_NUM; i++)
    {
        key_fsm_obj_init(&keyFsmObjList[i], keyFsmTableList[i], KEY_FSM_TABLE_SIZE);
        key_obj_init(&keyObjList[i], &keyGpioObjList[i], KEY_TMR, &keyFsmObjList[i], KEY_LONG_PRESS_JUDGE_NUM);
    }
    
    while(1)
    {
        //Key扫描、状态机处理
        for(uint8_t i = 0; i < KEY_NUM; i++)
        {
            key_scan(&keyObjList[i]);
            key_fsm_event_handle(&keyObjList[i]);
        }
        
        //检查是否要关闭Key定时器
        for(j = 0; j < KEY_NUM; j++)
        {
            if(keyObjList[j].pressFlag)
                break;
        }
        
        if(j == KEY_NUM)
            key_tmr_stop(&keyObjList[0]);
        
        keyTaskStackHighWaterMark = uxTaskGetStackHighWaterMark(NULL);
        
        vTaskDelay(10);
    }
}


/**
 * @brief   Key定时器中断服务函数
 * @param   None
 * @retval  None
 * @note    不可调用
*/
void KEY_TMR_IRQ_HANDLER(void)
{
    if(tmr_interrupt_flag_get(KEY_TMR, TMR_OVF_FLAG))
    {
        for(uint8_t i = 0; i < KEY_NUM; i++)
        {
            key_long_press_scan(&keyObjList[i]);
        }
        
        tmr_flag_clear(KEY_TMR, TMR_OVF_FLAG);
    }
}
