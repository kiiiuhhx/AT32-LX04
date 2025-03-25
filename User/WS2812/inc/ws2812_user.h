// File Name: ws2812_user.h
// Author: kukukukuku~
// Date: 2025-03-06

#ifndef __WS2812_USER_H
#define __WS2812_USER_H

#include "./WS2812/inc/ws2812_interface.h"

//WS2812信号时序对象
extern WS2812_TIMING_OBJ_T ws2812TimingObj;

//WS2812对象
extern WS2812_OBJ_T ws2812Obj;


extern uint8_t ws2812Red;
extern uint8_t ws2812Green;
extern uint8_t ws2812Blue;
extern uint8_t ws2812Flag;

#endif //__WS2812_USER_H
