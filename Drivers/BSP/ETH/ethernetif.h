/**
  ******************************************************************************
  * �ļ�����: eth.h
  * ��    ��: ӲʯǶ��ʽ�����Ŷ�
  * ��    ��: V1.0
  * ��д����: 2018-09-01
  * ��    ��: ethͷ�ļ�
  ******************************************************************************
  * ˵����
  * ����������Ӳʯstm32������YS-F4Proʹ�á�
  * 
  * �Ա���
  * ��̳��http://www.ing10bbs.com
  * ��Ȩ��ӲʯǶ��ʽ�����Ŷ����У��������á�
  ******************************************************************************
  */
#ifndef __eth_H
#define __eth_H
#ifdef __cplusplus
 extern "C" {
#endif

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "stm32F4xx_hal.h"
#include "main.h"

/* ˽�����Ͷ��� --------------------------------------------------------------*/

/* ˽�к궨�� ----------------------------------------------------------------*/
#define ETH_RCC_CLK_ENABLE()                __HAL_RCC_ETH_CLK_ENABLE()

#define ETH_GPIO_ClK_ENABLE()              {__HAL_RCC_GPIOA_CLK_ENABLE();__HAL_RCC_GPIOC_CLK_ENABLE();\
                                            __HAL_RCC_GPIOB_CLK_ENABLE();__HAL_RCC_GPIOG_CLK_ENABLE();}

#define GPIO_AFx_ETH                        GPIO_AF11_ETH
																						
#ifndef ETH_TX_DESC_CNT
 #define ETH_TX_DESC_CNT         4 
#endif
	 
#ifndef ETH_RX_DESC_CNT
 #define ETH_RX_DESC_CNT         4 
#endif
																						
/* ˽�б��� ------------------------------------------------------------------*/

/* ��չ���� ------------------------------------------------------------------*/

extern ETH_HandleTypeDef heth;

/* ˽�к���ԭ�� --------------------------------------------------------------*/

/* ������ --------------------------------------------------------------------*/

void MX_ETH_Init(void);
void PHY_Init(void);
void low_level_output(uint8_t *p,uint32_t length);
int bfin_EMAC_send (void *packet, int length);
int bfin_EMAC_recv (uint8_t * packet, size_t size);
void printfBuffer(uint8_t *dat, uint32_t len );


#ifdef __cplusplus
}
#endif
#endif /*__ eth_H */


/********** (C) COPYRIGHT 2019-2030 ӲʯǶ��ʽ�����Ŷ� *******END OF FILE******/
