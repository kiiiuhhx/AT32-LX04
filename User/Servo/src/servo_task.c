// File Name: servo_task.c
// Author: kukukukuku~
// Date: 2025-03-10

#include "./LCD/inc/lcd_user.h"
#include "./Servo/inc/servo_task.h"

//Servo任务句柄
TaskHandle_t servoTaskHandle;


/**
 * @brief   Servo任务
 * @param   arg:    任务参数
 * @retval  None
 * @note    None
*/
void servo_task(void *arg)
{
    volatile static UBaseType_t servoTaskStackHighWaterMark;
    
    static uint8_t servoDegreeLast = 0xFF;
    
    //Servo对象初始化
    servo_obj_init(&servoObj, SERVO_TMR, SERVO_TMR_CHANNEL);
    
    servo_set_degree(&servoObj, 90);
    servoDegree = 90;              
    
    while(1)
    {
        //Servo角度更新，给予LCD更新显示信号量
        if(servoDegreeLast != servoDegree)
        {
            servo_set_degree(&servoObj, servoDegree);
            servoDegreeLast = servoDegree;
            
            if(lcdPage.mainId == 1 && lcdObj.updateSemaphoreHandle)
                xSemaphoreGive(lcdObj.updateSemaphoreHandle);
        }
        
        servoTaskStackHighWaterMark = uxTaskGetStackHighWaterMark(NULL);
        
        vTaskDelay(10);
    }
}
