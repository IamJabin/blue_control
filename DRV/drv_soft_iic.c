#include "drv_soft_iic.h"
#include "ns_delay.h"

/**
 * @brief 初始化软件IIC
 * 
 * @return drv_soft_iic_status_t 
 */
drv_soft_iic_status_t drv_soft_iic_init(void)
{
    GPIO_InitType GPIO_InitStructure;

    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(SOFT_IIC_SDA_PORT));
    assert_param(IS_GPIO_ALL_PERIPH(SOFT_IIC_SCL_PORT));
    assert_param(IS_GPIO_PIN(SOFT_IIC_SDA_PIN));
    assert_param(IS_GPIO_PIN(SOFT_IIC_SCL_PIN));

    GPIO_SetBits(SOFT_IIC_SCL_PORT, SOFT_IIC_SCL_PIN);  
    GPIO_SetBits(SOFT_IIC_SDA_PORT, SOFT_IIC_SDA_PIN);  


    RCC_EnableAPB2PeriphClk(SOFT_IIC_SDA_GPIO_CLK, ENABLE);
    RCC_EnableAPB2PeriphClk(SOFT_IIC_SCL_GPIO_CLK, ENABLE);

    GPIO_InitStruct(&GPIO_InitStructure);
    GPIO_InitStructure.Pin = SOFT_IIC_SDA_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitPeripheral(SOFT_IIC_SDA_PORT, &GPIO_InitStructure);    

    GPIO_InitStruct(&GPIO_InitStructure);
    GPIO_InitStructure.Pin = SOFT_IIC_SCL_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitPeripheral(SOFT_IIC_SCL_PORT, &GPIO_InitStructure);  


    return DRV_SOFT_IIC_STATUS_OK;
}


/**
 * @brief 设置方向
 * 
 * @param dir @drv_soft_iic_dir_t
 */
void drv_soft_iic_sda_dir_set(drv_soft_iic_dir_t dir)
{ 
    uint32_t tmp_register = 0x00U, pos = 0, mode = 0;

    //(0b00: 输入模式 0b01: 通用输出模式)
    if (DRV_SOFT_IIC_DIR_INPUT == dir)
    {
        mode = 0x00;
    }
    else if (DRV_SOFT_IIC_DIR_OUTPUT == dir)
    {
        mode = 0x01;
    }        

    while((SOFT_IIC_SDA_PIN >> pos) != 0)
    {
        pos++;
    }
    pos -= 1;

    tmp_register = SOFT_IIC_SDA_PORT->PMODE;
    tmp_register &= ~(0x3 << (pos * 2U));               //清除
    tmp_register |= ((mode & 0x03U) << (pos * 2U));     //设置
    SOFT_IIC_SDA_PORT->PMODE = tmp_register;
}


void drv_soft_iic_start(void)
{
    //设置输入输出方向
    drv_soft_iic_sda_dir_set(DRV_SOFT_IIC_DIR_OUTPUT);
    //发送start信号
    GPIO_SetBits(SOFT_IIC_SDA_PORT, SOFT_IIC_SDA_PIN);
    GPIO_SetBits(SOFT_IIC_SCL_PORT, SOFT_IIC_SCL_PIN);
    delay_n_10us(1);
    GPIO_ResetBits(SOFT_IIC_SDA_PORT, SOFT_IIC_SDA_PIN);
    delay_n_10us(1);
    GPIO_ResetBits(SOFT_IIC_SCL_PORT, SOFT_IIC_SCL_PIN);
    delay_n_10us(1);
}


void drv_soft_iic_stop(void)
{
    //设置输入输出方向
    drv_soft_iic_sda_dir_set(DRV_SOFT_IIC_DIR_OUTPUT);
    //发送stop信号
    GPIO_ResetBits(SOFT_IIC_SDA_PORT, SOFT_IIC_SDA_PIN);
    GPIO_ResetBits(SOFT_IIC_SCL_PORT, SOFT_IIC_SCL_PIN);
    // GPIO_ResetBits(SOFT_IIC_SDA_PORT, SOFT_IIC_SDA_PIN);
    delay_n_10us(1);
    GPIO_SetBits(SOFT_IIC_SCL_PORT, SOFT_IIC_SCL_PIN);
    delay_n_10us(1);
    GPIO_SetBits(SOFT_IIC_SDA_PORT, SOFT_IIC_SDA_PIN);
    delay_n_10us(1);
}

/**
 * @brief 等待ACK应答，超时时间250个周期
 * 
 * @return drv_soft_iic_status_t 
 */
