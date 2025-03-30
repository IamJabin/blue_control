/**
  ******************************************************************************
  * @file    rs_queue_list_config.h
  * @author  qiakige
  * @version V1.0
  * @date    2023-09-21
  * @brief   ble组件队列配置头文件
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
/** @addtogroup DRV
* @{
*/
#ifndef __RS_QUEUE_LIST_CONFIG_H__
#define __RS_QUEUE_LIST_CONFIG_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "rs_queue_list.h"


extern rs_queue_list_t ble_tx_list;
extern rs_queue_list_t ble_rx_list;

void ble_list_init(void);


#ifdef __cplusplus
}
#endif

#endif

/**
  * @}
  */
/******************* (C) COPYRIGHT 2023 PengLi ******END OF FILE******************/
