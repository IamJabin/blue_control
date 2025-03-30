#ifndef __BSP_ALARM_H__
#define __BSP_ALARM_H__

#include "stdint.h"
#include "n32wb03x.h"

void RTC_CLKSourceConfig(uint8_t ClkSrc);

void RTC_DateAndTimeDefaultVale(void);
void RTC_PrescalerConfig(void);

void RTC_TimeShow(void);
void RTC_DateShow(void);
void RTC_AlarmShow(uint8_t AlarmX);
void EXTI8_RTCAlarm_Configuration(FunctionalState Cmd);



#endif /* __BSP_ALARM_H__ */

