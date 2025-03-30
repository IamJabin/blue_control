/**
 ******************************************************************************
 * @file    main.c
 * @author  qikaige
 * @version V1.0
 * @date    2023-10-26
 * @brief   ble
 ******************************************************************************
 * @attention
 *
 * DISCLAIMER: THIS SOFTWARE IS PROVIDED BY NATIONS "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * DISCLAIMED. IN NO EVENT SHALL NATIONS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * <h2><center>&copy; COPYRIGHT 2023 PengLi</center></h2>
 ******************************************************************************
 */

/**
 * @file main.c
 * @author Nations Firmware Team
 * @version v1.0.1
 *
 * @copyright Copyright (c) 2019, Nations Technologies Inc. All rights reserved.
 */

/** @addtogroup
 * @{
 */

/* Includes ------------------------------------------------------------------*/
// #include "FreeRTOS.h"
// #include "task.h"
#include "log.h"
#include "crc.h"
#include "rwip.h"
#include "main.h"
#include "ns_log.h"
#include "ns_ble.h"
#include "app_ble.h"
#include "bsp_led.h"
#include "n32wb03x.h"
#include "ns_sleep.h"
#include "ns_delay.h"
#include "app_usart.h"
#include "bsp_alarm.h"
#include "n32wb03x_rtc.h"
#include "bsp_calendar.h"
#include "app_func_code.h"
#include "rs_queue_list.h"
#include "rs_queue_list_config.h"
#include "ble_msg.h"
#include "app_rdtss.h"

//#include "drv_soft_iic.h"
#include "bsp_digitron.h"
#include "bsp_beep.h"
#include "bsp_timer.h"
#include "bsp_relay.h"
#include "bsp_key.h"

#if (CFG_APP_NS_IUS)
#include "ns_dfu_boot.h"
#endif

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define DEMO_STRING "\r\n Nations raw data transfer server(128bit UUID) demo \r\n"
#define TEST_STRING "\r\ABCDEFGHIJK\r\n"

/* Private constants ---------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

#define SYSTICK_1MS ((uint32_t)1000)

uint32_t Tick_num = 0;

/**
 * @brief  This function handles SysTick Handler.
 */
void SysTick_Handler(void)
{
    Tick_num++;
}

uint32_t SystickGetime(void)
{
    return Tick_num;
}

extern uint8_t ble_connected;

all_nodes_ack_msg_t msg_t;
/**
 * @brief  main function
 * @param
 * @return
 * @note   Note
 */
int main(void)
{

    uint32_t time_out = 0;

    NS_LOG_INIT();                  // 初始化用于输出LOG的串口
    ble_list_init();                // 创建用于蓝牙接收发送数据的队列
    bsp_InitDigitron();             // 数码管初始化
    bsp_beep_init();                // 蜂鸣器初始化
    bsp_timer_init();               // 定时器初始化
    bsp_relay_init();               // 继电器初始化
    app_ble_init();                 // 蓝牙协议栈初始化以及启动广播
    LedInit(LED1_PORT, LED1_PIN);   // LED灯初始化
    LedInit(LED2_PORT, LED2_PIN); 
    LedInit(LED3_PORT, LED3_PIN); 
		bsp_key_init();
    app_usart_configuration();
    
    LedOn(LED1_PORT, LED1_PIN);     //默认启动时LED全部亮
    LedOn(LED2_PORT, LED2_PIN);
    LedOn(LED3_PORT, LED3_PIN); 

    bsp_DigitronPowerBrightnessSet(BSP_DIGITRON_POWER_ON,BSP_DIGITRON_BRIGHTNESS_8); //启动数码管、设置显示亮度为8级
		bsp_DigitronDisplayTime(0); 	//数码管默认显示00:00
    bsp_beep_on();  							//蜂鸣器响一下
    delay_n_ms(10);
		bsp_beep_off();

		app_usart_dma_enable(ENABLE);
		//usart_tx_dma_send((uint8_t*)TEST_STRING, sizeof(TEST_STRING)); 
		NS_LOG_INFO("test\r\n");
		delay_n_10us(500);
		app_usart_dma_enable(DISABLE);

    //配置系统时钟1ms中断
    SystemCoreClockUpdate();
    SysTick_Config(SystemCoreClock / SYSTICK_1MS);  
    
    while (1)
    {
        rwip_schedule();        //蓝牙协议栈调度
        ble_recv_msg();         //蓝牙数据接收状态机
        bsp_beep_process();     //蜂鸣器非阻塞处理
        bsp_key_scan();         //按键扫描
        if (SystickGetime() - time_out > 2000)  //2000ms执行一次内部函数
        {
            LedBlink(LED1_PORT, LED1_PIN);      //LED1闪烁
            time_out = SystickGetime();
            if (ble_connected)  //连接成功LED3闪烁
            {
                LedBlink(LED2_PORT, LED2_PIN);		 
            }			
            //usart_tx_dma_send("test", 5);	
            //test github synchronization
        }
    }
}

/**
 * @brief  user handle before enter sleep mode
 * @param
 * @return
 * @note
 */
void app_sleep_prepare_proc(void)
{
}

/**
 * @brief  user handle after wake up from sleep mode
 * @param
 * @return
 * @note
 */
void app_sleep_resume_proc(void)
{
}

/**
 * @}
 */
