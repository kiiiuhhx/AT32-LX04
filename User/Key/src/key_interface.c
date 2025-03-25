// File Name: key_interface.c
// Author: kukukukuku~
// Date: 2025-03-11

#include "stddef.h"
#include "string.h"

#include "FreeRTOS.h"
#include "task.h"

#include "./Beep/inc/beep_user.h"
#include "./Key/inc/key_interface.h"
#include "./LCD/inc/lcd_user.h"

/**
 * @brief   Key GPIO对象初始化
 * @param   keyGpioObj:     Key GPIO对象
 * @param   port:           GPIO端口
 * @param   pin:            GPIO引脚
 * @param   pressPolarity:  Key按下时的电平
 * @retval  None
 * @note    None
*/
void key_gpio_obj_init(KEY_GPIO_OBJ_T * const keyGpioObj, gpio_type * const port, const uint32_t pin, const uint8_t pressPolarity)
{
    if(keyGpioObj == NULL || port == NULL)
        return;
    
    memset(keyGpioObj, 0, sizeof(KEY_GPIO_OBJ_T));
    keyGpioObj->port          = port;
    keyGpioObj->pin           = pin;
    keyGpioObj->pressPolarity = pressPolarity;
}


/**
 * @brief   Key状态机对象初始化
 * @param   keyFsmObj:          Key状态机对象
 * @param   keyFsmTable:        Key状态机表
 * @param   keyFsmTableSize:    Key状态机表项数
 * @retval  None
 * @note    None
*/
void key_fsm_obj_init(KEY_FSM_OBJ_T * const keyFsmObj, KEY_FSM_TABLE_T * const keyFsmTable, const uint16_t keyFsmTableSize)
{
    if(keyFsmObj == NULL || keyFsmTable == NULL || keyFsmTableSize == 0)
        return;
    
    memset(keyFsmObj, 0, sizeof(KEY_FSM_OBJ_T));
    keyFsmObj->table        = keyFsmTable;
    keyFsmObj->tableSize    = keyFsmTableSize;
    keyFsmObj->currentState = KEY_FSM_STATE_IDLE;
}



/**
 * @brief   Key对象初始化
 * @param   keyObj:             Key对象
 * @param   keyGpioObj:         Key GPIO对象
 * @param   tmr:                定时器
 * @param   keyFsmObj:          Key状态机对象
 * @param   longPressJudgeNum:  长按判定时间
 * @retval  None
 * @note    None
*/
void key_obj_init(KEY_OBJ_T * const keyObj, KEY_GPIO_OBJ_T * const keyGpioObj, tmr_type * const tmr, KEY_FSM_OBJ_T * const keyFsmObj, const uint16_t longPressJudgeNum)
{
    if(keyObj == NULL || keyGpioObj == NULL || keyFsmObj == NULL || longPressJudgeNum == 0)
        return;
    
    memset(keyObj, 0, sizeof(KEY_OBJ_T));
    keyObj->gpio              = keyGpioObj;
    keyObj->tmr               = tmr;
    keyObj->fsm               = keyFsmObj;
    keyObj->event             = KEY_FSM_EVENT_IDLE;
    keyObj->longPressJudgeNum = longPressJudgeNum;
}


/**
 * @brief   Key毫秒级延时
 * @param   ms: 延时时间
 * @retval  None
 * @note    静态
*/
static void key_delay_ms(const uint16_t ms)
{
    if(ms == 0)
        return;
    
    vTaskDelay(ms);
}


/**
 * @brief   启动Key定时器
 * @param   keyObj: Key对象
 * @retval  None
 * @note    静态
*/
static void key_tmr_start(KEY_OBJ_T * const keyObj)
{
    if(keyObj == NULL || keyObj->tmr == NULL)
        return;
    
    if(keyObj->tmr->ctrl1_bit.tmren == 0)
        tmr_counter_enable(keyObj->tmr, TRUE);
}


/**
 * @brief   关闭Key定时器
 * @param   keyObj: Key对象
 * @retval  None
 * @note    None
*/
void key_tmr_stop(KEY_OBJ_T * const keyObj)
{
    if(keyObj == NULL || keyObj->tmr == NULL)
        return;
    
    if(keyObj->tmr->ctrl1_bit.tmren == 1)
    {
        tmr_counter_enable(keyObj->tmr, FALSE);
        tmr_counter_value_set(keyObj->tmr, 0);
    }
}


