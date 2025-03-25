// File Name: led_interface.c
// Author: kukukukuku~
// Date: 2025-03-17

#include "stddef.h"
#include "string.h"
#include "./LED/inc/led_interface.h"

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
    const uint16_t blinkNum, const uint8_t polatiry, const uint8_t breathStepMin, const uint8_t breathStepMax)
{
    if(ledObj == NULL || gpioPort == NULL)
        return;
    
    memset(ledObj, 0, sizeof(LED_OBJ_T));
    ledObj->gpioPort      = gpioPort;
    ledObj->gpioPin       = gpioPin;
    ledObj->gpioPinSource = gpioPinSource;
    ledObj->gpioMux       = gpioMux;
    ledObj->tmr           = tmr;
    ledObj->tmrPulse      = tmrPulse,
    ledObj->tmrBreathArr  = tmrBreathArr;
    ledObj->tmrBlinkArr   = tmrBlinkArr;
    ledObj->blinkNum      = blinkNum;
    ledObj->breathStep    = 1;
    ledObj->breathStepMin = breathStepMin;
    ledObj->breathStepMax = breathStepMax;
    ledObj->mode          = tmr ? LED_MODE_BREATH : LED_MODE_SWITCH;
    ledObj->breathDir     = LED_BREATH_DIR_UP;
    ledObj->polarity      = polatiry;
}


/**
 * @brief   LED呼吸灯模式GPIO配置
 * @param   gpioPort:       LED GPIO端口
 * @param   gpioPin:        LED GPIO引脚
 * @param   gpioPinSource:  LED GPIO引脚源
 * @param   gpioMux:        LED GPIO引脚复用
 * @retval  None
 * @note    静态
*/
static void led_breath_mode_gpio_config(gpio_type * const gpioPort, const uint32_t gpioPin, const gpio_pins_source_type gpioPinSource, const gpio_mux_sel_type gpioMux)
{
    if(gpioPort == NULL)
        return;
    
    gpio_init_type gpioInitStruct = 
    {
        .gpio_pins           = gpioPin,
        .gpio_out_type       = GPIO_OUTPUT_PUSH_PULL,
        .gpio_pull           = GPIO_PULL_NONE,
        .gpio_mode           = GPIO_MODE_MUX,
        .gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER,
    };
    
    gpio_init(gpioPort, &gpioInitStruct);
    gpio_pin_mux_config(gpioPort, gpioPinSource, gpioMux);
}


/**
 * @brief   LED闪烁灯/开关灯模式GPIO配置
 * @param   gpioPort:   LED GPIO端口
 * @param   gpioPin:    LED GPIO引脚
 * @retval  None
 * @note    静态
*/
static void led_blink_or_switch_mode_gpio_config(gpio_type * const gpioPort, const uint32_t gpioPin)
{
    if(gpioPort == NULL)
        return;
    
    gpio_init_type gpioInitStruct = 
    {
        .gpio_pins           = gpioPin,
        .gpio_out_type       = GPIO_OUTPUT_PUSH_PULL,
        .gpio_pull           = GPIO_PULL_NONE,
        .gpio_mode           = GPIO_MODE_OUTPUT,
        .gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER,
    };
    
    gpio_init(gpioPort, &gpioInitStruct);
}


/**
 * @brief   启动LED定时器
 * @param   ledObj: LED对象
 * @retval  None
 * @note    静态
*/
static void led_tmr_start(LED_OBJ_T * const ledObj)
{
    if(ledObj == NULL || ledObj->tmr == NULL)
        return;
    
    if(ledObj->tmr->ctrl1_bit.tmren == 0)
        tmr_counter_enable(ledObj->tmr, TRUE);
}


/**
 * @brief   关闭LED定时器
 * @param   ledObj: LED对象
 * @retval  None
 * @note    None
*/
void led_tmr_stop(LED_OBJ_T * const ledObj)
{
    if(ledObj == NULL || ledObj->tmr == NULL)
        return;
    
    if(ledObj->tmr->ctrl1_bit.tmren == 1)
    {
        tmr_counter_enable(ledObj->tmr, FALSE);
        tmr_counter_value_set(ledObj->tmr, 0);
    }
}


