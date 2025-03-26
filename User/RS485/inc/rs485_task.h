// File Name: rs485_task.h
// Author: kukukukuku~
// Date: 2025-03-26

#ifndef __RS485_TASK_H
#define __RS485_TASK_H

#include "FreeRTOS.h"
#include "task.h"

#include "./RS485/inc/rs485_user.h"

//RS485任务句柄
extern TaskHandle_t rs485TaskHandle;


/**
 * @brief   RS485任务
 * @param   arg:    任务参数
 * @retval  None
 * @note    None
*/
void rs485_task(void *arg);

#endif //__RS485_TASK_H
