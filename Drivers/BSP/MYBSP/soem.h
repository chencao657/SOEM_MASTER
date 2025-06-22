/**
  ******************************************************************************
  * 文件名程: ecatuser.h
  * 作    者: 硬石嵌入式开发团队
  * 版    本: V1.0
  * 编写日期: 2018-09-01
  * 功    能: ecatuser头文件
  ******************************************************************************
  * 说明：
  * 本例程配套硬石stm32开发板YS-F4Pro使用。
  * 
  * 淘宝：
  * 论坛：http://www.ing10bbs.com
  * 版权归硬石嵌入式开发团队所有，请勿商用。
  ******************************************************************************
  */
#ifndef _ECATUSER_H_
#define _ECATUSER_H_

/* 包含头文件 ----------------------------------------------------------------*/
#include "ethercat.h"
#include "osal.h"

/* 私有类型定义 --------------------------------------------------------------*/
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
/* 私有宏定义 ----------------------------------------------------------------*/

/* 扩展变量 ------------------------------------------------------------------*/
extern int32_t ecOffset;

/* 私有函数原形 --------------------------------------------------------------*/

/* 函数体 --------------------------------------------------------------------*/

void ecat_loop_init(void);
void ecat_init(void);
void ecat_loop(void);
void ctrl_state(void);


#endif


/********** (C) COPYRIGHT 2019-2030 硬石嵌入式开发团队 *******END OF FILE******/

