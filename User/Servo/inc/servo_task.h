// File Name: servo_task.h
// Author: kukukukuku~
// Date: 2025-03-10

#ifndef __SERVO_TASK_H
#define __SERVO_TASK_H

#include "FreeRTOS.h"
#include "task.h"

#include "./Servo/inc/servo_user.h"

//Servo任务句柄
extern TaskHandle_t servoTaskHandle;


/**
 * @brief   Servo任务
 * @param   arg:    任务参数
 * @retval  None
 * @note    None
*/
void servo_task(void *arg);

#endif //__SERVO_TASK_H
