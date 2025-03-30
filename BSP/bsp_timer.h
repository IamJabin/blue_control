
#ifndef __BSP_TIMER_H__
#define __BSP_TIMER_H__

#include <stdint.h>

typedef enum
{
    BSP_TIMER_MODE_STOP = 0,
    BSP_TIMER_MODE_RUNNING,
}bsp_timer_mode_t;

typedef enum 
{
    BSP_TIMER_STATUS_OK = 0,
    BSP_TIMER_STATUS_ERROR,
}bsp_timer_status_t;

void bsp_timer_init(void);
bsp_timer_status_t bsp_timer_start(void);
void bsp_timer_stop(void);
bsp_timer_status_t bsp_timer_set(int32_t value);
void bsp_timer_get(uint32_t *value);

__weak void bsp_timer_stop_callback(void);
__weak void bsp_timer_start_callback(void);

#endif  // !__BSP_TIMER_H__
