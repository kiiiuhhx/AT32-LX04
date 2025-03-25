// File Name: init.c
// Author: kukukukuku~
// Date: 2025-02-21

#include "at32f421_clock.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "./ADC/inc/adc_init.h"
#include "./Beep/inc/beep_init.h"
#include "./Key/inc/key_init.h"
#include "./LCD/inc/lcd_init.h"
#include "./LED/inc/led_init.h"
#include "./PWM/inc/pwm_init.h"
#include "./Servo/inc/servo_init.h"
#include "./USART/inc/usart_init.h"
#include "./WS2812/inc/ws2812_init.h"

#include "./ADC/inc/adc_task.h"
#include "./Beep/inc/beep_task.h"
#include "./Key/inc/key_task.h"
#include "./LCD/inc/lcd_task.h"
#include "./LED/inc/led_task.h"
#include "./PWM/inc/pwm_task.h"
#include "./Servo/inc/servo_task.h"
#include "./USART/inc/usart_task.h"
#include "./WS2812/inc/ws2812_task.h"

/**
 * @brief   系统初始化
 * @param   None
 * @retval  None
 * @note    None
*/
void system_init(void)
{
    //配置系统时钟 --- 内部时钟PLL 120MHz AHB_DIV1 APB1_DIV1 APB2_DIV1
    system_clock_config();
    
    //中断优先级分组2 --- 2位抢占优先级 2位响应优先级
    nvic_priority_group_config(NVIC_PRIORITY_GROUP_2);
    
    crm_periph_clock_enable(CRM_ADC1_PERIPH_CLOCK, TRUE);
    crm_periph_clock_enable(CRM_DMA1_PERIPH_CLOCK, TRUE);
    crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);
    crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK, TRUE);
    crm_periph_clock_enable(CRM_GPIOC_PERIPH_CLOCK, TRUE);
    crm_periph_clock_enable(CRM_GPIOF_PERIPH_CLOCK, TRUE);
    crm_periph_clock_enable(CRM_SPI2_PERIPH_CLOCK, TRUE);
    crm_periph_clock_enable(CRM_TMR1_PERIPH_CLOCK, TRUE);
    crm_periph_clock_enable(CRM_TMR6_PERIPH_CLOCK, TRUE);
    crm_periph_clock_enable(CRM_TMR14_PERIPH_CLOCK, TRUE);
    crm_periph_clock_enable(CRM_TMR15_PERIPH_CLOCK, TRUE);
    crm_periph_clock_enable(CRM_TMR17_PERIPH_CLOCK, TRUE);
    crm_periph_clock_enable(CRM_USART1_PERIPH_CLOCK, TRUE);
    
    adc_init((uint32_t)adcObj.rawData);
    beep_init();
    key_init();
    lcd_init();
    led_init();
    pwm_init();
    servo_init();
    usart_config_init((uint32_t)usartObj.txBuffer, (uint32_t)usartObj.rxBuffer);
    ws2812_init();
}


/**
 * @brief   初始化任务
 * @param   arg:    任务参数
 * @retval  None
 * @note    None
*/
void init_task(void *arg)
{
    xTaskCreate(adc_task, "adc_task", 64, NULL, 6, &adcTaskHandle);
    xTaskCreate(beep_task, "beep_task", 64, NULL, 6, &beepTaskHandle);
    xTaskCreate(key_task, "key_task", 64, NULL, 5, &keyTaskHandle);
    xTaskCreate(lcd_task, "lcd_task", 128, NULL, 6, &lcdTaskHandle);
    xTaskCreate(led_task, "led_task", 64, NULL, 4, &ledTaskHandle);
    xTaskCreate(pwm_task, "pwm_task", 64, NULL, 3, &pwmTaskHandle);
    xTaskCreate(servo_task, "servo_task", 64, NULL, 4, &servoTaskHandle);
    xTaskCreate(usart_task, "usart_task", 64, NULL, 6, &usartTaskHandle);
    xTaskCreate(ws2812_task, "ws2812_task", 64, NULL, 2, &ws2812TaskHandle);
    
    vTaskDelete(NULL);
}
