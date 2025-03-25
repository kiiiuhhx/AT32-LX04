// File Name: servo_interface.c
// Author: kukukukuku~
// Date: 2025-03-10

#include "stddef.h"
#include "./Servo/inc/servo_interface.h"

/**
 * @brief   Servo对象初始化
 * @param   servoObj:   Servo对象
 * @param   tmr:        定时器
 * @param   tmrChannel: 定时器通道
 * @retval  None
 * @note    None
*/
void servo_obj_init(SERVO_OBJ_T * const servoObj, tmr_type * const tmr, const tmr_channel_select_type tmrChannel)
{
    if(servoObj == NULL || tmr == NULL)
        return;
    
    servoObj->tmr        = tmr;
    servoObj->tmrChannel = tmrChannel;
    servoObj->duty       = (tmr_channel_value_get(servoObj->tmr, servoObj->tmrChannel) + 1) * 100 / (tmr_period_value_get(servoObj->tmr) + 1);
    servoObj->degree     = 0;
}


/**
 * @brief   Servo计算角度对应的定时器通道值
 * @param   degree: 舵机角度
 * @retval  舵机角度对应的定时器通道值
 * @note    静态
*/
static uint16_t servo_pulse_calculate(const uint8_t degree)
{
    return degree > 180 ? 2500 - 1 : (uint16_t)(degree * 11.11 + 500.5) - 1;
}


/**
 * @brief   Servo设置角度值
 * @param   servoObj:   Servo对象
 * @param   degree:     Servo角度值
 * @retval  None
 * @note    None
*/
void servo_set_degree(SERVO_OBJ_T * const servoObj, const uint8_t degree)
{
    if(servoObj == NULL || degree > 180)
        return;
    
    tmr_channel_value_set(servoObj->tmr, servoObj->tmrChannel, servo_pulse_calculate(degree));
    
    servoObj->duty = (tmr_channel_value_get(servoObj->tmr, servoObj->tmrChannel) + 1) * 100 / (tmr_period_value_get(servoObj->tmr) + 1);
    servoObj->degree = degree;
}