/**
 * @brief   Key扫描
 * @param   keyObj: Key对象
 * @retval  None
 * @note    None
*/
void key_scan(KEY_OBJ_T * const keyObj)
{
    if(keyObj == NULL || keyObj->gpio == NULL || keyObj->tmr == NULL || keyObj->fsm == NULL)
        return;
    
    //按键按下
    if(keyObj->pressFlag == 0 && gpio_input_data_bit_read(keyObj->gpio->port, keyObj->gpio->pin) == keyObj->gpio->pressPolarity)
    {
        key_delay_ms(15);
        if(gpio_input_data_bit_read(keyObj->gpio->port, keyObj->gpio->pin) == keyObj->gpio->pressPolarity)
        {
            keyObj->pressFlag = 1;
            
            //开始长按扫描
            keyObj->longPressJudgeCnt = 0;
            keyObj->longPressScanFlag = 1;
            key_tmr_start(keyObj);
            
            //按键按下事件
            keyObj->event = KEY_FSM_EVENT_PRESS;
        }
    }
    //按键松开
    else if(keyObj->pressFlag && gpio_input_data_bit_read(keyObj->gpio->port, keyObj->gpio->pin) != keyObj->gpio->pressPolarity)
    {
        keyObj->pressFlag = 0;
        keyObj->longPressScanFlag = 0;
        keyObj->longPressJudgeCnt = 0;
        
        //按键松开事件
        keyObj->event = KEY_FSM_EVENT_RELEASE;
        
        //短按松开响铃
        if(keyObj->fsm->currentState == KEY_FSM_STATE_PRESS)
            beep_ring(&beepObj, KEY_PRESS_BEEP_RING_TIME);
    }
    
    //按键状态发生变化，给予LCD更新显示信号量
    if(keyObj->pressFlagLast != keyObj->pressFlag)
    {
        if(lcdObj.updateSemaphoreHandle)
            xSemaphoreGive(lcdObj.updateSemaphoreHandle);
        
        keyObj->pressFlagLast = keyObj->pressFlag;
    }
}


/**
 * @brief   Key长按扫描
 * @param   keyObj: Key对象
 * @retval  None
 * @note    只能在Key定时器中断服务函数中调用
*/
void key_long_press_scan(KEY_OBJ_T * const keyObj)
{
    if(keyObj == NULL || keyObj->gpio == NULL || keyObj->tmr == NULL || keyObj->fsm == NULL)
        return;
    
    if(keyObj->longPressScanFlag)
    {
        if(++keyObj->longPressJudgeCnt >= keyObj->longPressJudgeNum)
        {
            keyObj->longPressScanFlag = 0;
            keyObj->longPressJudgeCnt = 0;
            
            //长按事件
            keyObj->event = KEY_FSM_EVENT_LONG_PRESS;
            
            //长按松开响铃
            beep_ring(&beepObj, KEY_LONG_PRESS_BEEP_RING_TIME);
        }
    }
}


/**
 * @brief   Key状态机事件处理
 * @param   keyObj: Key对象
 * @retval  None
 * @note    None
*/
void key_fsm_event_handle(KEY_OBJ_T * const keyObj)
{
    static KEY_FSM_EVENT_T event;
        
    if(keyObj == NULL || keyObj->gpio == NULL || keyObj->tmr == NULL || keyObj->fsm == NULL)
        return;
    
    event = keyObj->event;
    
    for(uint16_t i = 0; i < keyObj->fsm->tableSize; i++)
    {
        if(event == keyObj->fsm->table[i].event && keyObj->fsm->currentState == keyObj->fsm->table[i].state)
        {
            keyObj->fsm->currentState = keyObj->fsm->table[i].nextState;
            
            if(keyObj->fsm->table[i].action)
                keyObj->fsm->table[i].action();
            
            break;
        }
    }
    
    //事件没在处理中被改变 或 当前事件不是长按保持事件，切换到空闲事件
    if(event == keyObj->event && event != KEY_FSM_EVENT_LONG_PRESS_KEEP)
        keyObj->event = KEY_FSM_EVENT_IDLE;
}
