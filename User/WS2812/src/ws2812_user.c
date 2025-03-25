// File Name: ws2812_user.c
// Author: kukukukuku~
// Date: 2025-03-06

#include "./WS2812/inc/ws2812_user.h"

//WS2812信号时序对象
WS2812_TIMING_OBJ_T ws2812TimingObj;

//WS2812对象
WS2812_OBJ_T ws2812Obj;

uint8_t ws2812Red;
uint8_t ws2812Green;
uint8_t ws2812Blue;
uint8_t ws2812Flag;
