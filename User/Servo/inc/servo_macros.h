// File Name: servo_macros.h
// Author: kukukukuku~
// Date: 2025-03-07

#ifndef __SERVO_MACROS_H
#define __SERVO_MACROS_H

#include "at32f421.h"

#define SERVO_GPIO_PORT                 GPIOA
#define SERVO_GPIO_PIN                  GPIO_PINS_8
#define SERVO_GPIO_MUX_PIN_SOURCE       GPIO_PINS_SOURCE8
#define SERVO_GPIO_MUX                  GPIO_MUX_2


#define SERVO_TMR                       TMR1
#define SERVO_TMR_CHANNEL               TMR_SELECT_CHANNEL_1
#define SERVO_TMR_PSC                   (120 - 1)
#define SERVO_TMR_ARR                   (20000 - 1)
#define SERVO_TMR_PULSE                 (500 - 1)

#endif //__SERVO_MACROS_H
