// File Name: pwm_interfce.h
// Author: kukukukuku~
// Date: 2025-03-07

#ifndef __PWM_INTERFACE_H
#define __PWM_INTERFACE_H

#include "./PWM/inc/pwm_macros.h"

//PWM对象
typedef struct PWM_OBJ_T
{
    tmr_type *tmr;
    tmr_channel_select_type tmrChannel;
    uint32_t freq;
    uint8_t duty;
}PWM_OBJ_T, *pPWM_OBJ_T;


/**
 * @brief   PWM对象初始化
 * @param   pwmObj:     PWM对象
 * @param   tmr:        定时器
 * @param   tmrChannel: 定时器通道
 * @retval  None
 * @note    None
*/
void pwm_obj_init(PWM_OBJ_T * const pwmObj, tmr_type * const tmr, const tmr_channel_select_type tmrChannel);


/**
 * @brief   PWM设置频率
 * @param   pwmObj:     PWM对象
 * @param   freq:       PWM频率
 * @retval  None
 * @note    freq >= 16 且 freq <= 100000
*/
void pwm_set_freq(PWM_OBJ_T * const pwmObj, const uint32_t freq);


/**
 * @brief   PWM设置占空比
 * @param   pwmObj:     PWM对象
 * @param   duty:       PWM占空比
 * @retval  None
 * @note    None
*/
void pwm_set_duty(PWM_OBJ_T * const pwmObj, const uint8_t duty);


/**
 * @brief   PWM更新频率和占空比
 * @param   pwmObj:     PWM对象
 * @retval  None
 * @note    None
*/
void pwm_freq_duty_update(PWM_OBJ_T * const pwmObj);

#endif //__PWM_INTERFACE_H
