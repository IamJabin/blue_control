/**
  ******************************************************************************
  * @file    rs_queue_list.c
  * @author  qikaige
  * @version V1.0
  * @date    2023-09-21
  * @brief   ble组件队列
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
/** @addtogroup ble
* @{
*/
#include "ble_msg.h"
#include "rs_queue_list.h"
#include <stdio.h>
#include "rs_queue_list_config.h"
#include "app_func_code.h"
#include <string.h>
#include "crc.h"
#include "ns_log.h"
#include "bsp_timer.h"
#include "rwip_config.h" 
#include "app_rdtss.h"
//#define  HDIV_API

extern uint32_t SystickGetime(void);

/**
 ******************************************************************************
 * @brief  队列初始化
 * @param  plist：队列指针
 * @param  pbuf：队列数据缓冲区指针
 * @param  len：队列的数据缓冲区长度
 * @return None.
 ******************************************************************************/
//static uint16_t timing_on_time;//定时开启
//static hosal_rtc_dev_t rtc_dev;//RTC功能开发

typedef struct
{
	uint8_t main_index;
	uint8_t data_index;

	uint16_t funcode;
	uint16_t payload_len;
	uint8_t  payload_buf[128];
	uint32_t crc32;

	uint8_t poscnt;
	uint8_t pos_buf[128];

	uint8_t msg_buf[128];
	uint16_t msg_len;
}rsc_recv_msg_t;

static rsc_recv_msg_t rs_com_in_recv_msg;



