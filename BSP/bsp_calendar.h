#ifndef __BSP_CALENDAR_H__
#define __BSP_CALENDAR_H__

#include "stdint.h"
#include "n32wb03x.h"

void RTC_CLKSourceConfig1(uint8_t ClkSrc);
void RTC_DateAndTimeDefaultVale1(void);
void RTC_PrescalerConfig1(void);
void TIM3_Configuration1(void);
void RTC_TimeShow1(void);
void RTC_DateShow1(void);

#endif /* __BSP_CALENDAR_H__ */

