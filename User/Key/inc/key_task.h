// File Name: key_task.h
// Author: kukukukuku~
// Date: 2025-03-11

#ifndef __KEY_TASK_H
#define __KEY_TASK_H

#include "FreeRTOS.h"
#include "task.h"

#include "./Key/inc/key_user.h"

//Key任务句柄
extern TaskHandle_t keyTaskHandle;


/**
 * @brief   Key任务
 * @param   arg:    任务参数
 * @retval  None
 * @note    None
*/
void key_task(void *arg);

#endif //__KEY_TASK_H
