// File Name: ws2812_macros.h
// Author: kukukukuku~
// Date: 2025-03-06

#ifndef __WS2812_MACROS_H
#define __WS2812_MACROS_H

#include "at32f421.h"

#define WS2812_GPIO_PORT                        GPIOB
#define WS2812_GPIO_PIN                         GPIO_PINS_15
#define WS2812_GPIO_MUX_PIN_SOURCE              GPIO_PINS_SOURCE15
#define WS2812_GPIO_MUX                         GPIO_MUX_0


#define WS2812_SPI                              SPI2


#define WS2812_HIGH                             (0xFC)
#define WS2812_LOW                              (0xC0)
#define WS2812_RESET                            (0x00)

#define WS2812_NUM                              (1)
#define WS2812_0_INDEX                          (0)

#endif // __WS2812_MACROS_H