drv_soft_iic_status_t drv_soft_iic_wait_ack(void)
{
    uint8_t error_time = 0;
    //设置输入输出方向
    drv_soft_iic_sda_dir_set(DRV_SOFT_IIC_DIR_INPUT);
    GPIO_SetBits(SOFT_IIC_SDA_PORT, SOFT_IIC_SDA_PIN);
    delay_n_10us(1);
    GPIO_SetBits(SOFT_IIC_SCL_PORT, SOFT_IIC_SCL_PIN);
    delay_n_10us(1);

    while (GPIO_ReadInputDataBit(SOFT_IIC_SDA_PORT, SOFT_IIC_SDA_PIN))
    {
        error_time++;
        if (error_time > 250)
        {
            //drv_soft_iic_stop();
            return DRV_SOFT_IIC_STATUS_NACK;
        }
    }

    GPIO_ResetBits(SOFT_IIC_SCL_PORT, SOFT_IIC_SCL_PIN);

    return DRV_SOFT_IIC_STATUS_OK;
}

void drv_soft_iic_ack(void)
{
    GPIO_ResetBits(SOFT_IIC_SCL_PORT, SOFT_IIC_SCL_PIN);
    drv_soft_iic_sda_dir_set(DRV_SOFT_IIC_DIR_OUTPUT);
    GPIO_ResetBits(SOFT_IIC_SDA_PORT, SOFT_IIC_SDA_PIN);
    delay_n_10us(1);
    GPIO_SetBits(SOFT_IIC_SCL_PORT, SOFT_IIC_SCL_PIN);
    delay_n_10us(1);
    GPIO_ResetBits(SOFT_IIC_SCL_PORT, SOFT_IIC_SCL_PIN);
}

void drv_soft_iic_nack(void)
{
    GPIO_ResetBits(SOFT_IIC_SCL_PORT, SOFT_IIC_SCL_PIN);
    drv_soft_iic_sda_dir_set(DRV_SOFT_IIC_DIR_OUTPUT);
    GPIO_SetBits(SOFT_IIC_SDA_PORT, SOFT_IIC_SDA_PIN);
    delay_n_10us(1);
    GPIO_SetBits(SOFT_IIC_SCL_PORT, SOFT_IIC_SCL_PIN);
    delay_n_10us(1);
    GPIO_ResetBits(SOFT_IIC_SCL_PORT, SOFT_IIC_SCL_PIN);
}
drv_soft_iic_status_t drv_soft_iic_send_byte(uint8_t byt)
{
    uint8_t i = 0;
    //设置输入输出方向
    drv_soft_iic_sda_dir_set(DRV_SOFT_IIC_DIR_OUTPUT);
    //拉低IIC时钟总线开始传输数据
    GPIO_ResetBits(SOFT_IIC_SCL_PORT, SOFT_IIC_SCL_PIN);

    for (i=0; i<8; i++)
    {
        if (byt & 0x80)
        {
            GPIO_SetBits(SOFT_IIC_SDA_PORT, SOFT_IIC_SDA_PIN);
        }
        else
        {
            GPIO_ResetBits(SOFT_IIC_SDA_PORT, SOFT_IIC_SDA_PIN);
        }
        byt <<= 1;
        delay_n_10us(1);
        GPIO_SetBits(SOFT_IIC_SCL_PORT, SOFT_IIC_SCL_PIN);
        delay_n_10us(1);
        GPIO_ResetBits(SOFT_IIC_SCL_PORT, SOFT_IIC_SCL_PIN);
        delay_n_10us(1);
    }
    return DRV_SOFT_IIC_STATUS_OK;
}

drv_soft_iic_status_t drv_soft_iic_read_byte(uint8_t ack, uint8_t *byt)
{
    uint8_t i = 0;
    //设置输入输出方向
    drv_soft_iic_sda_dir_set(DRV_SOFT_IIC_DIR_INPUT);
    for (i=0; i<8; i++)
    {
        GPIO_ResetBits(SOFT_IIC_SCL_PORT, SOFT_IIC_SCL_PIN);
        delay_n_10us(1);
        GPIO_SetBits(SOFT_IIC_SCL_PORT, SOFT_IIC_SCL_PIN);
        *byt <<= 1;
        if (GPIO_ReadInputDataBit(SOFT_IIC_SDA_PORT, SOFT_IIC_SDA_PIN))
        {
            *byt++;
        }
        delay_n_10us(1);
    }
    if (ack)
    {
        drv_soft_iic_ack();
    }
    else
    {
        drv_soft_iic_nack();
    }
    return DRV_SOFT_IIC_STATUS_OK;
}
