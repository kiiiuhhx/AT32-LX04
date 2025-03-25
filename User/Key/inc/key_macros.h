// File Name: key_macros.h
// Author: kukukukuku~
// Date: 2025-03-11

#ifndef __KEY_MACROS_H
#define __KEY_MACROS_H

#include "at32f421.h"

#define KEY_UP_GPIO_PORT                GPIOF
#define KEY_UP_GPIO_PIN                 GPIO_PINS_6

#define KEY_DN_GPIO_PORT                GPIOF
#define KEY_DN_GPIO_PIN                 GPIO_PINS_7

#define KEY_LT_GPIO_PORT                GPIOB
#define KEY_LT_GPIO_PIN                 GPIO_PINS_2

#define KEY_RT_GPIO_PORT                GPIOB
#define KEY_RT_GPIO_PIN                 GPIO_PINS_11

#define KEY_MD_GPIO_PORT                GPIOB
#define KEY_MD_GPIO_PIN                 GPIO_PINS_10


#define KEY_TMR                         TMR15
#define KEY_TMR_PSC                     (120 - 1)
#define KEY_TMR_ARR                     (1000 - 1)
#define KEY_TMR_IRQ                     TMR15_GLOBAL_IRQn
#define KEY_TMR_IRQ_HANDLER             TMR15_GLOBAL_IRQHandler


#define KEY_NUM                         (5)
#define KEY_UP_INDEX                    (0)
#define KEY_DN_INDEX                    (1)
#define KEY_LT_INDEX                    (2)
#define KEY_RT_INDEX                    (3)
#define KEY_MD_INDEX                    (4)

#define KEY_PRESS_POLARITY              (0)

#define KEY_LONG_PRESS_JUDGE_NUM        (1500)

#define KEY_FSM_TABLE_SIZE              (6)

#define KEY_PRESS_BEEP_RING_TIME        (50)
#define KEY_LONG_PRESS_BEEP_RING_TIME   (200)

#endif //__KEY_MACROS_H
