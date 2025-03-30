#ifndef __BSP_DIGITRON_H__
#define __BSP_DIGITRON_H__

#include <stdint.h>

#define BSP_DIGITRON_USING_HARDWARE_IIC 0

typedef enum
{
    BSP_DIGITRON_POWER_OFF = 0,
    BSP_DIGITRON_POWER_ON
}bsp_DigitronPower_t;

typedef enum
{
    BSP_DIGITRON_BRIGHTNESS_8 = 0,
    BSP_DIGITRON_BRIGHTNESS_1,
    BSP_DIGITRON_BRIGHTNESS_2,
    BSP_DIGITRON_BRIGHTNESS_3,
    BSP_DIGITRON_BRIGHTNESS_4,
    BSP_DIGITRON_BRIGHTNESS_5,
    BSP_DIGITRON_BRIGHTNESS_6,
    BSP_DIGITRON_BRIGHTNESS_7
}bsp_DigitronBrightness_t;


void bsp_InitDigitron(void);
void bsp_DigitronDisplay(uint8_t first, uint8_t second, uint8_t third, uint8_t fourth, uint8_t point);
void bsp_DigitronPowerBrightnessSet(bsp_DigitronPower_t power,bsp_DigitronBrightness_t brightness);
void bsp_DigitronDisplayTime(uint32_t time);
#endif // !__BSP_DIGITRON_H__
