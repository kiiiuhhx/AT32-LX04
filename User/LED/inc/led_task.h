// File Name: led_task.h
// Author: kukukukuku~
// Date: 2025-03-19

#ifndef __LED_TASK_H
#define __LED_TASK_H

#include "FreeRTOS.h"
#include "task.h"

#include "./LED/inc/led_user.h"

//LED任务句柄
extern TaskHandle_t ledTaskHandle;


/**
 * @brief   LED任务
 * @param   arg:    任务参数
 * @retval  None
 * @note    None
*/
void led_task(void *arg);

#endif //__LED_TASK_H
