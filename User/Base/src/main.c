// File Name: main.c
// Author: kukukukuku~
// Date: 2025-02-21

#include "FreeRTOS.h"
#include "task.h"
#include "./Base/inc/init.h"

/**
 * @brief   主函数
 * @param   None
 * @retval  退出状态
 * @note    None
*/
int main(void)
{
    system_init();
    
    xTaskCreate(init_task, "init_task", 64, NULL, 1, NULL);
    vTaskStartScheduler();
}
