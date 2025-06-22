/*
*********************************************************************************************************
*
*	模块名称 :soem主站应用层
*	文件名称 : 
*	版    本 : V1.0
*	说    明 : 控制汇川伺服
*						
*	修改记录 :
*		版本号     日期        作者      
*		V1.0    2025-05-14  shisanxiang1
*
*********************************************************************************************************
*/
#include "soem.h"
#include "stdio.h"
#include "string.h"
#include "stm32F4xx_hal.h"



/*  周期同步时间 */
#define SYNC0TIME 1000000
/*  sdo写操作调试信息输出宏 */
#define DEBUG 1


/* pdo结构体  */
PDO_Outputs *outputs1;
PDO_Input *inputs1;

char IOmap[4096];
uint32_t Motor_Run=0;
int expectedWKC;




/*
*********************************************************************************************************
*	函 数 名: int write8(uint16  slave, uint16  index, uint8  subindex, int value)
*	功能说明:sdo写8位
*	形    参: 
*********************************************************************************************************
*/
int write8(uint16  slave, uint16  index, uint8  subindex, int value)
{
	uint8 temp = value;
	/*  调用的是soem中coe库封装的sdo写函数 */
	int rtn = ec_SDOwrite(slave, index, subindex, FALSE, sizeof(temp), &temp, EC_TIMEOUTRXM );

	if (rtn == 0) { printf("SDOwrite to %#x failed !!! \r\n", index); }
	else if (DEBUG) { printf("SDOwrite to slave%d  index:%#x value:%x Successed !!! \r\n", slave, index, temp); }
	return rtn;
}

/*
*********************************************************************************************************
*	函 数 名: int write16(uint16  slave, uint16  index, uint8  subindex, int value)
*	功能说明:sdo写16位
*	形    参: 
*********************************************************************************************************
*/
int write16(uint16  slave, uint16  index, uint8  subindex, int value)
{
	uint16 temp = value;

	int rtn = ec_SDOwrite(slave, index, subindex, FALSE, sizeof(temp), &temp, EC_TIMEOUTRXM * 20);

	if (rtn == 0) { printf("SDOwrite to %#x failed !!! \r\n", index); }
	else if (DEBUG) { printf("SDOwrite to slave%d  index:%#x value:%x Successed !!! \r\n", slave, index, temp); }
	return rtn;
}

/*
*********************************************************************************************************
*	函 数 名: int write32(uint16 slave, uint16 index, uint8 subindex, int value)
*	功能说明:sdo写32位
*	形    参: 
*********************************************************************************************************
*/
int write32(uint16 slave, uint16 index, uint8 subindex, int value)
{
	uint32 temp = value;

	int rtn = ec_SDOwrite(slave, index, subindex, FALSE, sizeof(temp), &temp, EC_TIMEOUTRXM * 20);
	if (rtn == 0) { printf("SDOwrite to %#x failed !!! \r\n", index); }
	else if (DEBUG) { printf("SDOwrite to slave%d  index:%#x value:%x Successed !!! \r\n", slave, index, temp); }
	return rtn;
}

/*
*********************************************************************************************************
*	函 数 名: INOVANCE_Nsetup(uint16 slvcnt)
*	功能说明:汇川伺服可变pdo映射配置
*	形    参: 从站号
*********************************************************************************************************
*/
int INOVANCE_Nsetup(uint16 slvcnt)
{
	
	printf(" slvcnt = %d\r\n",slvcnt);
	/*  清除初始rpdo配置 */
	write8(slvcnt, 0x1C12, 00, 0);				      //清0x1C12数据		
	write8(slvcnt, 0x1600, 00, 0);				      //清0x1601数据		
	/* 1600h为映射参数配置，可配置子索引至具体6000h  */
	write32(slvcnt, 0x1600, 01, 0x60400010);		//增加0x1601子索引		控制字
	write32(slvcnt, 0x1600, 02, 0x60830020);		//增加0x1601子索引  	加速度
	write32(slvcnt, 0x1600, 03, 0x60840020);		//增加0x1601子索引		减速度
	write32(slvcnt, 0x1600, 04, 0x60FF0020);		//增加0x1601子索引		目标速度
	/*  表明配置了四个映射 */
	write8(slvcnt, 0x1600, 00, 4);				      //设置子索引数量
	/*  1c12h是汇川伺服唯一可变的rpdo，是pdo通讯配置 */
	write16(slvcnt, 0x1C12, 01, 0x1600);			  //设置RxPDO映射		
	/*  00索引是表明开启了几个pdo */	
	write8(slvcnt, 0x1C12, 00, 1);

	/* 手册原文：写入PDO映射组。按场景需求写入映射配置组，1C12h中预写入1600h/1701h~1705h的值；1C13h中
		预写入1A00h/1B01h~1B04h的值。注意：只有1600h和1A00h是可配置映射组，其他是固定映射配
		置； */
	
	/* 配置tpdo，和上面类似  */
	write8(slvcnt, 0x1C13, 00, 00);				      //清0x1C12数据							
	write8(slvcnt, 0x1A00, 00, 00);				      //清0x1600数据							
	write32(slvcnt, 0x1A00, 01, 0x60410010);		//增加0x1A01子索引			
  write32(slvcnt, 0x1A00, 02, 0x60640020);		//增加0x1A01子索引	
  write32(slvcnt, 0x1A00, 03, 0x606C0020);		//增加0x1A01子索引	
  write8(slvcnt, 0x1A00, 00, 03);				      //设置子索引数量
 
	write16(slvcnt, 0x1C13, 01, 0x1A00);			  //设置TxPDO映射
	write8(slvcnt, 0x1C13, 00, 01);
	
	write8(slvcnt, 0x6060, 00, 3);                   //设置为速度模式

  return 0;
}

