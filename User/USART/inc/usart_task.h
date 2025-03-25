// File Name: usart_task.h
// Author: kukukukuku~
// Date: 2025-03-25

#ifndef __USART_TASK_H
#define __USART_TASK_H

#include "FreeRTOS.h"
#include "task.h"

#include "./USART/inc/usart_user.h"

//USART任务句柄
extern TaskHandle_t usartTaskHandle;


/**
 * @brief   USART任务
 * @param   arg:    任务参数
 * @retval  None
 * @note    None
*/
void usart_task(void *arg);

#endif //__USART_TASK_H
