// File Name: adc_init.c
// Author: kukukukuku~
// Date: 2025-03-06

#include "./ADC/inc/adc_init.h"

/**
 * @brief   ADC GPIO配置
 * @param   None
 * @retval  None
 * @note    静态
*/
static void adc_gpio_config(void)
{
    gpio_init_type gpioInitStruct = 
    {
        .gpio_pins           = ADC_GPIO_PIN,
        .gpio_out_type       = GPIO_OUTPUT_PUSH_PULL,
        .gpio_pull           = GPIO_PULL_NONE,
        .gpio_mode           = GPIO_MODE_ANALOG,
        .gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER,
    };
    
    gpio_init(ADC_GPIO_PORT, &gpioInitStruct);
}


/**
 * @brief   ADC DMA配置
 * @param   bufferAddr: 数据缓存地址
 * @retval  None
 * @note    静态
*/
static void adc_dma_config(const uint32_t bufferAddr)
{
    dma_init_type dmaInitStruct = 
    {
        .peripheral_base_addr  = (uint32_t)&ADC_ADC->odt,
        .memory_base_addr      = bufferAddr,
        .direction             = DMA_DIR_PERIPHERAL_TO_MEMORY,
        .buffer_size           = ADC_CHANNEL_NUM,
        .peripheral_inc_enable = FALSE,
        .memory_inc_enable     = TRUE,
        .peripheral_data_width = DMA_PERIPHERAL_DATA_WIDTH_HALFWORD,
        .memory_data_width     = DMA_MEMORY_DATA_WIDTH_HALFWORD,
        .loop_mode_enable      = TRUE,
        .priority              = DMA_PRIORITY_HIGH,
    };
    
    dma_reset(ADC_DMA_CHANNEL);
    dma_init(ADC_DMA_CHANNEL, &dmaInitStruct);
    dma_channel_enable(ADC_DMA_CHANNEL, FALSE);
}


/**
 * @brief   ADC ADC配置
 * @param   None
 * @retval  None
 * @note    静态
*/
static void adc_adc_config(void)
{
    adc_base_config_type adcBaseStruct = 
    {
        .sequence_mode           = TRUE,
        .repeat_mode             = TRUE,
        .data_align              = ADC_RIGHT_ALIGNMENT,
        .ordinary_channel_length = ADC_CHANNEL_NUM,
    };
    
    crm_adc_clock_div_set(CRM_ADC_DIV_6);   //ADC最大时钟频率为20MHz(120 / 6 = 20MHz)
    
    adc_base_config(ADC_ADC, &adcBaseStruct);
    
    adc_ordinary_channel_set(ADC_ADC, ADC_ADC_CHANNEL, ADC_CHANNEL_ADC_INDEX + 1, ADC_SAMPLETIME_239_5);
    adc_ordinary_channel_set(ADC_ADC, ADC_TEMPERATURE_CHANNEL, ADC_CHANNEL_TEMPERATURE_INDEX + 1, ADC_SAMPLETIME_239_5);
    adc_ordinary_channel_set(ADC_ADC, ADC_VREF_CHANNEL, ADC_CHANNEL_VREF_INDEX + 1, ADC_SAMPLETIME_239_5);
    
    adc_ordinary_conversion_trigger_set(ADC1, ADC12_ORDINARY_TRIG_SOFTWARE, TRUE);
    adc_dma_mode_enable(ADC_ADC, TRUE);
    adc_tempersensor_vintrv_enable(TRUE);
    
    adc_enable(ADC_ADC, TRUE);
    adc_calibration_init(ADC_ADC);
    while(adc_calibration_init_status_get(ADC_ADC));
    adc_calibration_start(ADC_ADC);
    while(adc_calibration_status_get(ADC_ADC));
}


/**
 * @brief   ADC初始化
 * @param   bufferAddr: 数据缓存地址
 * @retval  None
 * @note    None
*/
void adc_init(const uint32_t bufferAddr)
{
    adc_gpio_config();
    adc_dma_config(bufferAddr);
    adc_adc_config();
}