void ble_recv_msg(void)
{
	static uint32_t timeout = 0;
	uint8_t tmp = 0;
	int i = 0;
	all_nodes_ack_msg_t ack_msg;
	/*从列表中读取数据*/
	if(rs_queue_list_get(&ble_rx_list, &tmp))
	{

		timeout = SystickGetime();
		NS_LOG_INFO("ci rv:%02x\r\n", tmp);
		switch(rs_com_in_recv_msg.main_index)
		{
			case 0: //1byte
				if(tmp == FRAME_HEADER1)
				{
					rs_com_in_recv_msg.main_index = 1;
					rs_com_in_recv_msg.msg_buf[rs_com_in_recv_msg.msg_len++] = tmp;
				}
				else
				{
					memset(&rs_com_in_recv_msg, 0, sizeof(rsc_recv_msg_t));    //清除所有状态
				}
			break;
			case 1://1byte
				if(tmp == FRAME_HEADER2)
				{
					rs_com_in_recv_msg.main_index = 2;
					rs_com_in_recv_msg.poscnt = 0;
					rs_com_in_recv_msg.msg_buf[rs_com_in_recv_msg.msg_len++] = tmp;
				}
				else
				{
					memset(&rs_com_in_recv_msg, 0, sizeof(rsc_recv_msg_t));    //清除所有状态
				}
			break;
			case 2://2byte
				rs_com_in_recv_msg.msg_buf[rs_com_in_recv_msg.msg_len++] = tmp;
				rs_com_in_recv_msg.pos_buf[rs_com_in_recv_msg.poscnt++] = tmp;
				/*功能码*/
				if(rs_com_in_recv_msg.poscnt == 2)
				{
					rs_com_in_recv_msg.funcode = ((uint16_t)rs_com_in_recv_msg.pos_buf[0] << 8) | rs_com_in_recv_msg.pos_buf[1];
					rs_com_in_recv_msg.poscnt = 0;
					rs_com_in_recv_msg.main_index = 3;
				}
			break;
			case 3:
				rs_com_in_recv_msg.msg_buf[rs_com_in_recv_msg.msg_len++] = tmp;
				switch(rs_com_in_recv_msg.data_index)
				{
					case 0:
						rs_com_in_recv_msg.pos_buf[rs_com_in_recv_msg.poscnt++] = tmp;
						/*数据长度*/
						if(rs_com_in_recv_msg.poscnt == 2)
						{
							//payload_len = 8;
							rs_com_in_recv_msg.payload_len = ((uint16_t)rs_com_in_recv_msg.pos_buf[0] << 8) | rs_com_in_recv_msg.pos_buf[1];
							rs_com_in_recv_msg.poscnt = 0;
							rs_com_in_recv_msg.data_index = 1;
						}
					break;
					case 1:
						rs_com_in_recv_msg.pos_buf[rs_com_in_recv_msg.poscnt++] = tmp;
						/*data段数据长度和实际数据长度相等*/
						if(rs_com_in_recv_msg.poscnt == rs_com_in_recv_msg.payload_len)
						{
							memcpy(rs_com_in_recv_msg.payload_buf, rs_com_in_recv_msg.pos_buf, rs_com_in_recv_msg.payload_len);
							rs_com_in_recv_msg.poscnt = 0;
							rs_com_in_recv_msg.data_index = 2;
						}
						else
						{
							NS_LOG_INFO("length not equal, length = %d\r\n", rs_com_in_recv_msg.payload_len);
						}
					break;
					case 2:
						rs_com_in_recv_msg.pos_buf[rs_com_in_recv_msg.poscnt++] = tmp;
						/*获取crc32校验值*/
						if(rs_com_in_recv_msg.poscnt == 4)
						{
							uint32_t l_crc32 = 0;

							rs_com_in_recv_msg.crc32 = ((uint32_t)rs_com_in_recv_msg.pos_buf[0] << 24) | \
												  ((uint32_t)rs_com_in_recv_msg.pos_buf[1] << 16) | \
												  ((uint32_t)rs_com_in_recv_msg.pos_buf[2] << 8)  | \
												  ((uint32_t)rs_com_in_recv_msg.pos_buf[3] << 0);
							l_crc32 = drv_crc32_calculate(rs_com_in_recv_msg.msg_buf, rs_com_in_recv_msg.msg_len - 4);
							if(rs_com_in_recv_msg.crc32 == l_crc32)
							{
								rs_com_in_recv_msg.poscnt = 0;
								rs_com_in_recv_msg.main_index = 4;
							}
							else
							{
								NS_LOG_INFO("com_in recv crc error, crc=0x%x\r\n", l_crc32);
							}
						}
					break;
				}
			break;
			case 4:
				rs_com_in_recv_msg.msg_buf[rs_com_in_recv_msg.msg_len++] = tmp;
				if(tmp == FRAME_END1)
				{
					rs_com_in_recv_msg.main_index = 5;
				}
				else
				{
					memset(&rs_com_in_recv_msg, 0, sizeof(rsc_recv_msg_t));    //清除所有状态
				}
			break;
			case 5:
				rs_com_in_recv_msg.msg_buf[rs_com_in_recv_msg.msg_len++] = tmp;
				if(tmp == FRAME_END2)
				{

					switch(rs_com_in_recv_msg.funcode)
					{
						case FUNC_CODE_TIMING:
							NS_LOG_INFO("FUNC_CODE_TIMING\r\n");
							bsp_timer_stop();
							bsp_timer_set(((rs_com_in_recv_msg.msg_buf[6] << 8 )| rs_com_in_recv_msg.msg_buf[7])*60);
							NS_LOG_INFO("time=%d\r\n", (rs_com_in_recv_msg.msg_buf[6] << 8 )| rs_com_in_recv_msg.msg_buf[7]);
							bsp_timer_start();
							ble_create_msg(FUNC_CODE_TIMING_ACK ,&ack_msg);
							rdtss_send_notify(ack_msg.buf, ack_msg.len);
						break;
						case FUNC_CODE_STOP:
							NS_LOG_INFO("FUNC_CODE_STOP\r\n");
							bsp_timer_stop();
							ble_create_msg(FUNC_CODE_STOP_ACK ,&ack_msg);
							rdtss_send_notify(ack_msg.buf, ack_msg.len);
						break;		
						case FUNC_CODE_GET_TIME:
							NS_LOG_INFO("FUNC_CODE_GET_TIME\r\n");
							ble_create_msg(FUNC_CODE_GET_TIME_ACK ,&ack_msg);
							rdtss_send_notify(ack_msg.buf, ack_msg.len);
						break;		
						default:
							NS_LOG_INFO("not expected function code\r\n");
						break;									
					}
					for(i = 0; i < rs_com_in_recv_msg.msg_len; i++)
					{
						NS_LOG_INFO("%02x ", rs_com_in_recv_msg.msg_buf[i]);
					}
					NS_LOG_INFO("\r\n");
					memset(&rs_com_in_recv_msg, 0, sizeof(rsc_recv_msg_t));    //清除所有状态
				}
				else
				{
					memset(&rs_com_in_recv_msg, 0, sizeof(rsc_recv_msg_t));    //清除所有状态
				}
			break;
			default:
			break;
		}

	}
	if(SystickGetime() - timeout > 50)
	{
		memset(&rs_com_in_recv_msg, 0, sizeof(rsc_recv_msg_t));    //清除所有状态
	}
}

