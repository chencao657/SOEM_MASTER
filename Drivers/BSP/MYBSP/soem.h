/**
  ******************************************************************************
  * �ļ�����: ecatuser.h
  * ��    ��: ӲʯǶ��ʽ�����Ŷ�
  * ��    ��: V1.0
  * ��д����: 2018-09-01
  * ��    ��: ecatuserͷ�ļ�
  ******************************************************************************
  * ˵����
  * ����������Ӳʯstm32������YS-F4Proʹ�á�
  * 
  * �Ա���
  * ��̳��http://www.ing10bbs.com
  * ��Ȩ��ӲʯǶ��ʽ�����Ŷ����У��������á�
  ******************************************************************************
  */
#ifndef _ECATUSER_H_
#define _ECATUSER_H_

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "ethercat.h"
#include "osal.h"

/* ˽�����Ͷ��� --------------------------------------------------------------*/
PACKED_BEGIN
typedef struct PACKED
{
	uint16 controlword;
	uint32 accelerAtion;
	uint32 decelerAtion;
	uint32 targetVelocity;
	
}PDO_Outputs; //1601
PACKED_END

PACKED_BEGIN
typedef struct PACKED
{
	uint16 statusWord;
	uint32 actualPostion;
	uint32 actualVelocity;
}PDO_Input;
PACKED_END
/* ˽�к궨�� ----------------------------------------------------------------*/

/* ��չ���� ------------------------------------------------------------------*/
extern int32_t ecOffset;

/* ˽�к���ԭ�� --------------------------------------------------------------*/

/* ������ --------------------------------------------------------------------*/

void ecat_loop_init(void);
void ecat_init(void);
void ecat_loop(void);
void ctrl_state(void);


#endif


/********** (C) COPYRIGHT 2019-2030 ӲʯǶ��ʽ�����Ŷ� *******END OF FILE******/

