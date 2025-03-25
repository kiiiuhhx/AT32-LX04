// File Name: led_macros.h
// Author: kukukukuku~
// Date: 2025-03-15

#ifndef __LED_MACROS_H
#define __LED_MACROS_H

#include "at32f421.h"

#define LED1_GPIO_PORT                      GPIOA
#define LED1_GPIO_PIN                       GPIO_PINS_7
#define LED1_GPIO_MUX_PIN_SOURCE            GPIO_PINS_SOURCE7
#define LED1_GPIO_MUX                       GPIO_MUX_5

#define LED2_GPIO_PORT                      GPIOA
#define LED2_GPIO_PIN                       GPIO_PINS_0


#define LED1_TMR                            TMR17
#define LED1_TMR_CHANNEL                    TMR_SELECT_CHANNEL_1
#define LED1_TMR_PSC                        (120 - 1)
#define LED1_TMR_BREATH_ARR                 (5000 - 1)
#define LED1_TMR_BLINK_ARR                  (1000 - 1)
#define LED1_TMR_PULSE                      (5000 - 1)
#define LED1_TMR_IRQ                        TMR17_GLOBAL_IRQn
#define LED1_TMR_IRQ_HANDLER                TMR17_GLOBAL_IRQHandler


#define LED1_POLARITY                       (0)
#define LED1_BREATH_STEP_MIN                (1)
#define LED1_BREATH_STEP_MAX                (20)


#define LED2_POLARITY                       (0)

#endif //__LED_MACROS_H
