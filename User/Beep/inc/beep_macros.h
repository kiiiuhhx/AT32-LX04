// File Name: beep_macros.h
// Author: kukukukuku~
// Date: 2025-03-06

#ifndef __BEEP_MACROS_H
#define __BEEP_MACROS_H

#include "at32f421.h"

#define BEEP_GPIO_PORT                      GPIOC
#define BEEP_GPIO_PIN                       GPIO_PINS_13


#define BEEP_TMR                            TMR6
#define BEEP_TMR_PSC                        (120 - 1)
#define BEEP_TMR_ARR                        (1000 - 1)
#define BEEP_TMR_IRQ                        TMR6_GLOBAL_IRQn
#define BEEP_TMR_IRQ_HANDLER                TMR6_GLOBAL_IRQHandler

#endif //__BEEP_MACROS_H
