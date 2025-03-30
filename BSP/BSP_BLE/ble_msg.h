/*
 *  crc.h
 *  Created on: 2023年9月21日
 *  Author: qikaige
 */

#ifndef _BLE_MSG_H_
#define _BLE_MSG_H_

#include "stdint.h"

#define ACK_ALL_NODES_MSG_LENGTH      64
typedef struct
{
    uint8_t  buf[ACK_ALL_NODES_MSG_LENGTH];
    uint16_t len;
}all_nodes_ack_msg_t;


void ble_recv_msg(void);
void ble_create_msg(uint16_t func_code, all_nodes_ack_msg_t *msg);


#endif /* APPLICATIONS_BLUETOOTH_MY_APP_BLE_SLAVE_CRC_H_ */