/**
 * @brief   LED设置模式
 * @param   ledObj: LED对象
 * @param   mode:   模式
 * @retval  None
 * @note    None
*/
void led_set_mode(LED_OBJ_T * const ledObj, const LED_MODE_T mode)
{
    if(ledObj == NULL || ledObj->gpioPort == NULL || ledObj->tmr == NULL)
        return;
    
    switch(mode)
    {
        case LED_MODE_BREATH:
        {
            led_breath_mode_gpio_config(ledObj->gpioPort, ledObj->gpioPin, ledObj->gpioPinSource, ledObj->gpioMux);
            tmr_period_value_set(ledObj->tmr, ledObj->tmrBreathArr);
            led_tmr_start(ledObj);
            break;
        }
        case LED_MODE_BLINK:
        {
            led_blink_or_switch_mode_gpio_config(ledObj->gpioPort, ledObj->gpioPin);
            tmr_period_value_set(ledObj->tmr, ledObj->tmrBlinkArr);
            led_tmr_start(ledObj);
            break;
        }
        case LED_MODE_SWITCH:
        {
            led_blink_or_switch_mode_gpio_config(ledObj->gpioPort, ledObj->gpioPin);
            led_tmr_stop(ledObj);
            break;
        }
        default:
            break;
    }
    
    ledObj->mode = mode;
}


/**
 * @brief   LED翻转
 * @param   ledObj: LED对象
 * @retval  None
 * @note    呼吸灯模式无效
*/
void led_toggle(LED_OBJ_T * const ledObj)
{
    if(ledObj == NULL || ledObj->gpioPort == NULL || ledObj->mode == LED_MODE_BREATH)
        return;
    
    gpio_bits_write(ledObj->gpioPort, ledObj->gpioPin, (confirm_state)!gpio_output_data_bit_read(ledObj->gpioPort, ledObj->gpioPin));
}


/**
 * @brief   打开LED
 * @param   ledObj: LED对象
 * @retval  None
 * @note    仅开关灯模式有效
*/
void led_on(LED_OBJ_T * const ledObj)
{
    if(ledObj == NULL || ledObj->gpioPort == NULL || ledObj->mode != LED_MODE_SWITCH)
        return;
    
    gpio_bits_write(ledObj->gpioPort, ledObj->gpioPin, (confirm_state)ledObj->polarity);
}


/**
 * @brief   关闭LED
 * @param   ledObj: LED对象
 * @retval  None
 * @note    仅开关灯模式有效
*/
void led_off(LED_OBJ_T * const ledObj)
{
    if(ledObj == NULL || ledObj->gpioPort == NULL || ledObj->mode != LED_MODE_SWITCH)
        return;
    
    gpio_bits_write(ledObj->gpioPort, ledObj->gpioPin, (confirm_state)!ledObj->polarity);
}


/**
 * @brief   LED设置呼吸步进
 * @param   ledObj:     LED对象
 * @param   breathStep: 呼吸步进
 * @retval  None
 * @note    None
*/
void led_set_breath_step(LED_OBJ_T * const ledObj, const uint8_t breathStep)
{
    if(ledObj == NULL || ledObj->gpioPort == NULL || ledObj->tmr == NULL || breathStep < ledObj->breathStepMin || breathStep > ledObj->breathStepMax)
        return;
    
    ledObj->breathStep = breathStep;
}


/**
 * @brief   LED设置闪烁时间
 * @param   ledObj:     LED对象
 * @param   blinkNum:   闪烁时间
 * @retval  None
 * @note    闪烁时间最小设置为100
*/
void led_set_blink_num(LED_OBJ_T * const ledObj, const uint16_t blinkNum)
{
    if(ledObj == NULL || ledObj->gpioPort == NULL || ledObj->tmr == NULL || blinkNum < 100)
        return;
    
    ledObj->blinkNum = blinkNum;
}


/**
 * @brief   LED定时器中断处理
 * @param   ledObj: LED对象
 * @retval  None
 * @note    只能在LED定时器中断服务函数中调用
*/
void led_tmr_irq_handler(LED_OBJ_T * const ledObj)
{
    if(ledObj->mode == LED_MODE_BREATH)
    {
        ledObj->blinkCnt = 0;
        
        if(ledObj->breathDir == LED_BREATH_DIR_UP)
        {
            if(ledObj->tmrPulse + ledObj->breathStep <= LED1_TMR_PULSE)
                ledObj->tmrPulse += ledObj->breathStep;
            else if(ledObj->tmrPulse < LED1_TMR_PULSE)
                ledObj->tmrPulse += 1;
            else
                ledObj->breathDir = LED_BREATH_DIR_DOWN;
        }
        else
        {
            if(ledObj->tmrPulse >= ledObj->breathStep)
                ledObj->tmrPulse -= ledObj->breathStep;
            else if(ledObj->tmrPulse > 0)
                ledObj->tmrPulse -= 1;
            else
                ledObj->breathDir = LED_BREATH_DIR_UP;
        }
        
        tmr_channel_value_set(LED1_TMR, LED1_TMR_CHANNEL, ledObj->tmrPulse);
    }
    else if(ledObj->mode == LED_MODE_BLINK)
    {
        ledObj->tmrPulse = 0;
        ledObj->breathDir = LED_BREATH_DIR_UP;
        
        if(++ledObj->blinkCnt >= ledObj->blinkNum)
        {
            ledObj->blinkCnt = 0;
            led_toggle(ledObj);
        }
    }
}
