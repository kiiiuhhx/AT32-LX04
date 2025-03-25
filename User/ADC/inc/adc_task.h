// File Name: adc_task.h
// Author: kukukukuku~
// Date: 2025-03-06

#ifndef __ADC_TASK_H
#define __ADC_TASK_H

#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"

#include "./ADC/inc/adc_user.h"

//ADC任务句柄
extern TaskHandle_t adcTaskHandle;


/**
 * @brief   ADC任务
 * @param   arg:    任务参数
 * @retval  None
 * @note    None
*/
void adc_task(void *arg);

#endif //__ADC_TASK_H
