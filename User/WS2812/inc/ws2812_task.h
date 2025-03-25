// File Name: ws2812_task.h
// Author: kukukukuku~
// Date: 2025-03-06

#ifndef __WS2812_TASK_H
#define __WS2812_TASK_H

#include "FreeRTOS.h"
#include "task.h"

#include "./WS2812/inc/ws2812_user.h"

//WS2812任务句柄
extern TaskHandle_t ws2812TaskHandle;


/**
 * @brief   WS2812任务
 * @param   arg:    任务参数
 * @retval  None
 * @note    None
*/
void ws2812_task(void *arg);

#endif //__WS2812_TASK_H
