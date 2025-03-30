/*
 *  crc.h
 *  Created on: 2023年9月21日
 *  Author: qikaige
 */

#ifndef _RS_QUEUE_LIST_H_
#define _RS_QUEUE_LIST_H_

#include "stdint.h"

typedef struct
{
    uint16_t head;              /* 队列前指针 */
    uint16_t trail;             /* 队列底指针 */
    uint16_t lenmax;            /* 队列长度   */
    uint8_t  *pdata;            /* 消息数组   */
}rs_queue_list_t;

void rs_queue_list_init(rs_queue_list_t *plist, uint8_t * pbuf, uint16_t len);
uint8_t rs_queue_list_clr(rs_queue_list_t *pmsg_queue);
uint8_t rs_queue_list_is_full(rs_queue_list_t *pmsg_queue);
uint8_t rs_queue_list_is_empty(rs_queue_list_t *pmsg_queue);
uint16_t rs_queue_list_get_total(rs_queue_list_t *pmsg_queue);
uint8_t rs_queue_list_post(rs_queue_list_t *pmsg_queue, uint8_t dat);
uint8_t rs_queue_list_get(rs_queue_list_t *pmsg_queue, uint8_t *rdata);

#endif /* APPLICATIONS_BLUETOOTH_MY_APP_BLE_SLAVE_CRC_H_ */
