// File Name: led_interface.h
// Author: kukukukuku~
// Date: 2025-03-17

#ifndef __LED_INTERFACE_H
#define __LED_INTERFACE_H

#include "./LED/inc/led_macros.h"

//LED呼吸方向
typedef enum LED_BREATH_DIR_T
{
    LED_BREATH_DIR_UP = 0,
    LED_BREATH_DIR_DOWN,
}LED_BREATH_DIR_T, *pLED_BREATH_DIR_T;

//LED模式
typedef enum LED_MODE_T
{
    LED_MODE_BREATH = 0,
    LED_MODE_BLINK,
    LED_MODE_SWITCH,
}LED_MODE_T, *pLED_MODE_T;

//LED对象
typedef struct LED_OBJ_T
{
    gpio_type *gpioPort;
    uint32_t gpioPin;
    gpio_pins_source_type gpioPinSource;
    gpio_mux_sel_type gpioMux;
    tmr_type *tmr;
    uint16_t tmrBreathArr;
    uint16_t tmrBlinkArr;
    uint16_t tmrPulse;
    uint16_t blinkCnt;
    uint16_t blinkNum;
    uint8_t breathStep;
    uint8_t breathStepMin;
    uint8_t breathStepMax;
    LED_BREATH_DIR_T breathDir;
    LED_MODE_T mode;
    uint8_t state;
    uint8_t polarity;
}LED_OBJ_T, *pLED_OBJ_T;


/**
 * @brief   LED对象初始化
 * @param   ledObj:         LED对象
 * @param   gpioPort:       GPIO端口
 * @param   gpioPin:        GPIO引脚
 * @param   gpioPinSource:  GPIO引脚源
 * @param   gpioMux:        GPIO引脚复用
 * @param   tmr:            定时器
 * @param   tmrPulse:       定时器脉宽
 * @param   tmrBreathArr:   呼吸灯模式定时器ARR寄存器值
 * @param   tmrBlinkArr:    闪烁等模式定时器ARR寄存器值
 * @param   blinkNum:       闪烁时间
 * @param   polatiry:       LED亮时的电平
 * @param   breathStepMin:  呼吸步进最小值
 * @param   breathStepMax:  呼吸步进最大值
 * @retval  None
 * @note    None
*/
void led_obj_init(LED_OBJ_T * const ledObj, gpio_type * const gpioPort, const uint32_t gpioPin, const gpio_pins_source_type gpioPinSource,
    const gpio_mux_sel_type gpioMux, tmr_type * const tmr, const uint16_t tmrPulse, const uint16_t tmrBreathArr, const uint16_t tmrBlinkArr,
    const uint16_t blinkNum, const uint8_t polatiry, const uint8_t breathStepMin, const uint8_t breathStepMax);


/**
 * @brief   LED设置模式
 * @param   ledObj: LED对象
 * @param   mode:   模式
 * @retval  None
 * @note    None
*/
void led_set_mode(LED_OBJ_T * const ledObj, const LED_MODE_T mode);


/**
 * @brief   LED翻转
 * @param   ledObj: LED对象
 * @retval  None
 * @note    呼吸灯模式无效
*/
void led_toggle(LED_OBJ_T * const ledObj);


/**
 * @brief   打开LED
 * @param   ledObj: LED对象
 * @retval  None
 * @note    仅开关灯模式有效
*/
void led_on(LED_OBJ_T * const ledObj);


/**
 * @brief   关闭LED
 * @param   ledObj: LED对象
 * @retval  None
 * @note    仅开关灯模式有效
*/
void led_off(LED_OBJ_T * const ledObj);


/**
 * @brief   LED设置呼吸步进
 * @param   ledObj:     LED对象
 * @param   breathStep: 呼吸步进
 * @retval  None
 * @note    None
*/
void led_set_breath_step(LED_OBJ_T * const ledObj, const uint8_t breathStep);


/**
 * @brief   LED设置闪烁时间
 * @param   ledObj:     LED对象
 * @param   blinkNum:   闪烁时间
 * @retval  None
 * @note    闪烁时间最小设置为100
*/
void led_set_blink_num(LED_OBJ_T * const ledObj, const uint16_t blinkNum);


/**
 * @brief   LED定时器中断处理
 * @param   ledObj: LED对象
 * @retval  None
 * @note    只能在LED定时器中断服务函数中调用
*/
void led_tmr_irq_handler(LED_OBJ_T * const ledObj);

#endif //__LED_INTERFACE_H
