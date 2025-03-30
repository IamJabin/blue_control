/**
 * @file bsp_timer.c
 * @author FanXinhao (1023405741@qq.com)
 * @brief 
 * @version 0.1
 * @date 2023-11-22
 * 
 * @copyright Copyright (c) 2023
 * 
 * @history
 * <table>
 * <tr><th>Date       <th>Version <th>Author    <th>Description
 * <tr><td>2023-11-22 <td>V0.1    <td>FanXinhao <td>
 * </table>
 */
/**
 * ����˳��
 * 1.����bsp_timer_init��ʼ��������ʼ����ʱ��
 * 2.����bsp_timer_set���ü�ʱʱ�䣨�ú�������Ҫ������ʱ֮ǰ���ã�
 * 3.����bsp_timer_start������ʱ
 **/
#include "bsp_timer.h"
#include "main.h"
#include "bsp_led.h"
#include "ns_log.h"
#include "bsp_digitron.h"
#include "bsp_beep.h"
#include "bsp_relay.h"

TIM_TimeBaseInitType TIM_TimeBaseStructure;
OCInitType TIM_OCInitStructure;
uint16_t PrescalerValue = 0;
int32_t remainder_time = 0;

bsp_timer_mode_t bsp_timer_mode = BSP_TIMER_MODE_STOP;

/**
 * @brief ��ʼ����ʱ��
 * 
 */
void bsp_timer_init(void)
{
    NVIC_InitType NVIC_InitStructure;
    /* PCLK1 = HCLK/4 */
    //RCC_ConfigPclk1(RCC_HCLK_DIV4);	//���øú����Ῠ��
    /* TIM6 clock enable */
    RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_TIM6, ENABLE);

    PrescalerValue = 64000-1;

    /* Time base configuration */
    //TIM_TimeBaseStructure.Period    = 65535;
    TIM_TimeBaseStructure.Period    = 1000;
    TIM_TimeBaseStructure.Prescaler = 64000-1;
    TIM_TimeBaseStructure.ClkDiv    = 0;
    TIM_TimeBaseStructure.CntMode   = TIM_CNT_MODE_UP;

    TIM_InitTimeBase(TIM6, &TIM_TimeBaseStructure);

    /* Prescaler configuration */
    TIM_ConfigPrescaler(TIM6, PrescalerValue, TIM_PSC_RELOAD_MODE_IMMEDIATE);

    /* Enable the TIM6 global Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel                   = TIM6_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority           = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);    

    /* TIM6 enable update irq */
    TIM_ConfigInt(TIM6, TIM_INT_UPDATE, ENABLE);

    /* TIM6 enable counter */
    //
}

/**
 * @brief ������ʱ��
 * 
 * @return bsp_timer_status_t 
 */
bsp_timer_status_t bsp_timer_start(void)
{
    if (remainder_time <= 0)
    {
        return BSP_TIMER_STATUS_ERROR;
    }
    bsp_relay_on();
    TIM_Enable(TIM6, ENABLE);
    bsp_timer_mode = BSP_TIMER_MODE_RUNNING;
    //�����̵���
    bsp_beep_on_nonblock(250);
    bsp_DigitronDisplayTime(remainder_time);    

    return BSP_TIMER_STATUS_OK;
}

/**
 * @brief ֹͣ��ʱ
 * 
 */
void bsp_timer_stop(void)
{
	if (bsp_timer_mode == BSP_TIMER_MODE_RUNNING)
    {
        TIM_Enable(TIM6, DISABLE);
        remainder_time = 0;
        bsp_timer_mode = BSP_TIMER_MODE_STOP;
        bsp_DigitronDisplay(0, 0, 0, 0, 1);
        bsp_beep_on_nonblock(500);
        bsp_relay_off();
    }
}

/**
 * @brief ���ü�ʱʱ��
 * 
 * @param value ���÷�Χ0~86400��
 * @return bsp_timer_status_t ��������BSP_TIMER_STATUS_OK���������ֵ����ERROR
 */
bsp_timer_status_t bsp_timer_set(int32_t value)
{
    //24*60*60 = 86400
    if (value > 86400)
    {
        return BSP_TIMER_STATUS_ERROR;
    }
    remainder_time = value;
    return BSP_TIMER_STATUS_OK;
}

/**
 * @brief ��ȡ��ǰʣ���ʱʱ��
 * 
 * @param value 
 */
void bsp_timer_get(uint32_t *value)
{
    *value = remainder_time;
}



__weak void bsp_timer_start_callback(void)
{
    //do nothing
}

__weak void bsp_timer_stop_callback(void)
{
    //do nothing
}

void TIM6_IRQHandler(void)
{

    if (TIM_GetIntStatus(TIM6, TIM_INT_UPDATE) != RESET)
    {
        TIM_ClrIntPendingBit(TIM6, TIM_INT_UPDATE);
        if (bsp_timer_mode == BSP_TIMER_MODE_RUNNING)
        {
            LedBlink(LED1_PORT, LED1_PIN);
            remainder_time--;
            if (remainder_time <= 0)
            {
                remainder_time = 0;
                bsp_timer_mode = BSP_TIMER_MODE_STOP;
                TIM_Enable(TIM6, DISABLE);
                //�رռ̵���
                bsp_relay_off();
                bsp_beep_on_nonblock(500);
            }
            bsp_DigitronDisplayTime(remainder_time);
        NS_LOG_INFO("TIM6 HANDLE\r\n");
        }
    }
		
}
