// File Name: pwm_task.h
// Author: kukukukuku~
// Date: 2025-03-07

#ifndef __PWM_TASK_H
#define __PWM_TASK_H

#include "FreeRTOS.h"
#include "task.h"

#include "./PWM/inc/pwm_user.h"

//PWM任务句柄
extern TaskHandle_t pwmTaskHandle;


/**
 * @brief   PWM任务
 * @param   arg:    任务参数
 * @retval  None
 * @note    None
*/
void pwm_task(void *arg);

#endif //__PWM_TASK_H
