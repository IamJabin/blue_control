/**
  ******************************************************************************
  * @file    app_func_code.h
  * @author  qikaige
  * @version V1.0
  * @date    2023-10-26
  * @brief
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
/** @addtogroup APP
* @{
*/
#ifndef __APP_FUNC_CODE_H__
#define __APP_FUNC_CODE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"




#define  FRAME_HEADER1       					0x50        	// 'P'
#define  FRAME_HEADER2       					0x4C        	// 'L'
#define  FRAME_END1          					0xA5
#define  FRAME_END2          					0x5A

#define  RESERVED_VAL  								0x00


//功能码
#define FUNC_CODE_TIMING    		     	0x0001     		
#define FUNC_CODE_TIMING_ACK				  0x0002     
#define FUNC_CODE_STOP    		     	  0x0003		
#define FUNC_CODE_STOP_ACK    		 	  0x0004	
#define FUNC_CODE_GET_TIME    		    0x0005
#define FUNC_CODE_GET_TIME_ACK    		0x0006


#ifdef __cplusplus
}
#endif

#endif

/**
  * @}
  */
/******************* (C) COPYRIGHT 2023 PengLi ******END OF FILE******************/
