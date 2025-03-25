// File Name: beep_interface.h
// Author: kukukukuku~
// Date: 2025-03-06

#ifndef __BEEP_INTERFACE_H
#define __BEEP_INTERFACE_H

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "./Beep/inc/beep_macros.h"

//Beep响铃电平
typedef enum BEEP_POLARITY_T
{
    BEEP_POLARITY_LOW = 0,
    BEEP_POLARITY_HIGH,
}BEEP_POLARITY_T, *pBEEP_POLARITY_T;

//Beep对象
typedef struct BEEP_OBJ_T
{
    gpio_type *gpioPort;
    uint32_t gpioPin;
    tmr_type *tmr;
    uint16_t delayCnt;
    uint16_t delayNum;
    BEEP_POLARITY_T polarity;
}BEEP_OBJ_T, *pBEEP_OBJ_T;


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
    tmr_type * const tmr, const BEEP_POLARITY_T polarity);


/**
 * @brief   Beep响铃
 * @param   beepObj:    Beep对象
 * @param   ringTime:   响铃时间
 * @retval  None
 * @note    None
*/
void beep_ring(BEEP_OBJ_T * const beepObj, const uint16_t ringTime);


/**
 * @brief   Beep定时器中断处理
 * @param   beepObj:    Beep对象
 * @retval  None
 * @note    只能在Beep定时器中断服务函数中调用
*/
void beep_tmr_irq_handler(BEEP_OBJ_T * const beepObj);

#endif // __BEEP_INTERFACE_H
