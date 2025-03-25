// File Name: adc_interface.h
// Author: kukukukuku~
// Date: 2025-03-06

#ifndef __ADC_INTERFACE_H
#define __ADC_INTERFACE_H

#include "./ADC/inc/adc_macros.h"

//ADC对象
typedef struct ADC_OBJ_T
{
    dma_channel_type *dmaChannel;
    adc_type *adc;
    adc_channel_select_type *adcChannel;
    uint32_t slideSumData[ADC_CHANNEL_NUM];
    uint32_t averageSumData[ADC_CHANNEL_NUM];
    uint16_t rawData[ADC_CHANNEL_NUM];
    uint16_t slideFilterData[ADC_CHANNEL_NUM];
    uint16_t averageFilterData[ADC_CHANNEL_NUM];
    uint8_t slideFilterDataNum;
    uint8_t averageFilterDataNum;
    uint8_t state;
}ADC_OBJ_T, *pADC_OBJ_T;


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
    adc_channel_select_type * const adcChannel, const uint8_t slideFilterDataNum, const uint8_t averageFilterDataNum);


/**
 * @brief   启动ADC测量
 * @param   adcObj: ADC对象
 * @retval  None
 * @note    None
*/
void adc_start(ADC_OBJ_T * const adcObj);


/**
 * @brief   停止ADC测量
 * @param   adcObj: ADC对象
 * @retval  None
 * @note    None
*/
void adc_stop(ADC_OBJ_T * const adcObj);


/**
 * @brief   ADC数据滤波处理
 * @param   adcObj: ADC对象
 * @retval  None
 * @note    None
*/
void adc_data_filter(ADC_OBJ_T * const adcObj);

#endif //__ADC_INTERFACE_H
