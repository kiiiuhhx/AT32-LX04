// File Name: servo_interface.h
// Author: kukukukuku~
// Date: 2025-03-10

#ifndef __SERVO_INTERFACE_H
#define __SERVO_INTERFACE_H

#include "./Servo/inc/servo_macros.h"

//Servo对象
typedef struct SERVO_OBJ_T
{
    tmr_type *tmr;
    tmr_channel_select_type tmrChannel;
    uint8_t duty;
    uint8_t degree;
}SERVO_OBJ_T, *pSERVO_OBJ_T;


/**
 * @brief   Servo对象初始化
 * @param   servoObj:   Servo对象
 * @param   tmr:        定时器
 * @param   tmrChannel: 定时器通道
 * @retval  None
 * @note    None
*/
void servo_obj_init(SERVO_OBJ_T * const servoObj, tmr_type * const tmr, const tmr_channel_select_type tmrChannel);


/**
 * @brief   Servo设置角度值
 * @param   servoObj:   Servo对象
 * @param   degree:     Servo角度值
 * @retval  None
 * @note    None
*/
void servo_set_degree(SERVO_OBJ_T * const servoObj, const uint8_t degree);

#endif //__SERVO_INTERFACE_H
