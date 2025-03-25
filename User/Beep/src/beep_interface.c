// File Name: beep_interface.c
// Author: kukukukuku~
// Date: 2025-03-06

#include "stddef.h"

#include "./Beep/inc/beep_interface.h"

/**
 * @brief   Beep对象初始化
 * @param   beepObj:    Beep对象
 * @param   gpioPort:   GPIO端口
 * @param   gpioPin:    GPIO引脚
 * @param   tmr:        定时器
 * @param   polarity:   响铃时的电平
 * @retval  None
 * @note    None
*/
void beep_obj_init(BEEP_OBJ_T * const beepObj, gpio_type * const gpioPort, const uint32_t gpioPin,
    tmr_type * const tmr, const BEEP_POLARITY_T polarity)
{
    if(beepObj == NULL)
        return;
    
    beepObj->gpioPort    = gpioPort;
    beepObj->gpioPin     = gpioPin;
    beepObj->tmr         = tmr;
    beepObj->delayCnt    = 0;
    beepObj->delayNum    = 0;
    beepObj->polarity    = polarity;
}


/**
 * @brief   Beep使能
 * @param   beepObj:    Beep对象
 * @retval  None
 * @note    静态
*/
static void beep_enable(BEEP_OBJ_T * const beepObj)
{
    if(beepObj == NULL || beepObj->gpioPort == NULL)
        return;
    
    gpio_bits_write(beepObj->gpioPort, beepObj->gpioPin, beepObj->polarity == BEEP_POLARITY_LOW ? FALSE : TRUE);
}


/**
 * @brief   Beep失能
 * @param   beepObj:    Beep对象
 * @retval  None
 * @note    静态
*/
static void beep_disable(BEEP_OBJ_T * const beepObj)
{
    if(beepObj == NULL || beepObj->gpioPort == NULL)
        return;

    gpio_bits_write(beepObj->gpioPort, beepObj->gpioPin, beepObj->polarity == BEEP_POLARITY_LOW ? TRUE : FALSE);
}


/**
 * @brief   启动Beep定时器
 * @param   beepObj:    Beep对象
 * @retval  None
 * @note    静态
*/
static void beep_tmr_enable(BEEP_OBJ_T * const beepObj)
{
    if(beepObj == NULL || beepObj->tmr == NULL)
        return;

    if(beepObj->tmr->ctrl1_bit.tmren == 0)
        tmr_counter_enable(beepObj->tmr, TRUE);
}


/**
 * @brief   关闭Beep定时器
 * @param   beepObj:    Beep对象
 * @retval  None
 * @note    静态
*/
static void beep_tmr_diasble(BEEP_OBJ_T * const beepObj)
{
    if(beepObj == NULL || beepObj->tmr == NULL)
        return;

    if(beepObj->tmr->ctrl1_bit.tmren == 1)
    {
        tmr_counter_enable(beepObj->tmr, FALSE);
        tmr_counter_value_set(BEEP_TMR, 0);
    }
}


/**
 * @brief   Beep响铃
 * @param   beepObj:    Beep对象
 * @param   ringTime:   响铃时间
 * @retval  None
 * @note    None
*/
void beep_ring(BEEP_OBJ_T * const beepObj, const uint16_t ringTime)
{
    if(beepObj == NULL || beepObj->tmr == NULL)
        return;
    
    beep_tmr_diasble(beepObj);
    
    beepObj->delayCnt = 0;
    beepObj->delayNum = ringTime;
    
    beep_enable(beepObj);
    beep_tmr_enable(beepObj);
}


/**
 * @brief   Beep定时器中断处理
 * @param   beepObj:    Beep对象
 * @retval  None
 * @note    只能在Beep定时器中断服务函数中调用
*/
void beep_tmr_irq_handler(BEEP_OBJ_T * const beepObj)
{
    if(beepObj == NULL || beepObj->tmr == NULL)
        return;

    if(++beepObj->delayCnt >= beepObj->delayNum)
    {
        beep_disable(beepObj);
        beep_tmr_diasble(beepObj);
    }
}
