// File Name: adc_task.c
// Author: kukukukuku~
// Date: 2025-03-06

#include "./ADC/inc/adc_task.h"
#include "./LCD/inc/lcd_user.h"

//ADC任务句柄
TaskHandle_t adcTaskHandle;


/**
 * @brief   ADC任务
 * @param   arg:    任务参数
 * @retval  None
 * @note    None
*/
void adc_task(void *arg)
{
    volatile static UBaseType_t adcTaskStackHighWaterMark;
    
    //ADC对象初始化
    adc_obj_init(&adcObj, ADC_DMA_CHANNEL, ADC_ADC, adcChannel, ADC_SLIDE_FILTER_DATA_NUM, ADC_AVERAGE_FILTER_DATA_NUM);
    
    adc_start(&adcObj);
    
    while(1)
    {
        //滑动算术平均滤波
        adc_data_filter(&adcObj);
        
        if(adcObj.state)
        {
            //计算温度值
            temperature = (1.28 - adcObj.averageFilterData[ADC_CHANNEL_TEMPERATURE_INDEX] * 3.3 / 4095) / (-0.0043) + 25;
            
            //计算外部参考电压
            externalVref = 1.2 * 4095 / adcObj.averageFilterData[ADC_CHANNEL_VREF_INDEX];
        }
        else
        {
            temperature = 99999;
            externalVref = -1;
        }
        
        //ADC数据更新，给予LCD更新显示信号量
        if(lcdPage.mainId == 0 && lcdObj.updateSemaphoreHandle)
            xSemaphoreGive(lcdObj.updateSemaphoreHandle);
        
        adcTaskStackHighWaterMark = uxTaskGetStackHighWaterMark(NULL);
        
        vTaskDelay(10);
    }
}
