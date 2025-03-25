// File Name: adc_macros.h
// Author: kukukukuku~
// Date: 2025-03-06

#ifndef __ADC_MACROS_H
#define __ADC_MACROS_H

#include "at32f421.h"

#define ADC_GPIO_PORT                   GPIOB
#define ADC_GPIO_PIN                    GPIO_PINS_0


#define ADC_DMA                         DMA1
#define ADC_DMA_CHANNEL                 DMA1_CHANNEL1


#define ADC_ADC                         ADC1
#define ADC_ADC_CHANNEL                 ADC_CHANNEL_8
#define ADC_TEMPERATURE_CHANNEL         ADC_CHANNEL_16
#define ADC_VREF_CHANNEL                ADC_CHANNEL_17


#define ADC_CHANNEL_NUM                 (3)
#define ADC_CHANNEL_ADC_INDEX           (0)
#define ADC_CHANNEL_TEMPERATURE_INDEX   (1)
#define ADC_CHANNEL_VREF_INDEX          (2)


#define ADC_SLIDE_FILTER_DATA_NUM       (5)
#define ADC_AVERAGE_FILTER_DATA_NUM     (3)

#endif //__ADC_MACROS_H
