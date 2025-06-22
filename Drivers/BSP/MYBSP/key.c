/*
*********************************************************************************************************
*
*	模块名称 : 独立按键驱动模块 
*	文件名称 : key.c
*	版    本 : V1.0
*	说    明 : 1.外部中断检测按键是否按下
*						 2.回调函数中将按键状态存入按键fifo
*						 3.调用getkey读取按键fifo
*
*	修改记录 :
*		版本号     日期        作者      
*		V1.0    2025-05-14  shisanxiang1
*
*********************************************************************************************************
*/

#include "key.h"
#include "gpio.h"
#include "usart.h"

/* 按键fifo结构体定义*/
KEY_FIFO_T s_tKey;
/*
*********************************************************************************************************
*	函 数 名:  HAL_GPIO_EXTI_Callbac
*	功能说明: hal库外部中断回调函数，调用bsp_PutKey存入按键值
*	形    参: 
*********************************************************************************************************
*/

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin==KEY1_GPIO_PIN)
  {
    if(HAL_GPIO_ReadPin(KEY1_GPIO,KEY1_GPIO_PIN)==KEY1_DOWN_LEVEL)
    {
			bsp_PutKey(1);
    }

  }
  else if(GPIO_Pin==KEY2_GPIO_PIN)
  {
    if(HAL_GPIO_ReadPin(KEY2_GPIO,KEY2_GPIO_PIN)==KEY2_DOWN_LEVEL)
    {
			bsp_PutKey(2);
    }

  }
  else if(GPIO_Pin==KEY3_GPIO_PIN)
  {
    if(HAL_GPIO_ReadPin(KEY3_GPIO,KEY3_GPIO_PIN)==KEY3_DOWN_LEVEL)
    {
			bsp_PutKey(3);
    }

  }
  else if(GPIO_Pin==KEY4_GPIO_PIN)
  {
    if(HAL_GPIO_ReadPin(KEY4_GPIO,KEY4_GPIO_PIN)==KEY4_DOWN_LEVEL)
    {
			bsp_PutKey(4);
    }
  }
}

/*
*********************************************************************************************************
*	函 数 名:key_fifoinit
*	功能说明: fifo初始化，主函数中需要调用一次key_fifoinit(&s_tKey);
*	形    参: &s_tKey
*********************************************************************************************************
*/
void key_fifoinit(KEY_FIFO_T *s_tKey)
{
/* 对按键FIFO读写指针清零 */
	s_tKey->Read = 0;
	s_tKey->Write = 0;
}


/*
*********************************************************************************************************
*	函 数 名: bsp_PutKey
*	功能说明: 将1个键值压入按键FIFO缓冲区。
*	形    参:  _KeyCode : 按键代码
*	返 回 值: 无
*********************************************************************************************************
*/

void bsp_PutKey(uint8_t _KeyCode)
{
    uint8_t nextWrite = (s_tKey.Write + 1) % KEY_FIFO_SIZE;
    
    if (nextWrite != s_tKey.Read)  // 检查FIFO是否已满
    {
        s_tKey.Buf[s_tKey.Write] = _KeyCode;
        s_tKey.Write = nextWrite;
    }
    else
    {
      /* fifo溢出处理 */
			// 不执行任何操作即可丢弃新事件
    }
}
/*
*********************************************************************************************************
*	函 数 名: bsp_GetKey
*	功能说明: 从按键FIFO缓冲区读取一个键值。
*	形    参: 无
*	返 回 值: 按键代码
*********************************************************************************************************
*/

uint8_t bsp_GetKey(void)
{
    uint8_t ret = KEY_NONE;

    if (s_tKey.Read != s_tKey.Write)  // 检查FIFO是否为空
    {
        ret = s_tKey.Buf[s_tKey.Read];
        s_tKey.Read = (s_tKey.Read + 1) % KEY_FIFO_SIZE;
    }
    return ret;
}



