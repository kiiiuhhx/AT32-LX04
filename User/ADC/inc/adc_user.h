// File Name: adc_user.h
// Author: kukukukuku~
// Date: 2025-03-06

#ifndef __ADC_USER_H
#define __ADC_USER_H

#include "./ADC/inc/adc_interface.h"

//ADC通道
extern adc_channel_select_type adcChannel[ADC_CHANNEL_NUM];

//ADC对象
extern ADC_OBJ_T adcObj;

//温度值
extern double temperature;

//外部参考电压
extern double externalVref;

#endif //__ADC_USER_H