/**
  * 函数功能: ecat初始化
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
void ecat_init(void)
{
	int chk, slc;
	int i;
	
    /* initialise SOEM, bind socket to ifname */
  if (ec_init("ServoDrive"))
	{
		printf("ec_init succeeded.\r\n");
        
		if ( ec_config_init(FALSE) > 0 )
		{
			printf("%x slaves found and configured.%x \r\n",ec_slave[1].eep_man,ec_slave[1].eep_id);
            
			if ( ec_slavecount >= 1 ) 
			{
				for(slc = 1; slc <= ec_slavecount; slc++)
				 {									 
					 //汇川伺服
					 if((ec_slave[slc].eep_man == 0x100000) && (ec_slave[slc].eep_id == 0xc010d))
					 {
							 printf("Found %s at position %d\r\n", ec_slave[slc].name, slc);
							 printf("Found %x at position %d\r\n", ec_slave[slc].configadr, slc);
							 ec_slave[slc].PO2SOconfig = &INOVANCE_Nsetup; 
					 }
				 }
       }
            
      ec_configdc();

			
			ec_dcsync0(1, TRUE, SYNC0TIME, 0); // SYNC0 on slave 1
			 
		  ec_config_map(&IOmap);
			 
			printf("segments : %d : %d %d %d %d\n",ec_group[0].nsegments ,ec_group[0].IOsegment[0],ec_group[0].IOsegment[1],ec_group[0].IOsegment[2],ec_group[0].IOsegment[3]);

			ec_statecheck(0, EC_STATE_SAFE_OP,  EC_TIMEOUTSTATE);
	
		  printf("Request operational state for all slaves\n");
		  expectedWKC = (ec_group[0].outputsWKC * 2) + ec_group[0].inputsWKC;
		  printf("Calculated workcounter %d\n", expectedWKC);
		  ec_slave[0].state = EC_STATE_OPERATIONAL;

			ec_send_processdata();
			
			ec_receive_processdata(EC_TIMEOUTRET);
				
			ec_writestate(0);
			do
		  {
				ec_statecheck(0, EC_STATE_OPERATIONAL, 50000);
		  }     
		  while (chk-- && (ec_slave[0].state != EC_STATE_OPERATIONAL));			
				 
			printf("DC capable : %d\r\n",ec_configdc());
			
			//HAL_Delay(100);
			
			for( i=0; i<10; i++ )
			{
				ec_slave[0].state = EC_STATE_OPERATIONAL;
				ec_writestate(0);
			}
				 
			if (ec_slave[0].state == EC_STATE_OPERATIONAL )
			{					
				outputs1 = (PDO_Outputs *)ec_slave[1].outputs;
				inputs1  = (PDO_Input *)ec_slave[1].inputs;

				Motor_Run = 1;

				printf("all slaves reached operational state.\r\n");
			}			
			else
			{
				printf("E/BOX not found in slave configuration.\r\n");
			}
		}
		else
		{
			printf("No slaves found!\r\n");
		}
	}
	else
	{
		printf("No socket connection Excecute as root\r\n");
	}
    
}

/*
*********************************************************************************************************
*	函 数 名: ecat_loop(void)
*	功能说明:	pdo发送和接受循环，在tim4的中断回调中调用
*	形    参: 
*********************************************************************************************************
*/
void ecat_loop(void)
{
	if (Motor_Run>0)
	{
		ec_send_processdata();
		
		ec_receive_processdata(EC_TIMEOUTRET);        
		
	}
}

/*
*********************************************************************************************************
*	函 数 名: ctrl_state(void)
*	功能说明:伺服控制
*	形    参: 
*********************************************************************************************************
*/
void ctrl_state(void)
{
    switch (outputs1->controlword)
    {
			/*  控制字为0时 */
    case 0:
	      outputs1->decelerAtion=139810130;
		    outputs1->accelerAtion=139810130;
		/*
				每秒27962026个指令，电机编码器为23为，电子齿轮比默认为1，每圈是2^23为8388608/1
				27962026*1/83880=3r/s
		*/
		    outputs1->targetVelocity=27962026;
		/*  伺服无故障→伺服准备好 */
        outputs1->controlword = 6;
        break;
		/*  控制字为6时 */
    case 6:
				/*  伺服准备好→等待打开伺服使能 */
        outputs1->controlword = 7;
        break;
		/*  控制字为7时 */
    case 7:
			/*  等待打开伺服使能→伺服运行 */
        outputs1->controlword = 0x0f;
        break;
    case 0x0f:
        break;
    default:
        outputs1->controlword = 6;
        break;
    }
    printf("\r\nctl = %d  \r\n",outputs1->controlword);
}


