/**
  ******************************************************************************
  * @file    rs_queue_list_config.c
  * @author  qikaige
  * @version V1.0
  * @date    2023-09-21
  * @brief   ble组件队列配置
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
#include "rs_queue_list_config.h"
#include <stdio.h>

#define    BLE_LIST_TX_NUMBER      		512
#define    BLE_LIST_RX_NUMBER      		512

uint8_t ble_tx_buf[BLE_LIST_TX_NUMBER];
uint8_t ble_rx_buf[BLE_LIST_RX_NUMBER];



rs_queue_list_t ble_tx_list;
rs_queue_list_t ble_rx_list;


/**
  ******************************************************************************
  * @brief  ble List列表初始化
  * @param  None.
  * @retval None.
  ******************************************************************************/
void ble_list_init(void)
{
	rs_queue_list_init(&ble_tx_list, ble_tx_buf, BLE_LIST_TX_NUMBER);
	rs_queue_list_init(&ble_rx_list, ble_rx_buf, BLE_LIST_RX_NUMBER);
}

/**
  * @}
  */
/******************* (C) COPYRIGHT 2023 PengLi ******END OF FILE******************/

