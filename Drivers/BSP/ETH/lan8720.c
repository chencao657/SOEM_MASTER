/**
  ******************************************************************************
  * �ļ�����: lan8720.c 
  * ��    ��: ӲʯǶ��ʽ�����Ŷ�
  * ��    ��: V1.0
  * ��д����: 2018-09-01
  * ��    ��: LWIP - TCP
  ******************************************************************************
  * ˵����
  * ����������Ӳʯstm32������YS-F4Proʹ�á�
  * 
  * �Ա���
  * ��̳��http://www.ing10bbs.com
  * ��Ȩ��ӲʯǶ��ʽ�����Ŷ����У��������á�
  ******************************************************************************
  */
/* ����ͷ�ļ� ----------------------------------------------------------------*/
	
#include "lan8720.h"
#include "usart.h"

#include "stm32F4xx_hal.h"
#include "stm32F4xx_hal_eth.h"
/* ˽�����Ͷ��� --------------------------------------------------------------*/

/* ˽�к궨�� ----------------------------------------------------------------*/

/* ˽�б��� ------------------------------------------------------------------*/
uint32_t lan8720addr;

/* ��չ���� ------------------------------------------------------------------*/
extern ETH_HandleTypeDef heth;

/* ˽�к���ԭ�� --------------------------------------------------------------*/

/* ������ --------------------------------------------------------------------*/
/**
  * ��������: LAN872��ʼ��
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: 
  */
int LAN8720_Init(void)
{
	uint32_t regvalue = 0, addr = 0;
	HAL_StatusTypeDef rtn;

	
	lan8720addr = 32;
	for(addr = 0; addr <= lan8720addr; addr ++)
	{
		rtn = HAL_ETH_ReadPHYRegister(&heth,  LAN8720_SMR, &regvalue);
		
		if( rtn != HAL_OK)
		{
			printf("Read Address %d error \r\n",addr);
			continue;
		}
		
		if((regvalue & LAN8720_SMR_PHY_ADDR) == addr)
		{
			lan8720addr = addr;
			
			break;
		}
	}
	
	printf("lan8720addr = %d\r\n",addr);
	
	if( lan8720addr > 31 )
			return -1;

	
	rtn = HAL_ETH_WritePHYRegister(&heth,LAN8720_BCR,LAN8720_BCR_SOFT_RESET);

	if( rtn != HAL_OK )
	{
		return -1;
	}
	do
	{
		rtn = HAL_ETH_ReadPHYRegister(&heth,  LAN8720_BCR, &regvalue);
		if( rtn != HAL_OK )
		{
				printf("HAL_ETH_ReadPHYRegister LAN8720_BCR Error \r\n");
				return -1;
		}
	}while( regvalue & LAN8720_BCR_SOFT_RESET );
	
	LAN8720A_StartAutoNego();	 
	
	return 0;
}

