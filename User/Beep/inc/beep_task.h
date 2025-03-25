// File Name: beep_task.h
// Author: kukukukuku~
// Date: 2025-03-06

#ifndef __BEEP_TASK_H
#define __BEEP_TASK_H

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "./Beep/inc/beep_user.h"

//Beep任务句柄
extern TaskHandle_t beepTaskHandle;


/**
 * @brief   Beep任务
 * @param   arg:    任务参数
 * @retval  None
 * @note    None
*/
void beep_task(void *arg);

#endif //__BEEP_TASK_H
