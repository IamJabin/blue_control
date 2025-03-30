#include "bsp_digitron.h"
#include "n32wb03x_gpio.h"
#include "drv_soft_iic.h"
#include "n32wb03x_i2c.h"

static uint8_t g_digitron_num[16] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
static uint8_t g_digitron_address[4] = {0x68, 0x6a, 0x6c, 0x6e};

/**
 * @brief 初始化数码管
 * 
 */
void bsp_InitDigitron(void)
{
    //初始化IIC
#if (BSP_DIGITRON_USING_HARDWARE_IIC == 1)
    //初始化硬件IIC

#else
    drv_soft_iic_init();
#endif
    
    //关闭数码管（尝试和数码管通信）
}

/**
 * @brief 设置电源和亮度
 * 
 * @param power 电源
 * @param brightness 亮度 
 */
void bsp_DigitronPowerBrightnessSet(bsp_DigitronPower_t power,bsp_DigitronBrightness_t brightness)
{
    uint8_t data = 0x00;

    data |= (brightness << 4);
    data |= (power << 0);
    
    drv_soft_iic_start();
    drv_soft_iic_send_byte(0x48);   //发送控制命令
    drv_soft_iic_wait_ack();
    drv_soft_iic_send_byte(data);   //设置电源和亮度
    drv_soft_iic_wait_ack();
    drv_soft_iic_stop();
}

/**
 * @brief 显示数字和点
 * 
 * @param first 
 * @param second 
 * @param third 
 * @param fourth 上述取值范围 0~15
 * @param point  0：不显示时钟 1：显示时钟点
 */
void bsp_DigitronDisplay(uint8_t first, uint8_t second, uint8_t third, uint8_t fourth, uint8_t point)
{
    uint8_t i = 0;
    uint8_t data_array[4] = {0};
    
    data_array[0] = first;
    data_array[1] = second;
    data_array[2] = third;
    data_array[3] = fourth;

    bsp_InitDigitron();    //每次使用之前需要重新初始化，原因不明，现象为每次连接手机之后无法发送SDA\SCL时序

    for (i=0; i<4; i++)
    {
        drv_soft_iic_start();
        drv_soft_iic_send_byte(g_digitron_address[i]);   //发送显存地址
        drv_soft_iic_wait_ack();
        if (point == 0)
        {
            drv_soft_iic_send_byte(g_digitron_num[data_array[i]]);   //设置数据
        }
        else
        {
            drv_soft_iic_send_byte(g_digitron_num[data_array[i]] | 0x80);   //设置数据
        }
        
        drv_soft_iic_wait_ack();
        drv_soft_iic_stop();
    } 
}

/**
 * @brief 将秒数转换为小时、分钟、秒
 * 
 * @param second 
 * @param hour 
 * @param miniute 
 * @param second1 
 */
void bsp_timer_second2hms(int32_t second, uint8_t *hour, uint8_t *miniute, uint8_t *second1)
{
    *hour = second / 3600;
    *miniute = (second % 3600) / 60;
    *second1 = (second % 3600) % 60;
}

void bsp_DigitronDisplayTime(uint32_t time)
{
    uint8_t hour, miniute, second;  
    bsp_timer_second2hms(time, &hour, &miniute, &second);
    if (time < 3600)
    {
        bsp_DigitronDisplay(miniute/10, miniute%10, second/10, second%10, 1);  
    }
    else
    {
        bsp_DigitronDisplay(hour/10, hour%10, miniute/10, miniute%10, 1);          
    }
}

