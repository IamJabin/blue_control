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
#include "rs_queue_list.h"
#include <stdio.h>

//#define  HDIV_API

/**
 ******************************************************************************
 * @brief  队列初始化
 * @param  plist：队列指针
 * @param  pbuf：队列数据缓冲区指针
 * @param  len：队列的数据缓冲区长度
 * @return None.
 ******************************************************************************/
void rs_queue_list_init(rs_queue_list_t *plist, uint8_t * pbuf, uint16_t len)
{
    // 参数判断
    if (plist == NULL)
    {
        while(1);
    }

    // 初始化队列
    plist->head  = 0;
    plist->trail = 0;
    plist->pdata = pbuf;

    // 数据指针为空时，将队列长度设置为0
    if (pbuf == NULL)
    {
        plist->lenmax = 0;
    }
    else
    {
        plist->lenmax = len;
    }
}

/**
 ******************************************************************************
 * @brief  将缓冲区清空
 * @param  pmsg_queue：队列指针
 * @return 0：参数有误  1：缓冲区清空成功.
 ******************************************************************************/
uint8_t rs_queue_list_clr(rs_queue_list_t *pmsg_queue)
{
    // 参数检查
    if (pmsg_queue == NULL)
    {
        return 0;
    }

    // 初始化队列
    pmsg_queue->head  = 0;
    pmsg_queue->trail = 0;

    return 1;
}

/**
 ******************************************************************************
 * @brief  检查队列是否为满
 * @param  pmsg_queue：队列指针
 * @return 0：不满  1：满
 ******************************************************************************/
uint8_t rs_queue_list_is_full(rs_queue_list_t *pmsg_queue)
{
    uint16_t tmp;

    // 参数检查
    if (pmsg_queue == NULL || pmsg_queue->pdata == NULL || pmsg_queue->lenmax == 0)
    {
        return 1;
    }

	#ifndef HDIV_API
		// 检查是否为满
		tmp = (pmsg_queue->trail + 1) % pmsg_queue->lenmax;
    #else
		tmp = drv_hdiv_get_remainder(pmsg_queue->trail + 1, pmsg_queue->lenmax);
	#endif
    if (pmsg_queue->head == tmp)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 ******************************************************************************
 * @brief  检查队列是否为空
 * @param  pmsg_queue：队列指针
 * @return 0：不为空  1：为空
 ******************************************************************************/
uint8_t rs_queue_list_is_empty(rs_queue_list_t *pmsg_queue)
{
    // 参数检查
    if (pmsg_queue == NULL || pmsg_queue->pdata == NULL || pmsg_queue->lenmax == 0)
    {
        return 1;
    }

    // 检查是否为空
    if (pmsg_queue->head == pmsg_queue->trail)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 ******************************************************************************
 * @brief  获取队列中消息的数量
 * @param  pmsg_queue：队列指针
 * @return 队列中数据数量
 ******************************************************************************/
uint16_t rs_queue_list_get_total(rs_queue_list_t *pmsg_queue)
{
    uint16_t tmp;

    // 参数检查
    if (pmsg_queue == NULL || pmsg_queue->pdata == NULL || pmsg_queue->lenmax == 0)
    {
        return 0;
    }

    // 计算数量
    if (pmsg_queue->head == pmsg_queue->trail)
    {
        return 0;
    }
    else if (pmsg_queue->head < pmsg_queue->trail)
    {
        return (pmsg_queue->trail - pmsg_queue->head);
    }
    else
    {
        tmp = pmsg_queue->lenmax - pmsg_queue->head;
        return (tmp + pmsg_queue->trail);
    }
}


/**
 ******************************************************************************
 * @brief  向输出队列中增加一个数据
 * @param  pmsg_queue：队列指针
 * @param  dat：要写入的数据
 * @return 1：写入失败  0：写入成功
 ******************************************************************************/
uint8_t rs_queue_list_post(rs_queue_list_t *pmsg_queue, uint8_t dat)
{
    uint16_t tmp;

    // 参数检查
    if (pmsg_queue == NULL || pmsg_queue->pdata == NULL || pmsg_queue->lenmax == 0)
    {
        return 1;
    }

	#ifndef HDIV_API
		// 队列已经满了
		tmp = (pmsg_queue->trail + 1) % pmsg_queue->lenmax;
	#else
		tmp = drv_hdiv_get_remainder(pmsg_queue->trail + 1, pmsg_queue->lenmax);
	#endif
    if (tmp == pmsg_queue->head)
    {
        return 1;
    }

    // 写入数据
    *(pmsg_queue->pdata + pmsg_queue->trail) = dat;
    pmsg_queue->trail = tmp;

    return 0;
}

/**
 ******************************************************************************
 * @brief  从输出队列中获取一个字节的数据
 * @param  pmsg_queue：队列指针
 * @param  rdata：获取数据指针
 * @return 0：队列中无数据，获取失败  1：获取数据成功
 ******************************************************************************/
uint8_t rs_queue_list_get(rs_queue_list_t *pmsg_queue, uint8_t *rdata)
{
    // 参数检查
    if (pmsg_queue == NULL || pmsg_queue->pdata == NULL || pmsg_queue->lenmax == 0)
    {
        return 0;
    }

    // 数据为空
    if (pmsg_queue->head == pmsg_queue->trail)
    {
        return 0;
    }

    // 取出数据
    *rdata = *(pmsg_queue->pdata + pmsg_queue->head);
	#ifndef HDIV_API
		pmsg_queue->head = (pmsg_queue->head + 1) % pmsg_queue->lenmax;
    #else
		pmsg_queue->head = drv_hdiv_get_remainder(pmsg_queue->head + 1, pmsg_queue->lenmax);
	#endif
    return 1;
}




/**
  * @}
  */
/******************* (C) COPYRIGHT 2023 PengLi ******END OF FILE******************/