/**
  ******************************************************************************
  * @brief  ble com_in创建消息
  * @param  func_code：功能码
  * @param  msg：消息体
  * @retval None.
  ******************************************************************************/
void ble_create_msg(uint16_t func_code, all_nodes_ack_msg_t *msg)
{
	uint16_t i = 0;
	uint16_t index = 0;
	uint16_t length_point = 0;
	uint16_t data_section_len = 0;
	uint32_t crc32 = 0;
	uint32_t remainder_time = 0;

	/*帧头：2byte*/
	msg->buf[index++] = FRAME_HEADER1;
	msg->buf[index++] = FRAME_HEADER2;


	/*功能码：2byte*/
	msg->buf[index++] = (func_code >> 8) & 0x00FF;
	msg->buf[index++] = (func_code >> 0) & 0x00FF;

	length_point = index;
	index += 2;     //数据长度占用2byte，先占位2byte

	switch(func_code)
	{
		case FUNC_CODE_TIMING_ACK:
			msg->buf[index++] = 0xFF;
			msg->buf[index++] = 0xFF;
		break;

		case FUNC_CODE_STOP_ACK:
			msg->buf[index++] = 0xFF;
			msg->buf[index++] = 0xFF;
		break;

		case FUNC_CODE_GET_TIME_ACK:
		//将剩余时间以MS的形式传入
			bsp_timer_get(&remainder_time);
			remainder_time = remainder_time/60;
			msg->buf[index++] = (remainder_time >> 8) & 0x00FF;
			msg->buf[index++] = remainder_time & 0x00FF;
		break;

		default:

		break;
	}

	data_section_len = index - length_point - 2;

	/*数据长度：2byte*/
	msg->buf[length_point++] = (uint8_t)(data_section_len >>  8) & 0xFF;
	msg->buf[length_point++] = (uint8_t)(data_section_len >>  0) & 0xFF;

	/*crc校验值：4byte*/
	crc32 = drv_crc32_calculate(msg->buf, index);
	msg->buf[index++] = (uint8_t)(crc32 >> 24) & 0xFF;
	msg->buf[index++] = (uint8_t)(crc32 >> 16) & 0xFF;
	msg->buf[index++] = (uint8_t)(crc32 >>  8) & 0xFF;
	msg->buf[index++] = (uint8_t)(crc32 >>  0) & 0xFF;


	msg->buf[index++] = FRAME_END1;
	msg->buf[index++] = FRAME_END2;

	msg->len = index;


	for(i = 0; i < msg->len; i++)
	{
		NS_LOG_DEBUG("%02x ", msg->buf[i]);
	}
	NS_LOG_DEBUG("\r\n");
}


/**
  * @}
  */
/******************* (C) COPYRIGHT 2023 PengLi ******END OF FILE******************/