/**
  * ��������: LAN8720��ȡ����״̬
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
int LAN8720_GetLinkState(void)
{
	uint32_t readval = 0;
	int32_t rtn;
	
	//��״̬�Ĵ���
	rtn = HAL_ETH_ReadPHYRegister(&heth,  LAN8720_BSR, &readval);
	if( rtn != HAL_OK )
	{
	    printf("HAL_ETH_ReadPHYRegister LAN8720_BSR Error \r\n");
			return LAN8720_STATUS_READ_ERROR;
	}
	
	//�ٶ�һ��״̬�Ĵ���
	rtn = HAL_ETH_ReadPHYRegister(&heth, LAN8720_BSR, &readval);
	if( rtn != HAL_OK )
	{
	//    printf("HAL_ETH_ReadPHYRegister LAN8720_BSR Error \r\n");
			return LAN8720_STATUS_READ_ERROR;
	}
	
	if((readval & LAN8720_BSR_LINK_STATUS) == 0)
	{
		
			return LAN8720_STATUS_LINK_DOWN;    
	}
	
	/* Check Auto negotiaition */
	printf("111readval = %x \r\n",readval);
	rtn = HAL_ETH_ReadPHYRegister(&heth,  LAN8720_BCR, &readval);
	if( rtn != HAL_OK )
	{
			return LAN8720_STATUS_READ_ERROR;
	}
	printf("222readval = %x \r\n",readval);	
	if((readval & LAN8720_BCR_AUTONEGO_EN) != LAN8720_BCR_AUTONEGO_EN)
	{
		if(((readval & LAN8720_BCR_SPEED_SELECT) == LAN8720_BCR_SPEED_SELECT) && ((readval & LAN8720_BCR_DUPLEX_MODE) == LAN8720_BCR_DUPLEX_MODE)) 
		{
				return LAN8720_STATUS_100MBITS_FULLDUPLEX;
		}
		else if ((readval & LAN8720_BCR_SPEED_SELECT) == LAN8720_BCR_SPEED_SELECT)
		{
				return LAN8720_STATUS_100MBITS_HALFDUPLEX;
		}        
		else if ((readval & LAN8720_BCR_DUPLEX_MODE) == LAN8720_BCR_DUPLEX_MODE)
		{
				return LAN8720_STATUS_10MBITS_FULLDUPLEX;
		}
		else
		{
				return LAN8720_STATUS_10MBITS_HALFDUPLEX;
		}  		
	}
	else /* Auto Nego enabled */
	{
		rtn = HAL_ETH_ReadPHYRegister(&heth,  LAN8720_PHYSCSR, &readval);
		if( rtn != HAL_OK )
		{
				return LAN8720_STATUS_READ_ERROR;
		}
		
		/* Check if auto nego not done */
		if((readval & LAN8720_PHYSCSR_AUTONEGO_DONE) == 0)
		{
				return LAN8720_STATUS_AUTONEGO_NOTDONE;
		}
		
		if((readval & LAN8720_PHYSCSR_HCDSPEEDMASK) == LAN8720_PHYSCSR_100BTX_FD)
		{
				return LAN8720_STATUS_100MBITS_FULLDUPLEX;
		}
		else if ((readval & LAN8720_PHYSCSR_HCDSPEEDMASK) == LAN8720_PHYSCSR_100BTX_HD)
		{
				return LAN8720_STATUS_100MBITS_HALFDUPLEX;
		}
		else if ((readval & LAN8720_PHYSCSR_HCDSPEEDMASK) == LAN8720_PHYSCSR_10BT_FD)
		{
				return LAN8720_STATUS_10MBITS_FULLDUPLEX;
		}
		else
		{
				return LAN8720_STATUS_10MBITS_HALFDUPLEX;
		}				
	}
}

/**
  * ��������: ��ȡPHY�Ĵ���ֵ
  * �������: reg��Ҫ��ȡ�ļĴ�����ַ
  * �� �� ֵ: 0����ȡ�ɹ���-1����ȡʧ��
  * ˵    ��: ��
  */
int32_t ETH_PHY_ReadReg(uint16_t reg,uint32_t *regval)
{
    if(HAL_ETH_ReadPHYRegister(&heth,reg,regval)!=HAL_OK)
        return -1;
    return 0;
}

/**
  * ��������: ��LAN8720Aָ���Ĵ���д��ֵ
  * �������: reg��Ҫ��ȡ�ļĴ�����ַ ��value:Ҫд���ֵ
  * �� �� ֵ: 0��д��������-1��д��ʧ��
  * ˵    ��: ��
  */
int32_t ETH_PHY_WriteReg(uint16_t reg,uint16_t value)
{
    uint32_t temp=value;
    if(HAL_ETH_WritePHYRegister(&heth,reg,temp)!=HAL_OK)
        return -1;
    return 0;
}

/**
  * ��������: ����LAN8720A����Э�̹���
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
void LAN8720A_StartAutoNego(void)
{
    uint32_t readval=0;
    ETH_PHY_ReadReg(LAN8720_BCR,&readval);
    readval|=LAN8720_BCR_AUTONEGO_EN;
    ETH_PHY_WriteReg(LAN8720_BCR,readval);
}


/********** (C) COPYRIGHT 2019-2030 ӲʯǶ��ʽ�����Ŷ� *******END OF FILE******/

