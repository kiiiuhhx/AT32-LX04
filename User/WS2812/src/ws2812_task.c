// File Name: ws2812_task.c
// Author: kukukukuku~
// Date: 2025-03-06

#include "./WS2812/inc/ws2812_task.h"

//WS2812任务句柄
TaskHandle_t ws2812TaskHandle;


/**
 * @brief   WS2812任务
 * @param   arg:    任务参数
 * @retval  None
 * @note    None
*/
void ws2812_task(void *arg)
{
    volatile static UBaseType_t ws2812TaskStackHighWaterMark;
    
    //WS2812信号时序对象初始化
    ws2812_timing_obj_init(&ws2812TimingObj, WS2812_SPI, WS2812_HIGH, WS2812_LOW, WS2812_RESET);
    
    //WS2812对象初始化
    ws2812_obj_init(&ws2812Obj, &ws2812TimingObj);
    
    while(1)
    {
        ws2812_set_red(&ws2812Obj, ws2812Red, WS2812_0_INDEX);
        ws2812_set_green(&ws2812Obj, ws2812Green, WS2812_0_INDEX);
        ws2812_set_blue(&ws2812Obj, ws2812Blue, WS2812_0_INDEX);
        ws2812_refresh(&ws2812Obj);
        
        switch(ws2812Flag)
        {
            case 0:
                if(++ws2812Red >= 16)
                    ws2812Flag++;
                break;
            case 1:
                if(--ws2812Red <= 0)
                    ws2812Flag++;
                break;
            case 2:
                if(++ws2812Red >= 16)
                    ws2812Flag++;
                ws2812Green = ws2812Red;
                break;
            case 3:
                if(--ws2812Red <= 0)
                    ws2812Flag++;
                ws2812Green = ws2812Red;
                break;
            case 4:
                if(++ws2812Green >= 16)
                    ws2812Flag++;
                break;
            case 5:
                if(--ws2812Green <= 0)
                    ws2812Flag++;
                break;
            case 6:
                if(++ws2812Green >= 16)
                    ws2812Flag++;
                ws2812Blue = ws2812Green;
                break;
            case 7:
                if(--ws2812Green <= 0)
                    ws2812Flag++;
                ws2812Blue = ws2812Green;
                break;
            case 8:
                if(++ws2812Blue >= 16)
                    ws2812Flag++;
                break;
            case 9:
                if(--ws2812Blue <= 0)
                    ws2812Flag++;
                break;
            case 10:
                if(++ws2812Blue >= 16)
                    ws2812Flag++;
                ws2812Red = ws2812Blue;
                break;
            case 11:
                if(--ws2812Blue <= 0)
                    ws2812Flag = 0;
                ws2812Red = ws2812Blue;
                break;
            default:
                break;
        }
        
        ws2812TaskStackHighWaterMark = uxTaskGetStackHighWaterMark(NULL);
        
        vTaskDelay(100);
    }
}
