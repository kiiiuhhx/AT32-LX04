// File Name: pwm_interface.c
// Author: kukukukuku~
// Date: 2025-03-07

#include "stddef.h"
#include "./PWM/inc/pwm_interface.h"

/**
 * @brief   PWM对象初始化
 * @param   pwmObj:     PWM对象
 * @param   tmr:        定时器
 * @param   tmrChannel: 定时器通道
 * @retval  None
 * @note    None
*/
void pwm_obj_init(PWM_OBJ_T * const pwmObj, tmr_type * const tmr, const tmr_channel_select_type tmrChannel)
{
    if(pwmObj == NULL || tmr == NULL)
        return;
    
    pwmObj->tmr = tmr;
    pwmObj->tmrChannel = tmrChannel;
    pwmObj->freq = 120000000 / ((tmr_div_value_get(pwmObj->tmr) + 1) * (tmr_period_value_get(pwmObj->tmr) + 1));
    pwmObj->duty = (tmr_channel_value_get(pwmObj->tmr, pwmObj->tmrChannel) + 1) * 100 / (tmr_period_value_get(pwmObj->tmr) + 1);
}


/**
 * @brief   PWM设置频率
 * @param   pwmObj:     PWM对象
 * @param   freq:       PWM频率
 * @retval  None
 * @note    freq >= 16 且 freq <= 100000
*/
void pwm_set_freq(PWM_OBJ_T * const pwmObj, const uint32_t freq)
{
    if(pwmObj == NULL || pwmObj->tmr == NULL || freq < 16 || freq > 100000)
        return;
    
    tmr_period_value_set(pwmObj->tmr, 120000000 / ((tmr_div_value_get(pwmObj->tmr) + 1) * freq) - 1);
}


/**
 * @brief   PWM设置占空比
 * @param   pwmObj:     PWM对象
 * @param   duty:       PWM占空比
 * @retval  None
 * @note    None
*/
void pwm_set_duty(PWM_OBJ_T * const pwmObj, const uint8_t duty)
{
    uint32_t dutyValue;
    
    if(pwmObj == NULL || pwmObj->tmr == NULL || duty > 100)
        return;
    
    dutyValue = (tmr_period_value_get(pwmObj->tmr) + 1) * duty / 100;
        
    tmr_channel_value_set(pwmObj->tmr, pwmObj->tmrChannel, dutyValue ? dutyValue - 1 : 0);
}


/**
 * @brief   PWM更新频率和占空比
 * @param   pwmObj:     PWM对象
 * @retval  None
 * @note    None
*/
void pwm_freq_duty_update(PWM_OBJ_T * const pwmObj)
{
    if(pwmObj == NULL || pwmObj->tmr == NULL)
        return;
    
    pwmObj->freq = 120000000 / ((tmr_div_value_get(pwmObj->tmr) + 1) * (tmr_period_value_get(pwmObj->tmr) + 1));
    pwmObj->duty = (tmr_channel_value_get(pwmObj->tmr, pwmObj->tmrChannel) + 1) * 100 / (tmr_period_value_get(pwmObj->tmr) + 1);
}
