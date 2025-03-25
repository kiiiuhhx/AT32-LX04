// File Name: pwm_macros.h
// Author: kukukukuku~
// Date: 2025-03-07

#ifndef __PWM_MACROS_H
#define __PWM_MACROS_H

#include "at32f421.h"

#define PWM_GPIO_PORT                           GPIOB
#define PWM_GPIO_PIN                            GPIO_PINS_1
#define PWM_GPIO_MUX_PIN_SOURCE                 GPIO_PINS_SOURCE1
#define PWM_GPIO_MUX                            GPIO_MUX_0


#define PWM_TMR                                 TMR14
#define PWM_TMR_CHANNEL                         TMR_SELECT_CHANNEL_1
#define PWM_TMR_PSC                             (120 - 1)
#define PWM_TMR_ARR                             (100 - 1)
#define PWM_TMR_PULSE                           (10 - 1)

#endif //__PWM_MACROS_H
