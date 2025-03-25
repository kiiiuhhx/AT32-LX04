// File Name: adc_user.c
// Author: kukukukuku~
// Date: 2025-03-06

#include "./ADC/inc/adc_user.h"

//ADC通道
adc_channel_select_type adcChannel[ADC_CHANNEL_NUM] = {ADC_ADC_CHANNEL, ADC_TEMPERATURE_CHANNEL, ADC_VREF_CHANNEL};

//ADC对象
ADC_OBJ_T adcObj;

//温度值
double temperature;

//外部参考电压
double externalVref;
