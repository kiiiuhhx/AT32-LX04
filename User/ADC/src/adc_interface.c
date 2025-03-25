// File Name: adc_interface.c
// Author: kukukukuku~
// Date: 2025-03-06

#include "stddef.h"
#include "string.h"

#include "./ADC/inc/adc_interface.h"

/**
 * @brief   ADC对象初始化
 * @param   adcObj:                 ADC对象
 * @param   dmaChannel:             DMA通道
 * @param   adc:                    ADC外设
 * @param   adcChannel:             ADC通道
 * @param   slideFilterDataNum:     滑动滤波数据样本数量
 * @param   averageFilterDataNum:   算术平均滤波数据样本数量
 * @retval  None
 * @note    None
*/
void adc_obj_init(ADC_OBJ_T * const adcObj, dma_channel_type * const dmaChannel, adc_type * const adc,
    adc_channel_select_type * const adcChannel, const uint8_t slideFilterDataNum, const uint8_t averageFilterDataNum)
{
    if(adcObj == NULL || dmaChannel == NULL || adc == NULL || adcChannel == NULL)
        return;
    
    memset(adcObj, 0, sizeof(ADC_OBJ_T));
    adcObj->dmaChannel           = dmaChannel;
    adcObj->adc                  = adc;
    adcObj->adcChannel           = adcChannel;
    adcObj->slideFilterDataNum   = slideFilterDataNum < 5 ? 5 : slideFilterDataNum;
    adcObj->averageFilterDataNum = averageFilterDataNum < 3 ? 3 : averageFilterDataNum;
}


/**
 * @brief   启动ADC测量
 * @param   adcObj: ADC对象
 * @retval  None
 * @note    None
*/
void adc_start(ADC_OBJ_T * const adcObj)
{
    if(adcObj == NULL || adcObj->dmaChannel == NULL || adcObj->adc == NULL || adcObj->adcChannel == NULL)
        return;
    
    dma_channel_enable(adcObj->dmaChannel, FALSE);
    dma_data_number_set(adcObj->dmaChannel, ADC_CHANNEL_NUM);
    dma_channel_enable(adcObj->dmaChannel, TRUE);
    adc_ordinary_software_trigger_enable(adcObj->adc, TRUE);
    
    adcObj->state = 1;
}


/**
 * @brief   停止ADC测量
 * @param   adcObj: ADC对象
 * @retval  None
 * @note    None
*/
void adc_stop(ADC_OBJ_T * const adcObj)
{
    if(adcObj == NULL || adcObj->dmaChannel == NULL || adcObj->adc == NULL || adcObj->adcChannel == NULL)
        return;

    adc_ordinary_software_trigger_enable(adcObj->adc, FALSE);
    dma_channel_enable(adcObj->dmaChannel, FALSE);
    
    adcObj->state = 0;
}


/**
 * @brief   ADC数据滤波处理
 * @param   adcObj: ADC对象
 * @retval  None
 * @note    None
*/
void adc_data_filter(ADC_OBJ_T * const adcObj)
{
    static uint8_t sampleEnoughFlag;
    static uint8_t slideSampleIndex;
    static uint8_t averageSampleIndex;
    
    if(adcObj == NULL || adcObj->dmaChannel == NULL || adcObj->adc == NULL || adcObj->adcChannel == NULL)
        return;
    
    if(adcObj->state == 0)
    {
        sampleEnoughFlag = 0;
        slideSampleIndex = 0;
        averageSampleIndex = 0;
    }
    else
    {
        //滑动滤波
        //样本数量足够
        if(sampleEnoughFlag)
        {
            //先减去平均值再滑动
            for(uint8_t i = 0; i < ADC_CHANNEL_NUM; i++)
            {
                adcObj->slideSumData[i] -= adcObj->slideSumData[i] / adcObj->slideFilterDataNum;
                adcObj->slideSumData[i] += adcObj->rawData[i];
                adcObj->slideFilterData[i] = adcObj->slideSumData[i] / adcObj->slideFilterDataNum;
                adcObj->averageSumData[i] += adcObj->slideFilterData[i];
            }
        }
        //样本数量不足
        else
        {
            if(++slideSampleIndex >= adcObj->slideFilterDataNum)
                sampleEnoughFlag = 1;
            
            //直接滑动
            for(uint8_t i = 0; i < ADC_CHANNEL_NUM; i++)
            {
                adcObj->slideSumData[i] += adcObj->rawData[i];
                adcObj->slideFilterData[i] = adcObj->slideSumData[i] / slideSampleIndex;
                adcObj->averageSumData[i] += adcObj->slideFilterData[i];
            }
        }
        
        //根据滑动滤波的数据，进行算术平均滤波
        if(++averageSampleIndex >= adcObj->averageFilterDataNum)
        {
            averageSampleIndex = 0;
            
            for(uint8_t i = 0; i < ADC_CHANNEL_NUM; i++)
            {
                adcObj->averageFilterData[i] = adcObj->averageSumData[i] / adcObj->averageFilterDataNum;
                adcObj->averageSumData[i] = 0;
            }
        }
    }
}
