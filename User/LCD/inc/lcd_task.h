// File Name: lcd_task.h
// Author: kukukukuku~
// Date: 2025-03-06

#ifndef __LCD_TASK_H
#define __LCD_TASK_H

#include "FreeRTOS.h"
#include "task.h"

#include "./LCD/inc/lcd_user.h"

//LCD任务句柄
extern TaskHandle_t lcdTaskHandle;


/**
 * @brief   LCD任务
 * @param   arg:    任务参数
 * @retval  None
 * @note    None
*/
void lcd_task(void *arg);

#endif //__LCD_TASK_H
