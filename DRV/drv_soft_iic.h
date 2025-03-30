#ifndef __DRV_SOFT_IIC_H__
#define __DRV_SOFT_IIC_H__

#include "n32wb03x.h"

#define SOFT_IIC_SDA_GPIO_CLK RCC_APB2_PERIPH_GPIOB
#define SOFT_IIC_SCL_GPIO_CLK RCC_APB2_PERIPH_GPIOB
#define SOFT_IIC_SDA_PORT GPIOB
#define SOFT_IIC_SDA_PIN  GPIO_PIN_6
#define SOFT_IIC_SCL_PORT GPIOB
#define SOFT_IIC_SCL_PIN  GPIO_PIN_7

typedef enum
{
    DRV_SOFT_IIC_DIR_INPUT = 0,
    DRV_SOFT_IIC_DIR_OUTPUT,
}drv_soft_iic_dir_t;

typedef enum
{
    DRV_SOFT_IIC_STATUS_OK = 0,
    DRV_SOFT_IIC_STATUS_ERROR,
    DRV_SOFT_IIC_STATUS_NACK,
}drv_soft_iic_status_t;

drv_soft_iic_status_t drv_soft_iic_init(void);
void drv_soft_iic_start(void);
void drv_soft_iic_stop(void);
drv_soft_iic_status_t drv_soft_iic_wait_ack(void);
void drv_soft_iic_ack(void);
drv_soft_iic_status_t drv_soft_iic_send_byte(uint8_t byt);
drv_soft_iic_status_t drv_soft_iic_read_byte(uint8_t ack, uint8_t *byt);

#endif // !__DRV__
